/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sceneserver.cpp,v 1.3 2003/08/31 21:53:45 fruit Exp $

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

#include "sceneserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/corecontext.h>
#include <salt/vector.h>
#include <salt/frustum.h>
#include "scene.h"
#include "../physicsserver/world.h"
#include "../physicsserver/space.h"
#include "../controlaspect/controlaspect.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

SceneServer::SceneServer() :
    Leaf()
{
}

SceneServer::~SceneServer()
{
}

boost::shared_ptr<Scene> SceneServer::CreateScene(const std::string &location)
{
    shared_ptr<CoreContext> context = GetCore()->CreateContext();
    shared_ptr<Scene> scene = shared_static_cast<Scene>(context->New("oxygen/Scene", location));

    if (scene.get() != NULL)
    {
        mActiveScene = scene;
    }

    return scene;
}

bool SceneServer::SetActiveScene(const std::string &location)
{
    shared_ptr<Scene> scene = shared_static_cast<Scene>(GetCore()->Get(location));

    if (scene.get() != NULL)
    {
        mActiveScene = scene;
        return true;
    }

    return false;
}

/*!
  Update the scene graph. I do not make the distinction between 'controllers'
  and 'scene graph nodes' ... this forces me to perform several update passes
  through the hierarchy. Nevertheless, the more homogenuous structure is worth
  it.

  Example Hierarchy:

  scene
  |-   transform
  |-   body
  |               |-  quakecontroller
  |
  |-   camera

  The following passes are performed:

  1st pass:       PrePhysicsUpdate(deltaTime)
  The first update pass is responsible for updating the internal state of
  an object. For example, the quakecontroller has received input from
  the main application in terms of state changes (StrafeLeft, Forward, etc..).
  As the controller is physics driven, it has to translate these commands
  to actual forces, which act on the body. This is done during this pass.

  [Physics + Collision called]

  2nd pass:       PostPhysicsUpdate()
  This pass can be used to affect other nodes. For example, 'body' has
  undergone physics simulation and collision detection. It has reached its
  final 'simulated' position, which has to be     passed to the parent (transform).

  3rd pass:       UpdateHierarchy()
  As the second pass affects the position of objects, we have to recalculate
  the transformation hierarchy before we can display the scene.
*/

void SceneServer::Update(float deltaTime)
{
    if (deltaTime > 0.0f && mActiveScene)
    {
        // update control aspects
        TLeafList controlAspects;
        TLeafList::iterator i;

        mActiveScene->GetChildrenSupportingClass("ControlAspect", controlAspects);

        for (i = controlAspects.begin(); i != controlAspects.end(); ++i)
        {
            shared_static_cast<ControlAspect>(*i)->PerformKills();
        }


        mActiveScene->PrePhysicsUpdate(deltaTime);

        // determine collisions
        shared_ptr<Space> space = shared_static_cast<Space>(mActiveScene->GetChildOfClass("Space"));
        if (space.get() != NULL)
        {
            space->Collide();
        }

        // do physics
        shared_ptr<World> world = shared_static_cast<World>(mActiveScene->GetChildOfClass("World"));
        if (world.get() != NULL)
        {
            world->Step(deltaTime);
        }

        mActiveScene->PostPhysicsUpdate();

        mActiveScene->UpdateHierarchy();
    }
}
