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
#ifndef OXYGEN_AGENTCONTROL_H
#define OXYGEN_AGENTCONTROL_H

#include "netcontrol.h"
#include <oxygen/oxygen_defines.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#ifndef Q_MOC_RUN
#include <boost/thread/barrier.hpp>
#endif

namespace oxygen
{
/** \class AgentControl is a NetControl node that manages the
    communication with agents in cooperation with the
    GameControlServer.
 */

class OXYGEN_API AgentControl : public NetControl
{
public:
    AgentControl();
    virtual ~AgentControl();

    /** called when a new client connects */
    virtual void ClientConnect(boost::shared_ptr<Client> client);

    /** called when a client disconnects */
    virtual void ClientDisconnect(boost::shared_ptr<Client> client);

    /** forwards all pending messages from all connected agents to the
        GameControlServer */
    virtual void StartCycle();

    /** sends sense updates to all connected agents */
    virtual void SenseAgent();

    /** generates sense updates for all connected agents */
    virtual void EndCycle();

    /** sets the AgentControl's sync mode */
    void SetSyncMode(bool syncMode);

    /** sets the AgentControl's sync mode */
    void SetMultiThreaded(bool multiThreaded);

protected:
    virtual void OnLink();

    /** returns if the agents are synced with the srever */
    bool AgentsAreSynced();

    /** the thread function which does EndCycle for one agent in
     *  multi-threaded mode. */
    void AgentThread(const boost::shared_ptr<Client> &client);

    /** forwards all pending messages from a specific agent to the
        GameControlServer */
    void StartCycle(const boost::shared_ptr<Client> &client,
                    boost::shared_ptr<NetBuffer> &netBuff);

    /** generates sense updates for a specific agent */
    void EndCycle(const boost::shared_ptr<Client> &client);

    /** waits for all agent threads to catch up */
    void WaitMaster();

    /** called in an agent thread to wait for the master thread to signal
     * a new task */
    void WaitSlave(boost::barrier* &currentBarrier);

protected:
    /** cached reference to the GameControlServer */
    CachedPath<GameControlServer> mGameControlServer;

    /** stores sense information to be sent to the clients after receiving
     * their commands
     */
    std::vector<std::string> mClientSenses;

    /** indicates if we should wait for the agents to let the simulation
     * proceed to the next cycle
     */
    bool mSyncMode;

    /** indicates if the AgentControl runs in multi-threads */
    bool mMultiThreads;

    /** barrier object for synchronizing threads in multi-threaded mode */
    boost::barrier *mThreadBarrier;
    boost::barrier *mThreadBarrierNew;

    /** boost thread group to create a new thread when an agent connects  */
    boost::thread_group mThreadGroup;
    int nThreads;

    /** indicates what should happen in the agent thread right now */
    enum { STARTCYCLE, SENSEAGENT, ENDCYCLE } mThreadAction;
};

DECLARE_CLASS(AgentControl);

} // namespace oxygen

#endif // OXYGEN_AGENTCONTROL_H
