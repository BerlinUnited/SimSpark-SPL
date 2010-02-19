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

#include "odespace.h"
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/space.h>

using namespace boost;
using namespace oxygen;

void ODESpace::collisionNearCallback(void* data, dGeomID obj1, dGeomID obj2)
{
    Space* space = (Space*) data;
    space->HandleCollide((long) obj1, (long) obj2);
}

ODESpace::ODESpace() : ODEPhysicsObject()
{
}

void ODESpace::Collide(long space, Space* callee)
{
    dSpaceID ODESpace = (dSpaceID) space;
    dSpaceCollide(ODESpace, callee, collisionNearCallback);
}

void ODESpace::Collide2(long obj1, long obj2, Space* callee)
{
    dGeomID ODEObj1 = (dGeomID) obj1;
    dGeomID ODEObj2 = (dGeomID) obj2;
    dSpaceCollide2(ODEObj1, ODEObj2, callee, &collisionNearCallback);
}

long ODESpace::GetParentSpaceID(long spaceID)
{
    dGeomID ODESpace = (dGeomID) spaceID;
    dSpaceID parentSpace = dGeomGetSpace(ODESpace);
    return (long) parentSpace;
}

long ODESpace::CreateContactGroup()
{
    // create a joint group for the contacts
    dJointGroupID ODEContactGroup = dJointGroupCreate(0);

    return (long) ODEContactGroup;
}

void ODESpace::PostPhysicsUpdateInternal(long contactGroup)
{
    dJointGroupID ODEContactGroup = (dJointGroupID) contactGroup;
    // remove all contact joints
    dJointGroupEmpty(ODEContactGroup);
}

long ODESpace::CreateSpace(long spaceID){
    dSpaceID ODESpace = (dSpaceID) spaceID;
    dSpaceID CreatedSpace = dHashSpaceCreate(ODESpace);
    return (long) CreatedSpace;
}

void ODESpace::DestroySpace(long contactGroup, long spaceID)
{
    dJointGroupID ODEContactGroup = (dJointGroupID) contactGroup;
    dSpaceID ODESpace = (dSpaceID) spaceID;

    if (ODEContactGroup)
        {
            dJointGroupDestroy(ODEContactGroup);
        }

    // release the ODE space
    dSpaceDestroy(ODESpace);
}

bool ODESpace::ObjectIsSpace(long objectID){
    dGeomID ODEGeom = (dGeomID) objectID;
    return dGeomIsSpace(ODEGeom);
}

long ODESpace::FetchBody(long geomID){
    dGeomID ODEGeom = (dGeomID) geomID;
    dBodyID ODEBody = dGeomGetBody(ODEGeom);
    return (long) ODEBody;
}

long ODESpace::FetchSpace(long geomID){
    dGeomID ODEGeom = (dGeomID) geomID;
    dSpaceID ODESpace = dGeomGetSpace(ODEGeom);
    return (long) ODESpace;
}

bool ODESpace::AreConnectedWithJoint(long bodyID1, long bodyID2){
    if ((!bodyID1) || (!bodyID2))
        return false;

    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnectedExcluding(ODEBody1, ODEBody2, dJointTypeContact);
}

void ODESpace::CollideInternal(boost::shared_ptr<Collider> collider, 
                              boost::shared_ptr<Collider> collidee,
                              long geomID1, long geomID2)
{
    dGeomID geom1 = (dGeomID) geomID1;
    dGeomID geom2 = (dGeomID) geomID2;
    // dSpaceCollide(), is guaranteed to pass all potentially
    // intersecting geom pairs to the callback function, but depending
    // on the internal algorithms used by the space it may also make
    // mistakes and pass non-intersecting pairs. Thus we can not
    // expect that dCollide() will return contacts for every pair
    // passed to the callback.
    static const int nContacts = 4;
    static dContact contacts[nContacts];

    int n = dCollide (geom1, geom2, nContacts,
                     &contacts[0].geom, sizeof(dContact));

    if (n == 0)
    {
        return;
    }


    for (int i=0;i<n;++i)
        {
            // notify the collider nodes
            collider->OnCollision(collidee,(GenericContact&) contacts[i],Collider::CT_DIRECT);
            collidee->OnCollision(collider,(GenericContact&) contacts[i],Collider::CT_SYMMETRIC);
        }
}
