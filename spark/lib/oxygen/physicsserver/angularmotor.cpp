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

AngularMotor::AngularMotor() : Joint(),
  mKe(4.3),
  mKt(4.3),
  mR(6.44),
  mI(0),
  mTempEnvironment(20),
  mTempMotor(mTempEnvironment),
  mThermalConductivity(0.025),
  mHeatCapacity(26),
  mTempProtection(80),
  mProtectionStiffness(0.8),
  mMaxTorque(0),
  mStiffness(1),
  mActualStiffness(1)
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

    mHingeJoint = FindParentSupportingClass<HingeJoint>().lock();

    if (mHingeJoint.get() == 0)
    {
        GetLog()->Error()
            << "(" << GetClass()->GetName()
            << ") ERROR: found no Joint parent\n";
    }

    mTempMotor = mTempEnvironment;
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
    Vector3f globalAxis = axis;//GetWorldTransform() * axis;
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
  return mHingeJoint->GetAngle(); // HACK
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

float AngularMotor::GetTorque() const
{
    return mAngularMotorImp->GetTorque(mJointID);
}

void AngularMotor::PrePhysicsUpdateInternal(float deltaTime)
{
  const float v = gAbs(gDegToRad(GetAngularMotorVelocity(Joint::AI_FIRST)));
  const float t = GetTorque();

  mI = t / mKt;
  float Pr = mI * mI * mR;
  float P = mKe * v * mI + Pr;
  float E = P * deltaTime;

  updateStiffnessControl();

  if ( mBattery.get() != 0 )
  {
    if (!mBattery->Consume(E))
    {
      SetMaxMotorForce(Joint::AI_FIRST, 0);
    }
  }

  mTempMotor += (Pr - mThermalConductivity*(mTempMotor-mTempEnvironment)) * deltaTime / mHeatCapacity;
}

void AngularMotor::SetBattery(const std::string& batteryPath)
{
  mBattery.reset();
  if (batteryPath.empty())
  {
    return;
  }

  boost::shared_ptr<Leaf> mySelf = shared_static_cast<Leaf>
      (GetSelf().lock());

  boost::shared_ptr<Leaf> leaf = GetCore()->Get(batteryPath,mySelf);

  if (leaf.get() == 0)
  {
    GetLog()->Error()
        << "(HingeJoint) ERROR: cannot find node '"
        << batteryPath << "'\n";
    return;
  }

  boost::shared_ptr<Battery> battery = shared_dynamic_cast<Battery>(leaf);

  if (battery.get() == 0)
  {
    GetLog()->Error()
        << "(HingeJoint) ERROR: node '"
        << batteryPath << "' is not a Battery node \n";
    return;
  }

  mBattery = battery;
}

bool AngularMotor::Disabled()
{
  if ( mBattery.get() != 0 && mBattery->IsEmpty() )
  {
    return true;
  }

  return (mTempMotor > mTempProtection);
}

void AngularMotor::updateStiffnessControl()
{
    mActualStiffness = mTempMotor < mTempProtection ? mStiffness : std::min(mStiffness, mProtectionStiffness);
    SetMaxMotorForce(Joint::AI_FIRST, mActualStiffness * mMaxTorque);
}
