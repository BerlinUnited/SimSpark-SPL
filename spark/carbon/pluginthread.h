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

#ifndef PLUGINTHREAD_H
#define PLUGINTHREAD_H
/**
 * \file pluginthread.h
 * \brief Contains PluginThread class, which can start gui Plugins and SignalPlugins in a new thread.
 */

#include "simulationthread.h"

class AbstractPlugin;

/*!
 \brief A SimulationTask which starts a Plugin or SignalPlugin in a new thread.

 PluginThreads are a SimulationTask created in a Simualtion when loading a SimulationSetup that contains a PluginThread TaskDefinition.
 The PluginThreads TaskDefinition contains a class name, a plugin caption and a plugin type. When the Simulation is started the plugin that
 matches this description is searched and executed in the PluginThread.
 The run() method is called by the PluginThread when the SimulationTask is executed and it is only executed once (it wont be executed again as long
 as the SimulationTask is still running). Other threads (usually the main thread) may call the stop(), pause() or restart().
 Those three functions are handed to the plugin that is being executed.\n

 The plugins are expected to behave in the following way:\n
 When run() is called, the Plugin is expected to start its execution. The PluginThread will emit running() immediately before the plugins run() is called.\n
 When stop() is called, the Plugin is expected to return from its run() routine soon. The PluginThread will emit finished() immediately after the
 plugins run() returned.\n
 When pause(true) is called, the PluginThread is expected to halt until pause(false) or stop() is called.
 The PluginThread will emit paused(true) when the plugin either emits paused() (SignalPlugin or AttachableFrame) or returns true in the pause() function.\n
 When restart() is called, the Plugin is expected to reset its state if possible and run again from the beginning or continue running respectively.
*/
class PluginThread : public SimulationThread
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic constructor.

     The TaskDefinition::ETaskType of the definition has to be TaskDefinition::TT_PLUGINTHREAD or a SimulationTask::IllegalTaskTypeException is thrown.

     \param definition definition for the SimulationTask
     \throw SimulationTask::IllegalTaskTypeException thrown if the TaskDefinition has a wrong task type
    */
    PluginThread(boost::shared_ptr<TaskDefinition> definition);
    /*!
     \brief Basic destructor.

     Removes reference to plugin but does not delete it.
    */
    virtual ~PluginThread();

    /*!
     \brief Returns true if a plugin was found and is currently referenced by this thread.
    */
    bool hasPlugin();

public:
   // virtual public functions

    /*!
     \brief Initializes and xecutes the plugin if possible.

     If the initialization failed, following stop(), pause() and restart() calls will fail too.

     Called by SimulationTask::runTask().
    */
    virtual void run();
    /*!
     \brief Stops the plugin if possible.
    */
    virtual void stop();
    /*!
     \brief Pauses the plugin if possible.
    */
    virtual void pause(bool state);
    /*!
     \brief Restarts the plugin if possible.
    */
    virtual void restart();

private:
    // private functions

    /*!
     \brief Checks if a plugin is being referenced and logs an error message if not.

     Used by stop(), pause() and restart().

     \returns true if there is a legal plugin initialized
    */
    bool assertPlugin();
    /*!
     \brief Initializes the PluginThread with the plugin defined in its TaskDefinition.

     The plugin is searched in the SimulationManager by its name, caption and or plugin type. If there are multiple matches the SimulationManager will
     send a warning and the first match will be chosen.

     If there are no matches or the found plugins has an execution type (AbstractPlugin::EExecutionType) which forbids the plugin to be executed in another
     thread an error is sent and the initialization fails.

     \returns true if initialization succeeded
    */
    bool initPlugin();

public slots:	
    //public slots
    
    /*!
     \brief Notifies the thread that the plugin is running.

     Signal is sent by the Plugin (if it is a QObject derived Plugin).
    */
    void pluginRunning();
    /*!
     \brief Notifies the thread that the plugin is paused.

     Signal is sent by the Plugin (if it is a QObject derived Plugin).
    */
    void pluginPaused();
    /*!
     \brief Notifies the thread that the plugin is finished.

     Signal is sent by the Plugin (if it is a QObject derived Plugin).
    */
    void pluginFinished();
    /*!
     \brief Notification that a plugin was deleted and is going to be removed from the PluginManager.

     This should not happen and notofies about an error. It means a plugin running in a Simulation was removed by the Gui without ending the Simulation first.
     If the pointer is identical to the contained plugin, the plugin reference removed.

     \param plugin pointer to plugin
    */
    void updatePluginRemove(AbstractPlugin* plugin);
    /*!
     \brief Notification that a plugin is going to be deleted from the PluginManager.

     This should not happen. A plugin running in a Simualtion is going to be deleted in the SimulationManager without ending the current Simulation.
     If the pointer is identical to the contained plugin, the execution is stopped and the plugin reference removed.

     \param plugin pointer to plugin
    */
    void updatePluginDelete(AbstractPlugin* plugin);

private:
    // private members

    AbstractPlugin* mPlugin; /*!< Pointer to the plugin. */
};

#endif //PLUGINTHREAD_H
