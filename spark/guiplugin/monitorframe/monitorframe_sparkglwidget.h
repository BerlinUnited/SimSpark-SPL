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

#ifndef MONITORFRAME_SPARKGLWIDGET_H
#define MONITORFRAME_SPARKGLWIDGET_H
/**
 * \file monitorframe_sparkglwidget.h
 * \brief Contains MonitorFrameUtil::SparkGLWidget class.
 */

#include <kerosin/inputserver/input.h>
#include <glwidget.h>

#include "monitorframe_sparkglrender.h"

namespace kerosin
{
    class InputSystem;
    class RenderServer;
    class RenderNode;
}

class SparkController;
class QGLContext;

namespace MonitorFrameUtil
{

/*!
 \brief A GLWidget which offers a rendering context to SimSpark and renders a SimSpark scene and CustomRender nodes.

 At first initialization the SparkGLWidget initializes basic example rendering of GLWidget and offers an OpenGL-Context to the
 OpenGLManager. Offering can be toggled by the user.\n
 During the initialization of SimSpark rendering is stopped, the rendering context is released and textures can be initialized by SimSpark.
 After that, the SparkGLWidget resumes rendering using a MonitorFrameUtil::SparkGLRender object.\n
*/
class SparkGLWidget : public GLWidget
{
  Q_OBJECT

public:
    // public functions

    /*!
     \brief Complete constructor.

     \param parent parent widget.
     \param timerInterval render timer interval in ms
     \param name widget name, used in OpenGLManager
     \param shareWidget widget to use OpenGlContext from (shared context)
    */
    SparkGLWidget(QWidget *parent = 0, int timerInterval = 20, const QString& name = ("SparkGLWidget"), GLWidget* shareWidget = 0);
    /*!
     \brief Destructor.
    */
    virtual ~SparkGLWidget();

    /*!
     \brief Variable initialization.

    */
    void initVar();

    /*!
     \brief Widget initialization reset.

     If not initialized (after reset) renders base widgets example rendering.
    */
    void reset();

    /*!
     \brief Initializes spark rendering.

     \param controller spark controller to interact with
     \param cameraPath path of the camera in zeitgeist scene graph
     \param sceneServerPath path of the scene server in zeitgeist scene graph
     \param renderServerPath path of the render server in zeitgeist scene graph
     \param renderControlPath path of the render control in zeitgeist scene graph
     \return true if initialization was successful
    */
    bool initSpark(boost::shared_ptr<SparkController> controller,
        const QString& cameraPath, const QString& sceneServerPath, 
        const QString& renderServerPath, const QString& renderControlPath);

public:
    //public virtual functions

    /*!
     \brief OpenGlInizialization.

     If SimSpark rendering is not initialized, will call base class example initialization, otherwise
     will init OpenGl for spark rendering.
    */
    virtual void initializeGL();
    /*!
     \brief Resize the widget.

     \param width new width
     \param height new height
    */
    virtual void resizeGL( int width, int height);
    /*!
     \brief Called by paint event. Calls rendering.
    */
    virtual void paintGL();
  
    /*!
     \brief Paint event. Calls paintGL()

     \param e event
    */
    virtual void paintEvent(QPaintEvent* e);
    /*!
     \brief Double click event. Picks at clicked position.

     \param e event
    */
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    /*!
     \brief Mouse move event.

     If SimSpark rendering is initialized, sends Input object to the input server.
     Updates mouse position and rotates camera if button held down.
     \param e event
    */
    virtual void mouseMoveEvent(QMouseEvent* e);
    /*!
     \brief Mouse press event.

     If SimSpark rendering is initialized, sends Input object to the input server.
     Start camera rotation.
     \param e event
    */
    virtual void mousePressEvent(QMouseEvent* e );
    /*!
     \brief Mouse release event.

     If SimSpark rendering is initialized, sends Input object to the input server.
     Stop camera rotation.
     \param e event
    */
    virtual void mouseReleaseEvent(QMouseEvent* e );
    /*!
     \brief Key down event.

     If SimSpark rendering is initialized, sends Input object to the input server.
     \param e event
    */
    virtual void keyPressEvent(QKeyEvent* e);
    /*!
     \brief Key up event.

     If SimSpark rendering is initialized, sends Input object to the input server.
     \param e event
    */
    virtual void keyReleaseEvent(QKeyEvent* e);
    /*!
     \brief Resize event. Calls resizeGL().

     \param e event
    */
    virtual void resizeEvent(QResizeEvent* e);
    /*!
     \brief Wheel event.

      If SimSpark rendering is initialized, sends Input object to the input server.
     \param e event
    */
    virtual void wheelEvent(QWheelEvent* e);
    /*!
     \brief Mouse enter event. Starts updating mouse position.

     \param e event
    */
    virtual void enterEvent(QEvent* e);
    /*!
     \brief Mouse leave event. Stops updating mouse position.

     \param e event
    */
    virtual void leaveEvent(QEvent* e);
    /*!
     \brief Widget move event. Redraws.

     \param e event
    */
    virtual void moveEvent(QMoveEvent* e);
    /*!
     \brief Focus event. Grabs keyboard.

     \param e event
    */
    virtual void focusInEvent(QFocusEvent* e);
    /*!
     \brief Focus out event. Releases keyboard.

     \param e event
    */
    virtual void focusOutEvent(QFocusEvent* e);
    /*!
     \brief Close event.

     \param e event
    */
    virtual void closeEvent(QCloseEvent* e);
    /*!
     \brief Hide event.

     \param e event
    */
    virtual void hideEvent(QHideEvent* e);
    /*!
     \brief Show event.

     \param e event
    */
    virtual void showEvent(QShowEvent* e);

protected:
    // protected functions

    /*!
     \brief Add an input to the input servers input queue.

     \param input input to add
    */
    void addInput(const kerosin::Input& input);

public: signals:
    //public signals

    /*!
     \brief Emitted when a node was picked.

     Forwarded by MonitorFrame and offered as coomunication output.
     \param path
    */
    void renderNodePick(QString path);

private:
    // private members

    bool mInit; /*!< True after spark rendering initialization. */
    int mErr; /*!< OpenGl error code storage. */

    boost::shared_ptr<SparkGLRender> mRender; /*!< Renderer. */

    boost::shared_ptr<SparkController> mController; /*!< Spark controller to render from. */
    boost::shared_ptr<kerosin::InputSystem> mInputSystem; /*!< nput system for SimSpark. */
    bool mMouseCaptured; /*!< True if mouse is captured. */

    long mLastMouseX; /*!< Last mouse x position. */
    long mLastMouseY; /*!< Last mouse y position. */
    float mMouseInputFactor; /*!< Factor for mouse movement distance inputs. */
};

} //namespace MonitorFrameUtil

#endif //MONITORFRAME_SPARKGLWIDGET_H
