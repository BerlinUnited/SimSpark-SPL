/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: camera_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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

#include "camera.h"

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;

FUNCTION(Camera,setViewport)
{
    int inX;
    int inY;
    int inWidth;
    int inHeight;

    if (
        (in.GetSize() != 4) ||
        (! in.GetValue(in[0],inX)) ||
        (! in.GetValue(in[1],inY)) ||
        (! in.GetValue(in[2],inWidth)) ||
        (! in.GetValue(in[3],inHeight))
        )
        {
            return false;
        }

    obj->SetViewport(inX,inY,inWidth,inHeight);
    return true;
}

FUNCTION(Camera,getViewportX)
{
    return obj->GetViewportX();
}

FUNCTION(Camera,getViewportY)
{
    return obj->GetViewportY();
}

FUNCTION(Camera,getViewportWidth)
{
    return obj->GetViewportWidth();
}

FUNCTION(Camera,getViewportHeight)
{
    return obj->GetViewportHeight();
}

FUNCTION(Camera,setFOV)
{
    float inFov;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inFov))
        )
        {
            return false;
        }

    obj->SetFOV(inFov);
    return true;
}

FUNCTION(Camera,getFOV)
{
    return obj->GetFOV();
}

FUNCTION(Camera,setZNear)
{
    float inZNear;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inZNear))
        )
        {
            return false;
        }

    obj->SetZNear(inZNear);
    return true;
}

FUNCTION(Camera,adjustZNear)
{
    float inZNear;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inZNear))
        )
        {
            return false;
        }

    obj->AdjustZNear(inZNear);
    return true;
}

FUNCTION(Camera,getZNear)
{
    return obj->GetZNear();
}

FUNCTION(Camera,setZFar)
{
    float inZFar;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inZFar))
        )
        {
            return false;
        }

    obj->SetZFar(inZFar);
    return true;
}

FUNCTION(Camera,adjustZFar)
{
    float inZFar;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inZFar))
        )
        {
            return false;
        }

    obj->AdjustZFar(inZFar);
    return true;
}

FUNCTION(Camera,getZFar)
{
    return obj->GetZFar();
}

FUNCTION(Camera,lookAt)
{
    float inX;
    float inY;
    float inZ;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inX)) ||
        (! in.GetValue(in[1],inY)) ||
        (! in.GetValue(in[2],inZ))
        )
        {
            return false;
        }

    obj->LookAt(salt::Vector3f(inX,inY,inZ));
    return true;
}

void CLASS(Camera)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/BaseNode);
    DEFINE_FUNCTION(setViewport);
    DEFINE_FUNCTION(getViewportX);
    DEFINE_FUNCTION(getViewportY);
    DEFINE_FUNCTION(getViewportWidth);
    DEFINE_FUNCTION(getViewportHeight);
    DEFINE_FUNCTION(setFOV);
    DEFINE_FUNCTION(getFOV);
    DEFINE_FUNCTION(setZNear);
    DEFINE_FUNCTION(adjustZNear);
    DEFINE_FUNCTION(getZNear);
    DEFINE_FUNCTION(setZFar);
    DEFINE_FUNCTION(adjustZFar);
    DEFINE_FUNCTION(getZFar);
    DEFINE_FUNCTION(lookAt);
}
