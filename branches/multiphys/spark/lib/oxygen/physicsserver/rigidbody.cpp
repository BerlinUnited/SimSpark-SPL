/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body.cpp 106 2009-11-19 10:10:50Z a-held $

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
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/ode/oderigidbody.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

RigidBody::RigidBody() : Body()
{
    mRigidBodyImp = boost::shared_ptr<ODERigidBody>(new ODERigidBody());
}

RigidBody::~RigidBody()
{
}

long RigidBody::GetBodyID() const
{
    return mRigidBodyImp->GetBodyID();
}

void RigidBody::Enable()
{
    mRigidBodyImp->Enable();
}

void RigidBody::Disable()
{
    mRigidBodyImp->Disable();
}

bool RigidBody::IsEnabled() const
{
    return mRigidBodyImp->IsEnabled();
}

void RigidBody::UseGravity(bool f)
{
    mRigidBodyImp->UseGravity(f);
}

bool RigidBody::UsesGravity() const
{
    return mRigidBodyImp->UsesGravity();
}

bool RigidBody::CreateBody()
{
    long bodyID = mRigidBodyImp->GetBodyID();

    if (bodyID != 0)
        {
            return true;
        }

    long world = (long) GetWorldID();
    if (world == 0)
        {
            return false;
        }
    
    mRigidBodyImp->CreateBody(world);
    
    if (mRigidBodyImp->GetBodyID() == 0)
        {
            GetLog()->Error()
                << "(Body) ERROR: could not create new ODE body\n";
            return false;
        }

    return true;
}

void RigidBody::DestroyPhysicsObject()
{
    mRigidBodyImp->DestroyPhysicsObject();
}

void RigidBody::OnLink()
{
    PhysicsObject::OnLink();

    if (! CreateBody())
        {
            return;
        }

    // let the body, take on the world space position of the parent
    mRigidBodyImp->BodySetData(this);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (GetParent().lock());

    const Matrix& mat = baseNode->GetWorldTransform();
    mRigidBodyImp->SetRotation(mat);
    mRigidBodyImp->SetPosition(mat.Pos());
}

void RigidBody::SetMass(float mass)
{
    mRigidBodyImp->SetMass(mass);
}

float RigidBody::GetMass() const
{
    return mRigidBodyImp->GetMass();
}

void RigidBody::SetMassParameters(const float& mass){
    mRigidBodyImp->SetMassParameters(mass);
}

void RigidBody::SetSphere(float density, float radius)
{
    mRigidBodyImp->SetSphere(density, radius);
}

void RigidBody::AddSphere(float density, float radius, const Matrix& matrix)
{
    mRigidBodyImp->AddSphere(density, radius, matrix);
}

void RigidBody::SetSphereTotal(float total_mass, float radius)
{
    mRigidBodyImp->SetSphereTotal(total_mass, radius);
}

void RigidBody::AddSphereTotal(float total_mass, float radius, const Matrix& matrix)
{
    mRigidBodyImp->AddSphereTotal(total_mass, radius, matrix);
}

void RigidBody::SetBox(float density, const Vector3f& size)
{
    mRigidBodyImp->SetBox(density, size);
}

void RigidBody::AddBox(float density, const Vector3f& size, const Matrix& matrix)
{
    mRigidBodyImp->AddBox(density, size, matrix);
}

void RigidBody::SetBoxTotal(float total_mass, const Vector3f& size)
{
    mRigidBodyImp->SetBoxTotal(total_mass, size);
}

void RigidBody::AddBoxTotal(float total_mass, const Vector3f& size, const Matrix& matrix)
{
    mRigidBodyImp->AddBoxTotal(total_mass, size, matrix);
}

void RigidBody::SetCylinder (float density, float radius, float length)
{
    mRigidBodyImp->SetCylinder(density, radius, length);
}

void RigidBody::AddCylinder (float density, float radius, float length, const Matrix& matrix)
{
    mRigidBodyImp->AddCylinder(density, radius, length, matrix);
}

void RigidBody::SetCylinderTotal(float total_mass, float radius, float length)
{
    mRigidBodyImp->SetCylinderTotal(total_mass, radius, length);
}

void RigidBody::AddCylinderTotal(float total_mass, float radius, float length, const Matrix& matrix)
{
    mRigidBodyImp->AddCylinderTotal(total_mass, radius, length, matrix);
}

void RigidBody::SetCapsule (float density, float radius, float length)
{
    mRigidBodyImp->SetCapsule(density, radius, length);
}

void RigidBody::AddCapsule (float density, float radius, float length, const Matrix& matrix)
{
    mRigidBodyImp->AddCapsule(density, radius, length, matrix);
}

void RigidBody::SetCapsuleTotal(float total_mass, float radius, float length)
{
    mRigidBodyImp->SetCapsuleTotal(total_mass, radius, length);
}

void RigidBody::AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix)
{
    mRigidBodyImp->AddCapsuleTotal(total_mass, radius, length, matrix);
}

Vector3f RigidBody::GetVelocity() const
{
    return mRigidBodyImp->GetVelocity();
}

void RigidBody::SetVelocity(const Vector3f& vel)
{
    mRigidBodyImp->SetVelocity(vel);
}

void RigidBody::SetRotation(const Matrix& rot)
{
    mRigidBodyImp->SetRotation(rot);
}

salt::Matrix RigidBody::GetRotation() const
{
    return mRigidBodyImp->GetRotation();
}

Vector3f RigidBody::GetLocalAngularVelocity() const
{
    return mRigidBodyImp->GetLocalAngularVelocity();
}

Vector3f RigidBody::GetAngularVelocity() const
{
    return mRigidBodyImp->GetAngularVelocity();
}

void RigidBody::SetAngularVelocity(const Vector3f& vel)
{
    mRigidBodyImp->SetAngularVelocity(vel);
}

void RigidBody::SynchronizeParent() const
{
    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (GetParent().lock());
    
    Matrix mat = mRigidBodyImp->GetSynchronisationMatrix();

    baseNode->SetWorldTransform(mat);
}

void RigidBody::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // Check whether mass/body has been translated
    if (mRigidBodyImp->GetMassTransformed())
    {
        Vector3f massTrans = mRigidBodyImp->GetMassTrans();
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
                pos = pos + massTrans;
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
            worldTransform.Pos() = worldTransform.Pos() + massTrans;
            transform->SetWorldTransform(worldTransform);
        }
        
        mRigidBodyImp->SetMassTrans(Vector3f(0,0,0));
        mRigidBodyImp->SetMassTransformed(false);
    }
}


void RigidBody::PostPhysicsUpdateInternal()
{
    SynchronizeParent();
}

shared_ptr<RigidBody> RigidBody::GetBody(long id)
{
    long bodyID = (long) id;
    if (bodyID == 0)
        {
            return shared_ptr<RigidBody>();
        }

    RigidBody* bodyPtr = RigidBodyInt::GetBodyPointer(bodyID);

    if (bodyPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (RigidBody) no body found for BodyID "
                 << id << "\n";
            return shared_ptr<RigidBody>();
        }

    shared_ptr<RigidBody> body = shared_static_cast<RigidBody>
        (bodyPtr->GetSelf().lock());

    if (body.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (RigidBody) got no shared_ptr for dBodyID "
                 << id << "\n";
        }

    return body;
}

void RigidBody::AddForce(const Vector3f& force)
{
    mRigidBodyImp->AddForce(force);
}

Vector3f RigidBody::GetForce() const
{
    return mRigidBodyImp->GetForce();
}

void RigidBody::AddTorque(const Vector3f& torque)
{
    mRigidBodyImp->AddTorque(torque);
}

void RigidBody::SetPosition(const Vector3f& pos)
{
    mRigidBodyImp->SetPosition(pos);
}

Vector3f RigidBody::GetPosition() const
{
    return mRigidBodyImp->GetPosition();
}

void RigidBody::TranslateMass(const Vector3f& v)
{
    mRigidBodyImp->TranslateMass(v);
}
