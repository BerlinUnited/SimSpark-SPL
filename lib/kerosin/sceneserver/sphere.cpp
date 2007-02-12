/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sphere.cpp,v 1.9 2007/02/12 19:45:10 rollmark Exp $

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
#include "sphere.h"
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>

using namespace kerosin;
using namespace zeitgeist;
using namespace boost;
using namespace salt;

Sphere::Sphere() : SingleMatNode()
{
}

Sphere::~Sphere()
{
}

void Sphere::OnLink()
{
    Load("StdUnitSphere");
}
float Sphere::GetRadius() const
{
    return mScale[0];
}

void Sphere::SetRadius(float radius)
{
    mScale[0] = radius;
    mScale[1] = radius;
    mScale[2] = radius;
    CalcBoundingBox();
}
