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

#include "monitorframe.h"
#include "monitorframe_sparkglwidget.h"
#include "export.h"

#include <pluginmanager.h>
#include <menuobjecttoolbar.h>
#include <settings.h>
#include <simulationmanager.h>
#include <communicationmanager.h>
#include <carbon.h>
#include <serverthread.h>
#include <sparkcontroller.h>
#include <openglmanager.h>

#include <QToolBar>
#include <QSize>
#include <QLayout>
#include <QToolButton>
#include <QGLContext>

using namespace boost;
using namespace MonitorFrameUtil;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(MonitorFrame, "monitorframe", "Monitor", 1)

//Static class information
void MonitorFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION,
        tr("The monitor frame offers an OpenGL window to the OpenGL-Manager of the application.\n"
        "By default it is rendering an example scene, but it can be used to render an externally initialized scene like a simspark simulation server scene."));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":monitor");

    //Tags
    QStringList tags;
    tags << "monitor" << "rendering" << "opengl";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

MonitorFrame::MonitorFrame() :
    mSettings(getCarbon()->getSettings()),
    mSimulationManager(getCarbon()->getSimulationManager())
{
    //Ui Setup
    ui.setupUi(this);
    loadFrameDesign();

    //Member initialization
    mRenderingInitialized = false;
    mClosed = false;
    mGLContainerLayout = new QVBoxLayout();
    mGLContainerLayout->setContentsMargins(0,0,0,0);
    ui.glWidgetContainer->setLayout(mGLContainerLayout);

    //Create GlWidget and register it to be able to hold Spark resources (or others)
    OpenGLManager* manager = getCarbon()->getOpenGLManager();
    mGL = new SparkGLWidget(ui.glWidgetContainer, 20);
    manager->registerGLWidget(mGL, OpenGLManager::GWF_SERVER);
    mGLContainerLayout->addWidget(mGL);
    mGL->makeCurrent();
    mGL->start(); //Calls initialization and renders basic image
    //Receive both blocked and non blocked signals to state in receiveGLStateSignal
    manager->connectGLStateSignal(mGL, this, SLOT(receiveGLStateSignal(GLWidget*, OpenGLManager::EGLStateSignal)), true, true);

    int error = glGetError();
    if (error)
        LOG_ERROR() << "GL-Error " << error;

    //Connections
    connect(getCarbon(), SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));
    connect(mGL, SIGNAL(renderNodePick(QString)), this, SIGNAL(renderNodePick(QString)));

    //Communication
    CommunicationManager* commManager = getCarbon()->getCommunicationManager();
    commManager->registerSender(this, "Server_LeafPicks", SIGNAL(renderNodePick(QString)));
}

MonitorFrame::~MonitorFrame()
{
    cleanup(true);
}

void MonitorFrame::init(const QStringList& parameters)
{
    //Load variables and initialize state
    loadDefaultSettings();
    loadSettings();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

bool MonitorFrame::isInitialized() const
{
    return mRenderingInitialized;
}

GLWidget* MonitorFrame::getGLWidget() const
{
    return mGL;
}

void MonitorFrame::updateState(EMonitorFrameState state)
{
    OpenGLManager* manager = getCarbon()->getOpenGLManager();
    if (state == MFS_WAITING)
    {
        bool ok = manager->registerGLWidget(mGL, OpenGLManager::GWF_NORMAL);
        if (ok)
        {
            ui.headerLabel->setText(tr("Auto connection to Server deactivated."));
            ui.activateButton->setText(tr("Activate"));
            LOG_INFO() << "Dectivated monitor frame. Will not connect to OpenGL context.";
        }
        else
            LOG_ERROR() << "GLWidget registration failed.";

        mDefaultState = state;
        mState = state;
    }
    else if (state == MFS_READY_SERVER)
    {
        bool ok = manager->registerGLWidget(mGL, OpenGLManager::GWF_SERVER);
        if (ok)
        {
            ui.headerLabel->setText(tr("Waitig to connect to Server."));
            ui.activateButton->setText(tr("Deactivate"));
            LOG_INFO() << "Activated monitor frame. Will connect to Server OpenGL context.";
        }
        else
            LOG_ERROR() << "GLWidget registration failed.";

        mDefaultState = state;
        mState = state;
    }
    else if (state == MFS_RUNNING)
    {
        bool ok = manager->registerGLWidget(mGL, OpenGLManager::GWF_NORMAL);
        if (ok)
        {
            ui.headerLabel->setText(tr("Server rendering established."));
            ui.activateButton->setText(tr("Quit"));
            LOG_DEBUG() << "Displaying Server OpenGL context.";
        }
        else
            LOG_ERROR() << "GLWidget registration failed.";

        ui.headerWidget->setVisible(false);

        mState = state;
    }
    else
    {
        LOG_WARNING() << "Unknown state: " << state;
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void MonitorFrame::loadDefaultSettings()
{
    mState = MFS_READY_SERVER;
}

void MonitorFrame::loadSettings()
{
    mSettings->beginInstanceGroup(this, true); //lock
    mDefaultState = (EMonitorFrameState)mSettings->instanceValue("mDefaultState", (int)mDefaultState).toInt();
    mSettings->endGroup(true); //unlock

    updateState(mState);
}

void MonitorFrame::saveSettings()
{
    mSettings->beginInstanceGroup(this, true); //lock
    mSettings->setInstanceValue("mDefaultState", (int)mDefaultState);
    mSettings->endGroup(true); //unlock
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void MonitorFrame::initRendering()
{
    if (mRenderingInitialized)
        return;

    boost::shared_ptr<ServerThread> server = mSimulationManager->getSparkServerThread();
    if (!server.get())
    {
        LOG_INFO() << "No spark server thread found.";
        return;
    }

    boost::shared_ptr<SparkController> controller = server->getSparkController();
    if (!controller.get())
    {
        LOG_INFO() << "No spark controller found. Spark server is not initialized yet.";
        return;
    }

    if (!controller->getSpark().get())
    {
        LOG_INFO() << "Spark not initialized yet.";
        return;
    }

    QString cameraPath        = "/usr/scene/camera/camera";
    QString sveneServerPath   = "/sys/server/scene";
    QString renderServerPath  = "/sys/server/render";
    QString renderControlPath = "/sys/server/simulation/RenderControl";

    //Load settings for initialization
    if (mSettings->beginGroup(this, true)) //lock
    {
        cameraPath        = mSettings->value("sparkCameraPath",        cameraPath).toString();
        sveneServerPath   = mSettings->value("sparkSveneServerPath",   sveneServerPath).toString();
        renderServerPath  = mSettings->value("sparkRenderServerPath",  renderServerPath).toString();
        renderControlPath = mSettings->value("sparkRenderControlPath", renderControlPath).toString();

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to read camera, server, renderServer and renderControl paths. Using default values.";

    //Create
    OpenGLManager* manager = getCarbon()->getOpenGLManager();
    mGL->setUpdatesEnabled(false);
    mGL->getTimer().stop();

    //Init spark gl widget
    LOG_INFO() << "Initializing SparkGLWidget. Using node paths:\n"
        << "SparkCameraPath = " << cameraPath << "\n"
        << "SparkSveneServerPath = " << sveneServerPath << "\n"
        << "SparkRenderServerPath = " << renderServerPath << "\n"
        << "SparkRenderControlPath = " << renderControlPath;
    bool ok = ((SparkGLWidget*)mGL)->initSpark(controller, cameraPath, sveneServerPath, renderServerPath, renderControlPath); //Init spark
    if (ok)
    {
        mGL->init(); //re-initGL

        //Update state and display
        mRenderingInitialized = true;
        updateState(MFS_RUNNING);
    }
    else
    {
        LOG_INFO() << "Failed to initialize SparkGLWidget.";
    }

    //Start rendering again
    mGL->setUpdatesEnabled(true);
    mGL->start();
}

void MonitorFrame::receiveGLStateSignal(GLWidget* widget, OpenGLManager::EGLStateSignal signal)
{
    if (widget != mGL)
        return;

    if (signal == OpenGLManager::GSS_ACCESS)
    {
        LOG_INFO() << "GLWidget accessed externally. Stopping rendering.";
    }
    else if (signal == OpenGLManager::GSS_END_ACCESS)
    {
        LOG_INFO() << "External GLWidget access finished. Reinitialize rendering.";
        widget->makeCurrent();
        initRendering();
    }
    else if (signal == OpenGLManager::GSS_OWNED)
    {
        LOG_WARNING() << "External GLContext ownership declared. This behaviour is not specified for MonitorFrame.";
    }
    else if (signal == OpenGLManager::GSS_FREED)
    {
        LOG_WARNING() << "External GLContext removal declared. This behaviour is not specified for MonitorFrame.";
    }
    else
    {
        LOG_WARNING() << "Unhandled GLState signal " << signal << " for owned GLWidget.";
    }
}

void MonitorFrame::toggleReadyState()
{
    if      (mState == MFS_WAITING)      mState = MFS_READY_SERVER;
    else if (mState == MFS_READY_SERVER) mState = MFS_WAITING;
    else if (mState == MFS_RUNNING)
    {
        LOG_INFO() << "Todo: Quit rendering.";
    }
    else
    {
        LOG_ERROR() << "Illegal state.";
    }

    updateState(mState);
}

void MonitorFrame::cleanup(bool destructor)
{
    if (mClosed)
        return;

    mClosed = true;

    //Save settings
    saveSettings();

    //Unregister GL
    getCarbon()->getOpenGLManager()->unregisterGLWidget(mGL);

    //Close Gl
    mGL->deleteLater();

    //Unregister communication
    getCarbon()->getCommunicationManager()->unregisterSender(this, !destructor); //Important! unregistering without removing the connection itself, this already happened in destructor
}
