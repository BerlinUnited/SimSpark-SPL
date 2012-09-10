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

#include "kickeffector.h"

using namespace oxygen;

FUNCTION(KickEffector,setKickMargin)
{
    float inMargin;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inMargin))
        )
        {
            return false;
        }

    obj->SetKickMargin(inMargin);
    return true;
}

FUNCTION(KickEffector,setForceFactor)
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

FUNCTION(KickEffector,setTorqueFactor)
{
    float inTorqueFactor;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inTorqueFactor))
        )
        {
            return false;
        }

    obj->SetTorqueFactor(inTorqueFactor);
    return true;
}

FUNCTION(KickEffector,setSteps)
{
    int steps;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], steps))
        )
        {
            return false;
        }

    obj->SetSteps(steps);
    return true;
}

FUNCTION(KickEffector,setNoiseParams)
{
    float inForce;
    float inTheta;
    float inPhiEnd;
    float inPhiMid;

    if (
        (in.GetSize() != 4) ||
        (! in.GetValue(in[0], inForce)) ||
        (! in.GetValue(in[1], inTheta)) ||
        (! in.GetValue(in[2], inPhiEnd)) ||
        (! in.GetValue(in[3], inPhiMid))
        )
        {
            return false;
        }

    obj->SetNoiseParams(inForce,inTheta,inPhiEnd,inPhiMid);
    return true;
}

FUNCTION(KickEffector,setMaxPower)
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

FUNCTION(KickEffector,setAngleRange)
{
    float inMin;
    float inMax;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inMin)) ||
        (! in.GetValue(in[1], inMax))
        )
        {
            return false;
        }

    obj->SetAngleRange(inMin,inMax);
    return true;
}

void CLASS(KickEffector)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Effector);
    DEFINE_FUNCTION(setKickMargin);
    DEFINE_FUNCTION(setForceFactor);
    DEFINE_FUNCTION(setTorqueFactor);
    DEFINE_FUNCTION(setSteps);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(setMaxPower);
    DEFINE_FUNCTION(setAngleRange);
}
