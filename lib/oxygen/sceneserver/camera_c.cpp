/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: camera_c.cpp,v 1.4 2004/02/21 15:30:44 fruit Exp $

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

FUNCTION(setViewport)
{
        if (in.size() == 4)
        {
                Camera *cam = static_cast<Camera*>(obj);
                cam->SetViewport(any_cast<int>(in[0]), any_cast<int>(in[1]), any_cast<int>(in[2]), any_cast<int>(in[3]));
        }
}

FUNCTION(setFOV)
{
        if (in.size() == 1)
        {
                Camera *cam = static_cast<Camera*>(obj);
                cam->SetFOV(any_cast<float>(in[0]));
        }
}

FUNCTION(setZNear)
{
        if (in.size() == 1)
        {
                Camera *cam = static_cast<Camera*>(obj);
                cam->SetZNear(any_cast<float>(in[0]));
        }
}

FUNCTION(setZFar)
{
        if (in.size() == 1)
        {
                Camera *cam = static_cast<Camera*>(obj);
                cam->SetZFar(any_cast<float>(in[0]));
        }
}

void CLASS(Camera)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/BaseNode);
        DEFINE_FUNCTION(setViewport);
        DEFINE_FUNCTION(setFOV);
        DEFINE_FUNCTION(setZNear);
        DEFINE_FUNCTION(setZFar);
}
