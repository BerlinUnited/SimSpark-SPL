/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.cpp 115 2009-12-07 08:43:40Z a-held $

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
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/ode/odecollider.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/leaf.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace salt;
using namespace zeitgeist;
using namespace boost;
using namespace std;

ODECollider::ODECollider() : ODEPhysicsObject(), mODEGeom(0)
{
    mGeomID = 0;
}

ODECollider::~ODECollider()
{
}

void ODECollider::OnLink()
{/*
    //PhysicsObject::OnLink();


    //weak_ptr<Node> parent = GetParent();
    if (
        (mODEGeom == 0) //||
        //(parent.expired())
        )
        {
            return;
        }

   // shared_ptr<TransformCollider> tcParent =
   //     shared_dynamic_cast<TransformCollider>(parent.lock());

    if (tcParent.get() != 0)
        {
            // our parent is an ODE transform geom that encapsulates
            // this geom, so register ourself to it. This geom must
            // not directly register to a space or a body.
            dGeomTransformSetGeom((dGeomID) tcParent->GetGeomID(), mODEGeom);
            return;
        }

    // this geom is independent, so register to space and body
    // if we have a space add the geom to it
    //long spaceID = FindSpaceID();
    if (
        (spaceID) &&
        (! dSpaceQuery((dSpaceID) spaceID, mODEGeom))
        )
        {
            dGeomSetData(mODEGeom, this);
            dSpaceAdd((dSpaceID) spaceID, mODEGeom);
        }
    // if there is a Body below our parent, link to it
    shared_ptr<RigidBody> body = shared_static_cast<RigidBody>
        (parent.lock()->GetChildOfClass("RigidBody"));

    if (body.get() != 0)
        {
            dGeomSetBody (mODEGeom, (dBodyID) body->GetBodyID());
        } else
            {
                // no body node found, setup initial position and
                // orientation identical to the parent node
                SetRotation(GetWorldTransform());
                SetPosition(Vector3f(0,0,0));
            }
*/}

void ODECollider::OnUnlink()
{
    //PhysicsObject::OnUnlink();

    // remove collision geometry from space
    long space = GetParentSpaceID();

    if (
        (mODEGeom == 0) ||
        (space == 0)
        )
        {
            return;
        }

    if (space)
    {
        dSpaceRemove((dSpaceID) space, mODEGeom);
    }
}

void ODECollider::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
   // if (FindChildSupportingClass<CollisionHandler>(false).get() == 0)
   //     {
            // for convenience we add a ContactJointHandler if no
            // other handler is registered. This behaviour covers the
            // majority of all use cases and eases the creation of
            // Colliders.
            AddCollisionHandler("oxygen/ContactJointHandler");
    //    }
}

long ODECollider::GetGeomID()
{
    return (long) mODEGeom;
}

bool ODECollider::AddCollisionHandler(const std::string& handlerName)
{/*
    shared_ptr<CollisionHandler> handler =
        shared_dynamic_cast<CollisionHandler>(GetCore()->New(handlerName));

    if (handler.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (Collider) Cannot create CollisionHandler "
                << handlerName << "\n";
            return false;
        }

    return AddChildReference(handler);
*/}

void ODECollider::OnCollision (boost::shared_ptr<Collider> collidee,
                            dContact& contact, ECollisionType type)

{/*
    TLeafList handlers;
    ListChildrenSupportingClass<CollisionHandler>(handlers);

    for (
         TLeafList::iterator iter = handlers.begin();
         iter != handlers.end();
         ++iter
         )
        {
            shared_ptr<CollisionHandler> handler =
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
*/}

/*shared_ptr<Collider> ODECollider::GetCollider(long geomID)
{
    if (geomID == 0)
        {
            return shared_ptr<Collider>();
        }

    Collider* collPtr =
        static_cast<Collider*>(dGeomGetData((dGeomID) geomID));

    if (collPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) no Collider found for GeomID "
                 << geomID << "\n";
            return shared_ptr<Collider>();
        }

    shared_ptr<Collider> collider = shared_static_cast<Collider>
        (collPtr->GetSelf().lock());

    if (collider.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) got no shared_ptr for GeomID "
                 << geomID << "\n";
        }

    return collider;
}*/

void ODECollider::SetRotation(const Matrix& rot)
{
    dMatrix3 ODEMatrix;
    GenericPhysicsObject& matrixRef = (GenericPhysicsObject&) ODEMatrix; 
    ConvertRotationMatrix(rot, matrixRef);
    dGeomSetRotation(mODEGeom, ODEMatrix);
}

void ODECollider::SetPosition(const Vector3f& pos)
{
    //Vector3f globalPos(GetWorldTransform() * pos);
    //dGeomSetPosition (mODEGeom, globalPos[0], globalPos[1], globalPos[2]);
}

void ODECollider::SetLocalPosition(const Vector3f& pos)
{
    dGeomSetPosition (mODEGeom, pos[0], pos[1], pos[2]);
}

Vector3f ODECollider::GetPosition() const
{
    const dReal* pos = dGeomGetPosition(mODEGeom);
    return Vector3f(pos[0],pos[1],pos[2]);
}

long ODECollider::GetParentSpaceID()
{
    if (mODEGeom == 0)
        {
            return 0;
        }

    return (long) dGeomGetSpace(mODEGeom);
}

bool ODECollider::Intersects(boost::shared_ptr<Collider> collider)
{
    if (
        (mODEGeom == 0) ||
        (collider.get() == 0)
        )
        {
            return false;
        }

    dContactGeom contact;

    return dCollide
        (
         mODEGeom,
         (dGeomID) collider->GetGeomID(),
         1, /* ask for at most one collision point */
         &contact,
         sizeof(contact)
         ) > 0;
}

void ODECollider::DestroyPhysicsObject()
{
    if (! mODEGeom)
        {
            return;
        }

    dGeomDestroy(mODEGeom);
    mODEGeom = 0;
}

void ODECollider::AddNotCollideWithColliderName(const std::string & colliderName, bool isAdd)
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

const ODECollider::TColliderNameSet& ODECollider::GetNotCollideWithSet() const
{
    return mNotCollideWithSet;
}
