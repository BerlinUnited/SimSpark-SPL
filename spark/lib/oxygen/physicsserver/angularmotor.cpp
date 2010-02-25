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

#include <oxygen/physicsserver/angularmotor.h>
#include <oxygen/physicsserver/int/angularmotorint.h>
#include <oxygen/physicsserver/int/jointint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

boost::shared_ptr<AngularMotorInt> AngularMotor::mAngularMotorImp;

AngularMotor::AngularMotor() : Joint()
{

}

AngularMotor::~AngularMotor()
{
}

void AngularMotor::OnLink()
{
    Joint::OnLink();

    if (mAngularMotorImp.get() == 0)
        mAngularMotorImp = shared_dynamic_cast<AngularMotorInt>
            (GetCore()->New("AngularMotorImp"));

    long world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mJointID = mAngularMotorImp->CreateAngularMotor(world);
}

void AngularMotor::SetMode(int mode)
{
    if (mode == 0)
        mAngularMotorImp->SetModeUserMode(mJointID);
    else mAngularMotorImp->SetModeEulerMode(mJointID);
}

int AngularMotor::GetMode()
{
    return mAngularMotorImp->GetMode(mJointID);
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

    mAngularMotorImp->SetNumAxes(num, mJointID);
}

int AngularMotor::GetNumAxes()
{
    return mAngularMotorImp->GetNumAxes(mJointID);
}

void AngularMotor::SetMotorAxis(EAxisIndex idx, EAxisAnchor anchor,
                                const salt::Vector3f& axis)
{
    Vector3f globalAxis = GetWorldTransform() * axis;
    mAngularMotorImp->SetMotorAxis(idx, anchor, globalAxis, mJointID);
}

AngularMotor::EAxisAnchor AngularMotor::GetAxisAnchor(EAxisIndex idx)
{
    return static_cast<EAxisAnchor>(mAngularMotorImp->GetAxisAnchor(idx, mJointID));
}

Vector3f AngularMotor::GetMotorAxis(EAxisIndex idx)
{
    return mAngularMotorImp->GetMotorAxis(idx, mJointID);
}

void AngularMotor::SetAxisAngle(EAxisIndex idx, float degAngle)
{
    mAngularMotorImp->SetAxisAngle(idx, degAngle, mJointID);
}

float AngularMotor::GetAxisAngle(EAxisIndex idx)
{
    return mAngularMotorImp->GetAxisAngle(idx, mJointID);
}

float AngularMotor::GetAxisAngleRate(EAxisIndex idx)
{
    return mAngularMotorImp->GetAxisAngleRate(idx, mJointID);
}

void AngularMotor::SetParameter(int parameter, float value)
{
    mJointImp->SetParameter(parameter, value, mJointID);
}

float AngularMotor::GetParameter(int parameter) const
{
    return mJointImp->GetParameter(parameter, mJointID);
}
