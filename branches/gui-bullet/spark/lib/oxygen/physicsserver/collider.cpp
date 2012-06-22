/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.cpp 179 2010-02-28 01:33:40Z marianbuchta $

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
#include <oxygen/physicsserver/genericphysicsobjects.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/int/colliderint.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/leaf.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace salt;
using namespace boost;
using namespace std;

boost::shared_ptr<ColliderInt> Collider::mColliderImp;

Collider::Collider() : PhysicsObject(), mGeomID(0)
{

}

Collider::~Collider()
{
}

void Collider::OnLink()
{
    PhysicsObject::OnLink();
    
    if (mColliderImp.get() == 0)
        mColliderImp = shared_dynamic_cast<ColliderInt>
            (GetCore()->New("ColliderImp"));
    
    if (mGeomID == 0) return;
    
    boost::weak_ptr<Node> parent = GetParent();
    if (parent.expired()) return; 

    boost::shared_ptr<TransformCollider> tcParent =
        shared_dynamic_cast<TransformCollider>(parent.lock());

    if (tcParent.get() != 0)
        {
            // our parent is an ODE transform geom that encapsulates
            // this geom, so register ourself to it. This geom must
            // not directly register to a space or a body.
            mColliderImp->TransformSetGeom(tcParent->GetGeomID(), mGeomID);

            return;
        }

    // this geom is independent, so register to space and body
    // if we have a space add the geom to it
    long spaceID = FindSpaceID();
    mColliderImp->SetSpace(spaceID, mGeomID, this);
    
    // if there is a Body below our parent, link to it
    boost::shared_ptr<RigidBody> body = shared_static_cast<RigidBody>
        (parent.lock()->GetChildOfClass("RigidBody"));

    if (body.get() != 0) 
        mColliderImp->SetBody(body->GetBodyID(), mGeomID);
        else{
                // no body node found, setup initial position and
                // orientation identical to the parent node
                SetRotation(GetWorldTransform());
                SetPosition(Vector3f(0,0,0));
            }
}

void Collider::OnUnlink()
{
    PhysicsObject::OnUnlink();
    long space = GetParentSpaceID();

    if (space && mGeomID)
    {
        // remove collision geometry from space
        mColliderImp->RemoveFromSpace(mGeomID, space);
    }
}

void Collider::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (FindChildSupportingClass<CollisionHandler>(false).get() == 0)
        {
            // for convenience we add a ContactJointHandler if no
            // other handler is registered. This behaviour covers the
            // majority of all use cases and eases the creation of
            // Colliders.
            AddCollisionHandler("oxygen/ContactJointHandler");
        }
}

long Collider::GetGeomID()
{
    return mGeomID;
}

bool Collider::AddCollisionHandler(const std::string& handlerName)
{
    boost::shared_ptr<CollisionHandler> handler =
        shared_dynamic_cast<CollisionHandler>(GetCore()->New(handlerName));

    if (handler.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (Collider) Cannot create CollisionHandler "
                << handlerName << "\n";
            return false;
        }

    return AddChildReference(handler);
}

void Collider::OnCollision (boost::shared_ptr<Collider> collidee,
                            GenericContact& contact, ECollisionType type)
{    
    TLeafList handlers;
    ListChildrenSupportingClass<CollisionHandler>(handlers);

    for (
         TLeafList::iterator iter = handlers.begin();
         iter != handlers.end();
         ++iter
         )
        {
            boost::shared_ptr<CollisionHandler> handler =
                shared_static_cast<CollisionHandler>(*iter);

            if (
                (type == CT_SYMMETRIC) &&
                (! handler->IsSymmetricHandler())
                )
                {
                    continue;
                }

            handler->HandleCollision(collidee, contact);
        }
}

boost::shared_ptr<Collider> Collider::GetCollider(long geomID)
{
    if (geomID == 0)
        {
            return boost::shared_ptr<Collider>();
        }

    Collider* collPtr =
        mColliderImp->GetColliderPointer(geomID);

    if (collPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) no Collider found for GeomID "
                 << geomID << "\n";
            return boost::shared_ptr<Collider>();
        }

    boost::shared_ptr<Collider> collider = shared_static_cast<Collider>
        (collPtr->GetSelf().lock());

    if (collider.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) got no boost::shared_ptr for GeomID "
                 << geomID << "\n";
        }

    return collider;
}

void Collider::SetRotation(const Matrix& rot)
{
    mColliderImp->SetRotation(rot, mGeomID);
}

void Collider::SetPosition(const Vector3f& pos)
{
    Vector3f globalPos(GetWorldTransform() * pos);
    mColliderImp->SetPosition(globalPos, mGeomID);
}

void Collider::SetLocalPosition(const Vector3f& pos)
{
    mColliderImp->SetLocalPosition(pos, mGeomID);
}

Vector3f Collider::GetPosition() const
{
    return mColliderImp->GetPosition(mGeomID);
}

long Collider::GetParentSpaceID()
{
    if (mGeomID == 0)
        {
            return 0;
        }

    return mColliderImp->GetParentSpaceID(mGeomID);
}

bool Collider::Intersects(boost::shared_ptr<Collider> collider)
{
    if (
        (mGeomID == 0) ||
        (collider.get() == 0)
        )
        {
            return false;
        }

    return mColliderImp->Intersect(collider, mGeomID);
}

void Collider::DestroyPhysicsObject()
{
    if (! mGeomID)
        {
            return;
        }

    mColliderImp->DestroyGeom(mGeomID);
    mGeomID = 0;
}

void Collider::AddNotCollideWithColliderName(const std::string& colliderName, bool isAdd)
{
    TColliderNameSet::iterator it = mNotCollideWithSet.find(colliderName);

    if (isAdd)
    {
        if (it == mNotCollideWithSet.end())
        {
            //I'm not have this one
            mNotCollideWithSet.insert(colliderName);
        }
    }
    else
    {
        if (it != mNotCollideWithSet.end())
        {
            //Remove
            mNotCollideWithSet.erase(it);
        }
    }
}

const Collider::TColliderNameSet& Collider::GetNotCollideWithSet() const
{
    return mNotCollideWithSet;
}
