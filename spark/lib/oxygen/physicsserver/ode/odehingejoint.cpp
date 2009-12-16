/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hingejoint.cpp 112 2009-12-02 10:06:02Z a-held $

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
#include <oxygen/physicsserver/ode/odehingejoint.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

ODEHingeJoint::ODEHingeJoint() : ODEGeneric6DOFJoint()
{
}

void ODEHingeJoint::CreateHingeJoint(long world)
{
    mODEJoint = dJointCreateHinge((dWorldID) world, 0);
}

void ODEHingeJoint::SetAnchor(const Vector3f& anchor)
{
    dJointSetHingeAnchor (mODEJoint, anchor[0], anchor[1], anchor[2]);
}

Vector3f ODEHingeJoint::GetAnchor1()
{
    dReal anchor[3];
    dJointGetHingeAnchor (mODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}

Vector3f ODEHingeJoint::GetAnchor2()
{
    dReal anchor[3];
    dJointGetHingeAnchor2(mODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    
    return pos;
}

void ODEHingeJoint::SetAxis(const Vector3f& axis)
{
    dJointSetHingeAxis(mODEJoint, axis[0], axis[1], axis[2]);
}

Vector3f ODEHingeJoint::GetAxis()
{
    dReal axis[3];
    dJointGetHingeAxis(mODEJoint, axis);
    return Vector3f (axis[0], axis[1], axis[2]);
}

float ODEHingeJoint::GetAngle() const
{
    return gRadToDeg(dJointGetHingeAngle(mODEJoint));
}

float ODEHingeJoint::GetAngleRate() const
{
    return gRadToDeg(dJointGetHingeAngleRate(mODEJoint));
}

void ODEHingeJoint::SetParameter(int parameter, float value)
{
    dJointSetHingeParam(mODEJoint, parameter, value);
}

float ODEHingeJoint::GetParameter(int parameter) const
{
    return dJointGetHingeParam(mODEJoint, parameter);
}
