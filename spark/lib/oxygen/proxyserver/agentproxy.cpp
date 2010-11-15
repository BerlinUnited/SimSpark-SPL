/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Dec 31 2009
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004-2009 RoboCup Soccer Server 3D Maintenance Group
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
#include "agentproxy.h"
#include <rcssnet/exception.hpp>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/simulationserver/netcontrol.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace rcss::net;
using boost::shared_ptr;

AgentProxy::AgentProxy(int cycleMillisecs) : Node(),
    mCycleMillisecs(cycleMillisecs), mFinished(false),
    mAgentBuffer(new NetBuffer)
{
}

AgentProxy::~AgentProxy()
{
}

void AgentProxy::Start(boost::shared_ptr<rcss::net::Socket> agentSocket,
           rcss::net::Addr serverAddress)
{
    try
    {
        mAgentSocket = agentSocket;
        mServerSocket = NetControl::CreateSocket(NetControl::ST_TCP);
        if (!mServerSocket)
        {
            mFinished = true;
            return;
        }
        GetLog()->Normal() << "(AgentProxy) '" << GetName() << "'connecting to "
                << serverAddress << "\n";
        mServerSocket->connect(serverAddress);
        if (mServerSocket->isConnected())
        {
            cout << "(AgentProxy) '" << GetName() << "' connected successfully"
                    << endl;
        }

        // assure that a NetMessage object is registered
        mNetMessage = FindChildSupportingClass<NetMessage>();
        if (mNetMessage.get() == 0)
        {
            mNetMessage = shared_ptr<NetMessage>(new NetMessage());
        }

        mAgentConnectionThread = boost::thread(
            &AgentProxy::AgentConnectionHandler, this);
        mServerConnectionThread = boost::thread(
            &AgentProxy::ServerConnectionHandler, this);
        return;
    }
    catch (BindErr error)
    {
        GetLog()->Error() << "(AgentProxy) '" << GetName()
                << "' failed to bind socket with '" << error.what() << "'"
                << endl;
    }
    catch (ConnectErr error)
    {
        GetLog()->Error() << "(AgentProxy) '" << GetName()
                << "' connection failed with: '" << error.what() << "'" << endl;
    }

    // executed on error
    mServerSocket->close();
    mFinished = true;
}

void AgentProxy::Stop()
{
    mFinished = true;
    mServerConnectionThread.join();
}

void AgentProxy::ServerConnectionHandler()
{
    string syncMsg("(sync)");
    string servermsg, agentmsg;
    boost::system_time cycleFinishTime = boost::get_system_time()
            + boost::posix_time::milliseconds(mCycleMillisecs);
    shared_ptr<NetBuffer> netbuf(new NetBuffer);
    TRawBuffer recvbuf;

    mNetMessage->PrepareToSend(syncMsg);
    while (!mFinished)
    {
        try
        {
            servermsg.clear();
            boost::thread::sleep(cycleFinishTime);

            boost::mutex::scoped_lock agentBufLock(mAgentBufferMutex);
            while (!mAgentBuffer->IsEmpty() &&
                    mNetMessage->Extract(mAgentBuffer, agentmsg))
            {
                mNetMessage->PrepareToSend(agentmsg);
                mServerSocket->send(agentmsg.data(), agentmsg.size());
            }
            agentBufLock.unlock();

            mServerSocket->send(syncMsg.data(), syncMsg.size());
            do
            {
                int retval = mServerSocket->recv(recvbuf.data(), recvbuf.size());
                if (retval > 0)
                    netbuf->AddFragment(string(recvbuf.data(), recvbuf.size()));
                else
                {
                    GetLog()->Error()
                        << "(AgentProxy) ERROR: '" << GetName()
                        << "' recv returned error on a client socket '"
                        << strerror(errno) << "' " << endl;
                    mFinished = true;
                    break;
                }
            } while (!mNetMessage->Extract(netbuf, servermsg));

            if (!servermsg.empty())
            {
                cycleFinishTime = boost::get_system_time()
                        + boost::posix_time::milliseconds(mCycleMillisecs);

                mNetMessage->PrepareToSend(servermsg);
                mAgentSocket->send(servermsg.data(), servermsg.size(),
                    Socket::DONT_CHECK);
            }
        }
        catch (boost::thread_interrupted e)
        {
        }
    }
    mServerSocket->close();
    mFinished = true;
    mAgentConnectionThread.join();
}

void AgentProxy::AgentConnectionHandler()
{
    TRawBuffer recvbuf;

    while (!mFinished)
    {
        int retval = mAgentSocket->recv(recvbuf.data(), recvbuf.size());
        if (retval > 0)
        {
            boost::mutex::scoped_lock agentBufLock(mAgentBufferMutex);
            mAgentBuffer->AddFragment(string(recvbuf.data(), recvbuf.size()));
        }
        else if (retval <= 0 && errno != EAGAIN)
        {
            GetLog()->Error() << "(AgentProxy) ERROR: '" << GetName()
                    << "' recv returned error on a client socket '"
                    << strerror(errno) << "' " << endl;
            mFinished = true;
        }
    }
}
