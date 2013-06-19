/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

void
CollisionHandler::OnLink()
{
    UpdateCached();
}

void
CollisionHandler::ResetCache()
{
    mCollider.reset();
    mWorld.reset();
    mSpace.reset();
}

void
CollisionHandler::UpdateCached()
{
    // setup the scene, world, space and collider references
    boost::shared_ptr<Scene> scene = GetScene();

    ResetCache();
    if (scene.get() == 0)
        {
            return;
        }

    mWorld = static_pointer_cast<World>(scene->GetChildOfClass("World"));
    if (mWorld.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no World node\n";
        }

    mSpace = static_pointer_cast<Space>(scene->GetChildOfClass("Space"));
    if (mSpace.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no Space node\n";
        }

    mCollider = static_pointer_cast<Collider>(GetParent().lock());
    if (mCollider.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no parent Collider node\n";
        }
}

void
CollisionHandler::OnUnlink()
{
    BaseNode::OnUnlink();
    ResetCache();
}
