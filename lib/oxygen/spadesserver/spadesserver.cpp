/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.1.2.9.2.5 2003/12/02 17:19:58 rollmark Exp $

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
#include <spades/SimEngine.hpp>
#include "spadescreatesenseevent.h"

#if THIS_IS_A_DEMO_ONLY
#include <oxygen/sceneserver/basenode.h>
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
SpadesServer::GetTimePerStep()
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

#if THIS_IS_A_DEMO_ONLY
      // test the loop
      // print the current location of the sphere collider
      shared_ptr<BaseNode> sphereNode =
        shared_static_cast<BaseNode>(GetCore()->Get("/usr/scene/sphere"));

      if (sphereNode.get() != 0)
      {
          const salt::Matrix& transform = sphereNode->GetWorldTransform();
          const salt::Vector3f& pos = transform.Pos();
          std::cout << "found the sphereNode at " << pos[0] << "," << pos[1] << "," << pos[2] << std::endl;
      }
#endif

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
    GetLog()->Debug() << "updated the scene by " << steps - i << " * "
                      << timePerStep << " seconds.\n";

#endif

    // return the simulation time when loop stopped
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
SpadesServer::GetGameControlServer()
{
    return shared_static_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));
}

DataArray
SpadesServer::getMonitorHeaderInfo()
{
    shared_ptr<MonitorServer> monitorServer =
        GetMonitorServer();

    if (monitorServer != NULL)
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

    if (monitorServer != NULL)
        {
            return DataArray(monitorServer->GetMonitorInfo());
        } else
            {
                return DataArray();
            }
}

void
SpadesServer::parseMonitorMessage (const char* data, unsigned datalen)
{
    shared_ptr<MonitorServer> monitorServer =
        GetMonitorServer();

    if (monitorServer != NULL)
        {
            return monitorServer->ParseMonitorMessage(string(data,datalen));
        }
}

SimTime
SpadesServer::getMinActionLatency() const
{
    return 0;
}

SimTime
SpadesServer::getMinSenseLatency() const
{
    return 0;
}

ActEvent*
SpadesServer::parseAct(SimTime t, AgentID a, const char* data, unsigned datalen) const
{
    return 0;
}

void
SpadesServer::pauseModeCallback()
{
    // no time to pause
    mSimEngine->changeSimulationMode(SM_RunNormal);
}

bool
SpadesServer::agentConnect(AgentID agent, AgentTypeDB::AgentTypeConstIterator /*at*/)
{
    // try to register the agent to the game control server
    shared_ptr<GameControlServer> gcs = GetGameControlServer();

    if (
        (gcs.get() == NULL) ||
        (! gcs->AgentConnect(static_cast<int>(agent)))
        )
        {
            return false;
        }

    //
    // Should we also us an InitSenseEvent as the spades sample does ?
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
    if (gcs.get() == NULL)
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

