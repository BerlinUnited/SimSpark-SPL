/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004-2008 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmeshiniteffector.cpp 181 2010-02-28 10:55:34Z marianbuchta $

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
#include "staticmeshiniteffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/physicsserver/body.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <kerosin/sceneserver/staticmesh.h>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace kerosin;

StaticMeshInitEffector::StaticMeshInitEffector() 
    : InitEffector()
{
}

StaticMeshInitEffector::~StaticMeshInitEffector()
{
}

void
StaticMeshInitEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    if ( ( mAction.get() == 0 ) ||
         (mGameState.get() == 0) ||
         (mAgentAspect.get() == 0)
        )
    {
        return;
    }
    InitEffector::PrePhysicsUpdateInternal(deltaTime);
    
    // search for the AgentState
    boost::shared_ptr<AgentState> state = shared_static_cast<AgentState>
        (mAgentAspect->GetChildOfClass("AgentState",true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (StaticMeshInitEffector) cannot find AgentState\n";

        return;
    }

    TTeamIndex team = state->GetTeamIndex();
    string teamMat = "matTeam";
    if (team == TI_LEFT)
    {
        teamMat = "matLeft";
    } else if (team == TI_RIGHT)
    {
        teamMat = "matRight";
    } else {
        GetLog()->Error() << "ERROR: (StaticMeshInitEffector) Found no team\n";
        return;
    }
    
    int unum = state->GetUniformNumber();
    string unumMat = "matNum" + lexical_cast<string>(unum);
    
    // get parent of the agent aspect
    boost::shared_ptr<Node> parent = shared_dynamic_cast<Node>
        (mAgentAspect->GetParent().lock());

    if (parent.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (SingleMatInitEffector) cannot find parent of agent aspect\n";
            return;
        }

    // build list of meshes, searching recursively
    TLeafList meshes;
    parent->ListChildrenSupportingClass<StaticMesh>(meshes,true);

    // change the meshes for new team material
    for (
        TLeafList::iterator iter = meshes.begin();
        iter != meshes.end();
        ++iter
        )
    {
        boost::shared_ptr<StaticMesh> m = shared_static_cast<StaticMesh>(*iter);
        m->ChangeMaterial("matTeam",teamMat);
        m->ChangeMaterial("matNum",unumMat);
    }
    
    // set the scene modified, the monitor will update
    boost::shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() ==0)
    {
        GetLog()->Error()
            << "(SingleMatInitEffector) ERROR: SceneServer not found\n";
        return;
    }

    boost::shared_ptr<Scene> scene = sceneServer->GetActiveScene();
    if (scene.get() == 0)
    {
        GetLog()->Error()
            << "(SingleMatInitEffector) ERROR: Scene not found\n";
        return;
    }
    scene->SetModified(true);
}

