/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.cpp,v 1.9 2004/04/07 08:34:27 rollmark Exp $

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
#include "collisionhandler.h"
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace std;

Collider::Collider() : ODEObject(), mODEGeom(0)
{
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

    if (mODEGeom == 0)
        {
            return;
        }

    // if there is a Body below our parent, link to it
    shared_ptr<Body> body = shared_static_cast<Body>
        (make_shared(GetParent())->GetChildOfClass("Body"));

    if (body.get() != 0)
        {
            dGeomSetBody (mODEGeom, body->GetODEBody());
        }

    // if we have a space add the geom to it
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            return;
        }

    mSpace = shared_static_cast<Space>(scene->GetChildOfClass("Space"));
    if (mSpace.get() == 0)
        {
            return;
        }

    dSpaceID space = mSpace->GetODESpace();

    if (
        (space) &&
        (! dSpaceQuery(space, mODEGeom))
        )
        {
            dGeomSetData(mODEGeom, this);
            dSpaceAdd(space, mODEGeom);
        }
}

void Collider::OnUnlink()
{
    ODEObject::OnUnlink();

    if (
        (mSpace.get() == 0) ||
        (mODEGeom == 0)
        )
        {
            return;
        }

    // remove collision geometry from space
    dSpaceID space = mSpace->GetODESpace();

    if (
        (space) &&
        (dSpaceQuery(space, mODEGeom))
        )
        {
            dSpaceRemove(space, mODEGeom);
        }

    mSpace.reset();
}

void Collider::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (GetChildSupportingClass("CollisionHandler").get() == 0)
        {
            // for convenience we add a ContactJointHandler if no
            // other handler is registered. This behaviour covers the
            // majority of all use cases and eases the creation of
            // Colliders.
            AddCollisionHandler("oxygen/ContactJointHandler");
        }
}

dGeomID Collider::GetODEGeom()
{
    return mODEGeom;
}

bool Collider::AddCollisionHandler(const std::string& handlerName)
{
    GetCore()->New(handlerName);

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
}

void Collider::OnCollision (boost::shared_ptr<Collider> collidee,
                            dContact& contact, ECollisionType type)

{
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
}

shared_ptr<Collider> Collider::GetCollider(dGeomID id)
{
    if (id == 0)
        {
            return shared_ptr<Collider>();
        }

    Collider* collPtr =
        static_cast<Collider*>(dGeomGetData(id));

    if (collPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) no Collider found for dGeomID "
                 << id << "\n";
            return shared_ptr<Collider>();
        }

    shared_ptr<Collider> collider = shared_static_cast<Collider>
        (make_shared(collPtr->GetSelf()));

    if (collider.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Collider) got no shared_ptr for dGeomID "
                 << id << "\n";
        }

    return collider;
}

void Collider::SetPosition(salt::Vector3f pos)
{
    dGeomSetPosition (mODEGeom, pos[0], pos[1], pos[2]);
}

bool Collider::Intersects(boost::shared_ptr<Collider> collider)
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
         collider->GetODEGeom(),
         1, /* ask for at most one collision point */
         &contact,
         sizeof(contact)
         ) > 0;
}

