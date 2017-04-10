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

#include "sparkcontroller.h"
#include "sparkcommandqueue.h"
#include "sparkcommands.h"
#include "serverthread.h"
#include "carbon.h"
#include "simulationmanager.h"
#include "openglmanager.h"
#include "glwidget.h"
#include "simspark.h"
#include "menumanager.h"
#include "settings.h"
#include "cutelogger/logstream.h"

#include <oxygen/simulationserver/simulationserver.h>
#include <kerosin/kerosin.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <zeitgeist/logserver/logserver.h>

#include <QFileInfo>

#define SPARK_RUNTIME_ACCESS_MAX 1000

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SparkController::SparkController() :
    mSettings(Carbon::get()->getSettings()),
    mLogAccessSemaphore(1),
    mRuntimeAccessSemaphore(SPARK_RUNTIME_ACCESS_MAX),
    mExclusiveRuntimeAccessSemaphore(1)
{
    mSpark = boost::shared_ptr<SimSpark>();

    //Load log level from settings
    mLogLevel = Logger::Debug;
    if (mSettings->beginGroup("gui_SparkController", true)) //lock
    {
        mLogLevel = (Logger::LogLevel)mSettings->value("mLogLevel", 0).toInt();
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to load SparkController settings from. Using default values.";

    //Send log level changes to menu manager
    MenuManager* manager = Carbon::get()->getMenuManager();
    connect(this, SIGNAL(logLevelChanged(SparkController*, Logger::LogLevel)), manager, 
        SLOT(updateSparkLogLevel(SparkController*, Logger::LogLevel)), Qt::QueuedConnection);

    emit logLevelChanged(this, mLogLevel);
}

SparkController::~SparkController()
{
    boost::shared_ptr<GuiSimControl> control(mSimControl.lock());
    if (control.get() && mCommandQueue != 0)
        control->unregisterSimControl(mCommandQueue);
    mCommandQueue.reset();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

boost::shared_ptr<SimSpark> SparkController::getSpark() const
{
    return mSpark;
}

boost::weak_ptr<GuiSimControl> SparkController::getSimulationControl()
{
    return mSimControl;
}

void SparkController::initLogging(int sourceId)
{
    if (!assertSpark())
    {
        LOG_ERROR() << "Could not initialize spark logging. No spark object found.";
        return;
    }

    if (mLogStreams.size() > 0)
    {
        LOG_DEBUG() << "Reinitializing spark logging. Deleting old log streams.";
    }

    //Delete current logstreams if any
    for (auto it = mLogStreams.begin(); it != mLogStreams.end(); it++)
    {
        mSpark->GetLog()->RemoveStream(&**it);
    }

    //Add one log stream for every Spark log level / Gui log level
    if (mLogLevel <= Logger::Debug)   addLogStream(Logger::Debug, sourceId);
    if (mLogLevel <= Logger::Info)    addLogStream(Logger::Info, sourceId);
    if (mLogLevel <= Logger::Warning) addLogStream(Logger::Warning, sourceId);
    if (mLogLevel <= Logger::Error)   addLogStream(Logger::Error, sourceId);

    mLogSourceId = sourceId;
}

void SparkController::setLogProperties(Logger::LogLevel logLevel, int sourceId)
{
    setLogPropertiesProtected(logLevel, sourceId, true);
}

int SparkController::initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, const QStringList& parameters, 
    EResourceLocation resourceLocation, bool restart, const QString& logSourceName)
{
    if (!restart && getSpark().get() != 0)
        return 0;

    SimulationManager* manager = Carbon::get()->getSimulationManager();

    std::vector<std::string> locations;
    if (resourceLocation == RL_NONE)
    {
        //No additional resources
    }
    else if (resourceLocation == RL_DEFAULT || resourceLocation == RL_DEFAULT_AND_INCLUDE)
    {
        //Add the main scripts path to resource locations  
        QString path = manager->findScript(scriptPath);
        if (path.compare("") == 0)
            LOG_WARNING() << "Script path " << scriptPath << " not found.";
        else
        {
            QFileInfo info(path);
            locations.push_back(info.absolutePath().toStdString());
        }
    }
    else if (resourceLocation == RL_INCLUDE || resourceLocation == RL_DEFAULT_AND_INCLUDE)
    {
        //Add include directories to resource locations
        manager->lockIncludePaths();
        for (auto it = manager->getScriptIncludeDirectories().begin(); it != manager->getScriptIncludeDirectories().end(); it++)
        {
            locations.push_back(it->toStdString());
        }
        manager->unlockIncludePaths();
    }
    else
    {
        LOG_ERROR() << "Unknown resource location: " << resourceLocation;
    }
    
    return initSpark(scriptPath, secondaryScriptPath, additionalScripts, parameters, locations, restart, logSourceName);
}

int SparkController::initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, 
    const QStringList& parameters, const std::string& resourceLocation, bool restart, const QString& logSourceName)
{
    if (!restart && mSpark.get() != 0)
        return 0;

    std::vector<std::string> locations;
    locations.push_back(resourceLocation);
    return initSpark(scriptPath, secondaryScriptPath, additionalScripts, parameters, locations, restart, logSourceName);
}

int SparkController::initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, 
    const QStringList& parameters, const std::vector<std::string>& resourceLocations, bool restart, const QString& logSourceName)
{
    if (!restart && mSpark.get() != 0)
        return 0;

    //Store data that is not used by simspark itself
    mAdditionalScripts = additionalScripts;
    mParameters = parameters;

    //Primary Script
    QFileInfo info(scriptPath);
    if (!info.exists() || !info.isFile())
    {
        info = QFileInfo(Carbon::get()->getSimulationManager()->findScript(scriptPath));

        if (!info.exists())
        {
            LOG_ERROR() << "Cant find script " << scriptPath;
        }
    }

    QString absFilePath = info.absoluteFilePath();

    //Secondary Script (path & name)
    QFileInfo info2(secondaryScriptPath);
    if (!info2.exists())
    {
        info2 = QFileInfo(Carbon::get()->getSimulationManager()->findScript(secondaryScriptPath));

        if (!info2.exists())
        {
            LOG_ERROR() << "Cant find script path " << secondaryScriptPath;
        }
    }
    QString absSecondaryPath = info2.absolutePath();
    QString secondaryScriptName = info2.fileName();

    //Create scripts list with absolute paths
    std::vector<std::string> fullPathList;
    SimulationManager* manager = Carbon::get()->getSimulationManager();
    for (auto it = mAdditionalScripts.begin(); it != mAdditionalScripts.end(); it++)
    {
        //Get absolute script path
        QString absPath = manager->findScript(*it);
        if (absPath.compare("") == 0)
        {
            //Not found by manager, check if path can be found relatively
            QFileInfo info(*it);
            if (!info.exists())
            {
                LOG_WARNING() << "SimSpark initialization script " << *it << " not found. Specify its location absolutely or add its path to the include directories.";
            }
            else
            {
                absPath = info.absoluteFilePath();
            }
        }
        fullPathList.push_back(absPath.toStdString());
    }

    //Create or reinitialize Spark
    bool reset = false;
    if (mSpark.get() != 0 && restart)
    {
        //Reinitializing spark resumes the simulation servers run cycle.
        bool ok = mSpark->Reinit(absSecondaryPath.toStdString(), secondaryScriptName.toStdString(), absFilePath.toStdString(), fullPathList);
        if (!ok)
        {
            LOG_ERROR() << "Could not reinitialize SimSpark.";
            return 1;
        }
        reset = true;
    }
    else
    {
        //Create spark. Afterwards it is initialized, but not running.
        mSpark = boost::shared_ptr<SimSpark>(new SimSpark(absSecondaryPath.toStdString(), secondaryScriptName.toStdString(), absFilePath.toStdString(), fullPathList));
    }

    //Init logging
    if (logSourceName.compare("") != 0)
    {
        int logSource = Logger::getLogSource(logSourceName.toStdString());

        //If spark is not initialized yet, log streams are stored and added immediately after spark core initialization
        initLogging(logSource);
    }

    //First Spark init(only once)
    if (!reset)
    {
        //Add Resource locations
        for (int i = 0; i < (int)resourceLocations.size(); i++)
            mSpark->AddResourceLocation(resourceLocations.at(i));

        //Setup "commandline parameters" for spark
        //Todo: add support for parameters as list of strings to spark
        char** params = new char*[mParameters.size()];
        int run = 0;
        for (auto it = mParameters.begin(); it != mParameters.end(); it++)
        {
            params[run] = new char[it->size()];
            it->toStdString().copy(params[run], it->size());
            run++;
        }

        //Call init
        LOG_DEBUG() << "Spark::Init()";
        if (! mSpark->Init(mParameters.size(), params))
        {
            LOG_ERROR() << "Spark::Init() failed.";
            return 1;
        }

        //Initialize SimControl node and store weak ptr
        boost::shared_ptr<zeitgeist::Leaf> leaf(
            mSpark->GetCore()->CreateContext()->New("gui/GuiSimControl", "sys/server/simulation/GuiControl"));
        mSimControl = boost::weak_ptr<GuiSimControl>(boost::dynamic_pointer_cast<GuiSimControl>(leaf));
            
        //Init Command queue
        initCommandQueue();

        //Gui uses scene dictionary
        boost::shared_ptr<zeitgeist::ScriptServer> server(mSpark->GetCore()->GetScriptServer());
        if (server.get())
        {
            LOG_INFO() << "Trying to enable ruby scene dictionary at " << "'/sys/server/scene/RubySceneImporter'" << ".";
            bool ok = server->Eval("rubySceneImporter = get('/sys/server/scene/RubySceneImporter')");
            if (!ok)
                LOG_ERROR() << "Could not access ruby scene dictionary. Expected it to be at " << "'/sys/server/scene/RubySceneImporter'" << ".";
            else
            {
                ok = server->Eval("rubySceneImporter.enableSceneDictionary(true);");
                if (!ok)
                    LOG_ERROR() << "Could not enable ruby scene dictionary.";
            }
        }
        else
        {
            LOG_ERROR() << "ScriptServer not found. Cannot initialize RubySceneImporter dictionary.";
        }

        //Cleanup argv
        run = 0;
        for (auto it = mParameters.begin(); it != mParameters.end(); it++)
        {
            delete[] params[run];
            run++;
        }
        delete[] params;
    }
    else
    {
        //No futher initialization needed after Reinit()
    }

    return 0;
}

int SparkController::runSpark()
{
    if (!assertSpark())
        return 1;

    //Run Simulation loop
    if (!mLogAccessSemaphore.tryAcquire(1, 1000))
    {
        LOG_ERROR() << "Simspark access is locked. Timeout.";
        return 1;
    }

    //Check for gui-internal rendering context
    OpenGLManager* manager = Carbon::get()->getOpenGLManager();
    bool unique = false;
    bool lockedContext = false;
    GLWidget* widget = manager->getRegisteredWidget(OpenGLManager::GWF_SERVER, unique);
    if (widget != 0)
    {
        if (!unique)
            LOG_INFO() << "Found more than one rendering context to use for SimSpark. Choosing first widget " << widget->getName() << ".";

        //We will be updating the gl context of this widget, prevent rendering during simspark initialization
        //This will: lock rendering or gl-resize calls, disable update functions (resize, repaint), send a blocked lock signal, call makeCurrent() in this thread 
        lockedContext = manager->lockGLWidget(widget, OpenGLManager::GSS_ACCESS, true);
    }

    if (lockedContext)
        mSpark->AddScriptValue("gui_enableExternalRendering", SimSpark::RubyValue(true));
    else
    {
        if (mSpark->HasScriptValue("gui_enableExternalRendering"))
            mSpark->AddScriptValue("gui_enableExternalRendering", SimSpark::RubyValue()); //Reset variable to nil
    }

    //First run-step: execute run scripts
    int returning = mSpark->ExecuteRunScripts();

    if (lockedContext)
    {
        //Frees rendering context, Unlocks rendering, re-enables update functions, sends non-blocked end-access signal
        manager->unlockGLWidget(widget, OpenGLManager::GSS_END_ACCESS, false);
    }

    if (returning != 0)
    {
        LOG_ERROR() << "Error during execution of simulation run scripts.";
        mLogAccessSemaphore.release();
        return returning;
    }

    //Second run step: start simulation server
    LOG_INFO() << "Before starting SimSpark: Semaphore release";
    Carbon::get()->getSimulationManager()->getSparkServerThread()->mExecutionSemaphore.release();
    emit sparkRunning(this);
    returning = mSpark->Run();
    mLogAccessSemaphore.release();
    emit sparkFinished(this);
    
    return returning;
}

void SparkController::stopSpark()
{
    if (!assertSpark())
        return;

    //Signal spark to quit. It will leave its simulation servers run() method as soon as possible
    mSpark->Quit();
}

void SparkController::pauseSpark(bool pause)
{
    if (!assertSpark())
        return;

    if (pause)
    {
        //Pause spark
        bool paused = mSpark->Pause();
        if (paused)
        {
            mLogAccessSemaphore.release();
            emit sparkPaused(this);
        }
    }
    else
    {
        //Unpause spark
        if (!mSpark->IsPaused())
            return;

        if (!mLogAccessSemaphore.tryAcquire(1, 1000))
        {
            LOG_ERROR() << "SimSpark access is locked. Timeout.";
            return;
        }
        bool continued = mSpark->Continue();
        if (continued)
            emit sparkRunning(this);
        else
            mLogAccessSemaphore.release();
    }
}

void SparkController::resetSpark()
{
    if (!assertSpark())
        return;

    //Reset spark to initial state and pause it
    bool reset = mSpark->Reset();
    if (reset && mSpark->IsPaused())
        emit sparkPaused(this);
}


bool SparkController::queueCommand(SparkCommand::Command* command, int timeout, 
    const QObject* callback, const char* member, bool log_error)
{
    return queueCommand(boost::shared_ptr<SparkCommand::Command>(command), timeout, callback, member, log_error);
}

bool SparkController::queueCommand(int commandId, SparkCommand::Command* command, int timeout, 
    const QObject* callback, const char* member, bool log_error)
{
    return queueCommand(commandId, boost::shared_ptr<SparkCommand::Command>(command), timeout, callback, member, log_error);
}

bool SparkController::queueCommand(boost::shared_ptr<SparkCommand::Command> command, int timeout, 
    const QObject* callback, const char* member, bool log_error)
{
    if (command.get() == 0)
        return false;

    if (mCommandQueue.get() == 0)
    {
        LOG_ERROR() << "No command queue.";
        return false;
    }

    if (callback != 0)
    {
        connect(&*command, SIGNAL(executed(int, bool)), callback, member);
    }

    return mCommandQueue->queueCommand(command, timeout, log_error);
}

bool SparkController::queueCommand(int commandId, boost::shared_ptr<SparkCommand::Command> command, int timeout, 
    const QObject* callback, const char* member, bool log_error)
{
    if (command.get() == 0)
        return false;

    command->setId(commandId);
    return queueCommand(command, timeout, callback, member, log_error);
}

bool SparkController::tryAcquireRuntimeAccess(int timeout, bool exclusive)
{
    //Check exclusive access
    bool ok = true;
    if (exclusive)
        ok = mExclusiveRuntimeAccessSemaphore.tryAcquire(1, timeout);

    if (!ok)
        return false;

    //Check normal access (waiting for other lockers to release)
    ok = mRuntimeAccessSemaphore.tryAcquire(exclusive ? SPARK_RUNTIME_ACCESS_MAX : 1, timeout);

    if (!ok)
    {
        if (exclusive) mExclusiveRuntimeAccessSemaphore.release();
        return false;
    }

    return true;
}

void SparkController::acquireRuntimeAccess(bool exclusive)
{
    //Lock exclusive access
    bool ok = true;
    if (exclusive)
        mExclusiveRuntimeAccessSemaphore.acquire(1);

    //Lock normal access (waiting for other lockers to release)
    mRuntimeAccessSemaphore.acquire(exclusive ? SPARK_RUNTIME_ACCESS_MAX : 1);
}

void SparkController::releaseRuntimeAccess(bool exclusive)
{
    //Release normal access
    mRuntimeAccessSemaphore.release(exclusive ? SPARK_RUNTIME_ACCESS_MAX : 1);

    //Release exclusive access
    if (exclusive)
        mExclusiveRuntimeAccessSemaphore.release();
}

void SparkController::releaseRuntimeAccessDelayed(bool exclusive)
{
    if (exclusive)
        queueCommand(new SparkCommand::ReleaseRuntimeAccessDelayed(1, true), 100, this, SLOT(checkReleaseRuntimeAccessDelayedExclusive(int, bool)));
    else
        queueCommand(new SparkCommand::ReleaseRuntimeAccessDelayed(1, false), 100, this, SLOT(checkReleaseRuntimeAccessDelayed(int, bool)));
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------
    
bool SparkController::runScript(const QString& path)
{
    if (!mSpark->IsInitialized())
    {
        LOG_ERROR() << "Cant execute script " << path << ". Spark has to be initialized first.";
        return false;
    }

    if (path.compare("") == 0)
    {
        LOG_ERROR() << "Cant execute script. Path is empty string.";
        return false;
    }

    SimulationManager* manager = Carbon::get()->getSimulationManager();
    boost::shared_ptr<zeitgeist::ScriptServer> scriptserver(mSpark->GetCore()->GetScriptServer());

    if (scriptserver.get() == 0)
    {
        LOG_ERROR() << "Cant execute script " << path << ". Script server was not initialized in the spark initialization script.";
        return false;
    }

    //Prevent acces to script include paths
    QString absPath = manager->findScript(path);
    if (absPath.compare("") == 0)
    {
        LOG_ERROR() << "Script " << path << " not found. Specify its location absolutely or add its path to the include directories.";
        return false;
    }

    //Run
    LOG_DEBUG() << "Running script " << path << "...";
    return mSpark->RunScript(absPath.toStdString());
}

void SparkController::addLogStream(Logger::LogLevel level, int sourceId)
{
    zeitgeist::LogServer::EPriorityLevel sparkLevel;
    switch (level)
    {
        case Logger::Debug:   sparkLevel = zeitgeist::LogServer::eDebug; break;
        case Logger::Info:    sparkLevel = zeitgeist::LogServer::eNormal; break;
        case Logger::Warning: sparkLevel = zeitgeist::LogServer::eWarning; break;
        case Logger::Error:   sparkLevel = zeitgeist::LogServer::eError; break;
        default:
            LOG_INFO() << "Cant add log stream with level " << level << ".";
            return;
        break;
    }

    //Add the stream
    mLogStreams.push_back(boost::shared_ptr<LogStream>(new LogStream(level, 1024, sourceId)));
    mSpark->AddLogStream(mLogStreams.back(), sparkLevel, true);
}
//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool SparkController::assertSpark() const
{
    if (mSpark.get() == 0)
    {
        LOG_ERROR() << "No spark object initialized.";
        return false;
    }

    return true;
}

void SparkController::initCommandQueue()
{    
    boost::shared_ptr<GuiSimControl> control(mSimControl.lock());
    if (control.get() == 0)
    {
        LOG_ERROR() << "No GuiSimControl node.";
        return;
    }
    
    if (mCommandQueue != 0)
    {
        control->unregisterSimControl(mCommandQueue);
    }

    mCommandQueue = boost::shared_ptr<SparkCommand::CommandQueue>(new SparkCommand::CommandQueue(this));
    control->registerSimControl(mCommandQueue);
}

void SparkController::setLogPropertiesProtected(Logger::LogLevel logLevel, int sourceId, bool useLogSemaphore)
{
    bool levelchanged = false;

    if (!mSpark.get())
    {
        LOG_DEBUG() << "Spark has not been initialized yet. Changing log level as preparation.";
        mLogLevel = logLevel;
        levelchanged = true;
    }

    if (useLogSemaphore)
    if (!mLogAccessSemaphore.tryAcquire())
    {
        LOG_INFO() << "Cant change log properties while SimSpark SimulationServer is running.";
        return;
    }

    if (!levelchanged)
    {
        //Only check source id for initialized spark
        if (sourceId == -1)
            sourceId = mLogSourceId;

        if (sourceId == -1)
        {
            LOG_DEBUG() << "Logging has not been initialized yet. Changing log level as preparation.";
            levelchanged = true;
            mLogLevel = logLevel;
            return;
        }
    }

    if (!levelchanged)
    {
        //Did source id change?
        if (sourceId != mLogSourceId)
        {
            levelchanged = mLogLevel != logLevel;

            //Reinit
            mLogLevel = logLevel;
            initLogging(sourceId);
        }
        else
        {
            //Did log level change?
            if (logLevel > mLogLevel)
            {
                //LogLevel raised, remove log streams with smaller level
                for (auto it = mLogStreams.begin(); it != mLogStreams.end();)
                {
                    if (it->get()->getLogLevel() < logLevel)
                    {
                         mSpark->GetLog()->RemoveStream(&**it);
                         it = mLogStreams.erase(it);
                    }
                    else
                        it++;
                }
                levelchanged = true;
            }
            else if (logLevel < mLogLevel)
            {
                //LogLevel lowered, add streams with smaller level
                for (int i = logLevel; i < mLogLevel-1; i++)
                {
                    addLogStream((Logger::LogLevel)i, sourceId);
                }
                levelchanged = true;
            }
            mLogLevel = logLevel;
        }
    }

    if (useLogSemaphore)
        mLogAccessSemaphore.release();

    if (levelchanged)
    {
        //Save changes to settings
        if (mSettings->beginGroup("gui_SparkController", true)) //lock
        {
            mSettings->setValue("mLogLevel", (int)mLogLevel);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings object to save SparkController settings to.";

        LOG_INFO() << "Spark log level changed to " << (int)mLogLevel;
        emit logLevelChanged(this, mLogLevel);
    }
}

//--------------------------------------------------------------
// Protected Slots
//--------------------------------------------------------------

void SparkController::checkReleaseRuntimeAccessDelayedExclusive(int id, bool success)
{
    if (!success)
    {
        //Spark not running, release either way
        releaseRuntimeAccess(true);
    }
    else
    {
        //Spark running and releasing was successful
    }
}

void SparkController::checkReleaseRuntimeAccessDelayed(int id, bool success)
{
    if (!success)
    {
        //Spark not running, release either way
        releaseRuntimeAccess(false);
    }
    else
    {
        //Spark running and releasing was successful
    }
}
