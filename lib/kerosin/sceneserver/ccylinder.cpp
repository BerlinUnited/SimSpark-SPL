/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ccylinder.cpp,v 1.2 2004/04/19 15:45:57 rollmark Exp $

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

CCylinder::CCylinder() : SingleMatNode(), mRadius(1.0f), mLength(1.0f)
{
}

CCylinder::~CCylinder()
{
}

void CCylinder::SetParams(float radius, float length)
{
    mRadius = radius;
    mLength = length;
}

void CCylinder::RenderInternal()
{
    shared_ptr<Material> material = GetMaterial();
    if (material.get() == 0)
        {
            return;
        }

    material->Bind();

    //
    // code adapted from ODE's drawstuff lib
    //

    // (float l, float r)
    int i,j;
    float tmp,nx,ny,nz,start_nx,start_ny,a,ca,sa,l;

    // number of sides to the cylinder (divisible by 4):
    const int capped_cylinder_quality = 3;
    const int n = capped_cylinder_quality*4;

    l = mLength * 0.5;
    a = g2PI / n;
    sa = gSin(a);
    ca = gCos(a);

    // draw cylinder body

    // normal vector = (0,ny,nz)
    ny = 1;
    nz = 0;

    glBegin (GL_TRIANGLE_STRIP);
    for (i=0; i<=n; ++i)
        {
            glNormal3d (ny,nz,0);
            glVertex3d (ny*mRadius,nz*mRadius,l);
            glNormal3d (ny,nz,0);
            glVertex3d (ny*mRadius,nz*mRadius,-l);

            // rotate ny,nz
            tmp = ca * ny - sa * nz;
            nz = sa*ny + ca*nz;
            ny = tmp;
        }

    glEnd();

    // draw first cylinder cap
    start_nx = 0;
    start_ny = 1;

    for (j=0; j<(n/4); ++j)
        {
            // get start_n2 = rotated start_n
            float start_nx2 =  ca*start_nx + sa*start_ny;
            float start_ny2 = -sa*start_nx + ca*start_ny;

            // get n=start_n and n2=start_n2
            nx = start_nx;
            ny = start_ny;
            nz = 0;
            float nx2 = start_nx2;
            float ny2 = start_ny2;
            float nz2 = 0;

            glBegin (GL_TRIANGLE_STRIP);

            for (i=0; i<=n; i++)
                {
                    glNormal3d (ny2,nz2,nx2);
                    glVertex3d (ny2*mRadius,nz2*mRadius,l+nx2*mRadius);
                    glNormal3d (ny,nz,nx);
                    glVertex3d (ny*mRadius,nz*mRadius,l+nx*mRadius);

                    // rotate n,n2
                    tmp = ca*ny - sa*nz;
                    nz = sa*ny + ca*nz;
                    ny = tmp;
                    tmp = ca*ny2- sa*nz2;
                    nz2 = sa*ny2 + ca*nz2;
                    ny2 = tmp;
                }

            glEnd();
            start_nx = start_nx2;
            start_ny = start_ny2;
        }

    // draw second cylinder cap
    start_nx = 0;
    start_ny = 1;

    for (j=0; j<(n/4); j++)
        {
            // get start_n2 = rotated start_n
            float start_nx2 = ca*start_nx - sa*start_ny;
            float start_ny2 = sa*start_nx + ca*start_ny;

            // get n=start_n and n2=start_n2
            nx = start_nx; ny = start_ny; nz = 0;
            float nx2 = start_nx2;
            float ny2 = start_ny2;
            float nz2 = 0;

            glBegin (GL_TRIANGLE_STRIP);

            for (i=0; i<=n; i++)
                {
                    glNormal3d (ny,nz,nx);
                    glVertex3d (ny*mRadius,nz*mRadius,-l+nx*mRadius);
                    glNormal3d (ny2,nz2,nx2);
                    glVertex3d (ny2*mRadius,nz2*mRadius,-l+nx2*mRadius);
                    // rotate n,n2
                    tmp = ca*ny - sa*nz;
                    nz = sa*ny + ca*nz;
                    ny = tmp;
                    tmp = ca*ny2- sa*nz2;
                    nz2 = sa*ny2 + ca*nz2;
                    ny2 = tmp;
                }

            glEnd();

            start_nx = start_nx2;
            start_ny = start_ny2;
        }

    glEnd();
}
