/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: material2dtexture_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "material2dtexture.h"

using namespace zeitgeist;
using namespace kerosin;
using namespace std;

FUNCTION(Material2DTexture,setDiffuseTexture)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inName))
        )
    {
        return false;
    }

    return obj->SetDiffuseTexture(inName);
}

FUNCTION(Material2DTexture,setNormalTexture)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inName))
        )
    {
        return false;
    }

    return obj->SetNormalTexture(inName);
}

FUNCTION(Material2DTexture,setSpecularTexture)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inName))
        )
    {
        return false;
    }

    return obj->SetSpecularTexture(inName);
}

void CLASS(Material2DTexture)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/MaterialSolid);
    DEFINE_FUNCTION(setDiffuseTexture);
    DEFINE_FUNCTION(setNormalTexture);
    DEFINE_FUNCTION(setSpecularTexture);
}
