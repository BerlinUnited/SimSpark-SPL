/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2003 Koblenz University
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

#include <oxygen/physicsserver/world.h>

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;

FUNCTION(World,setGravity)
{
    Vector3f inGravity;
    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(),inGravity))
        )
        {
            return false;
        }

    obj->SetGravity(inGravity);
    return true;
}

FUNCTION(World,setERP)
{
    float inERP;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inERP))
        )
        {
            return false;
        }

    obj->SetERP(inERP);
    return true;
}

FUNCTION(World,getERP)
{
    return obj->GetERP();
}

FUNCTION(World,setCFM)
{
    float inCFM;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inCFM))
        )
        {
            return false;
        }

    obj->SetCFM(inCFM);
    return true;
}

FUNCTION(World,getCFM)
{
    return obj->GetCFM();
}

FUNCTION(World,setAutoDisableFlag)
{
    bool inFlag;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inFlag))
        )
        {
            return false;
        }

    obj->SetAutoDisableFlag(inFlag);
    return true;
}

FUNCTION(World,getAutoDisableFlag)
{
    return obj->GetAutoDisableFlag();
}

FUNCTION(World,setContactSurfaceLayer)
{
    float inDepth;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inDepth))
        )
        {
            return false;
        }

    obj->SetContactSurfaceLayer(inDepth);
    return true;
}

FUNCTION(World,getContactSurfaceLayer)
{
    return obj->GetContactSurfaceLayer();
}

void CLASS(World)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/PhysicsObject);
    DEFINE_FUNCTION(setGravity);
    DEFINE_FUNCTION(setERP);
    DEFINE_FUNCTION(getERP);
    DEFINE_FUNCTION(setCFM);
    DEFINE_FUNCTION(getCFM);
    DEFINE_FUNCTION(setAutoDisableFlag);
    DEFINE_FUNCTION(getAutoDisableFlag);
    DEFINE_FUNCTION(setContactSurfaceLayer);
    DEFINE_FUNCTION(getContactSurfaceLayer);
}
