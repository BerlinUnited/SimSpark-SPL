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

#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/ode/odespace.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;

/** set of spaces with disabled inner collision */
Space::TSpaceIdSet Space::gDisabledInnerCollisionSet;

Space::Space() : PhysicsObject(), mSpaceID(0)
{
    mSpaceImp = shared_ptr<ODESpace>(new ODESpace());
}

Space::~Space()
{
}

long Space::GetSpaceID() const
{
    return mSpaceID;
}

long Space::GetODEJointGroup() const
{
    return mContactGroupID;
}

void Space::Collide()
{
    // bind collision callback function to this object
    Collide(mSpaceID);
}

void Space::Collide(long space)
{
    if (gDisabledInnerCollisionSet.find(space) == gDisabledInnerCollisionSet.end())
    {
        mSpaceImp->Collide(space, this);
    }
}

void Space::HandleSpaceCollide(long obj1, long obj2)
{    
    // collide all geoms internal to the space(s)    
    mSpaceImp->Collide2(obj1, obj2, this);

    if (mSpaceImp->ObjectIsSpace(obj1))
        {
            Collide(obj1);
        }

    if (mSpaceImp->ObjectIsSpace(obj2))
        {
            Collide(obj2);
        }
}

void Space::HandleCollide(long obj1, long obj2)
{  
    if (
        (mSpaceImp->ObjectIsSpace(obj1)) ||
        (mSpaceImp->ObjectIsSpace(obj2))
        )
        {
            // colliding a space with something
            HandleSpaceCollide(obj1, obj2);
            return;
        }

    // colliding two non-space geoms; reject collisions
    // between bodies that are connected with joints
    const long body1 = mSpaceImp->FetchBody(obj1);
    const long body2 = mSpaceImp->FetchBody(obj2);

    if (mSpaceImp->AreConnectedWithJoint(body1, body2))
        {
            return;
        }

    // if obj1 and obj2 are in a space that disabled inner collision,
    // reject the collision
    const long s1 = mSpaceImp->FetchSpace(obj1);
    const long s2 = mSpaceImp->FetchSpace(obj2);

    // get shared pointers to the two corresponding Collider nodes first
    shared_ptr<Collider> collider = Collider::GetCollider(obj1);
    shared_ptr<Collider> collidee = Collider::GetCollider(obj2);

    if (
        (collider.get() == 0) ||
        (collidee.get() == 0)
        )
      {
        return;
      }

    if (s1 == s2)
    {
        const oxygen::Collider::TColliderNameSet& collider_set = collider->GetNotCollideWithSet();
        const oxygen::Collider::TColliderNameSet& collidee_set = collidee->GetNotCollideWithSet();
        if (
            (collider_set.find(collidee->GetName()) != collider_set.end()) ||
            (collidee_set.find(collider->GetName()) != collidee_set.end())
            )
            {
                return;
            }
    }

    mSpaceImp->CollideInternal(collider, collidee, obj1, obj2);
}

void Space::OnUnlink()
{
    DisableInnerCollision(false);
    PhysicsObject::OnUnlink();
}

void Space::OnLink()
{
    PhysicsObject::OnLink();

    shared_ptr<Space> space = GetSpace();
    long spaceID = 0;

    if (space.get() != 0)
        {
            spaceID = space->GetSpaceID();
        }

    mSpaceID = mSpaceImp->CreateSpace(spaceID);
}

long Space::GetParentSpaceID()
{
    if (mSpaceID == 0)
        {
            return 0;
        }

    return mSpaceImp->GetParentSpaceID(mSpaceID);
}

bool Space::IsGlobalSpace()
{
    return
        (
        (mSpaceID != 0) &&
        (GetParentSpaceID() == 0)
        );
}

bool Space::ConstructInternal()
{
    mContactGroupID = mSpaceImp->ConstructInternal();
    
    return (mContactGroupID != 0);
}

void Space::PostPhysicsUpdateInternal()
{
    mSpaceImp->PostPhysicsUpdateInternal(mContactGroupID);
}

void Space::DestroySpaceObjects()
{
    boost::shared_ptr<Scene> scene = GetScene(); 

    if (scene.get() == 0)
        {
            return;
        }

    TLeafList objects;
    const bool recursive = true;
    scene->ListChildrenSupportingClass<PhysicsObject>(objects, recursive);

    bool globalSpace = IsGlobalSpace();
    shared_ptr<Space> self = shared_static_cast<Space>(GetSelf().lock());

    for (
         TLeafList::iterator iter = objects.begin();
         iter != objects.end();
         ++iter
         )
        {
            shared_ptr<PhysicsObject> object = shared_static_cast<PhysicsObject>(*iter);
            if (object == self)
            {
                continue;
            }

            // destroy objects registered to this space; the top level
            // space object also destroy any other ODE object
            const long parentSpace = object->GetParentSpaceID();
            if (
                (
                 (globalSpace) &&
                 (parentSpace == 0)
                 ) ||
                (parentSpace == mSpaceID)
                )
                {
                    object->DestroyPhysicsObject();
                }
        }
}

void Space::DestroyPhysicsObject()
{
    if (mSpaceID == 0)
        {
            return;
        }

    // make sure that all objects registered to this space are destroyed
    // before this space. Any other order provokes a segfault in ODE.
    DestroySpaceObjects();

    mSpaceImp->DestroySpace(mSpaceID, mContactGroupID);
    
    mSpaceID = 0;
    mContactGroupID = 0;
}

void Space::DisableInnerCollision(bool disable)
{
    if (mSpaceID == 0)
        {
            return;
        }
        
    if (disable)
        {
            gDisabledInnerCollisionSet.insert(mSpaceID);
            return;
        }
    
    TSpaceIdSet::iterator iter = gDisabledInnerCollisionSet.find(mSpaceID);
    if (iter == gDisabledInnerCollisionSet.end())
        {
            return;
        }

    gDisabledInnerCollisionSet.erase(iter);
}

bool Space::GetDisableInnerCollision() const
{
    TSpaceIdSet::const_iterator iter
        = gDisabledInnerCollisionSet.find(mSpaceID);

    return (iter != gDisabledInnerCollisionSet.end());
}
