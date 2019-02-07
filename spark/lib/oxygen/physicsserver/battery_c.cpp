/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Copyright (C) 2011 RoboCup Soccer Server 3D Maintenance Group
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

#include <oxygen/physicsserver/battery.h>

using namespace oxygen;

FUNCTION(Battery,setCapacity)
{
    float inValue;

    if (in.GetSize() != 1
        || (!in.GetValue(in[0], inValue)))
    {
      return false;
    }

    obj->SetCapacity(inValue);
    return true;
}

FUNCTION(Battery,setPower)
{
    float inValue;

    if (in.GetSize() != 1
        || (!in.GetValue(in[0], inValue)))
    {
      return false;
    }

    obj->SetPower(inValue);
    return true;
}

void CLASS(Battery)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/PhysicsObject);
        DEFINE_FUNCTION(setCapacity);
        DEFINE_FUNCTION(setPower);
}
