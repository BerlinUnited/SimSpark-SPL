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

#include "space.h"
#include "world.h"
#include "collider.h"
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;

/** set of spaces with disabled inner collision */
Space::TSpaceIdSet Space::gDisabledInnerCollisionSet;

void Space::collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2)
{
    Space *space = (Space*)data;
    space->HandleCollide(obj1, obj2);
}

Space::Space() : ODEObject(), mODESpace(0), mODEContactGroup(0)
{
}

Space::~Space()
{
}

dSpaceID Space::GetODESpace() const
{
  return mODESpace;
}

dJointGroupID Space::GetODEJointGroup() const
{
  return mODEContactGroup;
}

void Space::Collide()
{
    // bind collision callback function to this object
    dSpaceCollide(mODESpace, this, collisionNearCallback);
}

void Space::HandleSpaceCollide(dGeomID obj1, dGeomID obj2)
{
    // collide all geoms internal to the space(s)
    dSpaceCollide2 (obj1,obj2,this,&collisionNearCallback);

    if (dGeomIsSpace (obj1))
        {
            dSpaceCollide ((dSpaceID)(obj1),this,&collisionNearCallback);
        }

    if (dGeomIsSpace (obj2))
        {
            dSpaceCollide ((dSpaceID)(obj2),this,&collisionNearCallback);
        }
}

void Space::HandleCollide(dGeomID obj1, dGeomID obj2)
{
    if (
        (dGeomIsSpace (obj1)) ||
        (dGeomIsSpace (obj2))
        )
        {
            // colliding a space with something
            HandleSpaceCollide(obj1, obj2);
            return;
        }

    // colliding two non-space geoms; reject collisions
    // between bodies that are connected with joints
    const dBodyID b1 = dGeomGetBody(obj1);
    const dBodyID b2 = dGeomGetBody(obj2);

    if (
        (b1) && (b2) &&
        (dAreConnectedExcluding(b1,b2,dJointTypeContact))
        )
        {
            return;
        }

    // if obj1 and obj2 are in a space that disabled inner collision,
    // reject the collision
    const dSpaceID s1 = dGeomGetSpace(obj1);
    const dSpaceID s2 = dGeomGetSpace(obj2);

    if (
        (s1 == s2) &&
        (gDisabledInnerCollisionSet.find(s1) != gDisabledInnerCollisionSet.end())
        )
        {
            return;
        }

    
    // if obj1 and obj2 are in the same space, and 
    // obj1 is in obj2's "mNotCollideWithSet" or ojb2 is in obj1's
    // reject the collision

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
        const oxygen::Collider::TColliderNameSet & collider_set = collider->GetNotCollideWithSet();
        const oxygen::Collider::TColliderNameSet & collidee_set = collidee->GetNotCollideWithSet();
        if (
            (collider_set.find(collidee->GetName()) != collider_set.end()) ||
            (collidee_set.find(collider->GetName()) != collidee_set.end())
            )
            {
                return;
            }
    }

    // dSpaceCollide(), is guaranteed to pass all potentially
    // intersecting geom pairs to the callback function, but depending
    // on the internal algorithms used by the space it may also make
    // mistakes and pass non-intersecting pairs. Thus we can not
    // expect that dCollide() will return contacts for every pair
    // passed to the callback.
    static const int nContacts = 4;
    static dContact contacts[nContacts];

    int n = dCollide (obj1, obj2, nContacts,
                      &contacts[0].geom, sizeof(dContact));

    if (n == 0)
    {
        return;
    }


    for (int i=0;i<n;++i)
        {
            // notify the collider nodes
            collider->OnCollision(collidee,contacts[i],Collider::CT_DIRECT);
            collidee->OnCollision(collider,contacts[i],Collider::CT_SYMMETRIC);
        }
}

void Space::OnUnlink()
{
    DisableInnerCollision(false);
    ODEObject::OnUnlink();
}

void Space::OnLink()
{
    ODEObject::OnLink();

    shared_ptr<Space> space = GetSpace();
    dSpaceID spaceId = 0;

    if (space.get() != 0)
        {
            spaceId = space->GetODESpace();
        }

    mODESpace = dHashSpaceCreate(spaceId);
}

dSpaceID Space::GetParentSpaceID()
{
    if (mODESpace == 0)
        {
            return 0;
        }

    return dGeomGetSpace((dGeomID)mODESpace);
}

bool Space::IsGlobalSpace()
{
    return
        (
        (mODESpace != 0) &&
        (GetParentSpaceID() == 0)
        );
}

bool Space::ConstructInternal()
{
    // create a joint group for the contacts
    mODEContactGroup = dJointGroupCreate(0);

    return (mODEContactGroup != 0);
}

void Space::PostPhysicsUpdateInternal()
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}

void
Space::DestroySpaceObjects()
{
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            return;
        }

    TLeafList objects;
    const bool recursive = true;
    scene->ListChildrenSupportingClass<ODEObject>(objects, recursive);

    bool globalSpace = IsGlobalSpace();
    shared_ptr<Space> self = shared_static_cast<Space>(GetSelf().lock());

    for (
         TLeafList::iterator iter = objects.begin();
         iter != objects.end();
         ++iter
         )
        {
            shared_ptr<ODEObject> object = shared_static_cast<ODEObject>(*iter);
            if (object == self)
            {
                continue;
            }

            // destroy objects registered to this space; the top level
            // space object also destroy any other ODE object
            const dSpaceID parentSpace = object->GetParentSpaceID();
            if (
                (
                 (globalSpace) &&
                 (parentSpace == 0)
                 ) ||
                (parentSpace == mODESpace)
                )
                {
                    object->DestroyODEObject();
                }
        }
}

void
Space::DestroyODEObject()
{
    if (! mODESpace)
        {
            return;
        }

    // make sure that all objects registered to this space are destroyed
    // before this space. Any other order provokes a segfault in ODE.
    DestroySpaceObjects();

    if (mODEContactGroup)
        {
            dJointGroupDestroy(mODEContactGroup);
            mODEContactGroup = 0;
        }

    // release the ODE space
    dSpaceDestroy(mODESpace);
    mODESpace = 0;
}

void Space::DisableInnerCollision(bool disable)
{
    if (mODESpace == 0)
        {
            assert(false);
            return;
        }

    if (disable)
        {
            gDisabledInnerCollisionSet.insert(mODESpace);
            return;
        }

    TSpaceIdSet::iterator iter = gDisabledInnerCollisionSet.find(mODESpace);
    if (iter == gDisabledInnerCollisionSet.end())
        {
            return;
        }

    gDisabledInnerCollisionSet.erase(iter);
}

bool Space::GetDisableInnerCollision() const
{
    TSpaceIdSet::const_iterator iter
        = gDisabledInnerCollisionSet.find(mODESpace);

    return (iter != gDisabledInnerCollisionSet.end());
}

