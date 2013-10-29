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

#include "monitorframe_sparkglwidget.h"

#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QHideEvent>
#include <QShowEvent>
#include <QFocusEvent>
#include <QMoveEvent>

#include <openglmanager.h>
#include <simspark.h>
#include <sparkcontroller.h>
#include <carbon.h>
#include <sparkcontext.h>
#include <inputqt.h>

#include <zeitgeist/logserver/logserver.h>
#include <kerosin/inputserver/inputserver.h>
#include <kerosin/inputserver/inputsystem.h>
#include <kerosin/renderserver/renderserver.h>
#include <kerosin/renderserver/rendernode.h>

using namespace boost;
using namespace zeitgeist;
using namespace kerosin;
using namespace salt;

namespace MonitorFrameUtil
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SparkGLWidget::SparkGLWidget(QWidget *parent, int timerInterval, const QString& name, GLWidget* shareWidget)
    : GLWidget(parent, timerInterval, name, shareWidget), mErr(0)
{
    initVar();
}

SparkGLWidget::~SparkGLWidget()
{

}

void SparkGLWidget::initVar()
{
    mMouseCaptured = false;
    mInit = false;

    mMouseInputFactor = 2;
    mLastMouseX = 0;
    mLastMouseY = 0;
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void SparkGLWidget::reset()
{
    mInit = false;
}

//--------------------------------------------------------------
// Public virtual
//--------------------------------------------------------------

void SparkGLWidget::initializeGL()
{
    if (mErr = glGetError())
    {
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " Before initialization.";
    }

    if (!mInit)
    {
        //Base widget initialization
        GLWidget::initializeGL();
        return;
    }

    LOG_INFO() << "Initializing SparkGLWidget...";

    glClearColor( 0.5, 0.5, 0.5, 0.0 );
    mRender->setAmbientColor(0.5, 0.5, 0.5, 0.0); // Background color

    glShadeModel( GL_SMOOTH );          // Shading

    if (mErr = glGetError())
    {
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " After initialization.";
    }
}

void SparkGLWidget::paintGL()
{
    if (!mInit)
    {
        GLWidget::paintGL();
        return;
    }

    //Prevent deletion in simspark
    if (mController->tryAcquireRuntimeAccess(1000, false))
    {

        if (mErr = glGetError())
            LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " Before rendering.";

        mRender->render(size().width(), size().height());
        mController->releaseRuntimeAccess();
    }
    else
    {
        LOG_ERROR() << "Cant render in SparkGlWidget: SparkController access is locked.";
    }
}

void SparkGLWidget::resizeGL( int width, int height)
{
    //if (!mInit)
    {
        GLWidget::resizeGL(width, height);
    }
}

void SparkGLWidget::paintEvent(QPaintEvent* e)
{
    GLWidget::paintEvent(e);
}

void SparkGLWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (!mInit) //no interaction before spark initialization
        return;

    if (e->button() == Qt::LeftButton)
    {
        //Lock to make current
        if (tryLock(1000))
        {
            //Make current
            makeCurrent();
            if (mErr = glGetError())
                LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " Before picking.";

            //Pick a node and swap buffers
            boost::weak_ptr<kerosin::RenderNode> picked(mRender->pickNode(e->x(), e->y(), size().width(), size().height()));
            swapBuffers();
            if (mErr = glGetError())
                LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " After picking.";

            boost::shared_ptr<kerosin::RenderNode> locked(picked.lock());
            if (locked.get())
                emit renderNodePick(QString(locked->GetFullPath().c_str()));

            //Release context
            doneCurrent();
            unlock();
        }
        else
        {
            LOG_ERROR() << "Could not lock access to GLContext.";
        }
    }
    else if (e->button() == Qt::RightButton)
    {
    }

    GLWidget::mouseDoubleClickEvent(e);
}

void SparkGLWidget::mouseMoveEvent(QMouseEvent * e)
{
    //LOG_INFO() << "mouseMoveEvent";
    e->accept();

    Input xinput(Input::eAxis, Input::IC_AXISX);
    xinput.mData.l = static_cast<long>((e->x() - mLastMouseX) * mMouseInputFactor);
    xinput.mModState = 0;
    addInput(xinput);

    Input yinput(Input::eAxis, Input::IC_AXISY);
    yinput.mData.l = static_cast<long>((e->y() - mLastMouseY) * mMouseInputFactor);
    yinput.mModState = 0;
    addInput(yinput);

    mLastMouseX = e->x();
    mLastMouseY = e->y();

    GLWidget::mouseMoveEvent(e);
}

void SparkGLWidget::mousePressEvent(QMouseEvent * e )
{
    setFocus(); // grabs keyboard
    e->accept();

    mLastMouseX = e->x();
    mLastMouseY = e->y();

    kerosin::Input::TInputCode code =
        (e->button() == Qt::LeftButton) ? Input::IC_MOUSE_LEFT :
        ((e->button() == Qt::RightButton) ? Input::IC_MOUSE_RIGHT :
        ((e->button() == Qt::MiddleButton) ? Input::IC_MOUSE_MIDDLE : -1));

    if (code != -1)
    {
        Input input(Input::eButton, code, 1);
        input.mData.l = 1;
        input.mModState = 0;

        addInput(input);
        e->accept();
    }

    if (e->button() == Qt::RightButton)
    {
        //Todo: context menu when right clicking
        /*
            weak_ptr<RenderNode> node = PickNode();
            shared_ptr<Leaf> selection =
                SparkContext::GetInstance().GetSelection().lock();

            if (
                (! node.expired()) &&
                (node.lock() == selection)
                )
                {
                    wxMenu* menu =
                        SparkContext::GetInstance().GetContextMenu(selection);

                    if (menu != 0)
                        {
                            PopupMenu(menu);
                            return;
                        }
                }
        }*/
    }

    if (!e->isAccepted())
        GLWidget::mousePressEvent(e);
}

void SparkGLWidget::mouseReleaseEvent(QMouseEvent * e )
{
    mLastMouseX = e->x();
    mLastMouseY = e->y();

    kerosin::Input::TInputCode code =
        (e->button() == Qt::LeftButton) ? Input::IC_MOUSE_LEFT :
        ((e->button() == Qt::RightButton) ? Input::IC_MOUSE_RIGHT :
        ((e->button() == Qt::MiddleButton) ? Input::IC_MOUSE_MIDDLE : -1));

    if (code != -1)
    {
        Input input(Input::eButton, code);
        input.mData.l = 0;
        input.mModState = 0;

        addInput(input);
        e->accept();
    }
    else
        GLWidget::mouseReleaseEvent(e);
}

void SparkGLWidget::keyPressEvent(QKeyEvent* e)
{
    //Handle bindable keys
    addInput(QTInput(*e, true));
    e->accept();

    //GLWidget::keyPressEvent(e);
}

void SparkGLWidget::keyReleaseEvent(QKeyEvent* e)
{
    // handle specific keys
    if ((e->modifiers() == 0) &&
        (e->key() == Qt::Key_Escape))
    {
        //SparkContext::GetInstance().ResetSelection();
    }
    else
    {
        addInput(QTInput(*e, false));
    }

    e->accept();

    //GLWidget::keyReleaseEvent(e);
}

void SparkGLWidget::resizeEvent(QResizeEvent* e)
{
    GLWidget::resizeEvent(e);
}

void SparkGLWidget::wheelEvent(QWheelEvent* e)
{
    GLWidget::wheelEvent(e);
}

void SparkGLWidget::enterEvent(QEvent* e)
{
    GLWidget::enterEvent(e);
}

void SparkGLWidget::leaveEvent(QEvent* e)
{
    GLWidget::leaveEvent(e);
}

void SparkGLWidget::moveEvent(QMoveEvent* e)
{
    GLWidget::moveEvent(e);
}

void SparkGLWidget::focusInEvent(QFocusEvent* e)
{
    GLWidget::focusInEvent(e);
}

void SparkGLWidget::focusOutEvent(QFocusEvent* e)
{
    GLWidget::focusOutEvent(e);
}

void SparkGLWidget::closeEvent(QCloseEvent* e)
{
    GLWidget::closeEvent(e);
}

void SparkGLWidget::hideEvent(QHideEvent* e)
{
    GLWidget::hideEvent(e);
}

void SparkGLWidget::showEvent(QShowEvent* e)
{
    GLWidget::showEvent(e);
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool SparkGLWidget::initSpark(boost::shared_ptr<SparkController> controller,
    const QString& cameraPath, const QString& sceneServerPath,
    const QString& renderServerPath, const QString& renderControlPath)
{
    mController = controller;

    //Check spark
    shared_ptr<SimSpark> spark = controller->getSpark();
    if (spark.get() == 0)
    {
        LOG_ERROR() << "SimSpark not found.";
        return false;
    }

    //Get input server
    shared_ptr<InputServer> inputServer = spark->GetInputServer();
    if (inputServer.get() != 0)
    {
        mInputSystem = inputServer->GetInputSystem();
    }
    else
    {
        LOG_ERROR() << "InputSystem not found.";
        return false;
    }

    //Create renderer
    mRender = boost::shared_ptr<SparkGLRender>(
        new SparkGLRender(
            cameraPath.toStdString(), sceneServerPath.toStdString(),
            renderServerPath.toStdString(), renderControlPath.toStdString())
        );

    //Initialize renderer
    bool ok = mRender->init(spark);
    if (!ok)
        LOG_ERROR() << "Error while initializing spark rendering.";

    mInit = ok;
    return ok;
}

void SparkGLWidget::addInput(const kerosin::Input& input)
{
    if (mInputSystem.get() == 0)
    {
        return;
    }

    //Add single input to input queue
    mInputSystem->AddInput(input);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

} //namespace MontorFrameUtil
