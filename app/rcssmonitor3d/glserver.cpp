/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.cpp,v 1.3.2.2 2004/01/25 13:07:05 rollmark Exp $

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
#include "glserver.h"

GLserver::GLserver(int width, int height,
                   salt::Vector3f camPos,
                   salt::Vector3f lookAtPos,
                   salt::Vector3f up, bool wire)
{
    mCamPos  = camPos;
    mLookAt  = lookAtPos;
    mUp      = up;

    mWidth     = width;
    mHeight    = height;
    mWireframe = wire;

    mCamera = Camera(camPos, lookAtPos, up);
}

//---------------------------------ApplyCamera--------------------------
//
//----------------------------------------------------------------------
void GLserver::ApplyCamera()
{
    mCamera.Look();
}

//---------------------------------initGL-------------------------------
//
// OpenGL initialisation function
//----------------------------------------------------------------------
void GLserver::InitGL (void)
{
    if(!mWireframe)
        {
            GLfloat position[] = { 0.5, 1.5, 1.0, 0.0 };
            GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
            GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

            glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_NORMALIZE);
            glShadeModel(GL_SMOOTH);

            glLightfv(GL_LIGHT0, GL_POSITION, position);
        }

    glEnable(GL_DEPTH_TEST);

    glViewport(0,0,mWidth,mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)mWidth/(GLfloat)mHeight,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//--------------------------drawGround-----------------------------------
//
// draws a virtual grid ground out of several green lines
//-----------------------------------------------------------------------
void GLserver::DrawGround(salt::Vector3f gridPos, float szX, float szZ)
{
    glPushMatrix();
    glTranslatef(gridPos[0],gridPos[1], gridPos[2]);

    //draw the lines GREEN
    glBegin(GL_LINES);

    for(float x = 0; x <= szX; ++x)
        {
            // Do the vertical lines (along the Z)
            glVertex3f(x, 0, 0);
            glVertex3f(x, 0, szZ);
        }

    for(float z = 0; z <= szZ; ++z)
        {
            // Do the vertical lines (along the Z)
            glVertex3f(0, 0, z);
            glVertex3f(szX, 0, z);
        }

    glEnd();

    glPopMatrix();
}

//------------------------drawWireBox-------------------------------------
//
// draws a wireframbox with given dimensions to position 'boxPos'
//-----------------------------------------------------------------------
void GLserver::DrawWireBox(salt::Vector3f boxPos, salt::Vector3f sz)
{
    glPushMatrix();
    glTranslatef(boxPos[0],boxPos[1], boxPos[2]);

    // ground plane
    glBegin(GL_LINE_LOOP);
    glVertex3f(0    ,0,0);
    glVertex3f(sz[0],0,0);
    glVertex3f(sz[0],0,sz[2]);
    glVertex3f(0    ,0,sz[2]);
    glEnd();

    // top plane
    glBegin(GL_LINE_LOOP);
    glVertex3f(0    ,sz[1],0);
    glVertex3f(sz[0],sz[1],0);
    glVertex3f(sz[0],sz[1],sz[2]);
    glVertex3f(0    ,sz[1],sz[2]);
    glEnd();

    // sides
    glBegin(GL_LINES);
    glVertex3f(0    ,0,0);
    glVertex3f(0    ,sz[1],0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(sz[0],0,0);
    glVertex3f(sz[0],sz[1],0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(sz[0],0,sz[2]);
    glVertex3f(sz[0],sz[1],sz[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0    ,0,sz[2]);
    glVertex3f(0    ,sz[1],sz[2]);
    glEnd();

    glPopMatrix();
}

//------------------------drawSphere-------------------------------------
//
// draws a solid sphere with given radius to position 'spherePos'
//-----------------------------------------------------------------------
void GLserver::DrawSphere(salt::Vector3f spherePos,float radius)
{
    glPushMatrix();
    glTranslatef(spherePos[0],spherePos[1], spherePos[2]);
    if(!mWireframe) glutSolidSphere(radius, 10, 10);
    else glutWireSphere(radius, 10, 10);
    glPopMatrix();
}



