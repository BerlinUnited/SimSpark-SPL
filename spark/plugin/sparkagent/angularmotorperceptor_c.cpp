/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group

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

#include "angularmotorperceptor.h"

FUNCTION(AngularMotorPerceptor,setSenseAngle)
{
    bool v;

    if (
        (in.GetSize() != 1) ||
        (!in.GetValue(in.begin(), v))
        )
        {
            return false;
        }

    obj->SetSenseAngle(v);
    return true;
}

FUNCTION(AngularMotorPerceptor,setSenseRate)
{
    bool v;

    if (
        (in.GetSize() != 1) ||
        (!in.GetValue(in.begin(), v))
        )
        {
            return false;
        }

    obj->SetSenseRate(v);
    return true;
}

FUNCTION(AngularMotorPerceptor,setSenseTorque)
{
    bool v;

    if (
        (in.GetSize() != 1) ||
        (!in.GetValue(in.begin(), v))
        )
        {
            return false;
        }

    obj->SetSenseTorque(v);
    return true;
}

FUNCTION(AngularMotorPerceptor,setSenseCurrent)
{
    bool v;

    if (
        (in.GetSize() != 1) ||
        (!in.GetValue(in.begin(), v))
        )
        {
            return false;
        }

    obj->SetSenseCurrent(v);
    return true;
}

FUNCTION(AngularMotorPerceptor,setSenseTemperature)
{
    bool v;

    if (
        (in.GetSize() != 1) ||
        (!in.GetValue(in.begin(), v))
        )
        {
            return false;
        }

    obj->SetSenseTemperature(v);
    return true;
}

void CLASS(AngularMotorPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setSenseAngle);
    DEFINE_FUNCTION(setSenseRate);
    DEFINE_FUNCTION(setSenseTorque);
    DEFINE_FUNCTION(setSenseCurrent);
    DEFINE_FUNCTION(setSenseTemperature);
}
