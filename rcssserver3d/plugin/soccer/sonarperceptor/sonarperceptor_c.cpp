/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "sonarperceptor.h"

using namespace boost;
using namespace oxygen;


FUNCTION(SonarPerceptor, setMaxEchoes)
{
    unsigned int inMaxEchoes;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inMaxEchoes))
        )
        {
            return false;
        }

    obj->SetMaxEchoes(inMaxEchoes);
    return true;
}

FUNCTION(SonarPerceptor, setRange)
{
    float inMin;
    float inMax;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inMin)) ||
        (! in.GetValue(in[1],inMax))
        )
        {
            return false;
        }

    obj->SetRange(inMin, inMax);
    return true;
}

FUNCTION(SonarPerceptor, setSenseCone)
{
    unsigned int inAngle;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inAngle))
        )
        {
            return false;
        }

    obj->SetSenseCone(inAngle);
    return true;
}

FUNCTION(SonarPerceptor,addNoise)
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


void CLASS(SonarPerceptor)::DefineClass()
{
  DEFINE_BASECLASS(oxygen/Perceptor);
  DEFINE_FUNCTION(setMaxEchoes);
  DEFINE_FUNCTION(setRange);
  DEFINE_FUNCTION(setSenseCone);
  DEFINE_FUNCTION(addNoise);
}
