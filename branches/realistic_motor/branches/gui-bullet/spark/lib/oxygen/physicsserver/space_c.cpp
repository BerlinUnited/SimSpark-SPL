/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space_c.cpp 176 2010-02-25 12:19:37Z a-held $

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

#include "space.h"

using namespace oxygen;

FUNCTION(Space,disableInnerCollision)
{
    bool inSet;
    if ((in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet)))
    {
        return false;
    }

    obj->DisableInnerCollision(inSet);
    return true;
}

void CLASS(Space)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/PhysicsObject);
    DEFINE_FUNCTION(disableInnerCollision);
}
