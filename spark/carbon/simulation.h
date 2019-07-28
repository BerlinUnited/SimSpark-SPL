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

#ifndef SIMULATIONSTATE_H
#define SIMULATIONSTATE_H
/**
 * \file simulation.h
 * \brief Contains Simulation class, which holds the state of the current simulation of the SimulationManager.
 */

#include "simulationsetup.h"
#include "simulationtask.h"

#include <QString>
#include <QSettings>
#include <QObject>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class ServerThread;

/*!
 \brief Class for running a Simulation in the SimulationManager.

 <b>General:</b>\n
 A SimulationSetup in the SimulationManager list or loaded from a file can be used to initialize a Simulation.
 The Simulation contains a definition of the SimulationSetup, a filename to save the Simulation to, and the current
 SimulationTasks (threads and processes) as defined in the SimulationSetup.
 There is always at most one Simulation initialized.
 Running tasks are stopped before the Simulation is closed and a Simulation has to be closed before initializing a new one.\n

 <b>Saving:</b>\n
 Simulation save files are basically a changed version of the initial SimulationSetup (containing the state after all the
 changes made after initialization). The difference is, that they contain a reference to a data file. Before closing all
 gui plugins of the setup are nofitied to save session data to the data object and when starting a new Simulation with this
 SimulationSetup save file the data file is loaded and handed to the plugins so they can recover their previous state.\n

 <b>Setup:</b>\n
 When initializing the Simulation with a SimulationSetup, the setup is deep-copied. This means, even though the setup in the current
 Simulation might have the same name and data as a SimulationSetup in the SimulationManagers preset setup list, they are different objects
 and contain different shared pointers to PluginDefinitions and TaskDefinitions. Changing the setup of the current Simulation will not
 affect the preset SimulationSetups. adter initializing a SimulationTask, it contain a shared pointer to the TaskDefinition in the
 initialized SimulationSetup. Changing the TaskDefinition of a SimulationTask will change the SimulationSetup of the current simulation.\n

 <b>Execution:</b>\n
 The SimulationManager only creates one single Simulation at the start.
 New Simulations are initialized by loading a new SimulationSetup in this Simulation.
 This way, plugins do not have to reconnect to the Simulations signals.
 Various signals of the Simulation like aboutToBeStopped(), running(), paused() etc. inform about the state of the Simulation.\n

 The Simulation can be initialized, run (if initialized or stopped), stopped (if running or paused), or paused (if running).
 Any of these calls to the Simulation does the same with all the contained SimulationTasks, unless specified otherwise.
 It is possible to start the Simulation without starting the tasks, and it is possible to start, pause or stop a SimulationTasks one
 at a time. However, there are two restrictions:\n
 1) Tasks can only be running (or paused), when the Simulation is running (or paused). Running a task will start the Simulation.
 Stopping the Simulation will stop all the tasks.\n
 2) Because of limitations of the SimSpark simulation engine, there has to be a single one server. Freely stopping it when there are
 dependant tasks will cause a crash. Therefore, when using a SimulationServer, its execution state is linked to the Simulation.
 Stopping the server will stop the Simulation. Starting the Simulation will always start the server.
 See SimSpark documentation for further information.\n

 \sa SimulationManager, SimSpark
*/
class Simulation : public QObject
{
    Q_OBJECT

public:
    // public types

    /*!
     \brief Helper-Type containing a list of SimulationTask pointers.
    */
    typedef std::vector<boost::shared_ptr<SimulationTask> > TTaskList;

public:
    // public functions
    /*!
     \brief Constructor.
    */
    Simulation();
    /*!
     \brief Destructor.
    */
    virtual ~Simulation();

    /*!
     \brief Returns const pointer to the SimulationSetup.
    */
    const boost::shared_ptr<SimulationSetup>& getSetup() const;
    /*!
     \brief Returns pointer to the SimulationSetup.

     \warning The SimulationSetup is used by plugins to read and edit data of the current Simulations.
     Changing the setup during a running simulation might result in undefined behaviour.
     It is meant to update the definitions of the current Simulation whenever the actual state of the Simulation was edited (like adding a new plugin or a new task).
     When changing the currently initialized setup make sure that the Simulation itself applies the same changes.
    */
    boost::shared_ptr<SimulationSetup>& getSetupNc();

    /*!
     \brief Returns the target filename of the simulation state.
    */
    const QString& getFilename() const;
    /*!
     \brief Returns a pointer to the data settings object.
    */
    boost::shared_ptr<Settings> getData() const;

    /*!
     \brief Returns the list of all initialized task.
    */
    const TTaskList& getTaskList() const;
    /*!
     \brief Returns the index of a task or 0 if it is not in the managed list.
    */
    int getTaskIndex(const SimulationTask* task) const;
    /*!
     \brief Returns the index of a task by its id or 0 if it is not in the managed list.
    */
    int getTaskIndex(int taskId) const;
    /*!
     \brief Returns the pointer to a task by its index or a null pointre if the index is invalid.
    */
    boost::shared_ptr<SimulationTask> getTask(int index) const;

    /*!
     \brief Returns the state of a thread in the list, or TES_UNDEFINED if not found.
    */
    SimulationTask::ETaskExecutionState getTaskExecutionState(const SimulationTask*) const;

    /*!
     \brief Returns true if the Simulation has been initialized.
    */
    bool isInitialized() const;
    /*!
     \brief Returns true if the Simulation has been initialized and is running.
    */
    bool isRunning() const;
    /*!
     \brief Returns true if the Simulation has been initialized, was running and is currently paused.
    */
    bool isPaused() const;
    /*!
     \brief Returns true if the specified simulation setup is owned by this simulation.
    */
    bool hasSetup(const SimulationSetup& setup) const;

    /*!
     \brief Initializes a setup in this simulation. Only works if the setup is closed.
    */
    bool initialize(const SimulationSetup& setup);
    /*!
     \brief Runs or unpauses the Simulation.

     The Simulation can be executed without starting all of the tasks. If the Simulation contains a server, it is dependent on the server.
     When a simulation with a server definition is started, the server is always started. The other tasks are optional.

     \param runTasks if true, all of the SimulationTasks are started too
    */
    bool run(bool runTasks = true);
    /*!
     \brief Pauses the Simulation if running.
    */
    bool pause();
    /*!
     \brief Stops the Simulation and asks if its data should be saved.

     If stopserver is false, the main spark server is not stopped but only resetted and paused.
    */
    bool stop(bool stopserver = false);
    /*!
     \brief Restart the Simulation by stopping and running it again.
    */
    bool restart();

    /*!
     \brief Load data file for this simulation and call all plugins loadData().

     \return true if loading the file was successful
    */
    bool loadData();
    /*!
     \brief Call saveData() for all plugins and save the data to a data file.

     \return true if saving was successful or there was nothing to save
    */
    bool saveData();

    /*!
     \brief Execute a single task of a simulation. Starts the simulation without the other threads if neccessary.
    */
    bool runTask(SimulationTask* task);
    /*!
     \brief Pauses a single task of running simulation.
    */
    bool pauseTask(SimulationTask* task);
    /*!
     \brief Stops a single task of running simulation.
    */
    bool stopTask(SimulationTask* task);

    /*!
     \brief Adds a new task to the setup definition and the currently initialized list of tasks.

     The task definition will be copied into the SimulationSetup. A Shared pointer is then given to the new SimulationTask and is returned.
     If the definition is illegal, no task is created and a null pointer is returned.

     \param taskDefinition task to add
     \return pointer to added task if adding was successful, or 0 pointer
    */
    boost::shared_ptr<TaskDefinition> addNewTask(TaskDefinition& taskDefinition);
    /*!
     \brief Removes a new task from the currently initialized list of tasks.

     Can only be done if the simulation is not running.

     \param task task to remove
     \return true if removing was successful
    */
    bool removeTask(const SimulationTask* task);

    /*!
     \brief Emits pluginListChanged().
    */
    void updatePluginListChanged();

private:
    // private functions

    /*!
     \brief clears the current tasks and SimulationTask list, emitting one taskRemoved() and taskListChanged() signal for each removed task.

     \warning Must only be used when all tasks have been stopped (Simulation is not running or paused).
    */
    void clearTasks();
    /*!
     \brief Adds a task to the currently initialized list of tasks.

     The task will keep a pointer to the task definition.
     It has to be exactly the definition that is stored in the SimulationSetup of the initialized Simulation.
     Otherwise changes by the gui in the definition will most likely not be stored in the setup and cant be saved in a savestate or a preset SimulationSetup.

     \param taskDefinition definition of task to add
     \return true if adding was successful
    */
    bool addTask(boost::shared_ptr<TaskDefinition> taskDefinition);
    /*!
     \brief Initializes the setups of the current SimulationState.

     \return true if initialization was successful
    */
    bool initializeTasks();

    /*!
     \brief Start the specified task.

     Depending on the tasks state it is started, unpaused or restarted.

     \param task task to start
     \param restart if true, a running task is restarted
    */
    void startTask(boost::shared_ptr<SimulationTask> task, bool restart);

public: signals:
    //public signals

    /*!
     \brief Emitted after a the simulation has been initialized.
    */
    void initialized();
    /*!
     \brief Emitted after a the simulation has been set to running. Tasks might still need some time to startup.
    */
    void running();
    /*!
     \brief Emitted directly before the simulation is stopped. Very last chance to store or fetch data.
    */
    void aboutToBeStopped();
    /*!
     \brief Emitted after a the simulation has been stopped. Tasks might still need some time to shutdown.
    */
    void stopped();
    /*!
     \brief Emitted after a the simulation has been paused. Tasks might still need some time to react.
    */
    void paused();

    /*!
     \brief Emitted when the Simulation was saved.

     \param path path of the file
    */
    void saved(QString path);

   /*!
     \brief Emitted when a task was added to the currently initialized task list. Its state at this time is preinitialized (TES_UNDEFINED).

     Since only the main thread adds new tasks, the index can be used for immediate access to the task list by index. The task id is unique and does never change.

     \param index index of the new task
     \param id id of the new task
    */
    void taskAdded(int index, int id);
    /*!
     \brief Emitted when a task was removed from the currently initialized task list.

     \param index former index of the removed task
     \param id former id of the removed task
    */
    void taskRemoved(int index, int id);
    /*!
     \brief Emitted when the task list changed (a new task was added or old task was removed).

     \sa taskAdded(), taskRemoved()
    */
    void taskListChanged();
    /*!
     \brief Emitted when the execution state of a currently initialized task has changed.

     \param index index of the task
     \param id id of the task
     \param state new state of the task
    */
    void taskStateChanged(int index, int id, SimulationTask::ETaskExecutionState state);
    /*!
     \brief Emitted when the task definition of a currently initialized task has changed.

     \param index index of the task
     \param id id of the task
    */
    void taskDefinitionChanged(int index, int id);
    /*!
      \brief Emitted after a part of the gui added, removed or changed a plugin and a definition in an initialized simulation.
    */
    void pluginListChanged();

    /*!
     \brief Emitted when a server thread was created.

     The server thread is then accessible statically via the SimulationManager.
     \param server pointr to server thread
    */
    void serverCreated(boost::shared_ptr<ServerThread> server);

public slots:
    //public slots

    /*!
     \brief Emits taskStateChanged(). Called by the SimulationTask whenever its execution state changed.

     \param id id of the task
     \param state new state of the task
    */
    void onTaskStateChanged(int id, SimulationTask::ETaskExecutionState state);

    /*!
     \brief Emits taskDefinitionChanged(). Called by the SimulationTask whenever its task definition changed.

     \param id id of the task
    */
    void onTaskDefinitionChanged(int id);

private:
    // private members

    boost::shared_ptr<SimulationSetup> mSetup; /*!< Initialized setup. */
    QString mFileName;                         /*!< Target filename. */
    boost::shared_ptr<Settings> mData;         /*!< State data. */

    bool mRunning;      /*!< True if the Simulation is running.*/
    bool mPaused;       /*!< True if the Simulation is paused.*/
    bool mInitialized;  /*!< True if the Simulation is initialized.*/

    TTaskList mTasks; /*!< List of initialized tasks. */

};

#endif //SIMULATIONSTATE_H
