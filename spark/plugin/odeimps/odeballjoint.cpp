/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: balljoint.cpp 112 2009-12-02 10:06:02Z a-held $

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
#include "odeballjoint.h"

using namespace oxygen;
using namespace boost;
using namespace salt;

ODEBallJoint::ODEBallJoint() : ODEGeneric6DOFJoint()
{
}

long ODEBallJoint::CreateBallJoint(long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dJointID ODEJoint = dJointCreateBall(ODEWorld, 0);
    return (long) ODEJoint;
}

void ODEBallJoint::SetAnchor(const Vector3f& gAnchor, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetBallAnchor (ODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);
}

Vector3f ODEBallJoint::GetAnchor1(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetBallAnchor (ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}

Vector3f ODEBallJoint::GetAnchor2(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetBallAnchor2(ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}
