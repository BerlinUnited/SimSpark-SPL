/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.cpp,v 1.5 2004/04/21 18:35:50 rollmark Exp $

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

using namespace salt;

GLServer::GLServer(int width, int height,
                   Vector3f camPos,
                   Vector3f lookAtPos,
                   Vector3f up, bool wire)
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
    glDisable(GL_LIGHTING);
}

void GLServer::DrawTextPix(const char* text, Vector2f pix, ETextAlign ta)
{

    int width = GetTextWidth(text);
    switch (ta)
    {
    case eCENTER:
        pix[0] = (mWidth - width)/ 2.0 + pix[0];
        break;
    case eRIGHT:
        pix[0] = mWidth - width + pix[0];
        break;
    default: ;
    }

    const Vector2f pos(pix[0] / (float)mWidth * 2 - 1.0f,
                       1.0f - (pix[1] / (float)mHeight * 2));
    DrawText(text,pos);
}

int
GLServer::GetTextHeight() const
{
    // currently only GLUT_BITMAP_9_BY_15 is used
    return 15;
}

int
GLServer::GetTextWidth(const char* text) const
{
    // currently only GLUT_BITMAP_9_BY_15 is used
    int width = 0;
    for (const char* s = text; *s; ++s)
    {
        width += glutBitmapWidth(GLUT_BITMAP_9_BY_15, *s);
    }
    return width;
}

void GLServer::DrawText3D(const char* text, Vector3f pos)
{
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);

    glRasterPos3f(pos[0],pos[1],pos[2]);
    for (const char* s = text; *s; ++s)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
        }

    glEnable (GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
}

//--------------------------drawText-------------------------------------
//draws a given text string onto the screen at position pos;; (-1,1)
//is top left, (+1,-1) is bottom right of the viewport
//-----------------------------------------------------------------------
void GLServer::DrawText(const char* text, Vector2f pos)
{
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


void GLServer::DrawGroundRectangle(Vector3f pos, float szX, float szY,
                                   float angleDeg, float height)
{
    const int faceNum = 10;

    glPushMatrix();
    glRotatef(0,1,0,angleDeg);
    glTranslatef(pos[0],pos[1], pos[2]);
    glNormal3f(0,0,1);

    // store the sizes of our faces
    GLfloat deltaX = szX/faceNum;
    GLfloat deltaY = szY/faceNum;

    GLfloat x,y;

    for (int i=0; i<faceNum; i++)
        {
            y = i*deltaY;

            //draw face as Quadric strip
            glBegin(GL_QUAD_STRIP);
            for (int j=0; j<faceNum; j++)
                {
                    x = j*deltaX;
                    glVertex3f(x,y,height);
                    glVertex3f(x,y+deltaY,height);
                }
            glVertex3f(x+deltaX,y,height);
            glVertex3f(x+deltaX,y+deltaY,height);
            glEnd();
        }
    glPopMatrix();
}

void
GLServer::DrawCircle(const salt::Vector3f& pos, float radius,
                     float /*start_angle*/, float /*end_angle*/)
{
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    int num_lines = 36;

    for(int i =0;i<num_lines;i++)
    {
        double angle = i*2*M_PI/num_lines;
        glVertex3f(pos[0] + radius*cos(angle), pos[1] + radius*sin(angle), pos[2]);
    }
    glEnd();
    glPopMatrix();
}

//------------------------drawWireBox-------------------------------------
//
// draws a wireframbox with given dimensions to position 'boxPos'
//-----------------------------------------------------------------------
void GLServer::DrawWireBox(Vector3f boxPos, Vector3f sz)
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

//------------------------drawGoal-------------------------------------
//
// draws a goal with given dimensions to position 'goalPos'
//-----------------------------------------------------------------------
void GLServer::DrawGoal(Vector3f goalPos, Vector3f sz)
{
    GLUquadricObj *cyl;
    cyl = gluNewQuadric();

    glPushMatrix();
    glTranslatef(goalPos[0], goalPos[1], goalPos[2]);

    // draw goal sides as cylinders
    glPushMatrix();
    glTranslatef(0,0,sz[2]);
    glRotatef(-90.0f, 1,0,0);
    gluCylinder(cyl,0.1,0.1,sz[1],15,15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,sz[1],0);
    gluCylinder(cyl,0.1,0.1,sz[2],15,15);
    glPopMatrix();

    glPushMatrix();
    gluCylinder(cyl,0.1,0.1,sz[2],15,15);
    glPopMatrix();

    glPopMatrix();

    gluDeleteQuadric(cyl);
}

//------------------------drawSphere-------------------------------------
//
// draws a solid sphere with given radius to position 'spherePos'
//-----------------------------------------------------------------------
void GLServer::DrawSphere(Vector3f spherePos,float radius, int res)
{
    glPushMatrix();
    glTranslatef(spherePos[0],spherePos[1], spherePos[2]);
    if(!mWireframe) glutSolidSphere(radius, res, res);
    else glutWireSphere(radius, res, res);
    glPopMatrix();
}


//-----------------------DrawShadowOfSphere------------------------------
//
// draws the shadow of a sphere with given radius by scaling its size onto
// the ground (no y component)
//-----------------------------------------------------------------------
void GLServer::DrawShadowOfSphere(Vector3f spherePos,float radius)
{
    // distance between ground and shadows
    const float delta = 0.001f;

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);

    glTranslatef(spherePos[0], spherePos[1],delta);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(radius, 10, 10);

    glPopMatrix();
}

