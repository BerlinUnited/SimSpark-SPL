/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body.cpp,v 1.4.8.5 2004/02/08 15:26:06 rollmark Exp $

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

#include "body.h"
#include "world.h"
#include "../sceneserver/scene.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

Body::Body() : ODEObject(), mODEBody(0)
{
    mMaxSpeed = 10.0f;
    mLinearDrag = 0.3f;
}

Body::~Body()
{
    if (mODEBody)
    {
        dBodyDestroy(mODEBody);
        mODEBody = 0;
    }
}

dBodyID Body::GetODEBody() const
{
    return mODEBody;
}

void Body::Enable()
{
    dBodyEnable(mODEBody);
}

void Body::Disable()
{
    dBodyDisable(mODEBody);
}

bool Body::IsEnabled() const
{
    return (dBodyIsEnabled(mODEBody) != 0);
}

void Body::UseGravity(bool f)
{
    if (f == true)
    {
        // body is affected by gravity
        dBodySetGravityMode(mODEBody, 1);
    }
    else
    {
        // body is not affected by gravity
        dBodySetGravityMode(mODEBody, 0);
    }
}

bool Body::UsesGravity() const
{
    return (dBodyGetGravityMode(mODEBody) != 0);
}

void Body::SetMaxSpeed(float speed)
{
  mMaxSpeed = speed;
}

void Body::OnLink()
{
    ODEObject::OnLink();

    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
      {
        return;
      }

    mWorld = shared_static_cast<World>(scene->GetChildOfClass("World"));
    if (mWorld.get() == 0)
      {
        return;
      }

    dWorldID world = mWorld->GetODEWorld();
    if (world == 0)
      {
        return;
      }

    // create the managed body
    mODEBody = dBodyCreate(world);
    if (mODEBody == 0)
    {
      return;
    }

    // let the body, take on the world space position of the parent
    dBodySetData(mODEBody, this);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
      (make_shared(GetParent()));

    Matrix mat = baseNode->GetWorldTransform();
    dBodySetPosition(mODEBody, mat.Pos().x(), mat.Pos().y(), mat.Pos().z());
}

void Body::OnUnlink()
{
    if (mODEBody)
    {
        dBodyDestroy(mODEBody);
        mODEBody = 0;
    }

    mWorld.reset();
}

void Body::SetMass(float mass)
{
    dMass ODEMass;
    dBodyGetMass(mODEBody, &ODEMass);
    dMassAdjust(&ODEMass, mass);
}

void Body::SetSphere(float density, float radius)
{
    dMass ODEMass;
    dMassSetSphere(&ODEMass, density, radius);
    dMassAdjust(&ODEMass, 1.0f);
    dBodySetMass(mODEBody, &ODEMass);
}

Vector3f Body::GetVelocity() const
{
    const dReal* vel = dBodyGetLinearVel(mODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void Body::SetVelocity(const Vector3f& vel)
{
    dBodySetLinearVel(mODEBody, vel[0], vel[1], vel[2]);
}

Vector3f Body::GetAngularVelocity()
{
    const dReal* vel = dBodyGetAngularVel(mODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void Body::SetAngularVelocity(const Vector3f& vel)
{
  dBodySetAngularVel(mODEBody, vel[0], vel[1], vel[2]);
}

void Body::PostPhysicsUpdateInternal()
{
    const dReal* pos = dBodyGetPosition(mODEBody);
    const dReal* rot = dBodyGetRotation(mODEBody);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
      (make_shared(GetParent()));

    Matrix mat = baseNode->GetLocalTransform();
    mat.m[0] = rot[0];
    mat.m[1] = rot[4];
    mat.m[2] = rot[8];
    mat.m[3] = 0;
    mat.m[4] = rot[1];
    mat.m[5] = rot[5];
    mat.m[6] = rot[9];
    mat.m[7] = 0;
    mat.m[8] = rot[2];
    mat.m[9] = rot[6];
    mat.m[10] = rot[10];
    mat.m[11] = 0;
    mat.m[12] = pos[0];
    mat.m[13] = pos[1];
    mat.m[14] = pos[2];
    mat.m[15] = 1;
    baseNode->SetLocalTransform(mat);

    // slow down bodies

    //force = - drag_coefficient * mass * velocity
    dMass m;
    dBodyGetMass(mODEBody, &m);

    const dReal* vel = dBodyGetLinearVel(mODEBody);
    float speed = gSqrt(vel[0]*vel[0]+vel[1]*vel[1]+vel[2]*vel[2]);
    float factor = mLinearDrag;

    if (speed > mMaxSpeed)
    {
        factor += speed/mMaxSpeed - 1.0f;
    }

    dBodyAddForce(mODEBody, -factor*vel[0]*m.mass,
                  -factor*vel[1]*m.mass, -factor*vel[2]*m.mass);

    //torque = - angular_drag_coefficient * inertia_matrix * angular_velocity
    vel = dBodyGetAngularVel(mODEBody);
    dBodyAddTorque(mODEBody, -0.3f*vel[0], -0.3f*vel[1], -0.3f*vel[2]);
}

Body* Body::GetBody(dBodyID id)
{
  return id ?
    static_cast<Body*>(dBodyGetData(id)) : 0;
}

void
Body::AddForce(const Vector3f& force)
{
    dBodyAddForce(mODEBody, force.x(), force.y(), force.z());
}

void
Body::AddTorque(const Vector3f& torque)
{
    dBodyAddTorque(mODEBody, torque.x(), torque.y(), torque.z());
}

void Body::SetPosition(const Vector3f& pos)
{
  dBodySetPosition(mODEBody, pos.x(), pos.y(), pos.z());
  // the parent node will be updated in the next physics cycle
}


