/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.1.2.4 2003/11/17 16:18:48 fruit Exp $

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
#include "paramreader.h"

using namespace boost;
using namespace oxygen;
using namespace spades;

#define THIS_IS_A_DEMO_ONLY 1

#include <zeitgeist/corecontext.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <spades/SimEngine.hpp>

#if THIS_IS_A_DEMO_ONLY
#include <oxygen/sceneserver/basenode.h>
#endif

SpadesServer::SpadesServer() :
    zeitgeist::Leaf(), spades::WorldModel(), mSimEngine(0)
{
}

SpadesServer::~SpadesServer()
{
}

// methods for our framework

bool
SpadesServer::Init(const std::string& paramReaderName)
{
    // create the parameter reader
    mParamReader =
        shared_static_cast<ParamReader>(GetCore()->New(paramReaderName));

    if (!mParamReader)
    {
        // could not create parameter reader
        GetLog()->Error() << "ERROR: Unable to create "
                          << paramReaderName << "\n";
        return false;
    }

    AddChildReference(mParamReader);

    // initialize the parameter reader
    if (!mParamReader->Init())
    {
        // initializing the parameter reader failed
        GetLog()->Error() << "ERROR: Could not init "
                          << paramReaderName << "\n";
        return false;
    }

    return true;
}

// SPADES interface methods

EngineParam*
SpadesServer::parseParameters(int argc, const char *const *argv)
{
    // SimulationEngineMain uses the pointer we have to return to get the
    // command line options. It doesn't delete the ParamReader, so we can
    // return a simple pointer (otherwise our shared_ptr would become invalid)
    mParamReader->getOptions(argc, argv);

    spades::EngineParam* ep =
        shared_dynamic_cast<spades::EngineParam>(mParamReader).get();

    return ep;
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
    float time_per_step = 0.01f;
    int steps = time_desired - time_curr;

    if (steps <= 0)
    {
        GetLog()->Warning() << "WARNING: Will not simulate <= 0 steps\n";
        return time_curr;
    }


    shared_ptr<oxygen::SceneServer> sceneServer =
        shared_static_cast<oxygen::SceneServer>(GetCore()->Get("/sys/server/scene"));

#if THIS_IS_A_DEMO_ONLY
      // test the loop
      // print the current location of the sphere collider
      shared_ptr<oxygen::BaseNode> sphereNode =
        shared_static_cast<oxygen::BaseNode>(GetCore()->Get("/usr/scene/sphere"));

      if (sphereNode.get() != 0)
      {
          const salt::Matrix& transform = sphereNode->GetWorldTransform();
          const salt::Vector3f& pos = transform.Pos();
          std::cout << "found the sphereNode at " << pos[0] << "," << pos[1] << "," << pos[2] << std::endl;
      }
#endif

    if (sceneServer.get() != 0)
    {
        sceneServer->Update(time_per_step * steps);
        GetLog()->Debug() << "updated the scene by " << steps*time_per_step << " seconds.\n";
        return time_desired;
    } else {
        GetLog()->Warning() << "WARNING: No SceneServer present\n";
        return time_curr;
    }
}

DataArray
SpadesServer::getMonitorHeaderInfo()
{
    return DataArray();
}

DataArray
SpadesServer::getMonitorInfo(SimTime time)
{
    return DataArray();
}

void
SpadesServer::parseMonitorMessage (const char* data, unsigned datalen)
{
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
SpadesServer::agentConnect(AgentID agent, AgentTypeDB::AgentTypeConstIterator at)
{
    return false;
}

bool
SpadesServer::agentDisappear(AgentID agent, AgentLostReason reason)
{
    return false;
}

void
SpadesServer::notifyCommserverConnect(ServerID s)
{
}

void
SpadesServer::notifyCommserverDisconnect(ServerID s)
{
}

