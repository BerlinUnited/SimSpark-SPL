/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space.cpp 80 2009-07-30 17:25:42Z hedayat $

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

#include <oxygen/physicsserver/ode/odespace.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;

/** set of spaces with disabled inner collision */
ODESpace::TSpaceIdSet ODESpace::gDisabledInnerCollisionSet;

//void ODESpace::collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2)
//{
 //   Space *space = (Space*)data;
  //  space->HandleCollide(obj1, obj2);
//}

ODESpace::ODESpace() : ODEPhysicsObject(), mODESpace(0), mODEContactGroup(0)
{
}

long ODESpace::GetSpaceID() const
{
  return (long) mODESpace;
}

dJointGroupID ODESpace::GetODEJointGroup() const
{
  return mODEContactGroup;
}

void ODESpace::Collide()
{
    // bind collision callback function to this object
    Collide(mODESpace);
}

void ODESpace::Collide(dSpaceID space)
{
    if (gDisabledInnerCollisionSet.find(space) == gDisabledInnerCollisionSet.end())
    {
        //dSpaceCollide(space, this, Space::collisionNearCallback);
    }
}

void ODESpace::HandleSpaceCollide(dGeomID obj1, dGeomID obj2)
{
    // collide all geoms internal to the space(s)
    //dSpaceCollide2 (obj1,obj2,this,&Space::collisionNearCallback);

    if (dGeomIsSpace (obj1))
        {
            Collide((dSpaceID)obj1);
        }

    if (dGeomIsSpace (obj2))
        {
            Collide((dSpaceID)obj2);
        }
}

void ODESpace::HandleCollide(dGeomID obj1, dGeomID obj2)
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

    // get shared pointers to the two corresponding Collider nodes first
    shared_ptr<Collider> collider = Collider::GetCollider((long) obj1);
    shared_ptr<Collider> collidee = Collider::GetCollider((long) obj2);

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
            //collider->OnCollision(collidee,contacts[i],Collider::CT_DIRECT);
            //collidee->OnCollision(collider,contacts[i],Collider::CT_SYMMETRIC);
        }
}

void ODESpace::OnUnlink()
{
    DisableInnerCollision(false);
}

void ODESpace::OnLink()
{
    /*shared_ptr<Space> space = GetSpace();
    dSpaceID spaceID = 0;

    if (space.get() != 0)
        {
            spaceID = (dSpaceID) space->GetSpaceID();
        }

    mODESpace = dHashSpaceCreate(spaceID);*/
}

long ODESpace::GetParentSpaceID()
{
    if (mODESpace == 0)
        {
            return 0;
        }

    return (long) dGeomGetSpace((dGeomID)mODESpace);
}

bool ODESpace::IsGlobalSpace()
{
    return
        (
        (mODESpace != 0) &&
        (GetParentSpaceID() == 0)
        );
}

bool ODESpace::ConstructInternal()
{
    // create a joint group for the contacts
    mODEContactGroup = dJointGroupCreate(0);

    return (mODEContactGroup != 0);
}

void ODESpace::PostPhysicsUpdateInternal()
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}

void ODESpace::DestroySpaceObjects(){}
/*{
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
                (parentSpace == (long) mODESpace)
                )
                {
                    object->DestroyPhysicsObject();
                }
        }
}*/

void ODESpace::DestroyPhysicsObject()
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

void ODESpace::DisableInnerCollision(bool disable)
{
    if (mODESpace == 0)
        {
            //assert(false);
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

bool ODESpace::GetDisableInnerCollision() const
{
    TSpaceIdSet::const_iterator iter
        = gDisabledInnerCollisionSet.find(mODESpace);

    return (iter != gDisabledInnerCollisionSet.end());
}
