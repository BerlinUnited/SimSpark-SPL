/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.3.2.1 2004/01/08 12:39:37 rollmark Exp $

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
#include "spadesserver.h"

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace spades;
using namespace std;

#define THIS_IS_A_DEMO_ONLY 1

#include <zeitgeist/corecontext.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <spades/SimEngine.hpp>
#include "spadescreatesenseevent.h"
#include "spadesactevent.h"


#if THIS_IS_A_DEMO_ONLY
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/sceneserver/scene.h>
#endif

SpadesServer::SpadesServer() :
    zeitgeist::Node(), spades::WorldModel(),
    mSimEngine(0), mSimulationModeChanged(false)
{
}

SpadesServer::~SpadesServer()
{
}

bool
SpadesServer::ConstructInternal()
{
    // setup script variables used to customzize the SpadesServer
    GetScript()->CreateVariable("Spades.TimePerStep", 0.01f);

    return true;
}


spades::SimEngine*
SpadesServer::GetSimEngine()
{
    return mSimEngine;
}

float
SpadesServer::GetTimePerStep() const
{
    float time_per_step = 0.01f;
    GetScript()->GetVariable("Spades.TimePerStep", time_per_step);

    return time_per_step;
}

boost::shared_ptr<MonitorServer>
SpadesServer::GetMonitorServer()
{
    return shared_static_cast<MonitorServer>
        (
         GetCore()->Get("/sys/server/monitor")
         );
}

boost::shared_ptr<GameControlServer>
SpadesServer::GetGameControlServer() const
{
    return shared_dynamic_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));
}

void
SpadesServer::StartAgents(const AgentItem& ai)
{
    std::cerr << "SpadesServer::StartAgents(" << ai.mAgentType << ", " << ai.mNumber << ")\n";

    if (!mSimEngine || mSimEngine->getNumCommServers () < 1)
    {
        GetLog()->Error() << "(SpadesServer) No simulation engine or comm server, "
                          << "cannot start agents\n" << endl;
        return;
    }

    AgentTypeDB::AgentTypeConstIterator
        at = mSimEngine->getAgentTypeDB()->getAgentType(ai.mAgentType);

    if (at == mSimEngine->getAgentTypeDB()->nullIterator())
    {
        GetLog()->Error()
            << "ERROR: (SpadesServer) could not find agent type "
            << ai.mAgentType << endl;
        return;
    }

    int num = std::max(ai.mNumber, 0);

    while (num > 0)
    {
        if (mSimEngine->startNewAgent(at) == AGENTID_INVALID)
        {
            num = 0;
            GetLog()->Error()
                << "ERROR: (SpadesServer) starting agent of type " << ai.mAgentType
                << " failed" << endl;
        }
        --num;
    }
}

void
SpadesServer::Unpause()
{
    mNewSimulationMode = SM_RunNormal;
    mSimulationModeChanged = true;
}

void
SpadesServer::QueueAgents(const std::string& agentType, int num)
{
    mAgentQueue.push(AgentItem(agentType, num));
}

// ----------------------------------------------------------------------
// SPADES interface methods
EngineParam*
SpadesServer::parseParameters(int argc, const char *const *argv)
{
    mParamReader =
        shared_static_cast<ParamReader>(GetCore()->New("oxygen/ParamReader"));

    // SimulationEngineMain uses the ParamReader we have to get the
    // command line options. It doesn't delete the ParamReader, so we
    // can return a simple pointer
    mParamReader->getOptions(argc, argv);

#if THIS_IS_A_DEMO_ONLY
    // start an inprocess commserver
    mParamReader->setParam ("run_integrated_commserver", true);

    // send updates to the monitor every second cycle
    mParamReader->setParam ("monitor_interval",2);

    // don't send think time messages to connected agents
    mParamReader->setParam ("send_agent_think_times",false);
#endif

    return mParamReader.get();
}

bool
SpadesServer::initialize(SimEngine* pSE)
{
    mSimEngine = pSE;
    return true;
}

bool
SpadesServer::finalize()
{
    return true;
}

SimTime
SpadesServer::simToTime(SimTime time_curr, SimTime time_desired)
{
    int steps = time_desired - time_curr;
    if (steps <= 0)
    {
        GetLog()->Warning() << "WARNING: (SpadesServer) Will not simulate <= 0 steps\n";
        return time_curr;
    }

    shared_ptr<SceneServer> sceneServer =
        shared_static_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
    {
        GetLog()->Warning() << "WARNING: (SpadesServer) No SceneServer present\n";
        return time_curr;
    }

    shared_ptr<GameControlServer> gcs = GetGameControlServer();
    if (gcs.get() == 0)
    {
        GetLog()->Warning() << "WARNING: (SpadesServer) No GameControlServer present\n";
    }

    float timePerStep = GetTimePerStep();
    int i = steps;

    while (i > 0)
    {
        sceneServer->Update(timePerStep);
        gcs->Update(timePerStep);
        --i;
    }

#if THIS_IS_A_DEMO_ONLY
    //    GetLog()->Debug() << "(SpadesServer) time_curr=" << time_curr
    //                  << " time_desired=" << time_desired << endl;
    //    GetLog()->Debug() << "updated the scene by " << steps - i << " * "
    //                << timePerStep << " seconds.\n";

#endif

    // return the simulation time when the loop stopped
    // (the '- i' makes sense if we exit the while loop earlier)
    return time_desired - i;
}

DataArray
SpadesServer::getMonitorHeaderInfo()
{
    shared_ptr<MonitorServer> monitorServer =
        GetMonitorServer();

    if (monitorServer != 0)
        {
            return DataArray(monitorServer->GetMonitorHeaderInfo());
        } else
            {
                return DataArray();
            }
}

DataArray
SpadesServer::getMonitorInfo(SimTime /*time*/)
{
    shared_ptr<MonitorServer> monitorServer =
        GetMonitorServer();

    if (monitorServer != 0)
        {
            return DataArray(monitorServer->GetMonitorInfo());
        } else
            {
                return DataArray();
            }
}

void
SpadesServer::parseMonitorMessage(const char* data, unsigned datalen)
{
    shared_ptr<MonitorServer> monitorServer =
        GetMonitorServer();

    if (monitorServer != 0)
        {
            return monitorServer->ParseMonitorMessage(string(data,datalen));
        }
}

SimTime
SpadesServer::getMinActionLatency() const
{
    return 1;
}

SimTime
SpadesServer::getMinSenseLatency() const
{
    return 1;
}

ActEvent*
SpadesServer::parseAct(SimTime /*t*/, AgentID a, const char* data, unsigned datalen) const
{
    GetLog()->Normal() << "(SpadesServer) Agent " << a << " sent " << data << endl;

    const shared_ptr<GameControlServer> gcs = GetGameControlServer();
    if (gcs.get() == 0)
        {
            return 0;
        }

    shared_ptr<ActionObject::TList> actionList
        = gcs->Parse(a,std::string(data,datalen));

    if (actionList.get() == 0)
        {
            return 0;
        }

    float latency = gcs->GetActionLatency(a);
    SimTime arrival = mSimEngine->getSimulationTime() + static_cast<int>(latency / GetTimePerStep());

    return new SpadesActEvent(arrival, a, actionList);
}

void
SpadesServer::pauseModeCallback()
{
    // the first time pauseModeCallback will be called is immediatly after startup
    // when SPADES is in SM_PausedInitial mode
    while (!mAgentQueue.empty())
    {
        StartAgents(mAgentQueue.front());
        mAgentQueue.pop();
    }
    if (mSimulationModeChanged)
    {
        mSimEngine->changeSimulationMode(mNewSimulationMode);
        mSimulationModeChanged = false;
    }
    if (mSimEngine->getSimulationMode() == SM_PausedInitial)
        Unpause();
}

bool
SpadesServer::agentConnect(AgentID agent, AgentTypeDB::AgentTypeConstIterator /*at*/)
{
    GetLog()->Debug() <<
        "(SpadesServer) agentConnect " << agent << "\n";

    // try to register the agent to the game control server
    shared_ptr<GameControlServer> gcs = GetGameControlServer();

    if (
        (gcs.get() == 0) ||
        (! gcs->AgentConnect(static_cast<int>(agent)))
        )
        {
            return false;
        }

    //
    // Should we also use an InitSenseEvent as the spades sample does ?
    //

    // schedule the first SpadesCreateSenseEvent for the following
    // simulation step
    SpadesCreateSenseEvent* event = new SpadesCreateSenseEvent
        (mSimEngine->getSimulationTime() + 1, agent);

    mSimEngine->enqueueEvent(event);

    return true;
}

bool
SpadesServer::agentDisappear(AgentID agent, AgentLostReason /*reason*/)
{
    shared_ptr<GameControlServer> gcs = GetGameControlServer();
    if (gcs.get() == 0)
        {
            return false;
        }

    return gcs->AgentDisappear(static_cast<int>(agent));
}

void
SpadesServer::notifyCommserverConnect(ServerID /*s*/)
{
}

void
SpadesServer::notifyCommserverDisconnect(ServerID /*s*/
)
{
}

