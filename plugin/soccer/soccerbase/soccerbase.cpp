/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerbase.cpp,v 1.1.2.2 2004/02/06 10:03:07 rollmark Exp $

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
#include <oxygen/physicsserver/body.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/ball/ball.h>

using namespace boost;
using namespace oxygen;

bool
SoccerBase::GetSceneServer(const BaseNode& base,
                           shared_ptr<SceneServer>& scene_server)
{
    scene_server = shared_static_cast<SceneServer>
        (base.GetCore()->Get("/sys/server/scene"));

    if (scene_server.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") scene server not found.\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetTransformParent(const BaseNode& base,
                               shared_ptr<Transform>& transform_parent)
{
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
SoccerBase::GetAgentState(const BaseNode& base,
                          shared_ptr<AgentState>& agent_state)
{
    shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
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
SoccerBase::GetActiveScene(const BaseNode& base,
                           shared_ptr<Scene>& active_scene)
{
    shared_ptr<SceneServer> sceneServer;
    if (! GetSceneServer(base,sceneServer))
        {
            return false;
        }

    active_scene = sceneServer->GetActiveScene();
    if (active_scene.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") SceneServer reports no active scene\n";
        return false;
    }
    return true;
}

bool
SoccerBase::GetBody(const BaseNode& base, shared_ptr<Body>& body)
{
    shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
        return false;
    }

  body = shared_dynamic_cast<Body>(parent->GetChildOfClass("Body"));

  if (body.get() == 0)
    {
      base.GetLog()->Error()
          << "ERROR: (SoccerBase: " << base.GetName()
          << ") parent node has no Body child.";
      return false;
    }

  return true;
}

bool
SoccerBase::GetBall(const BaseNode& base, shared_ptr<Ball>& ball)
{
    shared_ptr<Scene> scene;
    if (! GetActiveScene(base,scene))
        {
            return false;
        }

    ball = shared_dynamic_cast<Ball>
        (base.GetCore()->Get(scene->GetFullPath() + "Ball"));

    if (ball.get() == 0)
        {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase: " << base.GetName()
                << " found no ball node\n";
            return false;
        }

    return true;
}

bool
SoccerBase::GetBallBody(const BaseNode& base, shared_ptr<Body>& body)
{
    shared_ptr<Scene> scene;
    if (! GetActiveScene(base,scene))
        {
            return false;
        }

   body = shared_dynamic_cast<Body>
        (base.GetCore()->Get(scene->GetFullPath() + "Ball/physics"));

   if (body.get() == 0)
       {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase: " << base.GetName()
                << " found no ball body node\n";
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
