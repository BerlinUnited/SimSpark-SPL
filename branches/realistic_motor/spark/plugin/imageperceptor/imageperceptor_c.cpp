/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

#include "imageperceptor.h"

using namespace boost;
using namespace oxygen;

FUNCTION(ImagePerceptor, setResolution)
{
    unsigned int w,h;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], w) ) ||
        (! in.GetValue(in[1], h) )
         )
    {
        return false;
    }

    obj->SetResolution(w, h);
    return true;
}

FUNCTION(ImagePerceptor, setFOV)
{
    float fov;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], fov) )
         )
    {
        return false;
    }

    obj->SetFOV(fov);
    return true;
}

FUNCTION(ImagePerceptor, setZNear)
{
    float zNear;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], zNear) )
         )
    {
        return false;
    }

    obj->SetZNear(zNear);
    return true;
}

FUNCTION(ImagePerceptor, setZFar)
{
    float zFar;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], zFar) )
         )
    {
        return false;
    }

    obj->SetZFar(zFar);
    return true;
}

FUNCTION(ImagePerceptor, setOffScreen)
{
    bool offScreen;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], offScreen) )
         )
    {
        return false;
    }

    obj->SetOffScreen(offScreen);
    return true;
}

void CLASS(ImagePerceptor)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Perceptor);
    DEFINE_FUNCTION(setResolution);
    DEFINE_FUNCTION(setFOV);
    DEFINE_FUNCTION(setZNear);
    DEFINE_FUNCTION(setZFar);
    DEFINE_FUNCTION(setOffScreen);
}
