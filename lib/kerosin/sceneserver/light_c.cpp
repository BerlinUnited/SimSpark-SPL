/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: light_c.cpp,v 1.5 2004/03/22 11:09:04 rollmark Exp $

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

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "light.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;

FUNCTION(Light,setRadius)
{
    float inRadius;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inRadius))
        )
        {
            return false;
        }

    obj->SetRadius(inRadius);
    return true;
}

FUNCTION(Light,getRadius)
{
    return obj->GetRadius();
}

FUNCTION(Light,setDiffuseColor)
{
    Vector3f inColor;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inColor))
        )
        {
            return false;
        }

    obj->SetDiffuseColor(inColor);
    return true;
}

void CLASS(Light)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/BaseNode);
    DEFINE_FUNCTION(setRadius);
    DEFINE_FUNCTION(getRadius);
    DEFINE_FUNCTION(setDiffuseColor);
}
