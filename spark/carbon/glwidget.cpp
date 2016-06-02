/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
   $Id$

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

#include "glwidget.h"
#include "carbon.h"

#include <QKeyEvent>
#include <QPaintEvent>
#include <QResizeEvent>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

GLWidget::GLWidget(QWidget* parent, int timerInterval, const QString& name, GLWidget* shareWidget) :
    QGLWidget(parent, shareWidget),
    mName(name)
{
    mTimer.setInterval(timerInterval);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    mHasRememberedSize = false;
}

GLWidget::~GLWidget()
{

}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void GLWidget::init()
{
    initializeGL();
}

void GLWidget::start()
{
    mTimer.start();
}

QTimer& GLWidget::getTimer()
{
    return mTimer;
}

const QString& GLWidget::getName() const
{
    return mName;
}

void GLWidget::lock()
{
    mContextLock.lock();
}

bool GLWidget::tryLock(int timeout)
{
    return mContextLock.tryLock(timeout);
}

void GLWidget::unlock()
{
    mContextLock.unlock();
}

void GLWidget::rememberResize(const QSize& size)
{
    mRememberedSize = size;
    mHasRememberedSize = true;
}

void GLWidget::applyRememberedResize()
{
    if (mHasRememberedSize)
        if(updatesEnabled())
        {
            resizeGL(mRememberedSize.width(), mRememberedSize.height()); 
            mHasRememberedSize = false;
        }
}

//--------------------------------------------------------------
// Protected virtual
//--------------------------------------------------------------

void GLWidget::keyPressEvent( QKeyEvent *e )
{
    //LOG_INFO() << "GlWidget KeyPress event: " << e->type();
}

void GLWidget::paintEvent( QPaintEvent *e )
{
    QGLWidget::paintEvent(e); // checks updatesEnabled() automatically
}

void GLWidget::resizeEvent( QResizeEvent *e )
{
    if (!updatesEnabled())
    {
        rememberResize(e->size());
        e->accept();
    }
    else
        QGLWidget::resizeEvent(e);
}

void GLWidget::mousePressEvent(QMouseEvent * e )
{
    setFocus();

    QGLWidget::mousePressEvent(e);
}

void GLWidget::timeOut()
{
    applyRememberedResize(); //Resize if enabled and queued during inavailability

    if (mContextLock.tryLock()) // prevent access to opengl context while rendering
    {
        makeCurrent();
        updateGL();
        doneCurrent(); //open the context to be used by another thread
        mContextLock.unlock();
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

/*!
  \brief Local helper-struct for a 2D-float-Point.
*/
struct Point {
    float x; /*!< x-coodinate */
    float y; /*!< y-coordinate */
};

GLint exampleObject = 0;
GLfloat xRot = 0.0, yRot = 0.0, zRot = 0.0, scale = 1.0;

void drawTriangle(Point a, Point b, Point c)
{  
    glBegin(GL_POLYGON);
        glVertex2f(a.x,a.y);
        glVertex2f(b.x,b.y);
        glVertex2f(c.x,c.y);
    glEnd();
} // end draw triangle

void drawSierpinski(Point a, Point b, Point c, int level)
{  Point m0, m1, m2;
    //Example rendering function

    if (level > 0) 
    {
        m0.x = (a.x+b.x) /2.0;
        m0.y = (a.y+b.y) /2.0;
        m1.x = (a.x+c.x) /2.0;
        m1.y = (a.y+c.y) /2.0;
        m2.x = (b.x+c.x) /2.0;
        m2.y = (c.y+b.y) /2.0;
        drawSierpinski(a,m0,m1,level-1);
        drawSierpinski(b,m2,m0,level-1);
        drawSierpinski(c,m1,m2,level-1);
   } 
   else 
       drawTriangle(a,b,c);
} // end draw Sierpinski

GLuint makeObject()
{  
    //Preparing example rendering
    GLuint list;
    list = glGenLists(1);
    glNewList(list, GL_COMPILE);
        // set the initial color
        glColor3f( 1.0, 0.0, 0.0 );
        // points for triangle to draw Sierpinski Gasket
        Point a, b, c;
        a.x=-0.8f; a.y=-0.8f;
        b.x=0.8f; b.y=-0.8f;
        c.x=0.0f; c.y=0.8f;
        drawSierpinski(a,b,c,4);
    glEndList();
    return list;
} // end make object

void GLWidget::paintGL()
{
    //LOG_DEBUG() << "Paint";
    //Example rendering

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_DEPTH_TEST );
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    // set the zoom according to the scale variable
    glScalef(scale, scale, scale);
    // set the image rotation up according to xRot, yRot, zRot
    glRotatef( xRot+=3.5, 1.0, 0.0, 0.0);
    glRotatef( yRot+=1.5, 0.0, 1.0, 0.0);
    glRotatef( zRot+=1.0, 0.0, 0.0, 1.0);

    glCallList(exampleObject);
}

void GLWidget::initializeGL()
{
    LOG_DEBUG() << "Initializing GLWidget (example rendering)...";
    //if (mContextLock.tryLock(1000)) // prevent access to opengl context while rendering
    {
        makeCurrent();

        glClearColor( 0.5, 0.5, 0.5, 0.0 ); // Let OpenGL clear to black
        exampleObject = makeObject(); // generate an OpenGL display list
        glShadeModel( GL_SMOOTH ); // smooth shading

        int error = glGetError();
        if (error)
            LOG_ERROR() << "Error initializing OpenGL: " << error;

        //doneCurrent();
        //mContextLock.unlock();
    }
    //else
        //LOG_ERROR() << "Initializing GLWidget failed. Could not access GL-Context.";
}

void GLWidget::resizeGL( int width, int height)
{
    //LOG_DEBUG() << "Resizing GLWidget to " << width << " " << height;
    if (mContextLock.tryLock(1000)) // prevent access to opengl context while rendering
    {
        makeCurrent();

        glViewport( 0, 0, (GLint)width, (GLint)height );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glFrustum(-1.0,1.0,-1.0,1.0,5,15.0);
        glMatrixMode( GL_MODELVIEW );
    
        int error = glGetError();
        if (error)
            LOG_ERROR() << "Error after GLWidget resize: " << error;

        doneCurrent();
        mContextLock.unlock();
    }
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Protected Slots
//--------------------------------------------------------------

void GLWidget::timeOutSlot()
{
    timeOut();
}

//--------------------------------------------------------------
// Public Slots
//--------------------------------------------------------------
