/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body.cpp,v 1.9 2004/03/22 18:11:07 fruit Exp $

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
#include "zeitgeist/logserver/logserver.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

Body::Body() : ODEObject(), mODEBody(0)
{
    // unrestricted by default
    mMaxSpeed = 0;
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

float Body::GetMaxSpeed()
{
    return mMaxSpeed;
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
        GetLog()->Error() << "(Body) ERROR: found no World node\n";
        return;
      }

    dWorldID world = mWorld->GetODEWorld();
    if (world == 0)
      {
        GetLog()->Error() << "(Body) ERROR: World returned empty ODE handle\n";
        return;
      }

    // create the managed body
    mODEBody = dBodyCreate(world);
    if (mODEBody == 0)
    {
      GetLog()->Error() << "(Body) ERROR: could not create new ODE body\n";
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

void
Body::SetMass(float mass)
{
    dMass ODEMass;
    dBodyGetMass(mODEBody, &ODEMass);
    dMassAdjust(&ODEMass, mass);
    dBodySetMass(mODEBody, &ODEMass);
}

float Body::GetMass()
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    return m.mass;
}

void Body::SetSphere(float density, float radius)
{
    dMass ODEMass;
    dMassSetSphere(&ODEMass, density, radius);
    dMassAdjust(&ODEMass, 1.0f);
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

void Body::SetRotation(const Matrix& rot)
{
    dMatrix3 ODEMatrix;
    dRSetIdentity(ODEMatrix);
    ODEMatrix[0]  = rot.m[0];
    ODEMatrix[4]  = rot.m[1];
    ODEMatrix[8]  = rot.m[2];
    ODEMatrix[1]  = rot.m[4];
    ODEMatrix[5]  = rot.m[5];
    ODEMatrix[9]  = rot.m[6];
    ODEMatrix[2]  = rot.m[8];
    ODEMatrix[6]  = rot.m[9];
    ODEMatrix[10] = rot.m[10];
    dBodySetRotation(mODEBody, ODEMatrix);
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
    // synchronize parent node with the bodies position and
    // orientation
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

    // enforce maximum speed
    if (mMaxSpeed == 0)
        {
            return;
        }

    Vector3f vel = GetVelocity();
    if (vel.Length() < mMaxSpeed)
        {
            return;
        }

    vel.Normalize();
    vel *= mMaxSpeed;

    SetVelocity(vel);
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


