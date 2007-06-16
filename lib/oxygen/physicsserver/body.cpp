/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body.cpp,v 1.21 2007/06/16 11:01:35 yxu Exp $

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
using namespace std;

Body::Body() : ODEObject(), mODEBody(0)
{
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

bool Body::CreateBody()
{
    if (mODEBody != 0)
        {
            return true;
        }

    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return false;
        }

    // create the managed body
    mODEBody = dBodyCreate(world);
    if (mODEBody == 0)
        {
            GetLog()->Error()
                << "(Body) ERROR: could not create new ODE body\n";
            return false;
        }

    return true;
}

void Body::OnLink()
{
    ODEObject::OnLink();

    if (! CreateBody())
        {
            return;
        }

    // let the body, take on the world space position of the parent
    dBodySetData(mODEBody, this);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (make_shared(GetParent()));

    const Matrix& mat = baseNode->GetWorldTransform();
    SetRotation(mat);
    SetPosition(mat.Pos());
}

void
Body::SetMass(float mass)
{
    dMass ODEMass;
    dBodyGetMass(mODEBody, &ODEMass);
    dMassAdjust(&ODEMass, mass);
    dBodySetMass(mODEBody, &ODEMass);
}

float
Body::GetMass() const
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    return m.mass;
}

void Body::GetMassParameters(dMass& mass) const
{
    dBodyGetMass(mODEBody, &mass);
}

void Body::SetMassParameters(const dMass& mass)
{
    dBodySetMass(mODEBody, &mass);
}

void Body::SetSphere(float density, float radius)
{
    dMass ODEMass;
    dMassSetSphere(&ODEMass, density, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetSphereTotal(float total_mass, float radius)
{
    dMass ODEMass;
    dMassSetSphereTotal(&ODEMass, total_mass, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetBox(float density, const Vector3f& size)
{
    dMass ODEMass;
    dMassSetBox(&ODEMass, density, size[0], size[1], size[2]);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetBoxTotal(float total_mass, const salt::Vector3f& size)
{
    dMass ODEMass;
    dMassSetBoxTotal(&ODEMass, total_mass, size[0], size[1], size[2]);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetCylinder (float density, float radius, float length)
{
    dMass ODEMass;

    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinder (&ODEMass, density, direction, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetCylinderTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;

    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinderTotal(&ODEMass, total_mass, direction, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetCappedCylinder (float density, float radius, float length)
{
    dMass ODEMass;

    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCappedCylinder (&ODEMass, density, direction, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::SetCappedCylinderTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;

    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCappedCylinderTotal(&ODEMass, total_mass,
                                direction, radius, length);
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

void
Body::SetRotation(const Matrix& rot)
{
    dMatrix3 m;
    ConvertRotationMatrix(rot,m);
    dBodySetRotation(mODEBody,m);
}

salt::Matrix
Body::GetRotation() const
{
    const dReal* m = dBodyGetRotation(mODEBody);
    salt::Matrix rot;
    ConvertRotationMatrix(m,rot);
    return rot;
}

Vector3f
Body::GetAngularVelocity() const
{
    const dReal* vel = dBodyGetAngularVel(mODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void
Body::SetAngularVelocity(const Vector3f& vel)
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

    Matrix mat;
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

    baseNode->SetWorldTransform(mat);
}

shared_ptr<Body> Body::GetBody(dBodyID id)
{
    if (id == 0)
        {
            return shared_ptr<Body>();
        }

    Body* bodyPtr =
        static_cast<Body*>(dBodyGetData(id));

    if (bodyPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Body) no body found for dBodyID "
                 << id << "\n";
            return shared_ptr<Body>();
        }

    shared_ptr<Body> body = shared_static_cast<Body>
        (make_shared(bodyPtr->GetSelf()));

    if (body.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Body) got no shared_ptr for dBodyID "
                 << id << "\n";
        }

    return body;
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

void
Body::SetPosition(const Vector3f& pos)
{
    dBodySetPosition(mODEBody, pos.x(), pos.y(), pos.z());
    // the parent node will be updated in the next physics cycle
}

Vector3f
Body::GetPosition() const
{
    const dReal* pos = dBodyGetPosition(mODEBody);
    return Vector3f(pos[0], pos[1], pos[2]);
}

void Body::TranslateMass(const salt::Vector3f& v)
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    dMassTranslate(&m,v[0],v[1],v[2]);
}
