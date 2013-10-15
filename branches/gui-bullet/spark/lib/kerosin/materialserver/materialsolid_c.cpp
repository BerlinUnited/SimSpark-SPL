/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialsolid_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "materialsolid.h"

using namespace zeitgeist;
using namespace kerosin;
using namespace salt;
using namespace boost;

static bool readRGBA(const zeitgeist::ParameterList& in, RGBA& m)
{
    if (
        (in.GetSize() != 4) ||
        (! in.GetValue(in[0], m.r())) ||
        (! in.GetValue(in[1], m.g())) ||
        (! in.GetValue(in[2], m.b())) ||
        (! in.GetValue(in[3], m.a()))
        )
        {
            return false;
        }

    return true;
}

#if 0
static bool
readRGBAVal(const zeitgeist::ParameterList& in, RGBA& m)
{
    int r,g,b;

    if (
        (in.GetSize() != 4) ||
        (! in.GetValue(in[0], r)) ||
        (! in.GetValue(in[1], g)) ||
        (! in.GetValue(in[2], b)) ||
        (! in.GetValue(in[3], m.a()))
        )
    {
        return false;
    }
    m.r() = r / 255.0;
    m.g() = g / 255.0;
    m.b() = b / 255.0;
    return true;
}
#endif

FUNCTION(MaterialSolid,setAmbient)
{
    RGBA m;
    if (! readRGBA(in,m))
        {
            return false;
        }

    obj->SetAmbient(m);
    return true;
}

FUNCTION(MaterialSolid,setDiffuse)
{
    RGBA m;
    if (! readRGBA(in,m))
        {
            return false;
        }

    obj->SetDiffuse(m);
    return true;
}

#if 0
FUNCTION(MaterialSolid,setDiffuseVal)
{
    RGBA m;
    if (! readRGBAVal(in,m))
    {
        return false;
    }

    obj->SetDiffuse(m);
    return true;
}
#endif

FUNCTION(MaterialSolid,setSpecular)
{
    RGBA m;
    if (! readRGBA(in,m))
        {
            return false;
        }

    obj->SetSpecular(m);
    return true;
}

FUNCTION(MaterialSolid,setEmission)
{
    RGBA m;
    if (! readRGBA(in,m))
        {
            return false;
        }

    obj->SetEmission(m);
    return true;
}

FUNCTION(MaterialSolid,setDepthCheck)
{
    bool depthCheck;
    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in[0], depthCheck))
        )
    {
        return false;
    }

    obj->SetDepthCheck(depthCheck);
    return true;
}

FUNCTION(MaterialSolid,setShininess)
{
    float val;
    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in[0], val))
        )
    {
        return false;
    }

    obj->SetShininess(val);
    return true;
}

void CLASS(MaterialSolid)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/Material);
    DEFINE_FUNCTION(setAmbient);
    DEFINE_FUNCTION(setDiffuse);
    //    DEFINE_FUNCTION(setDiffuseVal);
    DEFINE_FUNCTION(setSpecular);
    DEFINE_FUNCTION(setEmission);
    DEFINE_FUNCTION(setDepthCheck);
    DEFINE_FUNCTION(setShininess);
}
