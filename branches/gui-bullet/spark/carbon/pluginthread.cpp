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

#include "pluginthread.h"
#include "pluginmanager.h"
#include "plugin.h"
#include "signalplugin.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PluginThread::PluginThread(boost::shared_ptr<TaskDefinition> definition) :
    SimulationThread(definition)
{
    //Check definition type
    if (definition->getType() != TaskDefinition::TT_PLUGINTHREAD)
        throw SimulationTask::IllegalTaskTypeException("PluginThread initialization error.", definition->getType(), TaskDefinition::TT_PLUGINTHREAD);

    mPlugin = 0;

    //Connect state update signals
    //running(id) is emitted via updateRunning() when calling the plugins run() function
    //paused(id) is emitted via updatePaused() when the plugins pause() function returned true
    //finished(id) is emitted via updateFinished() when the QThread emits finished()
    //terminated is never emitted

    connect(Carbon::get()->getPluginManager(), SIGNAL(aboutToRemovePlugin(AbstractPlugin*)), this, SLOT(updatePluginRemove(AbstractPlugin*)));
    connect(Carbon::get()->getPluginManager(), SIGNAL(aboutToDeletePlugin(AbstractPlugin*)), this, SLOT(updatePluginDelete(AbstractPlugin*)));
}

PluginThread::~PluginThread()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

bool PluginThread::hasPlugin()
{
    return mPlugin != 0;
}

//--------------------------------------------------------------
// Public Virtual
//--------------------------------------------------------------

void PluginThread::run()
{
    bool success = initPlugin();
    
    if (!success)
    {
        LOG_ERROR() << "Could not run plugin because plugin creation failed.";
        return;
    }

    //Start plugin
    updateRunning();
    mPlugin->run();
    updateFinished();
}

void PluginThread::stop()
{
    if (!assertPlugin())
    {
        LOG_ERROR() << "Stop PluginThread failed.";
        return;
    }

    //StopPlugin
    mPlugin->stop();
}

void PluginThread::pause(bool state)
{
    if (!assertPlugin())
    {
        LOG_ERROR() << "Pause PluginThread failed.";
        return;
    }

    //Pause/Unpause plugin
    bool success = mPlugin->pause(state);

    if (success) 
        emit updatePaused(state);
}

void PluginThread::restart()
{
    if (!assertPlugin())
    {
        LOG_ERROR() << "Restart PluginThread failed.";
        return;
    }

    //Restart plugin
    mPlugin->restart();
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool PluginThread::assertPlugin()
{
    if (mPlugin == 0)
    {
        LOG_ERROR() << "No plugin reference found.";
        return false;
    }

    return true;
}

bool PluginThread::initPlugin()
{
    PluginManager* manager = Carbon::get()->getPluginManager();

    //For new plugins, executable denotes the executable plugin, class mainScript the plugin caption and secondary script the plugin type (as integer)
    AbstractPlugin* plugin = manager->findPlugin(getTaskDefinition().getFirst(), getTaskDefinition().getSecond(), 
        getTaskDefinition().getThird().compare("") == 0 ? -1 : getTaskDefinition().getThird().toInt());

    if (plugin == 0)
    {
        LOG_ERROR() << "Could not initialize PluginThread because plugin to start with was not found.";
        mPlugin = 0;
        return false;
    }

    if (plugin->getPluginType() == PT_FRAME)
    {
        LOG_ERROR() << "Could not initialize PluginThread because plugin is of type AttachableFrame. AttachableFrame plugins can only be started in the main thread.";
        mPlugin = 0;
        return false;
    }

    if (plugin->getExecutionType() == AbstractPlugin::ET_NEVER)
    {
        LOG_ERROR() << "Could not initialize PluginThread because the plugin is not executable. It has to have an execution type of ONCE, LOOP or ANY.";
        mPlugin = 0;
        return false;
    }

    mPlugin = plugin;

    if (mPlugin->isQObject())
    {
        //Connect to plugin update functions
        connect(mPlugin->getQObject(), SIGNAL(paused()), this, SLOT(pluginPaused()));
        connect(mPlugin->getQObject(), SIGNAL(running()), this, SLOT(pluginRunning()));
        connect(mPlugin->getQObject(), SIGNAL(finished()), this, SLOT(pluginFinished()));
    }

    return true;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PluginThread::pluginPaused()
{
    if (getExecutionState() == TES_RUNNING)
        updatePaused(true);
}

void PluginThread::pluginRunning()
{
    if (getExecutionState() == TES_PAUSED)
    {
        updatePaused(false);
    }
    else if (getExecutionState() != TES_RUNNING)
        updateRunning();
}

void PluginThread::pluginFinished()
{
    if (getExecutionState() != TES_FINISHED)
        updateFinished();
}

void PluginThread::updatePluginRemove(AbstractPlugin* plugin)
{
    if (mPlugin == 0)
        return;

    if (plugin != mPlugin)
        return; //not this plugin

    //Should not reach this point. Before plugin deletion the Reference should have been removed already.
    LOG_WARNING() << "A plugin running in a PluginThread was removed without stopping the thread.";

    mPlugin = 0;
}

void PluginThread::updatePluginDelete(AbstractPlugin* plugin)
{
    if (mPlugin == 0)
        return;

    if (plugin != mPlugin)
        return; //not this plugin

    //Plugin is going to be deleted by the SimulationManager. Should not be done before stopping the simulation, but the plugin is still existant.
    LOG_WARNING() << "A plugin running in a PluginThread is going to be removed without stopping the thread. Aborting plugin execution...";

    stop();

    int waited = 0;
    bool stopped = true;
    while (getExecutionState() != TES_FINISHED && getExecutionState() != TES_TERMINATED)
    {
        wait(20);
        if (waited == 100) //Wait 2 seconds
        {
            LOG_ERROR() << "Could not stop plugin. No response.";
            stopped = false;
            break;
        }
    }

    if (stopped)
    {
        LOG_INFO() << "Plugin " << mPlugin->getCaption() << " (" << mPlugin->getClassName() << ") stopped.";

        if (mPlugin->isQObject())
        {
            disconnect(mPlugin->getQObject(), SIGNAL(paused()), this, SLOT(pluginPaused()));
            disconnect(mPlugin->getQObject(), SIGNAL(running()), this, SLOT(pluginRunning()));
            disconnect(mPlugin->getQObject(), SIGNAL(finished()), this, SLOT(pluginFinished()));
        }
    }

    mPlugin = 0;
}
