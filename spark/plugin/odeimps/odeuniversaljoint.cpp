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

#include "odeuniversaljoint.h"

using namespace oxygen;
using namespace salt;

UniversalJointImp::UniversalJointImp() : Generic6DOFJointImp()
{
}

long UniversalJointImp::CreateUniversalJoint(long world)
{
    dWorldID ODEWorld = (dWorldID) world;
    dJointID ODEJoint = dJointCreateUniversal(ODEWorld, 0);
    return (long) ODEJoint;
}

void UniversalJointImp::SetAnchor(const Vector3f& anchor, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetUniversalAnchor (ODEJoint, anchor[0], anchor[1], anchor[2]);
}

Vector3f UniversalJointImp::GetAnchor1(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetUniversalAnchor(ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    return pos;
}

Vector3f UniversalJointImp::GetAnchor2(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetUniversalAnchor2(ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    return pos;
}

void UniversalJointImp::SetAxis1(const Vector3f & axis, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetUniversalAxis1(ODEJoint,axis[0],axis[1],axis[2]);
}

void UniversalJointImp::SetAxis2(const Vector3f & axis, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetUniversalAxis2(ODEJoint,axis[0],axis[1],axis[2]);
}

Vector3f UniversalJointImp::GetAxis1(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal axis[3];
    dJointGetUniversalAxis1(ODEJoint, axis);
    Vector3f vec = Vector3f(axis[0],axis[1],axis[2]);
    return vec;
}

Vector3f UniversalJointImp::GetAxis2(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal axis[3];
    dJointGetUniversalAxis2(ODEJoint, axis);
    Vector3f vec = Vector3f(axis[0],axis[1],axis[2]);
    return vec;
}

float UniversalJointImp::GetAngle1(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetUniversalAngle1(ODEJoint));
}

float UniversalJointImp::GetAngle2(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetUniversalAngle2(ODEJoint));
}

float UniversalJointImp::GetAngleRate1(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetUniversalAngle1Rate(ODEJoint));
}

float UniversalJointImp::GetAngleRate2(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetUniversalAngle2Rate(ODEJoint));
}
