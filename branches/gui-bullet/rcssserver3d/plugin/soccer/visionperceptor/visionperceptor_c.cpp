/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor_c.cpp 9 2008-11-24 02:39:02Z hedayat $

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

#include "visionperceptor.h"

using namespace boost;
using namespace oxygen;
using namespace std;

FUNCTION(VisionPerceptor,setNoiseParams)
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

FUNCTION(VisionPerceptor,addNoise)
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

FUNCTION(VisionPerceptor,useRandomNoise)
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

FUNCTION(VisionPerceptor,setSenseMyPos)
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

FUNCTION(VisionPerceptor,setStaticSenseAxis)
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

void CLASS(VisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(addNoise);
    DEFINE_FUNCTION(useRandomNoise);
    DEFINE_FUNCTION(setSenseMyPos);
    DEFINE_FUNCTION(setStaticSenseAxis);
}
