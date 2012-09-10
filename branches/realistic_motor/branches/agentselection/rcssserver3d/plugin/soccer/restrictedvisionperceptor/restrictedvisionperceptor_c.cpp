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

#include "restrictedvisionperceptor.h"

using namespace boost;
using namespace oxygen;
using namespace std;

FUNCTION(RestrictedVisionPerceptor,setNoiseParams)
{
    float inDist;
    float inPhi;
    float inTheta;
    float inErrorAbs;

    if (
        (in.GetSize() != 4) ||
        (! in.GetValue(in[0],inDist)) ||
        (! in.GetValue(in[1],inPhi)) ||
        (! in.GetValue(in[2],inTheta)) ||
        (! in.GetValue(in[3],inErrorAbs))
        )
        {
            return false;
        }

    obj->SetNoiseParams(inDist,inPhi,inTheta,inErrorAbs);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,addNoise)
{
    bool inAddNoise;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAddNoise))
        )
    {
        return false;
    }

    obj->AddNoise(inAddNoise);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setSenseMyPos)
{
    bool inSenseMyPos;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inSenseMyPos))
        )
        {
            return false;
        }

    obj->SetSenseMyPos(inSenseMyPos);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setStaticSenseAxis)
{
    bool inStaticAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inStaticAxis))
        )
        {
            return false;
        }

    obj->SetStaticSenseAxis(inStaticAxis);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setViewCones)
{
    int inHAngle;
    int inVAngle;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inHAngle)) ||
        (! in.GetValue(in[1],inVAngle))
        )
        {
            return false;
        }

    obj->SetViewCones(inHAngle,inVAngle);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setPanRange)
{
    int inLower;
    int inUpper;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inLower)) ||
        (! in.GetValue(in[1],inUpper))
        )
        {
            return false;
        }

    obj->SetPanRange(inLower,inUpper);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setTiltRange)
{
    int inLower;
    int inUpper;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inLower)) ||
        (! in.GetValue(in[1],inUpper))
        )
        {
            return false;
        }

    obj->SetTiltRange(inLower,inUpper);
    return true;
}

FUNCTION(RestrictedVisionPerceptor,setSenseLine)
{
    bool inSenseLine;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inSenseLine))
        )
        {
            return false;
        }

    obj->SetSenseLine(inSenseLine);
    return true;
}

void CLASS(RestrictedVisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(addNoise);
    DEFINE_FUNCTION(setSenseMyPos);
    DEFINE_FUNCTION(setStaticSenseAxis);
    DEFINE_FUNCTION(setViewCones);
    DEFINE_FUNCTION(setPanRange);
    DEFINE_FUNCTION(setTiltRange);
    DEFINE_FUNCTION(setSenseLine);
}
