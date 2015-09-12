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

#ifndef MONITORFRAME_H
#define MONITORFRAME_H
/**
 * \file monitorframe.h
 * \brief Contains MonitorFrame class.
 */

#include <attachableframe.h>
#include <openglmanager.h>

#include "ui_monitorframe.h"

class GLWidget;
class SimulationManager;
class QLayout;
class QGLContext;

/*!
 \brief Plugin that offers a SparkGLWidget for a SimSpark ServerThread.

*/
class MonitorFrame : public AttachableFrame
{
    Q_OBJECT
    GUI_FRAME

public:
    //public types

    /*!
     \brief State of the Frame.
    */
    enum EMonitorFrameState
    {
        MFS_WAITING = 0,  /*!< Waiting and rendering example animation. Does not offer the GLWidget to SimSpark server. */
        MFS_READY_SERVER, /*!< Waiting and rendering example animation, while offering the GLWidget to a SimSpark server. */
        MFS_RUNNING       /*!< Running rendering of a SimSpark server. */
    };

public:
    // public functions

    /*!
     \brief Default constructor.
    */
    MonitorFrame();
    /*!
     \brief Destructor.
    */
    virtual ~MonitorFrame();
    /*!
     \brief Post-construction initialization.
    */
    virtual void init(const QStringList& parameters);

protected:
    //protected functions

    /*!
     \brief Returns true if simspark rendering is initialized.
    */
    bool isInitialized() const;
    /*!
     \brief Returns the SparkGLWidget.
    */
    GLWidget* getGLWidget() const;

    /*!
     \brief Changes the monitor frames state and updates the display accordingly.

     \param state new state
    */
    void updateState(EMonitorFrameState state);

private:
    //private functions

    /*!
     \brief Loads settings from the QSettings object.
    */
    void loadSettings();
    /*!
     \brief Loads default settings from the Ui class.
    */
    void loadDefaultSettings();
    /*!
     \brief Saves current settings to the QSettings object.
    */
    void saveSettings();

public: signals:

    /*!
     \brief Emitted when a node in a spark rendering was double clicked.

     Offered as communication topic "Server_LeafPicks".
     \param path absolute path of the node in the scene graph
    */
    void renderNodePick(QString path);

public slots:

    /*!
     \brief Initializes SimSpark rendering.

     Called after the EGLStateSignal GSS_END_ACCESS was received by the OpenGLManager.
    */
    void initRendering();

    /*!
     \brief Makes the frame ready for deletion.
    */
    void cleanup(bool fromDestructor = false);

    /*!
     \brief Receive notification signal about GLWidget access.

     Stops example rendering after GSS_ACCESS was received and restarts after GSS_END_ACCESS
     was received.

     \param widget referenced widget
     \param signal signal from the caller
    */
    void receiveGLStateSignal(GLWidget* widget, OpenGLManager::EGLStateSignal signal);

    /*!
     \brief Switches bewteen waiting and server-ready state (if simspark rendering is not initialized already.)
    */
    void toggleReadyState();

private:
    // private members

    Ui::MonitorFrame ui;                   /*!< UI definition. */
    boost::shared_ptr<Settings> mSettings; /*!< Global Settings object. */
    SimulationManager* mSimulationManager; /*!< Pointer to simulation manager. */
    bool mClosed;                          /*!< True after cleanup was called. */

    QLayout* mGLContainerLayout; /*!< Parent layout of the SparkGLWidget. */
    GLWidget* mGL;               /*!< The SparkGLWidget. */

    bool mRenderingInitialized;       /*!< True after SimSpark server rendering was initialized. */
    EMonitorFrameState mState;        /*!< State of the MonitorFrame. */
    EMonitorFrameState mDefaultState; /*!< Default state for the MonitorFrame to start with in next program execution. */
};

#endif // MONITORFRAME_H
