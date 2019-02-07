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

#include "driveeffector.h"

using namespace oxygen;

FUNCTION(DriveEffector,setForceFactor)
{
    float inForceFactor;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inForceFactor))
        )
        {
            return false;
        }

    obj->SetForceFactor(inForceFactor);
    return true;
}

FUNCTION(DriveEffector,setSigma)
{
    float inSigma;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inSigma))
        )
        {
            return false;
        }

    obj->SetSigma(inSigma);
    return true;
}

FUNCTION(DriveEffector,setMaxPower)
{
    float inMaxPower;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inMaxPower))
        )
        {
            return false;
        }

    obj->SetMaxPower(inMaxPower);
    return true;
}

FUNCTION(DriveEffector,setConsumption)
{
    float inConsumption;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inConsumption))
        )
        {
            return false;
        }

    obj->SetConsumption(inConsumption);
    return true;
}

void
CLASS(DriveEffector)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Effector);
    DEFINE_FUNCTION(setForceFactor);
    DEFINE_FUNCTION(setSigma);
    DEFINE_FUNCTION(setMaxPower);
    DEFINE_FUNCTION(setConsumption);
}
