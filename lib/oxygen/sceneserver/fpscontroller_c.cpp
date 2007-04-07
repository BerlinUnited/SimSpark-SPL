/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2003 Koblenz University
$Id: fpscontroller_c.cpp,v 1.9 2007/04/07 13:12:06 jamu Exp $

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

#include "fpscontroller.h"

using namespace boost;
using namespace oxygen;

FUNCTION(FPSController,setVAngle)
{
    float inAngle;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAngle))
        )
        {
            return false;
        }

    obj->SetVAngle(inAngle);
    return true;
}

FUNCTION(FPSController,setHAngle)
{
    float inAngle;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAngle))
        )
        {
            return false;
        }

    obj->SetHAngle(inAngle);
    return true;
}

FUNCTION(FPSController,setAcceleration)
{
    float inAccel;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAccel))
        )
        {
            return false;
        }

    obj->SetAcceleration(inAccel);
    return true;
}

FUNCTION(FPSController,getAcceleration)
{
    return obj->GetAcceleration();
}

void CLASS(FPSController)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/BodyController);
    DEFINE_FUNCTION(setHAngle);
    DEFINE_FUNCTION(setVAngle);
    DEFINE_FUNCTION(setAcceleration);
    DEFINE_FUNCTION(getAcceleration);
}
