/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri Oct 20 2006
   Copyright (C) 2006 RoboCup Soccer Server 3D Maintenance Group

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
#include "cylinder.h"
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;

Cylinder::Cylinder() : SingleMatNode()
{
}

Cylinder::~Cylinder()
{
}

void Cylinder::SetParams(float radius, float length)
{
    ParameterList parameter;
    parameter.AddValue(radius);
    parameter.AddValue(length);

    Load("StdUnitCylinder",parameter);

    mRadius = radius;
    mLength = length;
}

void Cylinder::GetParams(float& radius, float& length) const
{
    radius = mRadius;
    length = mLength;
}

float Cylinder::GetRadius()
{
    return mRadius;
}

float Cylinder::GetLength()
{
    return mLength;
}
