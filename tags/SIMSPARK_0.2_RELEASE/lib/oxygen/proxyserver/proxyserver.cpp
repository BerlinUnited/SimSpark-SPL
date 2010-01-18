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
#include "proxyserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/simulationserver/netcontrol.h>
#include <rcssnet/exception.hpp>
#include "agentproxy.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace rcss::net;

ProxyServer::ProxyServer() : Leaf(),
    mRunning(false),
    mCycleMillisecs(0),
    mLocalAddr(0, Addr::ANY),
    mServerAddr(0, Addr::ANY),
    mSocket(NetControl::CreateSocket(NetControl::ST_TCP))
{
}

ProxyServer::~ProxyServer()
{
}

void ProxyServer::SetProxyPort(Addr::PortType port)
{
    mLocalAddr.setPort(port);
}

Addr::PortType ProxyServer::GetProxyPort() const
{
    return mLocalAddr.getPort();
}

void ProxyServer::SetServerAddress(const rcss::net::Addr &serverAddress)
{
    mServerAddr = serverAddress;
}

void ProxyServer::SetCycleLength(int millisecs)
{
    mCycleMillisecs = millisecs;
}

bool ProxyServer::Run()
{
    bool success = false;

    if (mLocalAddr.getPort() == 0)
    {
        GetLog()->Error()
                << "(ProxyServer) ERROR: local port has no been set in '"
                << GetClass()->GetName() << "'\n";
        return false;
    }
    if (mServerAddr.getPort() == 0)
    {
        GetLog()->Error()
                << "(ProxyServer) ERROR: server address has no been set in '"
                << GetClass()->GetName() << "'\n";
        return false;
    }
    if (!mSocket)
    {
        GetLog()->Error()
                << "(ProxyServer) ERROR: No valid socket has been created.\n";
        return false;
    }
    if (mCycleMillisecs == 0)
    {
        GetLog()->Error()
                << "(ProxyServer) ERROR: Cycle length not set.\n";
        return false;
    }

    int ret = mSocket->setReuseAddr(true);
    if (ret < 0)
    {
        GetLog()->Warning()
                << "(ProxyServer) failed to enable reuse of server socket "
                << "with '" << strerror(errno) << "'\n";
    }

    try
    {
        mSocket->bind(mLocalAddr);
        mSocket->listen(50);
        mRunning = true;

        do
        {
            Addr addr;
            boost::shared_ptr<Socket> socket(mSocket->accept(addr));
            if (socket)
            {
                GetLog()->Normal() << "(ProxyServer) accepted a new connection"
                    << " from " << socket->getPeer() << '\n';

                AgentProxy *agentProxy = new AgentProxy(mCycleMillisecs);
                agentProxy->Start(socket, mServerAddr);
                mClientProxyList.push_back(agentProxy);
            }
        } while (mRunning);
        success = true;
    }
    catch (BindErr error)
    {
        GetLog()->Error() << "(ProxyServer) failed to bind socket with '"
                << error.what() << "'" << endl;
    }
    catch (ListenErr error)
    {
        GetLog()->Error() << "(ProxyServer) failed to listen on socket with '"
                << error.what() << "'" << endl;
    }
    catch (AcceptErr error)
    {
        GetLog()->Error() << "(ProxyServer) '" << GetName()
                << "' failed to accept TCP connection with '" << error.what()
                << endl;
    }

    mSocket->close();
    return success;
}
