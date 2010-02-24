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
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/int/rigidbodyint.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <zeitgeist/logserver/logserver.h>
#include <iostream>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

boost::shared_ptr<RigidBodyInt> RigidBody::mRigidBodyImp;

RigidBody::RigidBody() : Body()
{

}

RigidBody::~RigidBody()
{
}

long RigidBody::GetBodyID() const
{
    return mBodyID;
}

void RigidBody::Enable()
{
    mRigidBodyImp->Enable(mBodyID);
}

void RigidBody::Disable()
{
    mRigidBodyImp->Disable(mBodyID);
}

bool RigidBody::IsEnabled() const
{
    return mRigidBodyImp->IsEnabled(mBodyID);
}

void RigidBody::UseGravity(bool f)
{
    mRigidBodyImp->UseGravity(f, mBodyID);
}

bool RigidBody::UsesGravity() const
{
    return mRigidBodyImp->UsesGravity(mBodyID);
}

bool RigidBody::CreateBody()
{
    if (mBodyID != 0)
        {
            return true;
        }

    if (mRigidBodyImp.get() == 0)
        mRigidBodyImp = shared_dynamic_cast<RigidBodyInt>
            (GetCore()->New("RigidBodyImp"));
            
    if (mRigidBodyImp.get() == 0)
        {
            //we can't use the logserver here
            std::cerr << "(RigidBody) ERROR: No implementation found at '/classes/RigidBodyImp'";
            return false;
        }

    long world = (long) GetWorldID();
    if (world == 0)
        {
            return false;
        }
    
    mBodyID = mRigidBodyImp->CreateBody(world);
    
    if (mBodyID == 0)
        {
            GetLog()->Error()
                << "(Body) ERROR: could not create new ODE body\n";
            return false;
        }

    return true;
}

void RigidBody::DestroyPhysicsObject()
{
    if (mBodyID == 0) return;
    
    mRigidBodyImp->DestroyRigidBody(mBodyID);
    mBodyID = 0;
}

void RigidBody::OnLink()
{
    PhysicsObject::OnLink();

    if (! CreateBody())
        {
            return;
        }

    // let the body, take on the world space position of the parent
    mRigidBodyImp->BodySetData(this, mBodyID);

    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (GetParent().lock());

    const Matrix& mat = baseNode->GetWorldTransform();
    mRigidBodyImp->SetRotation(mat, mBodyID);
    mRigidBodyImp->SetPosition(mat.Pos(), mBodyID);
}

void RigidBody::SetMass(float mass)
{
    mRigidBodyImp->SetMass(mass, mBodyID);
}

float RigidBody::GetMass() const
{
    return mRigidBodyImp->GetMass(mBodyID);
}

void RigidBody::SetMassParameters(const GenericMass& mass)
{
    mRigidBodyImp->SetMassParameters(mass, mBodyID);
}

void RigidBody::SetSphere(float density, float radius)
{
    mRigidBodyImp->SetSphere(density, radius, mBodyID);
}

void RigidBody::AddSphere(float density, float radius, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddSphere(density, radius, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetSphereTotal(float total_mass, float radius)
{
    mRigidBodyImp->SetSphereTotal(total_mass, radius, mBodyID);
}

void RigidBody::AddSphereTotal(float total_mass, float radius, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddSphereTotal(total_mass, radius, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetBox(float density, const Vector3f& size)
{
    mRigidBodyImp->SetBox(density, size, mBodyID);
}

void RigidBody::AddBox(float density, const Vector3f& size, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddBox(density, size, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetBoxTotal(float total_mass, const Vector3f& size)
{
    mRigidBodyImp->SetBoxTotal(total_mass, size, mBodyID);
}

void RigidBody::AddBoxTotal(float total_mass, const Vector3f& size, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddBoxTotal(total_mass, size, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetCylinder (float density, float radius, float length)
{
    mRigidBodyImp->SetCylinder(density, radius, length, mBodyID);
}

void RigidBody::AddCylinder (float density, float radius, float length, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddCylinder(density, radius, length, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetCylinderTotal(float total_mass, float radius, float length)
{
    mRigidBodyImp->SetCylinderTotal(total_mass, radius, length, mBodyID);
}

void RigidBody::AddCylinderTotal(float total_mass, float radius, float length, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddCylinderTotal(total_mass, radius, length, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetCapsule (float density, float radius, float length)
{
    mRigidBodyImp->SetCapsule(density, radius, length, mBodyID);
}

void RigidBody::AddCapsule (float density, float radius, float length, const Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddCapsule(density, radius, length, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

void RigidBody::SetCapsuleTotal(float total_mass, float radius, float length)
{
    mRigidBodyImp->SetCapsuleTotal(total_mass, radius, length, mBodyID);
}

void RigidBody::AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix)
{
    mMassTrans = mRigidBodyImp->AddCapsuleTotal(total_mass, radius, length, matrix, mMassTrans, mBodyID);
    mMassTransformed = true;
}

Vector3f RigidBody::GetVelocity() const
{
    return mRigidBodyImp->GetVelocity(mBodyID);
}

void RigidBody::SetVelocity(const Vector3f& vel)
{
    mRigidBodyImp->SetVelocity(vel, mBodyID);
}

void RigidBody::SetRotation(const Matrix& rot)
{
    mRigidBodyImp->SetRotation(rot, mBodyID);
}

salt::Matrix RigidBody::GetRotation() const
{
    return mRigidBodyImp->GetRotation(mBodyID);
}

Vector3f RigidBody::GetLocalAngularVelocity() const
{
    return mRigidBodyImp->GetLocalAngularVelocity(mBodyID);
}

Vector3f RigidBody::GetAngularVelocity() const
{
    return mRigidBodyImp->GetAngularVelocity(mBodyID);
}

void RigidBody::SetAngularVelocity(const Vector3f& vel)
{
    mRigidBodyImp->SetAngularVelocity(vel, mBodyID);
}

void RigidBody::SynchronizeParent() const
{
    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>
        (GetParent().lock());
    
    Matrix mat = mRigidBodyImp->GetSynchronisationMatrix(mBodyID);

    baseNode->SetWorldTransform(mat);
}

void RigidBody::PrePhysicsUpdateInternal(float /*deltaTime*/)
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

    RigidBody* bodyPtr = mRigidBodyImp->GetBodyPointer(bodyID);

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
    mRigidBodyImp->AddForce(force, mBodyID);
}

Vector3f RigidBody::GetForce() const
{
    return mRigidBodyImp->GetForce(mBodyID);
}

void RigidBody::AddTorque(const Vector3f& torque)
{
    mRigidBodyImp->AddTorque(torque, mBodyID);
}

void RigidBody::SetPosition(const Vector3f& pos)
{
    mRigidBodyImp->SetPosition(pos, mBodyID);
}

Vector3f RigidBody::GetPosition() const
{
    return mRigidBodyImp->GetPosition(mBodyID);
}

void RigidBody::TranslateMass(const Vector3f& v)
{
    mRigidBodyImp->TranslateMass(v, mBodyID);
}
