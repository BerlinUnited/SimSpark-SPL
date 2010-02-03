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

ODECollider::ODECollider() : ODEPhysicsObject()
{
}

ODECollider::~ODECollider()
{
}

Collider* ODECollider::GetColliderPointer(long geomID){
    dGeomID ODEGeom = (dGeomID) geomID;
    return static_cast<Collider*>(dGeomGetData(ODEGeom));
}

void ODECollider::SetRotation(const Matrix& rot, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dMatrix3 ODEMatrix;
    GenericPhysicsMatrix& matrixRef = (GenericPhysicsMatrix&) ODEMatrix; 
    ConvertRotationMatrix(rot, matrixRef);
    dGeomSetRotation(ODEGeom, ODEMatrix);
}

void ODECollider::SetPosition(const Vector3f& globalPos, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomSetPosition (ODEGeom, globalPos[0], globalPos[1], globalPos[2]);
}

void ODECollider::SetLocalPosition(const Vector3f& pos, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomSetPosition (ODEGeom, pos[0], pos[1], pos[2]);
}

Vector3f ODECollider::GetPosition(long geomID) const
{
    dGeomID ODEGeom = (dGeomID) geomID;
    const dReal* pos = dGeomGetPosition(ODEGeom);
    return Vector3f(pos[0],pos[1],pos[2]);
}

long ODECollider::GetParentSpaceID(long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    return (long) dGeomGetSpace(ODEGeom);
}

bool ODECollider::Intersect(boost::shared_ptr<Collider> collider, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dContactGeom contact;

    return dCollide
        (ODEGeom,
         (dGeomID) collider->GetGeomID(),
         1, /* ask for at most one collision point */
         &contact,
         sizeof(contact)
         ) > 0;
}

void ODECollider::DestroyGeom(long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomDestroy(ODEGeom);
}

void ODECollider::TransformSetGeom(long parentGeomID, long geomID){
    dGeomID parentODEGeom = (dGeomID) parentGeomID;
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomTransformSetGeom(parentODEGeom, ODEGeom);
}

void ODECollider::SetSpace(long spaceID, long geomID, Collider* collider){
    dSpaceID ODESpace = (dSpaceID) spaceID;
    dGeomID ODEGeom = (dGeomID) geomID;
    
    if (!spaceID) return;
    if (dSpaceQuery(ODESpace, ODEGeom)) return;
    
    dGeomSetData(ODEGeom, collider);
    dSpaceAdd(ODESpace, ODEGeom);
}

void ODECollider::SetBody(long bodyID, long geomID){
    dBodyID ODEBody = (dBodyID) bodyID;
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomSetBody(ODEGeom, ODEBody);
}

void ODECollider::RemoveFromSpace(long geomID, long spaceID){
    dGeomID ODEGeom = (dGeomID) geomID;
    dSpaceID ODESpace = (dSpaceID) spaceID;
    dSpaceRemove(ODESpace, ODEGeom);
}
