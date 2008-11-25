/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "netcontrol.h"
#include "netmessage.h"
#include <zeitgeist/logserver/logserver.h>
#include <rcssnet/exception.hpp>
#include <rcssnet/tcpsocket.hpp>
#include <rcssnet/udpsocket.hpp>
#include <sstream>
#include <cerrno>

#ifndef WIN32
#include <netinet/in.h>
#endif

using namespace rcss::net;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

NetControl::NetControl() : SimControlNode()
{
    mBufferSize = 64 * 1024;
    mBuffer     = shared_array<char>(new char[mBufferSize]);
    mSocketType = ST_TCP;
    mLocalAddr  = Addr(INADDR_ANY, INADDR_ANY);
    mClientId   = 1;
}

NetControl::~NetControl()
{
}

void NetControl::SetServerPort(Addr::PortType port)
{
    mLocalAddr.setPort(port);
}

rcss::net::Addr::PortType NetControl::GetServerPort()
{
    return mLocalAddr.getPort();
}

void NetControl::SetServerType(ESocketType type)
{
    mSocketType = type;
}

NetControl::ESocketType NetControl::GetServerType()
{
    return mSocketType;
}

boost::shared_ptr<Socket> NetControl::CreateSocket(ESocketType type)
{
    shared_ptr<Socket> socket;

    try
        {
            switch (type)
                {
                case ST_UDP:
                    socket = shared_ptr<Socket>(new UDPSocket());
                    break;

                case ST_TCP:
                    socket = shared_ptr<Socket>(new TCPSocket());
                    break;

                default:
                    cerr << "(NetControl) ERROR: unknown socket type "
                         << type << "\n";
                    break;
                }
        }

    catch (OpenErr error)
        {
            cerr << "(NetControl) failed to create socket with '"
                 << error.what()
                 << endl;
        }

    return socket;
}

string NetControl::DescribeSocketType()
{
    stringstream ss;

    switch (mSocketType)
        {
        case ST_UDP:
            ss << "UDP";
            break;

        case ST_TCP:
            ss << "TCP";
            break;

        default:
            ss << "(unknown socket type)";
            break;
        }

    ss << ":" << mLocalAddr.getPort();

    return ss.str();
}

void NetControl::InitSimulation()
{
    // assert that the local port has been set
    if (mLocalAddr.getPort() == INADDR_ANY)
        {
            GetLog()->Error()
                << "(NetControl) ERROR: local port has no been set in '"
                << GetClass()->GetName() << "'\n";
            return;
        }

    GetLog()->Normal() << "(NetControl) '" << GetName()
                       << "' setting up a server on " << DescribeSocketType() << std::endl;

    mSocket = CreateSocket(mSocketType);

    if (mSocket.get() == 0)
        {
            return;
        }

  int ret = mSocket->setNonBlocking(true);

  if (ret < 0)
      {
          GetLog()->Error()
              << "(NetControl) failed to set server socket to non "
              << "blocking mode with '"
              << strerror(errno) << "'\n";
          mSocket->close();
          return;
      }

  try
      {
          mSocket->bind(mLocalAddr);
      }

  catch (BindErr error)
      {
          GetLog()->Error() << "(NetControl) failed to bind socket with '"
                            << error.what() << "'"
                            << endl;

          mSocket->close();
          return;
      }

  try
      {
          if (mSocketType == ST_TCP)
              {
#ifdef SOMAXCONN
                  mSocket->listen(SOMAXCONN);
#else
                  mSocket->listen(50);
#endif
              }
      }

  catch (ListenErr error)
      {
          GetLog()->Error() << "(NetControl) failed to listen on socket with '"
                            << error.what() << "'"
                            << endl;

          mSocket->close();
          return;
      }

  // assure that a NetMessage object is registered
  mNetMessage = FindChildSupportingClass<NetMessage>();

  if (mNetMessage.get() == 0)
      {
          mNetMessage = shared_ptr<NetMessage>(new NetMessage());
      }
}

void NetControl::DoneSimulation()
{
    // reset the cached NetMessage reference
    mNetMessage.reset();

    // close all client connections
    while (! mClients.empty())
    {
        RemoveClient(mClients.begin());
    }

    // shutdown the server socket
    if (mSocket.get() != 0)
        {
            mSocket->close();
            GetLog()->Normal() << "(NetControl) '" << GetName()
                               << "' closed server socket "
                               << DescribeSocketType() << std::endl;
        }

    mSocket.reset();
    mClients.clear();
}

void NetControl::AddClient(const Addr& from, shared_ptr<Socket> socket)
{
    shared_ptr<Client> client(new Client(mClientId,from,socket));
    mClients[from] = client;

    GetLog()->Normal()
        << "(NetControl) '" << GetName() << "' accepted a "
        << ((socket.get() != 0) ? "TCP" : "UDP")
        << " connection from '"
        << from.getHostStr() << ":" << from.getPort()
        << "' id " << mClientId
        << endl;

    mClientId++;
    ClientConnect(client);
}

void NetControl::RemoveClient(TAddrMap::iterator iter)
{
    shared_ptr<Client> client = (*iter).second;
    ClientDisconnect(client);

    shared_ptr<Socket> socket = client->socket;

    GetLog()->Normal()
        << "(NetControl) '" << GetName() << "' closing a "
        << ((socket.get() != 0) ? "TCP" : "UDP")
        << " connection from '"
        << client->addr.getHostStr() << ":" << client->addr.getPort()
        << "' id " << client->id << endl;

    if (socket.get() != 0)
        {
            socket->close();
        }

    mClients.erase(iter);
}

void NetControl::RemoveClient(const Addr& from)
{
    TAddrMap::iterator mapIter = mClients.find(from);

    if (mapIter == mClients.end())
        {
            GetLog()->Warning()
                << "(NetControl) '" << GetName()
                << "' RemoveClient called with an unknown client address\n";
            return;
        }

    RemoveClient(mapIter);
}

void NetControl::ClientConnect(shared_ptr<Client> /*client*/)
{
    // empty callback, implemented in derived classes
}

void NetControl::ClientDisconnect(shared_ptr<Client> /*client*/)
{
    // empty callback, implemented in derived classes
}

void NetControl::SendClientMessage(shared_ptr<Client> client, const string& msg)
{
    if (client.get() == 0)
        {
            return;
        }

    int rval = 0;
    shared_ptr<Socket> socket = client->socket;

    if (socket.get() == 0)
        {
            // udp client
            if (mSocket.get() != 0)
                {
                    rval = mSocket->send(msg.data(), msg.size(), client->addr);
                }
        } else
            {
                // tcp client
                rval = socket->send(msg.data(), msg.size());
            }

    if (rval < 0)
        {
            GetLog()->Error()
                << "(NetControl::SendMessage) ERROR: '" << GetName()
                << "' send returned error '"
                << strerror(errno) << "' " << endl;
        }
}

void NetControl::SendClientMessage(const Addr& addr, const string& msg)
{
    TAddrMap::iterator iter = mClients.find(addr);

    if (iter == mClients.end())
        {
            GetLog()->Error()
                << "(NetControl::SendMessage) ERROR: unknown client address '"
                << addr.getHostStr() << ":" << addr.getPort() << "'\n";
            return;
        }

    SendClientMessage((*iter).second,msg);
}

void NetControl::AcceptTCPConnections()
{
    if (
        (mSocketType != ST_TCP) ||
        (mSocket.get() == 0)
        )
        {
            return;
        }

    Socket::SocketDesc fd = mSocket->getFD();

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);

    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;

    for(;;)
        {
#ifdef WIN32
            // maxFd is ignored on Win32 and is present just for api compatibility
            int maxFd = 0;
#else
            int maxFd = fd + 1;
#endif

            int ret = select(maxFd, &readfds, 0, 0, &time);

            if (ret == 0)
                {
                    // no more connections pending
                    break;
                }

            if (ret < 0)
                {
                    GetLog()->Error()
                        << "(NetControl) ERROR: '" << GetName()
                        << "' select returned error on server socket "
                        << DescribeSocketType()
                        << ' ' << strerror(errno) << "\n"
                        << "(NetControl) ERROR: closing server socket"
                        << endl;

                    mSocket->close();
                    mSocket.reset();
                    break;
                }

            try
                {
                    Addr addr;
                    shared_ptr<Socket> socket(mSocket->accept(addr));

                    if (socket.get() == 0)
                    {
                        return;
                    }

                    int ret = socket->setNonBlocking(true);

                    if (ret < 0)
                        {
                            GetLog()->Error()
                                << "(NetControl) failed to set client socket to"
                                << " non blocking mode with '"
                                << strerror(errno)
                                << "'. closing connection\n";

                            socket->close();
                        } else
                            {
                                AddClient(addr,socket);
                            }
                }

            catch (AcceptErr error)
                {
                    GetLog()->Error()
                        << "(NetControl) '" << GetName()
                        << "' failed to accept TCP connection with '"
                        << error.what() << endl;
                    break;
                }
        }
}

void NetControl::CloseDeadConnections()
{
    while (! mCloseClients.empty())
        {
            RemoveClient(mCloseClients.front());
            mCloseClients.pop_front();
        }
}

void NetControl::StartCycle()
{
    // read any pending messages, for UDP new source addresses are
    // registered as new clients
    ReadMessages();

    // if we manage a TCP server socket accept new client connections
    AcceptTCPConnections();
}

void NetControl::EndCycle()
{
    // close connections marked as dead during this cycle
    CloseDeadConnections();
}

void NetControl::ReadMessages()
{
    switch (mSocketType)
        {
        case ST_TCP:
            ReadTCPMessages();
            break;

        case ST_UDP:
            ReadUDPMessages();
            break;

        default:
            break;
        }
}

void NetControl::StoreFragment(const Addr& addr, int size)
{
    TBufferMap::iterator msgIter = mBuffers.find(addr);
    if (msgIter == mBuffers.end())
        {
            // allocate a new NetBuffer for the client
            mBuffers[addr] =
                (shared_ptr<NetBuffer>
                 (new NetBuffer(addr,string(mBuffer.get(),size)))
                 );
        } else
            {
                // append to an existing NetBuffer
                (*msgIter).second->AddFragment(string(mBuffer.get(),size));
            }
}

void NetControl::ReadUDPMessages()
{
    if (mSocket.get() == 0)
        {
            return;
        }

    Socket::SocketDesc fd = mSocket->getFD();

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);

    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;

    for(;;)
        {
#ifdef WIN32
            // maxFd is ignored on Win32 and is present just for api compatibility
            int maxFd = 0;
#else
            int maxFd = fd + 1;
#endif

            int ret = select(maxFd, &readfds, 0, 0, &time);

            if (ret == 0)
                {
                    // no data available
                    break;
                }

            if (ret < 0)
                {
                    GetLog()->Error()
                        << "(NetControl) ERROR: ReadUDPSocket '" << GetName()
                        << "' select returned error on server socket "
                        << DescribeSocketType()
                        << ' ' << strerror(errno) << endl;
                    break;
                }

            // read fragment
            Addr from;
            int rval = mSocket->recv(mBuffer.get(), mBufferSize, from);

            if (rval < 0)
                {
                    GetLog()->Error()
                        << "(NetControl) ERROR: ReadUDPSocket '" << GetName()
                        << "' recv returned error '"
                        << strerror(errno) << "' " << endl;
                    continue;
                }

            TAddrMap::iterator iter =  mClients.find(from);
            if (iter == mClients.end())
                {
                    // a new client connected
                    AddClient(from);
                }

            StoreFragment(from,rval);
        }
}

void NetControl::ReadTCPMessages()
{
    if (mClients.empty())
    {
        return;
    }

    // generate a set of client socket fds
    fd_set client_fds;
    FD_ZERO(&client_fds);

    int maxFd = 0;

    for (
         TAddrMap::iterator iter=mClients.begin();
         iter != mClients.end();
         ++iter
         )
        {
            const Socket::SocketDesc fd = (*iter).second->socket->getFD();
#ifdef WIN32
            // maxFd is ignored for Win32
            maxFd = 0;
#else
            maxFd = std::max<int>(fd,maxFd);
#endif
            FD_SET(fd,&client_fds);
        }

    // test for pending fragments
    for(;;)
        {
            timeval time;
            time.tv_sec = 0;
            time.tv_usec = 0;

            fd_set test_fds = client_fds;
            int ret = select(maxFd+1, &test_fds, 0, 0, &time);

            if (ret == 0)
                {
                    // no data available
                    break;
                }

            if (ret < 0)
                {
                    GetLog()->Error()
                        << "(NetControl) ERROR: '" << GetName()
                        << "' select returned error on client sockets '"
                        << strerror(errno) << "' " << endl;

                    break;
                }

            // read fragments
            for (
                 TAddrMap::iterator iter=mClients.begin();
                 iter != mClients.end();
                 ++iter
                 )
                {
                    const Socket::SocketDesc fd = (*iter).second->socket->getFD();
                    if (! FD_ISSET(fd, &test_fds))
                        {
                            continue;
                        }

                    // read a fragment
                    shared_ptr<Client>& client = (*iter).second;
                    int rval = client->socket->recv(mBuffer.get(),mBufferSize);

                    if (rval > 0)
                        {
                            StoreFragment(client->addr,rval);
                        } else
                            {
                                if (rval <= 0)
                                    {
                                        GetLog()->Error()
                                            << "(NetControl) ERROR: '" << GetName()
                                            << "' recv returned error on a client socket '"
                                            << strerror(errno) << "' " << endl;
                                    }

                                // mark the client connection to be
                                // closed and exclude it from further
                                // select() calls
                                FD_CLR(fd,&client_fds);
                                mCloseClients.push_back(client->addr);
                            }
                }
        }
}
