/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: axis.cpp,v 1.8 2008/02/24 10:17:31 rollmark Exp $

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
#include "axis.h"
#include "../openglserver/openglwrapper.h"
#include <kerosin/openglserver/glbase.h>

using namespace kerosin;

Axis::Axis()
    : BaseNode(), mSize(10.0f)
{
}

void Axis::RenderAxis(float size)
{
    RGBA colX(1,0,0,1);
    RGBA colY(0,1,0,1);
    RGBA colZ(0,0,1,1);

    // X
    glColor3fv(colX);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colX);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(size,0,0);
    glEnd();

    // Y
    glColor3fv(colY);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colY);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(0,size,0);
    glEnd();

    // Z
    glColor3fv(colZ);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colZ);
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(0,0,size);
    glEnd();
}

void Axis::RenderInternal()
{
    RenderAxis(mSize);
}

float Axis::GetSize()
{
    return mSize;
}

void Axis::SetSize(float size)
{
    mSize = std::max<float>(0.1f,size);
}

void Axis::ComputeBoundingBox()
{
    mLocalBoundingBox.minVec.Set(0.0, 0.0, 0.0);
    mLocalBoundingBox.maxVec.Set(mSize,mSize,mSize);
}
