/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: ccylindercollider_c.cpp,v 1.6 2004/03/22 10:56:05 rollmark Exp $

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

#include "ccylindercollider.h"

using namespace boost;
using namespace oxygen;

FUNCTION(CCylinderCollider,setParams)
{
    float inRadius;
    float inLength;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inRadius)) ||
        (! in.GetValue(in[1],inLength))
        )
        {
            return false;
        }

    obj->SetParams(inRadius,inLength);
    return true;
}

void CLASS(CCylinderCollider)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/Collider);
        DEFINE_FUNCTION(setParams);
}
