/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor_c.cpp,v 1.5 2004/03/22 11:20:59 rollmark Exp $

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

#include "perfectvisionperceptor.h"

using namespace boost;
using namespace oxygen;

FUNCTION(PerfectVisionPerceptor,setInversion)
{
    bool inInvertX;
    bool inInvertY;
    bool inInvertZ;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inInvertX)) ||
        (! in.GetValue(in[1],inInvertY)) ||
        (! in.GetValue(in[2],inInvertZ))
        )
        {
            return false;
        }

    obj->SetInversion(inInvertX,inInvertY,inInvertZ);
}

void
CLASS(PerfectVisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setInversion);
}
