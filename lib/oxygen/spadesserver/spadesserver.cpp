/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.3.2.4 2004/02/06 21:19:40 fruit Exp $

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
#include <oxygen/monitorserver/monitorserver.h>
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
    // setup script variables used to customize the SpadesServer
    GetScript()->CreateVariable("Spades.TimePerStep", 0.01f);
    GetScript()->CreateVariable("Spades.MonitorInterval", 4);
    GetScript()->CreateVariable("Spades.RunIntegratedCommserver", false);
    GetScript()->CreateVariable("Spades.SendAgentThinkTimes", false);

    return true;
}

void
SpadesServer::OnLink()
{
    mMonitorServer = shared_dynamic_cast<MonitorServer>
        (GetCore()->Get("/sys/server/monitor"));

    if (mMonitorServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SpadesServer) MonitorServer not found.\n";
        }

    mGameControlServer = shared_dynamic_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));

    if (mGameControlServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SpadesServer) GameControlServer not found.\n";
        }

    mSceneServer = shared_dynamic_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SpadesServer) SceneServer not found.\n";
        }
}

void
SpadesServer::OnUnlink()
{
    mMonitorServer.reset();
    mGameControlServer.reset();
    mSceneServer.reset();
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

bool
SpadesServer::GetRunIntegratedCommserver() const
{
    bool run_integrated_commserver = false;
    GetScript()->GetVariable("Spades.RunIntegratedCommserver",
                             run_integrated_commserver);

    return run_integrated_commserver;
}

int
SpadesServer::GetMonitorInterval() const
{
    int monitor_interval = 4;
    GetScript()->GetVariable("Spades.MonitorInterval", monitor_interval);

    return monitor_interval;
}

bool
SpadesServer::GetSendAgentThinkTimes() const
{
    bool send_agent_think_times = false;
    GetScript()->GetVariable("Spades.SendAgentThinkTimes",
                             send_agent_think_times);

    return send_agent_think_times;
}

boost::shared_ptr<GameControlServer>
SpadesServer::GetGameControlServer() const
{
    return mGameControlServer;
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

    // start an inprocess commserver
    mParamReader->setParam("run_integrated_commserver",
                           GetRunIntegratedCommserver());

    // send updates to the monitor every nth cycle
    mParamReader->setParam("monitor_interval", GetMonitorInterval());

    // don't send think time messages to connected agents
    mParamReader->setParam("send_agent_think_times", GetSendAgentThinkTimes());

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
        GetLog()->Warning() << "WARNING: (SpadesServer) will not simulate <= 0 steps\n";
        return time_curr;
    }

    if (
        (mSceneServer.get() == 0) ||
        (mGameControlServer.get() == 0)
        )
        {
            GetLog()->Warning() << "WARNING: (SpadesServer) SceneServer and/or GameControlServer missing.\n";
            return time_curr;
        }

    float timePerStep = GetTimePerStep();
    int i = steps;

    while (i > 0)
    {
        mSceneServer->Update(timePerStep);
        mGameControlServer->Update(timePerStep);
        --i;
    }

    //    GetLog()->Debug() << "(SpadesServer) time_curr=" << time_curr
    //                  << " time_desired=" << time_desired << endl;
    //    GetLog()->Debug() << "updated the scene by " << steps - i << " * "
    //                << timePerStep << " seconds.\n";

    // return the simulation time when the loop stopped
    // (the '- i' makes sense if we exit the while loop earlier)
    return time_desired - i;
}

DataArray
SpadesServer::getMonitorHeaderInfo()
{
    if (mMonitorServer.get() == 0)
        {
            return DataArray();
        }

    return DataArray(mMonitorServer->GetMonitorHeaderInfo());
}

DataArray
SpadesServer::getMonitorInfo(SimTime /*time*/)
{
    if (mMonitorServer.get() == 0)
        {
            return DataArray();
        }

    return DataArray(mMonitorServer->GetMonitorInfo());
}

void
SpadesServer::parseMonitorMessage(const char* data, unsigned datalen)
{
    if (mMonitorServer.get() == 0)
        {
            return;
        }

    return mMonitorServer->ParseMonitorMessage(string(data,datalen));
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
    if (mGameControlServer.get() == 0)
        {
            return 0;
        }

    shared_ptr<ActionObject::TList> actionList
        = mGameControlServer->Parse(a,std::string(data,datalen));

    if (actionList.get() == 0)
        {
            return 0;
        }

    float latency = mGameControlServer->GetActionLatency(a);
    SimTime arrival = mSimEngine->getSimulationTime() + static_cast<int>(latency / GetTimePerStep());

    return new SpadesActEvent(arrival, a, actionList);
}

void
SpadesServer::pauseModeCallback()
{
    // the first time pauseModeCallback will be called is immediatly after startup
    // when SPADES is in SM_PausedInitial mode
    while (! mAgentQueue.empty())
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
        {
            Unpause();
        }
}

bool
SpadesServer::agentConnect(AgentID agent, AgentTypeDB::AgentTypeConstIterator /*at*/)
{
    GetLog()->Normal() <<
        "(SpadesServer) agentConnect " << agent << "\n";

    // try to register the agent to the game control server
    shared_ptr<GameControlServer> gcs = GetGameControlServer();

    if (
        (mGameControlServer.get() == 0) ||
        (! mGameControlServer->AgentConnect(static_cast<int>(agent)))
        )
        {
            return false;
        }

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
    if (mGameControlServer.get() == 0)
        {
            return false;
        }

    return mGameControlServer->AgentDisappear(static_cast<int>(agent));
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

