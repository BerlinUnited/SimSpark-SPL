/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.h,v 1.1.2.6.2.3 2003/12/02 16:52:36 rollmark Exp $

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
#ifndef OXYGEN_SPADESSERVER_H
#define OXYGEN_SPADESSERVER_H

#include <spades/enginemain.hpp>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include <oxygen/monitorserver/monitorserver.h>
#include "paramreader.h"

namespace oxygen
{

class GameControlServer;

/*! The SpadesServer serves as an interface between the SceneServer and the agents
 */
class SpadesServer :
        public zeitgeist::Node,
        public spades::WorldModel
{
public:
    SpadesServer();
    ~SpadesServer();

    // Methods starting with small letters are SPADES interface methods.
    // Methods with a initial capital letter are additional methods needed
    // for the zeitgeist framework.

    /** called after the object has been created and attached to a
        core to allow for object dependent internal construction */
    virtual bool ConstructInternal();

    /** helper function to locate the game control server */
    boost::shared_ptr<GameControlServer> GetGameControlServer();

    /** returns the amount of time in seconds a single simulation step
        corresponds to
    */
    float GetTimePerStep();

    // SPADES interface methods start here

    /** You probably want to inherit some parameters from EngineParam
       and use that to parse the commandline. See the ParamReader
       class. */
    spades::EngineParam* parseParameters(int argc, const char* const *argv);

    /** Most of the real initialization/finalization work should be
     * done here.  We do this so that the SimEngine can initialize
     * after this class is created.
     *
     * @param pSE a pointer to the spades simulation engine
     * @return true
     */
    bool initialize(spades::SimEngine* pSE);

    /** returns a pointer to the spades simulation engine */
    spades::SimEngine* GetSimEngine();

    /** @return true */
    bool finalize();

    /** This function should advance the world forward. Events can be
     *  enqueed from this function. The time does *not* have to
     *  advance all the way to the time_desired, and if you put events
     *  in the queue, you should not advance past the time of those
     *  events. The simulation time here is an integer type, so the
     *  SpadesServer has to know how big a simulation step is.
     *
     *  @param time_curr the current simulation step
     *  @param time_desired the desired simulation step
     *  @return a new simulation time t (time_curr <= t <= time_desired)
     */
    spades::SimTime
    simToTime(spades::SimTime time_curr, spades::SimTime time_desired);

    /**  Monitors are programs that can open up a connection to the
     *  engine and get periodic updates about the state of the
     *  world. These next three functions support this.
     */

    /** This function is called once for every monitor. It should
     *  return any header/setup information that is needed.
     */
    spades::DataArray getMonitorHeaderInfo();

    /** This function will be called periodically to get information
     * about the current state of the world. The format is completely
     * determined by what the monitors will expect; no processing is
     * done by the simulation engine
     */
    spades::DataArray getMonitorInfo(spades::SimTime time);

    /** If a monitor sends information to the world model, this
     * function is called to process it. Note that only the data
     * section of the message (not the ID part which indicates that it
     * is a message for the world model and not the simulation engine)
     * is included here. If you need to keep the data, you must copy
     * it
     */
    void parseMonitorMessage(const char* data, unsigned datalen);

    /** There is a latency (in simulation time) between when an action
     * is sent by the agent and when it takes effect. This function
     * needs to return the minimum of all possible values of that
     * latency. This is used to reason about causality
     */
    spades::SimTime getMinActionLatency() const;

    /** Similar to getMinActionLatency, but the latency between when
     *  the sensation is generated from the world and when it is
     *  received by the agent
     */
    spades::SimTime getMinSenseLatency() const;

    /** This function parses the action string received from the
     *  agents Note that this method is const! parsing the act can
     *  *not* affect the world at all, since this could then violate
     *  causality.  Also, at some point this functionality may be
     *  moved to the communication server (which manages communication
     *  with the agents)
     */
    spades::ActEvent*
    parseAct(spades::SimTime t, spades::AgentID a, const char* data, unsigned datalen) const;

    /** When the simulation is paused, this is called peridically At
     *  some point, this function should unpause the simulation The
     *  simulation starts out in paused mode
     */
    void pauseModeCallback();

    /** The world model initiates startup requests for agents. This
     *  function is called once the agent has been started
     *  successfully
     */
    bool agentConnect(spades::AgentID agent, spades::AgentTypeDB::AgentTypeConstIterator at);

    /** Any time an agent leaves the simulation (whether by crash,
     *  graceful exit request, etc.), this function is called so that
     *  the world model can update it's data structures
     */
    bool agentDisappear(spades::AgentID agent, spades::AgentLostReason reason);

    /** Called every time a comm server connects. Usually, this can be
     *  ignored as the WorldModel does not need to know when and how
     *  many commservers connect, unless it wants to do it's own load
     *  balancing
     */
    void notifyCommserverConnect(spades::ServerID s);

    /** Called every time a comm server dsiconnects. Usually, this can
     *  be ignored as the WorldModel does not need to know when and
     *  how many commservers connect, unless it wants to do it's own
     *  load balancing
     */
    void notifyCommserverDisconnect(spades::ServerID s);

protected:
    /** helper function to locate the monitor server */
    boost::shared_ptr<MonitorServer> GetMonitorServer();

private:
    /** the Spades simulation engine */
    spades::SimEngine* mSimEngine;

    /** our commandline parser */
    boost::shared_ptr<ParamReader> mParamReader;
};

DECLARE_CLASS(SpadesServer);

} // namespace oxygen

#endif // OXYGEN_SPADESSERVER_H
