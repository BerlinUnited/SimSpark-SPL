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
#ifndef OXYGEN_AGENTPROXY_H
#define OXYGEN_AGENTPROXY_H

#ifndef Q_MOC_RUN
#include <boost/array.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/thread/thread.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/thread/mutex.hpp>
#endif
#include <oxygen/oxygen_defines.h>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include <rcssnet/addr.hpp>
#include <rcssnet/socket.hpp>
#include <oxygen/simulationserver/netbuffer.h>
#include <oxygen/simulationserver/netmessage.h>

namespace oxygen
{

/** \class AgentProxy TODO: add description
 */
class OXYGEN_API AgentProxy: public zeitgeist::Node
{
private:
    typedef boost::array<char, 32 * 1024> TRawBuffer;

public:
    AgentProxy(int cycleMillisecs = 0);
    virtual ~AgentProxy();

    /** starts the proxy for a single agent */
    void Start(boost::shared_ptr<rcss::net::Socket> agentSocket,
               rcss::net::Addr serverAddress);

    /** does the agent connection terminated so not */
    bool IsFinished()       { return mFinished; }

    /** stops the current running proxy (if running!) */
    void Stop();

private:
    /** manages server connection */
    void ServerConnectionHandler();

    /** manages agent connection */
    void AgentConnectionHandler();

private:
    const int mCycleMillisecs;

    /** shows if the proxy execution is finished */
    bool mFinished;
    boost::shared_ptr<NetMessage> mNetMessage;
    boost::shared_ptr<rcss::net::Socket> mServerSocket;
    boost::shared_ptr<rcss::net::Socket> mAgentSocket;
    boost::shared_ptr<NetBuffer> mAgentBuffer;
    boost::thread mServerConnectionThread;
    boost::thread mAgentConnectionThread;
    boost::mutex mAgentBufferMutex;
};

DECLARE_CLASS(AgentProxy);

} // namespace oxygen

#endif // OXYGEN_AGENTPROXY_H
