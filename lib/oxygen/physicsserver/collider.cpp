/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.cpp,v 1.4.8.1 2004/01/11 11:16:05 rollmark Exp $

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
#include "collider.h"
#include "space.h"
#include "body.h"
#include "world.h"
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/agentaspect/collisionperceptor.h>

using namespace boost;
using namespace oxygen;

Collider::Collider() : ODEObject(), mODEGeom(0)
{
  // set up default contact surface parameters
  mSurfaceParameter.mode = dContactBounce;
  mSurfaceParameter.mu = dInfinity;
  mSurfaceParameter.bounce = 0.8f;
  mSurfaceParameter.bounce_vel = 2.0f;
}

Collider::~Collider()
{
  if (mODEGeom)
    {
      dGeomDestroy(mODEGeom);
      mODEGeom = 0;
    }
}

void Collider::OnLink()
{
  ODEObject::OnLink();

  shared_ptr<Scene> scene = GetScene();

  if (scene.get() != NULL)
    {
      mWorld = shared_static_cast<World>(scene->GetChildOfClass("World"));
      mSpace = shared_static_cast<Space>(scene->GetChildOfClass("Space"));
      dSpaceID space = mSpace->GetODESpace();

      // if we have a space and an object, add it to the space
      if (mODEGeom && space && !dSpaceQuery(space, mODEGeom))
        {
          dGeomSetData(mODEGeom, this);
          dSpaceAdd(space, mODEGeom);
        }

      // if there is a Body hanging on our parent, link to it
      shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent())->GetChildOfClass("Body"));
      if (body.get() != NULL)
        {
          dGeomSetBody (mODEGeom, body->GetODEBody());
        }
    }
}

void Collider::OnUnlink()
{
  ODEObject::OnUnlink();

  dSpaceID space = mSpace->GetODESpace();

  // remove collision geometry from space
  if (mODEGeom && space && dSpaceQuery(space, mODEGeom))
    {
      dSpaceRemove(space, mODEGeom);
    }

  mSpace.reset();
}

void Collider::OnCollision(dGeomID collidee, dContact& contact)
{
  CreateContactJoint(collidee, contact);
  OnCollisionInternal(collidee, contact);
}

void Collider::SetSurfaceParameter(const dSurfaceParameters& surface)
{
  mSurfaceParameter = surface;
}


void Collider::CreateContactJoint(dGeomID collidee,dContact& contact)
{
  if (
      (mODEGeom == 0) ||
      (mWorld.get() == 0) ||
      (mSpace.get() == 0)
      )
    {
      return;
    }

  dBodyID myBody = dGeomGetBody(mODEGeom);
  dBodyID collideeBody = dGeomGetBody(collidee);

  // to create a contact joint at least one geom has to have a
  // corresponding body, i.e. it is sufficient that only this collider
  // has a corresponding body
  if (myBody == 0)
    {
      return;
    }

  // fill in the surface parameters
  contact.surface = mSurfaceParameter;

  // create the contact joint and attach it to the body
  dJointID joint = dJointCreateContact
    (mWorld->GetODEWorld(), mSpace->GetODEJointGroup(), &contact);
  dJointAttach (joint, myBody, collideeBody);
}

void Collider::OnCollisionInternal(dGeomID collidee,dContact& /*contact*/)
{
  // find the first CollisionPerceptor below our closest Transform node
  shared_ptr<Transform> transformParent = shared_static_cast<Transform>
    (make_shared(GetParentSupportingClass("Transform")));

  if (transformParent.get() == 0)
    {
      return;
    }

  shared_ptr<CollisionPerceptor> perceptor =
    shared_static_cast<CollisionPerceptor>
    (transformParent->GetChildOfClass("CollisionPerceptor", true));

  if (perceptor.get() == 0)
    {
      return;
    }

  // now find the closest Transform node above the collidee
  Collider* colNode = GetCollider(collidee);
  if (colNode == 0)
    {
      return;
    }

  shared_ptr<Transform> colTransformParent = shared_static_cast<Transform>
    (make_shared(colNode->GetParentSupportingClass("Transform")));

  if (colTransformParent.get() == 0)
    {
      return;
    }

  // notify the CollisionPerceptor
  perceptor->AddCollidee(colTransformParent);
}

Collider* Collider::GetCollider(dGeomID id)
{
  return id ?
    static_cast<Collider*>(dGeomGetData(id)) : 0;
}
