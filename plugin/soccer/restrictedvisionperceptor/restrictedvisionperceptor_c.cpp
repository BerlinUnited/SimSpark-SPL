/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: restrictedvisionperceptor_c.cpp,v 1.1 2006/02/28 17:20:45 jamu Exp $

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

FUNCTION(RestrictedVisionPerceptor,useRandomNoise)
{
    bool inRandomNoise;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inRandomNoise))
        )
    {
        return false;
    }

    obj->UseRandomNoise(inRandomNoise);
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

FUNCTION(RestrictedVisionPerceptor,setVisionAngles)
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

    obj->SetVisionAngles(inHAngle,inVAngle);
    return true;
}

void CLASS(RestrictedVisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(addNoise);
    DEFINE_FUNCTION(useRandomNoise);
    DEFINE_FUNCTION(setSenseMyPos);
    DEFINE_FUNCTION(setStaticSenseAxis);
    DEFINE_FUNCTION(setVisionAngles);
}
