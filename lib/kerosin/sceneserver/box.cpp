/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: box.cpp,v 1.1 2004/03/20 15:50:09 rollmark Exp $

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
#include "box.h"
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;

Box::Box() : SingleMatNode(), mExtents(1.0f,1.0f,1.0f)
{
}

Box::~Box()
{
}

void Box::SetExtents(const salt::Vector3f& extents)
{
  mExtents = extents;
}

const Vector3f& Box::GetExtents()
{
  return mExtents;
}

void Box::RenderInternal()
{
    if (mMaterial.get() == 0)
        {
            return;
        }

    mMaterial->Bind();

    glScalef(
             mExtents.x() * 0.5,
             mExtents.y() * 0.5,
             mExtents.z() * 0.5
             );

    static Vector3f v[8]  = {
      Vector3f(+1.0f,+1.0f,+1.0f),
      Vector3f(-1.0f,+1.0f,+1.0f),
      Vector3f(+1.0f,-1.0f,+1.0f),
      Vector3f(-1.0f,-1.0f,+1.0f),
      Vector3f(+1.0f,+1.0f,-1.0f),
      Vector3f(-1.0f,+1.0f,-1.0f),
      Vector3f(+1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f,-1.0f)
    };

    glBegin(GL_QUADS);

    // front
    glVertex3fv(v[0].GetData());
    glVertex3fv(v[1].GetData());
    glVertex3fv(v[3].GetData());
    glVertex3fv(v[2].GetData());
    // back
    glVertex3fv(v[6].GetData());
    glVertex3fv(v[7].GetData());
    glVertex3fv(v[5].GetData());
    glVertex3fv(v[4].GetData());
    // left
    glVertex3fv(v[5].GetData());
    glVertex3fv(v[7].GetData());
    glVertex3fv(v[3].GetData());
    glVertex3fv(v[1].GetData());
    // right
    glVertex3fv(v[0].GetData());
    glVertex3fv(v[2].GetData());
    glVertex3fv(v[6].GetData());
    glVertex3fv(v[4].GetData());
    // top
    glVertex3fv(v[4].GetData());
    glVertex3fv(v[5].GetData());
    glVertex3fv(v[1].GetData());
    glVertex3fv(v[0].GetData());
    // bottom
    glVertex3fv(v[2].GetData());
    glVertex3fv(v[3].GetData());
    glVertex3fv(v[7].GetData());
    glVertex3fv(v[6].GetData());

    glEnd();
}
