/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "singlematiniteffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
//#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/physicsserver/body.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
//#include <soccer/gamestateaspect/gamestateaspect.h>
#include <kerosin/sceneserver/singlematnode.h>
#include <sstream>
#include <string>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace kerosin;

SingleMatInitEffector::SingleMatInitEffector() 
    : InitEffector()
{
}

SingleMatInitEffector::~SingleMatInitEffector()
{
}

void
SingleMatInitEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    if ( ( mAction.get() == 0 ) ||
         (mGameState.get() == 0) ||
         (mAgentAspect.get() == 0)
        )
    {
        return;
    }
    InitEffector::PrePhysicsUpdateInternal(deltaTime);
    
    // body parts that should be colored in team colors
    std::vector<std::string> jersey;
    jersey.push_back("body");
    jersey.push_back("leftshoulder");
    jersey.push_back("rightshoulder");
    jersey.push_back("leftshank");
    jersey.push_back("rightshank");
    jersey.push_back("lowerTorso");
    
    // search for the AgentState
    boost::shared_ptr<AgentState> state = shared_static_cast<AgentState>
        (mAgentAspect->GetChildOfClass("AgentState",true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SingleMatInitEffector) cannot find AgentState\n";

        return;
    }

    TTeamIndex team = state->GetTeamIndex();
 
    std::ostringstream unumMat;
    std::string jerseyMaterial;
    
    
    if (team == TI_LEFT)
    {
        jerseyMaterial = "matLeft";
    } else if (team == TI_RIGHT)
    {
        jerseyMaterial = "matRight";
    } else {
        GetLog()->Error() << "ERROR: (SingleMatInitEffector) Found no team\n";
        return;
    }
    
    int unum = state->GetUniformNumber();
    
    unumMat << jerseyMaterial << unum;


    // get parent of the agent aspect
    boost::shared_ptr<Node> parent = shared_dynamic_cast<Node>
        (mAgentAspect->GetParent().lock());

    if (parent.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (SingleMatInitEffector) cannot find parent of agent aspect\n";
            
            return;
        }

    boost::shared_ptr<SingleMatNode> matNode;

    std::vector<std::string>::const_iterator it;

    for (it = jersey.begin(); it != jersey.end(); ++it)
    {
        boost::shared_ptr<Leaf> child = parent->GetChild((*it),true);

        if (child.get() != 0)
        {
            matNode = shared_static_cast<SingleMatNode>
                (child->FindChildSupportingClass<SingleMatNode>(true));

            if (matNode.get() == 0)
            {
                GetLog()->Error()
                    << "ERROR: (SingleMatInitEffector) cannot find SingleMatNode of " 
                    << (*it) << "\n";
                return;
            }
        }
        if ((*it) == "body")
        {
            GetLog()->Debug()<<"SingleMatInitEffector Body "<<unumMat.str()<<std::endl;
            matNode->SetMaterial(unumMat.str()); 
        } 
        else 
        {
            matNode->SetMaterial(jerseyMaterial); 
        }
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

