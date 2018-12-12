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
#ifndef OXYGEN_PROXYSERVER_H
#define OXYGEN_PROXYSERVER_H

#ifndef Q_MOC_RUN
#include <boost/ptr_container/ptr_list.hpp>
#endif
#include <oxygen/oxygen_defines.h>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <rcssnet/addr.hpp>
#include <rcssnet/socket.hpp>

namespace oxygen
{

class AgentProxy;

/** \class ProxyServer TODO: Add description
 */
class OXYGEN_API ProxyServer: public zeitgeist::Leaf
{
public:
    ProxyServer();
    virtual ~ProxyServer();

    /** sets the local port, on which connections are accepted */
    void SetProxyPort(rcss::net::Addr::PortType port);

    /** returns the local port, on which connections are accepted */
    rcss::net::Addr::PortType GetProxyPort() const;

    /** sets the server address to connect to */
    void SetServerAddress(const rcss::net::Addr &serverAddress);

    /** sets the length of a cycle in milli seconds */
    void SetCycleLength(int millisecs);

    /** starts the run loop of the proxy server, wait for incomming connection
     * requests */
    bool Run();

private:
    bool mRunning;
    int mCycleMillisecs;

    /** the local port, on which connections are accepted */
    rcss::net::Addr mLocalAddr;

    /** the server address to which we should connect */
    rcss::net::Addr mServerAddr;

    /** the socket used to accept connections */
    boost::shared_ptr<rcss::net::Socket> mSocket;

    boost::ptr_list<AgentProxy> mClientProxyList;
};

DECLARE_CLASS(ProxyServer);

} // namespace oxygen

#endif // OXYGEN_PROXYSERVER_H
