/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space.cpp,v 1.6 2004/04/07 08:35:54 rollmark Exp $

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

    // dSpaceCollide(), is guaranteed to pass all potentially
    // intersecting geom pairs to the callback function, but depending
    // on the internal algorithms used by the space it may also make
    // mistakes and pass non-intersecting pairs. Thus we can not
    // expect that dCollide() will return contacts for every pair
    // passed to the callback.
    dContact contact;
    if (! dCollide (obj1, obj2, 0, &contact.geom, sizeof(dContactGeom)))
    {
      return;
    }

    // get shared pointers to the two corresponding Collider nodes
    shared_ptr<Collider> collider = Collider::GetCollider(obj1);
    shared_ptr<Collider> collidee = Collider::GetCollider(obj2);

    if (
        (collider.get() == 0) ||
        (collidee.get() == 0)
        )
      {
        return;
      }

    // notify the collider nodes
    collider->OnCollision(collidee,contact,Collider::CT_DIRECT);
    collidee->OnCollision(collider,contact,Collider::CT_SYMMETRIC);
}

bool Space::ConstructInternal()
{
  // create the ode space, 0 indicates that this space should
  // not be inserted into another space, i.e. we always create a
  // toplevel space object
  mODESpace = dHashSpaceCreate(0);

  // create a joint group for the contacts
  mODEContactGroup = dJointGroupCreate(0);

  return (
          (mODESpace != 0) &&
          (mODEContactGroup != 0)
          );
}

void Space::PostPhysicsUpdateInternal()
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}

