/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: angularmotor.cpp,v 1.3 2007/02/12 19:24:00 rollmark Exp $

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
#include "angularmotor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

AngularMotor::AngularMotor() : Joint()
{
}

AngularMotor::~AngularMotor()
{
}

void AngularMotor::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mODEJoint = dJointCreateAMotor(world, 0);
}

void AngularMotor::SetMode(EMotorMode mode)
{
    dJointSetAMotorMode(mODEJoint,mode);
}

AngularMotor::EMotorMode AngularMotor::GetMode()
{
    return static_cast<EMotorMode>(dJointGetAMotorMode(mODEJoint));
}

void AngularMotor::SetNumAxes(int num)
{
    if (
        (num < 0) ||
        (num > 3)
        )
        {
            return;
        }

    dJointSetAMotorNumAxes(mODEJoint, num);
}

int AngularMotor::GetNumAxes()
{
    return dJointGetAMotorNumAxes(mODEJoint);
}

void AngularMotor::SetMotorAxis(EAxisIndex idx, EAxisAnchor anchor,
                                const salt::Vector3f& axis)
{
    Vector3f globalAxis = GetWorldTransform() * axis;
    dJointSetAMotorAxis (mODEJoint, idx, anchor,
                         globalAxis[0], globalAxis[1], globalAxis[2]);
}

AngularMotor::EAxisAnchor AngularMotor::GetAxisAnchor(EAxisIndex idx)
{
    return static_cast<EAxisAnchor>(dJointGetAMotorAxisRel (mODEJoint, idx));
}

Vector3f AngularMotor::GetMotorAxis(EAxisIndex idx)
{
    dVector3 dAxis;
    dJointGetAMotorAxis(mODEJoint,idx,dAxis);
    return Vector3f(dAxis[0],dAxis[1],dAxis[2]);
}

void AngularMotor::SetAxisAngle(EAxisIndex idx, float degAngle)
{
    dJointSetAMotorAngle(mODEJoint, idx, gDegToRad(degAngle));
}

float AngularMotor::GetAxisAngle(EAxisIndex idx)
{
    return gRadToDeg(dJointGetAMotorAngle(mODEJoint, idx));
}

float AngularMotor::GetAxisAngleRate(EAxisIndex idx)
{
    return gRadToDeg(dJointGetAMotorAngleRate(mODEJoint,idx));
}

void AngularMotor::SetParameter(int parameter, float value)
{
    dJointSetAMotorParam(mODEJoint, parameter, value);
}

float AngularMotor::GetParameter(int parameter) const
{
    return dJointGetAMotorParam(mODEJoint, parameter);
}



