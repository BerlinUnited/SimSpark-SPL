/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerbase.cpp,v 1.1.2.1 2004/02/05 10:09:03 fruit Exp $

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
#include "soccerbase.h"

using namespace boost;
using namespace oxygen;

bool
SoccerBase::GetSceneServer(const oxygen::BaseNode& base,
                           boost::shared_ptr<oxygen::SceneServer>& scene_server)
{
    scene_server = shared_static_cast<SceneServer>
        (base.GetCore()->Get("/sys/server/scene"));
    return true;
}


bool
SoccerBase::GetTransformParent(const oxygen::BaseNode& base,
                               boost::shared_ptr<oxygen::Transform>& transform_parent)
{
    // we want positions relative to the closest parent transform node
    transform_parent = shared_dynamic_cast<Transform>
        (make_shared(base.GetParentSupportingClass("Transform")));

    if (transform_parent.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") parent node is not derived from TransformNode\n";
        return false;
    }
    return true;
}


bool
SoccerBase::GetAgentState(const oxygen::BaseNode& base,
                          boost::shared_ptr<oxygen::Transform> parent,
                          boost::shared_ptr<AgentState>& agent_state)
{
    if (parent.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") cannot get AgentState with no parent node\n";
        return false;
    }

    agent_state =
        shared_dynamic_cast<AgentState>(parent->GetChild("AgentState"));

    if (agent_state.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") parent node has no AgentState child\n";
        return false;
    }
    return true;
}


bool
SoccerBase::GetActiveScene(const oxygen::BaseNode& base,
                           boost::shared_ptr<oxygen::SceneServer> scene_server,
                           boost::shared_ptr<oxygen::Scene>& active_scene)
{
    if (scene_server.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") no SceneServer to get active scene from\n";
        return false;
    }

    active_scene = scene_server->GetActiveScene();
    if (active_scene.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") SceneServer reports no active scene\n";
        return false;
    }
    return true;
}

salt::Vector3f
SoccerBase::FlipView(const salt::Vector3f& pos, TTeamIndex ti)
{
    salt::Vector3f newPos;
    switch (ti) {
    case TI_NONE:
        newPos[0] = 0.0;
        newPos[1] = 0.0;
        newPos[2] = 0.0;
        break;
    case TI_LEFT:
        newPos[0] = -pos[0];
        newPos[1] = pos[1];
        newPos[2] = -pos[2];
        break;
    case TI_RIGHT:
        newPos = pos;
        break;
    }
    return newPos;
}
