/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space.cpp,v 1.4.8.5 2004/01/12 18:16:57 rollmark Exp $

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

Space::Space() : mODESpace(0), mODEContactGroup(0)
{
}

Space::~Space()
{
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

void Space::OnLink()
{
  // create the ode space, 0 indicates that this space should
  // not be inserted into another space, i.e. we always create a
  // toplevel space object
  mODESpace = dHashSpaceCreate(0);

  if (mODESpace == 0)
    {
      return;
    }

  // create a joint group for the contacts
  mODEContactGroup = dJointGroupCreate(0);
}

void Space::OnUnlink()
{
    if (mODEContactGroup)
      {
        dJointGroupDestroy(mODEContactGroup);
        mODEContactGroup = 0;
      }

    // release the ODE space
    if (mODESpace)
      {
        dSpaceDestroy(mODESpace);
        mODESpace = 0;
      }
}

void Space::PostPhysicsUpdateInternal()
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}

