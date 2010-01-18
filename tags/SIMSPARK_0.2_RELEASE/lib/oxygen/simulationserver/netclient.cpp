/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "netclient.h"
#include <zeitgeist/logserver/logserver.h>
#include <rcssnet/exception.hpp>
#include <cerrno>

#ifndef WIN32
#include <netinet/in.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace rcss::net;
using namespace salt;
using namespace boost;
using namespace std;

NetClient::NetClient() : SimControlNode()
{
    mHost = "127.0.0.1";
    mPort = 3200;
    mBufferSize = 64 * 1024;
    mBuffer = shared_array<char>(new char[mBufferSize]);
    mType = NetControl::ST_TCP;
    mNetBuffer = shared_ptr<NetBuffer>(new NetBuffer());
}

NetClient::~NetClient()
{
}

void NetClient::SetServer(const std::string& host)
{
    mHost = host;
}

const std::string& NetClient::GetServer() const
{
    return mHost;
}

void NetClient::SetPort(int port)
{
    mPort = port;
}

int NetClient::GetPort() const
{
    return mPort;
}

void NetClient::SetClientType(NetControl::ESocketType type)
{
    mType = type;
}

NetControl::ESocketType NetClient::GetClientType()
{
    return mType;
}

bool NetClient::Connect()
{
    mSocket = NetControl::CreateSocket(mType);

    if (mSocket.get() == 0)
        {
            return false;
        }

    GetLog()->Normal()
        << "(NetClient) '" << GetName() << "'connecting to "
        << ((mType == NetControl::ST_UDP) ? "UDP " : "TCP ")
        << mHost << ":" << mPort << "\n";

    try
        {
            Addr local(INADDR_ANY,INADDR_ANY);
            mSocket->bind(local);
        }

    catch (BindErr error)
        {
            GetLog()->Error()
                << "(NetClient) '" << GetName()
                << "' failed to bind socket with '"
                << error.what() << "'" << endl;

            mSocket->close();
            return false;
        }

    try
        {
            Addr server(mPort,mHost);
            mSocket->connect(server);
        }

    catch (ConnectErr error)
        {
            GetLog()->Error()
                << "(NetClient) '" << GetName()
                << "' connection failed with: '"
                << error.what() << "'" << endl;
            mSocket->close();
            mSocket.reset();
            return false;
        }

    if (mSocket->isConnected())
        {
            cout << "(NetClient) '" << GetName()
                 << "' connected successfully" << endl;
        }

  // assure that a NetMessage object is registered
  mNetMessage = FindChildSupportingClass<NetMessage>();

  if (mNetMessage.get() == 0)
      {
          mNetMessage = shared_ptr<NetMessage>(new NetMessage());
      }

  return true;
}

void NetClient::SendMessage(const string& msg)
{
    if (mNetMessage.get() == 0)
        {
            return;
        }

    string preparedMsg = msg;
    mNetMessage->PrepareToSend(preparedMsg);;

    if (mSocket.get() == 0)
        {
            return;
        }

    int rval = 0;

    if (mType == NetControl::ST_UDP)
        {
            Addr server(mPort,mHost);
            rval = mSocket->send(msg.data(), msg.size(), server);
        } else
            {
                rval = mSocket->send(preparedMsg.data(), preparedMsg.size());
            }

    if (rval < 0)
        {
            GetLog()->Error()
                << "(NetClient::SendMessage) ERROR: "
                << "send returned error '"
                << strerror(errno) << "' " << endl;
        }
}

void NetClient::CloseConnection()
{
    if (mSocket.get() == 0)
        {
            return;
        }

    mSocket->close();
    mSocket.reset();

    GetLog()->Normal() << "(NetClient) '" << GetName()
                       << "' closed connection to "
                       << mHost << ":" << mPort << "\n";

    mNetMessage.reset();
}

void NetClient::ReadFragments()
{
    if (mSocket.get() == 0)
        {
            return;
        }

    for (;;)
        {
            // test for available data
            Socket::SocketDesc fd = mSocket->getFD();

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(fd,&readfds);

            timeval time;
            time.tv_sec = 0;
            time.tv_usec = 0;

#ifdef WIN32
            int maxFd = 0;
#else
            int maxFd = fd + 1;
#endif

            int rval = select(maxFd, &readfds, 0, 0, &time );

            if (rval == 0)
                {
                    break;
                }

            if (rval < 0)
                {
                    GetLog()->Error()
                        << "(NetClient) ERROR select on client "
                        << "socket failed with '"
                        << strerror(errno) << "'" << endl;
                    CloseConnection();
                    return;
                }

            rval = mSocket->recv(mBuffer.get(),mBufferSize);

            if (rval == 0)
                {
                    CloseConnection();
                    return;
                }

            if (rval < 0)
                {
                    GetLog()->Error()
                        << "(NetClient) '" << GetName()
                        << "' ERROR: '" << GetName()
                        << "' recv returned error '"
                        << strerror(errno) << "' " << endl;
                    return;
                }

            string fragment(mBuffer.get(),rval);
            mNetBuffer->AddFragment(string(mBuffer.get(),rval));
        }
}
