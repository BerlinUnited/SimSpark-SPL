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

#include "angularmotoreffector.h"

FUNCTION(AngularMotorEffector,setUseBacklash)
{
    bool use;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), use))
        )
        {
            return false;
        }


    obj->SetUseBacklash(use);
    return true;
}

FUNCTION(AngularMotorEffector,setDeadband)
{
    float deadband;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), deadband))
        )
        {
            return false;
        }


    obj->SetDeadband(deadband);
    return true;
}

FUNCTION(AngularMotorEffector,setMaxForce)
{
    float force;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), force))
        )
        {
            return false;
        }


    obj->SetMaxForce(force);
    return true;
}

void CLASS(AngularMotorEffector)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Effector);
    DEFINE_FUNCTION(setUseBacklash);
    DEFINE_FUNCTION(setDeadband);
    DEFINE_FUNCTION(setMaxForce);
}
