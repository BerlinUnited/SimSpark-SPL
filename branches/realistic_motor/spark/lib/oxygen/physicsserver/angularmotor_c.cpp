/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

using namespace boost;
using namespace oxygen;
using namespace salt;

FUNCTION(AngularMotor,setUserMode)
{
    obj->SetMode(0);
    return true;
}

FUNCTION(AngularMotor,setEulerMode)
{
    obj->SetMode(1);
    return true;
}

FUNCTION(AngularMotor,getMode)
{
    return static_cast<int>(obj->GetMode());
}

FUNCTION(AngularMotor,setNumAxes)
{
    int inNum;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inNum))
         )
    {
        return false;
    }

    obj->SetNumAxes(inNum);
    return true;
}

FUNCTION(AngularMotor,getNumAxes)
{
    return obj->GetNumAxes();
}

FUNCTION(AngularMotor,setMotorAxis)
{
    int inAxisIdx;
    // 0 global, 1 first body, 2 second body
    int inAxisAnchor;
    Vector3f inAxis;

    if (
        (in.GetSize() < 3) ||
        (! in.GetValue(in[0],inAxisIdx)) ||
        (! in.GetValue(in[1],inAxisAnchor)) ||
        (! in.GetValue(in[2],inAxis))
        )
        {
            return false;
        }

    obj->SetMotorAxis(static_cast<Joint::EAxisIndex>(inAxisIdx),
                      static_cast<AngularMotor::EAxisAnchor>(inAxisAnchor),
                      inAxis);
    return true;
}

FUNCTION(AngularMotor,getAxisAnchor)
{
    int inAxisIdx;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAxisIdx))
        )
    {
        return 0;
    }

    return obj->GetAxisAnchor(static_cast<Joint::EAxisIndex>(inAxisIdx));
}

FUNCTION(AngularMotor,setAxisAngle)
{
    int inAxisIdx;
    int inDegAngle;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxisIdx)) ||
        (! in.GetValue(in[1], inDegAngle))
        )
        {
            return false;
        }

    obj->SetAxisAngle(static_cast<Joint::EAxisIndex>(inAxisIdx),static_cast<float>(inDegAngle));
    return true;
}

FUNCTION(AngularMotor, getAxisAngle)
{
    int inAxisIdx;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inAxisIdx))
        )
        {
            return 0;
        }

    return obj->GetAxisAngle(static_cast<Joint::EAxisIndex>(inAxisIdx));
}

FUNCTION(AngularMotor,getAxisAngleRate)
{
    int inAxisIdx;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inAxisIdx))
        )
        {
            return 0;
        }

    return obj->GetAxisAngleRate(static_cast<Joint::EAxisIndex>(inAxisIdx));
}

FUNCTION(AngularMotor,setBattery)
{
    std::string inPath;

    if (in.GetSize() != 1
        || (!in.GetValue(in[0], inPath)))
    {
      return false;
    }

    obj->SetBattery(inPath);
    return true;
}

#define SET_ANGULAR_MOTOR_CONSTANT(functionName) \
  static zeitgeist::GCValue functionName(AngularMotor *obj, \
       __attribute__((unused)) const zeitgeist::ParameterList &in)\
{\
  float inValue;\
  if (in.GetSize() != 1 || (!in.GetValue(in[0], inValue))) { return false; }\
  obj->functionName(inValue);\
  return true;\
}

SET_ANGULAR_MOTOR_CONSTANT(setSpeedConstant)
SET_ANGULAR_MOTOR_CONSTANT(setTorqueConstant)
SET_ANGULAR_MOTOR_CONSTANT(setResistance)
SET_ANGULAR_MOTOR_CONSTANT(setEnvironmentTempeature)
SET_ANGULAR_MOTOR_CONSTANT(setThermalConductivity)
SET_ANGULAR_MOTOR_CONSTANT(setHeatCapacity)
SET_ANGULAR_MOTOR_CONSTANT(setProtectionTempeature)

void CLASS(AngularMotor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Joint);
    DEFINE_FUNCTION(setUserMode);
    DEFINE_FUNCTION(setEulerMode);
    DEFINE_FUNCTION(getMode);
    DEFINE_FUNCTION(setNumAxes);
    DEFINE_FUNCTION(getNumAxes);
    DEFINE_FUNCTION(setMotorAxis);
    DEFINE_FUNCTION(getAxisAnchor);
    DEFINE_FUNCTION(setAxisAngle);
    DEFINE_FUNCTION(getAxisAngle);
    DEFINE_FUNCTION(getAxisAngleRate);
    DEFINE_FUNCTION(setBattery);
    DEFINE_FUNCTION(setSpeedConstant);
    DEFINE_FUNCTION(setTorqueConstant);
    DEFINE_FUNCTION(setResistance);
    DEFINE_FUNCTION(setEnvironmentTempeature);
    DEFINE_FUNCTION(setThermalConductivity);
    DEFINE_FUNCTION(setHeatCapacity);
    DEFINE_FUNCTION(setProtectionTempeature);
}
