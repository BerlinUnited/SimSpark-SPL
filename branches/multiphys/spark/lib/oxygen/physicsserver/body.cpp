/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include "body.h"
#include "world.h"
#include "../sceneserver/scene.h"
#include "zeitgeist/logserver/logserver.h"
#include "../sceneserver/transform.h"
#include "transformcollider.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

Body::Body() : ODEObject(), mODEBody(0), mMassTrans(0,0,0), mMassTransformed(false)
{
}

Body::~Body()
{
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

void Body::DestroyODEObject()
{
    if (mODEBody == 0)
        {
            return;
        }

    dBodyDestroy(mODEBody);
    mODEBody = 0;
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
        (GetParent().lock());

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

void Body::PrepareSphere(dMass& mass, float density, float radius) const
{
    dMassSetSphere(&mass, density, radius);
}

void Body::SetSphere(float density, float radius)
{
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddSphere(float density, float radius, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    AddMass(ODEMass, matrix);
}

void Body::PrepareSphereTotal(dMass& mass, float total_mass, float radius) const
{
    dMassSetSphereTotal(&mass, total_mass, radius);
}

void Body::SetSphereTotal(float total_mass, float radius)
{
    dMass ODEMass;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddSphereTotal(float total_mass, float radius, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    AddMass(ODEMass, matrix);
}

void Body::PrepareBox(dMass& mass, float density, const Vector3f& size) const
{
    dMassSetBox(&mass, density, size[0], size[1], size[2]);
}

void Body::SetBox(float density, const Vector3f& size)
{
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddBox(float density, const Vector3f& size, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    AddMass(ODEMass, matrix);
}

void Body::PrepareBoxTotal(dMass& mass, float total_mass, const Vector3f& size) const
{
    dMassSetBoxTotal(&mass, total_mass, size[0], size[1], size[2]);
}

void Body::SetBoxTotal(float total_mass, const Vector3f& size)
{
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddBoxTotal(float total_mass, const Vector3f& size, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    AddMass(ODEMass, matrix);
}
/*
    dMass ODEMass;
    dBodyGetMass(mODEBody, &ODEMass);
    Vector3f trans(ODEMass.c[0], ODEMass.c[1], ODEMass.c[2]);
    
    if (trans.SquareLength() > 0)
    {
        mOrigin = mOrigin - trans;
        GetLog()->Warning() << "(Body::CenterMass) New origin: " << mOrigin << "\n";
        
        dMassTranslate(&ODEMass, -trans[0], -trans[1], -trans[2]);
        dBodySetMass(mODEBody, &ODEMass);
        
        Vector3f position = GetPosition();
        position = position + trans;
        SetPosition(position);
    
        mCOMShifted = true;
    }
*/
void Body::AddMass(const dMass& mass, const Matrix& matrix)
{
    dMass transMass(mass);

    dMatrix3 rot;
    ConvertRotationMatrix(matrix, rot);
    dMassRotate(&transMass,rot);

    const Vector3f& trans(matrix.Pos());
    dMassTranslate(&transMass,trans[0],trans[1],trans[2]);

    dMassTranslate(&transMass,mMassTrans[0],mMassTrans[1],mMassTrans[2]);
    
    dMass bodyMass;
    dBodyGetMass(mODEBody, &bodyMass);
    dMassAdd(&bodyMass, &transMass);

    /** ODE currently requires that the center mass is always in the
        origin of the body
    */
    Vector3f trans2(bodyMass.c[0], bodyMass.c[1], bodyMass.c[2]);

    dMassTranslate(&bodyMass, -trans2[0], -trans2[1], -trans2[2]);
    bodyMass.c[0] = bodyMass.c[1] = bodyMass.c[2] = 0.0f;
    dBodySetMass(mODEBody, (const dMass*)&bodyMass);

    // Move body so mass is at right position again
    SetPosition(GetPosition() + trans2);
    
    // Keep track of total translation of mass
    mMassTrans = mMassTrans - trans2;
    
    mMassTransformed = true;
}

void Body::PrepareCylinder (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinder (&mass, density, direction, radius, length);
}

void Body::SetCylinder (float density, float radius, float length)
{
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddCylinder (float density, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    AddMass(ODEMass, matrix);
}

void Body::PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinderTotal(&mass, total_mass, direction, radius, length);
}

void Body::SetCylinderTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddCylinderTotal(float total_mass, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    AddMass(ODEMass, matrix);
}

void Body::PrepareCapsule (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsule (&mass, density, direction, radius, length);
}

void Body::SetCapsule (float density, float radius, float length)
{
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddCapsule (float density, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    AddMass(ODEMass, matrix);
}

void Body::PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsuleTotal(&mass, total_mass, direction, radius, length);
}

void Body::SetCapsuleTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void Body::AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix)
{
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    AddMass(ODEMass, matrix);
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

void Body::SynchronizeParent() const
{
    const dReal* pos = dBodyGetPosition(mODEBody);
    const dReal* rot = dBodyGetRotation(mODEBody);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (GetParent().lock());

    
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

void Body::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // Check whether mass/body has been translated
    if (mMassTransformed)
    {
        weak_ptr<Node> parent = GetParent();

        // Update colliders (only those encapsulated in transform colliders)
        TLeafList transformColliders;
        parent.lock()->ListChildrenSupportingClass<TransformCollider>(transformColliders);

        for (TLeafList::iterator iter = transformColliders.begin(); iter != transformColliders.end(); ++iter)
        {
            // Only move non-transform colliders
            shared_ptr<Collider> collider = shared_static_cast<TransformCollider>(*iter)->FindChildSupportingClass<Collider>();
            if (collider.get())
            {
                Vector3f pos = collider->GetPosition();
                pos = pos + mMassTrans;
                collider->SetLocalPosition(pos);
            }
        }

        // Update visuals (or other transforms in the tree)
        TLeafList transforms;
        parent.lock()->ListShallowChildrenSupportingClass<Transform>(transforms);
        for (TLeafList::iterator iter = transforms.begin(); iter != transforms.end(); ++iter)
        {
            shared_ptr<Transform> transform = shared_dynamic_cast<Transform>(*iter);
            Matrix worldTransform = transform->GetWorldTransform();
            worldTransform.Pos() = worldTransform.Pos() + mMassTrans;
            transform->SetWorldTransform(worldTransform);
        }
        
        mMassTrans = Vector3f(0,0,0);
        mMassTransformed = false;
    }
}


void Body::PostPhysicsUpdateInternal()
{
    SynchronizeParent();
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
        (bodyPtr->GetSelf().lock());

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

void Body::TranslateMass(const Vector3f& v)
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    dMassTranslate(&m,v[0],v[1],v[2]);
}
