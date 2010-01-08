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

#include <oxygen/physicsserver/staticphysicsmethods.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/ode/odewrapper.h>

using namespace oxygen;
using namespace boost;

Collider* StaticPhysicsMethods::GetColliderPointer(long geomID){
    dGeomID ODEGeom = (dGeomID) geomID;
    return static_cast<Collider*>(dGeomGetData(ODEGeom));
}

Joint* StaticPhysicsMethods::GetJoint(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    return static_cast<Joint*>(dJointGetData(ODEJoint));
}

RigidBody* StaticPhysicsMethods::GetBodyPointer(long bodyID){
    dBodyID ODEBody = (dBodyID) bodyID;
    return static_cast<RigidBody*>(dBodyGetData(ODEBody));
}

bool StaticPhysicsMethods::AreConnected(long bodyID1, long bodyID2){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnected(ODEBody1, ODEBody2) == 1;
}

bool StaticPhysicsMethods::AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnectedExcluding(ODEBody1, ODEBody2, joint_type) == 1;
}
