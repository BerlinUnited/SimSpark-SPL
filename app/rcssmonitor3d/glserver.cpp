/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.cpp,v 1.4 2004/02/12 14:07:21 fruit Exp $

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

GLServer::GLServer(int width, int height,
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
void GLServer::ApplyCamera()
{
    //switch z-buffer on
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    //create a viewport
    glViewport(0,0,mWidth,mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //setup camera angle, ratio, near and far clipping plane
    gluPerspective(45.0f,(GLfloat)mWidth/(GLfloat)mHeight,0.1f,200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mCamera.Look();
}

//---------------------------------initGL-------------------------------
//
// OpenGL initialisation function
//----------------------------------------------------------------------
void GLServer::InitGL (void)
{
    //setup lightsource
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
}

void GLServer::DrawTextPix(const char* text, salt::Vector2f pix)
{
    const salt::Vector2f pos(
                             pix[0] / (float)mWidth * 2 - 1.0f,
                             1.0f - (pix[1] / (float)mHeight * 2)
                             );

    DrawText(text,pos);
}

int GLServer::GetTextHeight()
{
    // currently only GLUT_BITMAP_9_BY_15 is used
    return 15;
}

void GLServer::DrawText3D(const char* text, salt::Vector3f pos)
{
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_LIGHTING);

    glRasterPos3f(pos[0],pos[1],pos[2]);
    for (const char* s = text; *s; ++s)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
        }

    glEnable (GL_LIGHTING);
    glEnable (GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
}

//--------------------------drawText-------------------------------------
//draws a given text string onto the screen at position pos;; (-1,1)
//is top left, (+1,-1) is bottom right of the viewport
//-----------------------------------------------------------------------
void GLServer::DrawText(const char* text, salt::Vector2f pos)
{
    glDisable (GL_LIGHTING);
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_DEPTH_TEST);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho (0.0,mWidth,0.0,mHeight,-0.0,0.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glRasterPos2f(pos[0],pos[1]);
    for (const char* s = text; *s; ++s)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
        }
}

//--------------------------drawGround-----------------------------------
//
// draws a virtual grid ground out of several green lines
//-----------------------------------------------------------------------
void GLServer::DrawGround(salt::Vector3f gridPos, float szX, float szZ)
{
    // The ground is drawn as a set of quadric faces(strips)
    // faceNum stores the number of faces
    const int faceNum = 10;
    GLfloat x, z, deltaX, deltaZ;

    glPushMatrix();
    glTranslatef(gridPos[0],gridPos[1], gridPos[2]);

    // store the sizes of our faces
    // and create normal vector
    deltaX = szX/faceNum;
    deltaZ = szZ/faceNum;
    glNormal3f(0,1,0);
    for (int i=0; i<faceNum; i++)
        {
            z = i*deltaZ;
            //draw face as Quadric strip
            glBegin(GL_QUAD_STRIP);
            for (int j=0; j<faceNum; j++)
                {
                    x = j*deltaX;
                    glVertex3f(x,0,z);
                    glVertex3f(x,0,z+deltaZ);
                }
            glVertex3f(x+deltaX,0,z);
            glVertex3f(x+deltaX,0,z+deltaZ);
            glEnd();
        }
    glPopMatrix();
}

//------------------------drawWireBox-------------------------------------
//
// draws a wireframbox with given dimensions to position 'boxPos'
//-----------------------------------------------------------------------
void GLServer::DrawWireBox(salt::Vector3f boxPos, salt::Vector3f sz)
{
    glColor3ub(255,0,255);

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

//------------------------drawGoal-------------------------------------
//
// draws a goal with given dimensions to position 'goalPos'
//-----------------------------------------------------------------------
void GLServer::DrawGoal(salt::Vector3f goalPos, salt::Vector3f sz)
{
    GLUquadricObj *cyl;
    cyl = gluNewQuadric();

    glPushMatrix();
    glTranslatef(goalPos[0], goalPos[1], goalPos[2]);

    // draw goal sides as cylinders
    glPushMatrix();
    glTranslatef(0,sz[1],0);
    gluCylinder(cyl,0.1,0.1,sz[2],15,15);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90.0f, 1,0,0);
    gluCylinder(cyl,0.1,0.1,sz[1],15,15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,sz[2]);
    glRotatef(-90.0f, 1,0,0);
    gluCylinder(cyl,0.1,0.1,sz[1],15,15);
    glPopMatrix();

    glPopMatrix();

    gluDeleteQuadric(cyl);
}

//------------------------drawSphere-------------------------------------
//
// draws a solid sphere with given radius to position 'spherePos'
//-----------------------------------------------------------------------
void GLServer::DrawSphere(salt::Vector3f spherePos,float radius)
{
    glPushMatrix();
    glTranslatef(spherePos[0],spherePos[1], spherePos[2]);
    if(!mWireframe) glutSolidSphere(radius, 10, 10);
    else glutWireSphere(radius, 10, 10);
    glPopMatrix();
}
//-----------------------DrawShadowOfSphere------------------------------
//
// draws the shadow of a sphere with given radius by scaling its size onto
// the ground (no y component)
//-----------------------------------------------------------------------
void GLServer::DrawShadowOfSphere(salt::Vector3f spherePos,float radius)
{
    // distance between ground and shadows
    const float delta = 0.08f;

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);

    // draw the flat sphere just a 'bit'
    // ontop of the playing gound
    glTranslatef(spherePos[0], delta, spherePos[2]);
    glScalef(1.0f, 0.0f, 1.0f);
    glutSolidSphere(radius, 10, 10);

    glEnable(GL_LIGHTING);
    glPopMatrix();
}


