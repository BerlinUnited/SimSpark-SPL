/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hinge2joint.cpp 112 2009-12-02 10:06:02Z a-held $

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

#include <oxygen/physicsserver/ode/odehinge2joint.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

ODEHinge2Joint::ODEHinge2Joint() : ODEGeneric6DOFJoint()
{
}

long ODEHinge2Joint::CreateHinge2Joint(long world)
{
    dWorldID ODEWorld = (dWorldID) world;
    mODEJoint = dJointCreateHinge2(ODEWorld, 0);
    return (long) mODEJoint;
}

void ODEHinge2Joint::SetAnchor(const Vector3f& gAnchor,
                            const Vector3f& up,
                            const Vector3f& right)
{
    dJointSetHinge2Anchor (mODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);
    dJointSetHinge2Axis1(mODEJoint,up[0],up[1],up[2]);
    dJointSetHinge2Axis2(mODEJoint,right[0],right[1],right[2]);
}

Vector3f ODEHinge2Joint::GetAnchor1()
{
    dReal anchor[3];
    dJointGetHinge2Anchor (mODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    return pos;
}

Vector3f ODEHinge2Joint::GetAnchor2()
{
    dReal anchor[3];
    dJointGetHinge2Anchor2(mODEJoint, anchor);
    Vector3f pos = Vector3f(anchor[0],anchor[1],anchor[2]);
    return pos;
}

float ODEHinge2Joint::GetAngle()
{
    return gRadToDeg(dJointGetHinge2Angle1(mODEJoint));
}

float ODEHinge2Joint::GetAngleRate1()
{
    return gRadToDeg(dJointGetHinge2Angle1Rate(mODEJoint));
}

float ODEHinge2Joint::GetAngleRate2()
{
    return gRadToDeg(dJointGetHinge2Angle2Rate(mODEJoint));
}

void ODEHinge2Joint::SetParameter(int parameter, float value)
{
    dJointSetHinge2Param(mODEJoint, parameter, value);
}

float ODEHinge2Joint::GetParameter(int parameter) const
{
    return dJointGetHinge2Param(mODEJoint, parameter);
}
