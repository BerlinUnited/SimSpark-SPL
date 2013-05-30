/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: randomserver_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "randomserver.h"
#include <cmath>

using namespace boost;
using namespace zeitgeist;

FUNCTION(RandomServer,seed)
{
    int inSeed;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inSeed))
        )
        {
            return false;
        }

    obj->Seed(inSeed);
    return true;
}

FUNCTION(RandomServer,uniformRND)
{
    float inMin;
    float inMax;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inMin)) ||
        (! in.GetValue(in[1],inMax))
        )
        {
            return 0.0f;
        }

    return obj->GetUniformRandom(inMin,inMax);
}

FUNCTION(RandomServer,normalRND)
{
    float inMean;
    float inSigma;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inMean)) ||
        (! in.GetValue(in[1],inSigma))
        )
        {
            return 0.0f;
        }

    return obj->GetNormalRandom(inMean,inSigma);
}

FUNCTION(RandomServer,exponentialRND)
{
    float inLambda;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inLambda))
        )
        {
            return 0.0f;
        }

    return obj->GetExponentialRandom(inLambda);
}

void
CLASS(RandomServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(seed);
    DEFINE_FUNCTION(uniformRND);
    DEFINE_FUNCTION(normalRND);
    DEFINE_FUNCTION(exponentialRND);
}
