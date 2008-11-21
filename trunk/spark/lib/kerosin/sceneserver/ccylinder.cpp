/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ccylinder.cpp,v 1.3 2004/04/22 17:32:46 rollmark Exp $

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
#include "ccylinder.h"
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;

CCylinder::CCylinder() : SingleMatNode()
{
}

CCylinder::~CCylinder()
{
}

void CCylinder::SetParams(float radius, float length)
{
    ParameterList parameter;
    parameter.AddValue(radius);
    parameter.AddValue(length);

    Load("StdCCylinder",parameter);

    mRadius = radius;
    mLength = length;
}

void CCylinder::GetParams(float& radius, float& length) const
{
    radius = mRadius;
    length = mLength;
}

float CCylinder::GetRadius()
{
    return mRadius;
}

float CCylinder::GetLength()
{
    return mLength;
}
