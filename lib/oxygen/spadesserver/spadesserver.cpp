/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.2.2.1 2003/12/22 18:05:05 rollmark Exp $

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
#endif

SpadesServer::SpadesServer() :
    zeitgeist::Node(), spades::WorldModel(), mSimEngine(0)
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
    mParamReader->setParam ("run_integrated_commserver", true);
    mParamReader->setParam ("monitor_interval",2);
#endif

    return mParamReader.get();
}

bool
SpadesServer::initialize(SimEngine* pSE)
{
    mSimEngine = pSE;
    return true;
}

spades::SimEngine*
SpadesServer::GetSimEngine()
{
    return mSimEngine;
}


bool
SpadesServer::finalize()
{
    return true;
}

float
SpadesServer::GetTimePerStep() const
{
    float time_per_step = 0.01f;
    GetScript()->GetVariable("Spades.TimePerStep", time_per_step);

    return time_per_step;
}

SimTime
SpadesServer::simToTime(SimTime time_curr, SimTime time_desired)
{
    int steps = time_desired - time_curr;
    if (steps <= 0)
    {
        GetLog()->Warning() << "WARNING: Will not simulate <= 0 steps\n";
        return time_curr;
    }

    shared_ptr<SceneServer> sceneServer =
        shared_static_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
    {
        GetLog()->Warning() << "WARNING: No SceneServer present\n";
        return time_curr;
    }

    float timePerStep = GetTimePerStep();
    int i = steps;

    while (i > 0)
    {
        sceneServer->Update(timePerStep);
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
    return 90;
}

SimTime
SpadesServer::getMinSenseLatency() const
{
    return 10;
}

ActEvent*
SpadesServer::parseAct(SimTime t, AgentID a, const char* data, unsigned datalen) const
{
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
    SimTime arrival = t + static_cast<int>(latency / GetTimePerStep());

    return new SpadesActEvent(arrival, a, actionList);
}

void
SpadesServer::pauseModeCallback()
{
#if THIS_IS_A_DEMO_ONLY
    // start a 'default' agent
    if (mSimEngine->getNumCommServers () < 1)
        {
            GetLog()->Warning() << "(SpadesServer) No comm server yet, waiting\n" << endl;
            return;
        }

    AgentTypeDB::AgentTypeConstIterator
        at = mSimEngine->getAgentTypeDB()->getAgentType("default");

    if (at == mSimEngine->getAgentTypeDB()->nullIterator())
        {
            GetLog()->Error() <<
                "ERROR: (SpadesServer) could not find 'default' agent\n" << endl;
            return;
        }

    AgentID a = mSimEngine->startNewAgent(at);
    if (a == AGENTID_INVALID)
        {
            GetLog()->Error() <<
                "ERROR: (SpadesServer) could not start 'default' agent\n" << endl;
            return;
        }
#endif

    // no time to pause
    mSimEngine->changeSimulationMode(SM_RunNormal);
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

