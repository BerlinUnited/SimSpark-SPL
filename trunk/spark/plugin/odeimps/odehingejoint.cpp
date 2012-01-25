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

#include "odehingejoint.h"

using namespace oxygen;
using namespace boost;
using namespace salt;

HingeJointImp::HingeJointImp() : Generic6DOFJointImp()
{
}

long HingeJointImp::CreateHingeJoint(long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dJointID ODEJoint = dJointCreateHinge(ODEWorld, 0);
    return (long) ODEJoint;
}

void HingeJointImp::SetAnchor(const Vector3f& anchor, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetHingeAnchor (ODEJoint, anchor[0], anchor[1], anchor[2]);
}

Vector3f HingeJointImp::GetAnchor1(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetHingeAnchor (ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}

Vector3f HingeJointImp::GetAnchor2(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal anchor[3];
    dJointGetHingeAnchor2(ODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}

void HingeJointImp::SetAxis(const Vector3f& axis, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetHingeAxis(ODEJoint, axis[0], axis[1], axis[2]);
}

Vector3f HingeJointImp::GetAxis(long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dReal axis[3];
    dJointGetHingeAxis(ODEJoint, axis);
    return Vector3f (axis[0], axis[1], axis[2]);
}

float HingeJointImp::GetAngle(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetHingeAngle(ODEJoint));
}

float HingeJointImp::GetAngleRate(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetHingeAngleRate(ODEJoint));
}

float HingeJointImp::GetTorque(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointFeedback* fb = dJointGetFeedback(ODEJoint);
    if(fb)
      return dLENGTH(fb->t1) + dLENGTH(fb->t2);
    else
      return 0;
}
