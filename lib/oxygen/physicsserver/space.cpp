/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space.cpp,v 1.4.8.4 2004/01/11 11:27:07 rollmark Exp $

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

#include "space.h"
#include "world.h"
#include "collider.h"
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;

static void collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2)
{
    Space *space = (Space*)data;

    space->HandleCollide(obj1, obj2);
}

Space::Space() :
    mODESpace(0), mODEContactGroup(0), mWorld(0)
{
}

Space::~Space()
{
    if (mODEContactGroup)
      {
        dJointGroupDestroy(mODEContactGroup);
      }

    // release the ODE space
    if (mODESpace)
      {
        dSpaceDestroy(mODESpace);
      }
}

dSpaceID Space::GetODESpace() const
{
  return mODESpace;
}

dJointGroupID Space::GetODEJointGroup() const
{
  return mODEContactGroup;
}

void Space::Collide()
{
    // bind collision callback function to this object
    dSpaceCollide(mODESpace, this, collisionNearCallback);
}

void Space::HandleCollide(dGeomID obj1, dGeomID obj2)
{
    // return immediately if the two bodies corresponding to the
    // geoms are connected by a joint
    dBodyID ODEBody1 = dGeomGetBody(obj1);
    dBodyID ODEBody2 = dGeomGetBody(obj2);

    if (
        (ODEBody1) &&
        (ODEBody2) &&
        (dAreConnected (ODEBody1, ODEBody2))
        )
      {
        return;
      }

    // dSpaceCollide(), is guaranteed to pass all intersecting geom
    // pairs to the callback function, but depending on the internal
    // algorithms used by the space it may also make mistakes and pass
    // non-intersecting pairs. Thus we can not expect that dCollide()
    // will return contacts for every pair passed to the callback.
    dContact contact;
    if (! dCollide (obj1, obj2, 0, &contact.geom, sizeof(dContactGeom)))
    {
      return;
    }

    // lookup and notify the collider node
    Collider* collider = Collider::GetCollider(obj1);
    if (collider)
      {
        collider->OnCollision(obj2, contact);
      }
}

bool Space::ConstructInternal()
{
    if (! ODEObject::ConstructInternal())
      {
        return false;
      }

    // create the ode space, 0 indicates that this space should
    // not be inserted into another space, i.e. we always create a
    // toplevel space object
    mODESpace = dHashSpaceCreate(0);
    if (mODESpace == 0)
    {
        return false;
    }

    // create a joint group for the contacts
    mODEContactGroup = dJointGroupCreate(0);
    return (mODEContactGroup != 0);
}

void Space::OnLink()
{
    shared_ptr<Scene> scene = GetScene();

    if (scene.get() != NULL)
    {
        shared_ptr<World> world = shared_static_cast<World>(scene->GetChildOfClass("World"));
        if (world)
          {
            mWorld = world->GetODEWorld();
          }
        else
          {
            GetLog()->Error() << "ERROR: Could not find a World to link to..." << std::endl;
          }
    }
}

void Space::OnUnlink()
{
    mWorld = 0;
}

void Space::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}

