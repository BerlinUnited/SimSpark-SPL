/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: axis.cpp,v 1.4 2004/04/12 17:19:48 rollmark Exp $

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
#include "../openglserver/openglserver.h"
#include <kerosin/openglserver/glbase.h>

using namespace kerosin;

Axis::Axis() : mSize(10.0f)
{
}

void Axis::RenderInternal()
{
    RGBA colX(1,0,0,1);
    RGBA colY(0,1,0,1);
    RGBA colZ(0,0,1,1);

    // X
    glColor3fv(colX);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colX);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(mSize,0,0);
    glEnd();

    // Y
    glColor3fv(colY);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colY);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(0,mSize,0);
    glEnd();

    // Z
    glColor3fv(colZ);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,colZ);
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(0,0,mSize);
    glEnd();
}

float Axis::GetSize()
{
    return mSize;
}

void Axis::SetSize(float size)
{
    mSize = std::max<float>(0.1f,size);
}

