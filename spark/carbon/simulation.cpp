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

#include "simulation.h"
#include "pluginmanager.h"
#include "simulationmanager.h"
#include "sparksimulationthread.h"
#include "sparkcontroller.h"
#include "simulationprocess.h"
#include "settings.h"
#include "serverthread.h"

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Simulation::Simulation()
{
    mFileName = "";
    mData = shared_ptr<Settings>();
    mSetup = shared_ptr<SimulationSetup>(new SimulationSetup());
    
    mRunning = false;
    mInitialized = false;
    mPaused = false;
}

Simulation::~Simulation()
{

}

const shared_ptr<SimulationSetup>& Simulation::getSetup() const    
{
    return mSetup;
}

boost::shared_ptr<SimulationSetup>& Simulation::getSetupNc()
{
    return mSetup;
}

const QString& Simulation::getFilename() const         
{
    return mFileName;
}

shared_ptr<Settings> Simulation::getData() const
{
    return mData;
}

const Simulation::TTaskList& Simulation::getTaskList() const
{
    return mTasks;
}

int Simulation::getTaskIndex(const SimulationTask* task) const
{
    int run = 0;
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        if (&**it == task)
            return run;
        run++;
    }

    return -1;
}

int Simulation::getTaskIndex(int taskId) const
{
    int run = 0;
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        if (it->get()->getId() == taskId)
            return run;
        run++;
    }

    return -1;
}

shared_ptr<SimulationTask> Simulation::getTask(int index) const
{
    if (index >= 0 && (int)mTasks.size() < index)
        return mTasks.at(index);

    return shared_ptr<SimulationTask>();
}


SimulationTask::ETaskExecutionState Simulation::getTaskExecutionState(const SimulationTask* thread) const
{
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        if (it->get() == thread)
        {
            return it->get()->getExecutionState();
        }
    }

    return SimulationTask::TES_UNDEFINED;
}


bool Simulation::isInitialized() const
{
    return mInitialized;
}

bool Simulation::isRunning() const
{
    return mRunning;
}

bool Simulation::isPaused() const
{
    return mPaused;
}

bool Simulation::hasSetup(const SimulationSetup& setup) const
{
    return &*mSetup == &setup;
}

bool Simulation::initialize(const SimulationSetup& setup)
{
    if (isRunning() || isPaused())
    {
        LOG_ERROR() << "Cant initialize a new Simulation while the actual simulation is still running.";
        return false;
    }

    //Save old data

    //Initialize data
    LOG_DEBUG() << "Initializing Setup " << setup.getName() << ": Data...";
    mSetup->readData(setup, true); //Copy the setup without using the same shared pointer
    mFileName = QString("%1").arg(mSetup->getName());
    mData = shared_ptr<Settings>(new Settings(mFileName, 0, QSettings::IniFormat)); //Closing old data will save them automatically. 

    //Initialize plugins
    LOG_DEBUG() << "Initializing Setup " << setup.getName() << ": Plugins...";
    PluginManager* manager = Carbon::get()->getPluginManager();
    bool success = manager->loadPlugins(mSetup->getAddPlugins(), mSetup->getRemovePlugins(), mSetup->getReloadAll(), mSetup->getRemoveAll());
    if (!success)
    {
        LOG_ERROR() << "Failed to initialize Simulation: Error while loading Plugins.";
        return false;
    }

    //Initialize Tasks
    LOG_DEBUG() << "Initializing Setup " << setup.getName() << ": Tasks...";
    success = initializeTasks();
    if (!success)
    {
        LOG_ERROR() << "Failed to initialize Simulation: Error while initializing Tasks.";
        return false;
    }

    //Load plugin data
    loadData();

    mInitialized = true;
    emit initialized();

    return success;
}

bool Simulation::run(bool runTasks)
{
    int run = 0;
    int max = mSetup->getTaskDefinitions().size();

    if (mTasks.size() == 0)
    {
        LOG_INFO() << "No Tasks to run.";
        return true;
    }

    //Has server?
    bool hasServer = mSetup->hasTaskDefinition(TaskDefinition::TT_SERVERTHREAD);
    if (!hasServer)
    {
        LOG_DEBUG() << "Running Simulation without Server definition.";
    }

    if (isPaused())
        LOG_DEBUG() << "Unpausing " << mTasks.size() << " tasks.";
    else
        LOG_DEBUG() << "Executing " << mTasks.size() << " tasks.";

    //Which kind of spark server do we use?
    bool internalServer = true;
    int serverindex = mSetup->findNextTaskDefinition(0, TaskDefinition::TT_SERVERTHREAD);
    if (serverindex == -1)
    {
        internalServer = false;
    }

    //Simulation not paused anymore and running now
    mPaused = false;
    mRunning = true;
    emit running();

    //Start or unpause internal server first (server task is always started. after first execution it is in exec loop)
    if (internalServer && runTasks)
        startTask(mTasks.at(serverindex), true); // {#Spark-Reset#} when complete resetting is possible, the server may leave its exec loop and this reset flag is not needed anymore

    //Start the other tasks
    run = 0;
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        //Only start server once
        if (run == serverindex)
        {
            run++;
            continue;
        }

        //Only start one internal server
        if (it->get()->getTaskDefinition().isSpark())
        {
            LOG_INFO() << "Cant start more than one spark thread. Skipping task " << it->get()->getTaskDefinition().getName() << ".";
            run++;
            continue;
        }

        //Start or unpause task if runTasks is true. Servers are always started with the Simulation.
        if (/*it->get()->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD || */runTasks)
            startTask(*it, false);
    }

    return true;
}

bool Simulation::stop(bool stopserver)
{
    if (!isInitialized())
        return true;

    //Run first (some tasks may never exit when being paused)
    if (isRunning() && isPaused())
        run();

    //Last chance to save data for plugins
    emit aboutToBeStopped();

    bool skipServer = false;
    //Send stop signal
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        SimulationTask* task = it->get();
        /*if (task->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD && stopserver == false)
        {
            //Do not stop the main spark server if not explicitly ordered to, reset it instead
            skipServer = true;
            shared_dynamic_cast<SparkSimulationThread>(*it)->reset();
        }
        else*/
        {
            //Unpause
            if (task->statePaused())
                task->pause(false);

            //Stop plugin and/or process execution
            if (task->stateRunning())
                task->stop();

            //Quit thread wait-exec loop (if still running)
            if (stopserver && task->isThread())
                task->toThread()->shutdown();
        }
    }

    //Wait
    for (int run = 1; run < 401; run++)
    {
        //Count closing tasks
        int not_closed = 0;
        shared_ptr<SimulationTask> first = shared_ptr<SimulationTask>();
        bool skippingServer = false;
        for (auto it = mTasks.begin(); it != mTasks.end(); it++)
        {
            SimulationTask* task = it->get();

            //Wait for all threads. if stopserver is false, do not wait for spark server
            bool isServer = (task->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD);
            if (!isServer || stopserver == true)
            if (task->stateRunning())
            {
                not_closed++;
                first = shared_ptr<SimulationTask>(*it);
            }
            else
            {
                //isServer and stopall == false
            }
        }
        
        //One message every second
        if (not_closed > 0)
        {
            if (run % 20 == 0)
            {
                LOG_INFO() << "Waiting for " << not_closed << " tasks to finish... " << (run / 20);
            }
        }
        else
            break;

        //wait 50 msec
        if (first->isProcess())
        {
            //Wait for process
            first->toProcess()->waitForFinished(50);
        }
        else if (first->isThread())
        {
            //Wait for thread
            first->toThread()->wait(50);
        }
        else
        {
            LOG_FATAL() << "Illegal task type. Neither process nor thread.";
        }
    }
        
    //Terminate
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        SimulationTask* task = it->get();

        //Terminate remaining threads. If stopserver is false, dont terminate main server
        if (task->getTaskDefinition().getType() != TaskDefinition::TT_SERVERTHREAD || stopserver == true)
        if (task->stateRunning())
        {
            LOG_INFO() << "Terminating task " << task->getTaskDefinition().getName();
            if (task->isProcess())
            {
                //Terminate QProcess
                task->toProcess()->terminate();
            }
            else if (task->isThread())
            {
                //Terminate QThread
                task->toThread()->terminate();
            }
        }
    }

    mRunning = false;
    mPaused = false;
    LOG_INFO() << "Simulation stopped." << (skipServer ? " Spark server resetted." : "");

    emit stopped();
    return true;
}

bool Simulation::pause()
{
    if (isPaused())
    {
        LOG_INFO() << "Simulation already paused.";
        return false;
    }

    if (!isRunning())
    {
        LOG_INFO() << "Simulation is not running.";
        return false;
    }

    //Send pause signal
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        //Can only pause threads and observation of processes. Processes will continue running.
        it->get()->pause(true);
    }

    mPaused = true;    
    emit paused();

    return true;
}

bool Simulation::restart()
{
    if (!isInitialized())
    {
        LOG_ERROR() << "Simulation not initialized.";
        return false;
    }

    if (!isRunning() && !isPaused())
    {
        return run();
    }
    else 
    {
        LOG_INFO() << "Restarting Simulation.";

        //Stop threads and processes
        stop();

        //Re-run
        return run();
    }

    return false;
}

bool Simulation::loadData()
{
    if (!isInitialized())
        return false;

    //Load data for all plugins
    LOG_DEBUG() << "Loading data for all plugins of the Simulation...";
    PluginManager* manager = Carbon::get()->getPluginManager();
    return manager->loadData(mData);
}

bool Simulation::saveData()
{
    if (!isInitialized())
        return false;

    //Save data for all plugins
    LOG_DEBUG() << "Saving data for all plugins of the Simulation...";
    PluginManager* manager = Carbon::get()->getPluginManager();
    return manager->saveData(mData);
}

bool Simulation::runTask(SimulationTask* task)
{
    int index = getTaskIndex(task);
    if (index == -1)
    {
        LOG_ERROR() << "Task not found in current task list.";
        return false;
    }

    //Start simulation if not running (via SimulationManager so its signals are emitted too)
    if (!mRunning || mPaused)
    {
        bool success = Carbon::get()->getSimulationManager()->runSimulation(false);

        if (!success)
        {
            LOG_ERROR() << "Could not run the Simulation.";
            return false;
        }
    }

    if (task->getTaskDefinition().isSpark())
    {
        LOG_INFO() << "Before startTask: Acquire Execution Semaphore.";
        task->mExecutionSemaphore.acquire(1);
    }

    //If not running, start/unpause the task (only this task)
    startTask(mTasks.at(index), false);

    if (task->getTaskDefinition().isSpark())
    {
        LOG_INFO() << "After startTask: Awaiting Semaphore release.";
        if (task->mExecutionSemaphore.tryAcquire(1))
        {
            task->mExecutionSemaphore.release(1);
        }
        else
        {
            LOG_INFO() << "After startTask: Awaiting Semaphore release time out. Releasing.";
        }
        LOG_INFO() << "After startTask: Semaphore release.";
    }

    return true;
}

bool Simulation::pauseTask(SimulationTask* task)
{
    int index = getTaskIndex(task);
    if (index == -1)
    {
        LOG_ERROR() << "Task not found in current task list.";
        return false;
    }

    //Return if simulation is not running
    if (!mRunning)
    {
        LOG_INFO() << "Simulation is not running.";
        return false;
    }

    //If running, pause the task
    mTasks.at(index)->pause(true);

    return true;
}

bool Simulation::stopTask(SimulationTask* task)
{
    int index = getTaskIndex(task);
    if (index == -1)
    {
        LOG_ERROR() << "Task not found in current task list.";
        return false;
    }

    //Return if simulation is not running or paused
    if (!mRunning && !mPaused)
    {
        LOG_INFO() << "Simulation is not running.";
        return false;
    }

    //If the thread is the host thread, stop complete simulation (via SimulationManager so its signals are emitted too)
    shared_ptr<ServerThread> server(Carbon::get()->getSimulationManager()->getSparkServerThread());
    if (server.get() && &*server == task)
    {
        return Carbon::get()->getSimulationManager()->stopSimulation();
    }

    //Protect access to server (while stopping spark processes or threads, do not read from spark)
    bool exclusiveAccess = false;
    if (task->getTaskDefinition().isSpark())
    {
        bool hasServer = getSetup()->hasTaskDefinition(TaskDefinition::TT_SERVERTHREAD);
        if (hasServer)
            server = Carbon::get()->getSimulationManager()->getSparkServerThread();
        if (server.get() != 0)
        {
            server->getSparkController()->acquireRuntimeAccess(true);
            exclusiveAccess = true;
        }
    }

    //If running or paused, stop the task
    mTasks.at(index)->stop();

    //Release access to server after 1 cycle (so changes because of removed processes could be applied)
    if (exclusiveAccess)
    {
        server->getSparkController()->releaseRuntimeAccessDelayed(true);
    }

    return true;
}

shared_ptr<TaskDefinition> Simulation::addNewTask(TaskDefinition& taskDefinition)
{
    if (!isInitialized())
    {
        LOG_ERROR() << "Cannot add new task before initialization of the Simulation.";
        return shared_ptr<TaskDefinition>();
    }

    //Inserting copies the definition
    shared_ptr<TaskDefinition> newDefinition(mSetup->insertTask(taskDefinition));
    if (newDefinition.get())
    {
        //Create the task
        bool success = addTask(newDefinition);
        if (!success)
        {
            LOG_ERROR() << "Error while adding new task " << taskDefinition.getName() << ".";

            //Error, remove from list
            mSetup->eraseTask(&*newDefinition);
            return shared_ptr<TaskDefinition>();
        }
    }

    return newDefinition;
}

bool Simulation::removeTask(const SimulationTask* task)
{
    if (!isInitialized())
    {
        LOG_ERROR() << "Cant remove task. Simulation is not initialized.";
        return false;
    }

    if (isRunning() || isPaused())
    {
        LOG_INFO() << "Cant remove task from a running simulation.";
        return false;
    }

    int index = 0;
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        if (it->get() == task)
        {
            int id = mTasks.back()->getId();
            mTasks.erase(it);
            mSetup->eraseTask(index);
            emit taskRemoved(index, id);
            emit taskListChanged();
            return true;
        }
        index++;
    }

    return false;
}

void Simulation::updatePluginListChanged()
{
    emit pluginListChanged();
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void Simulation::clearTasks()
{
    while(mTasks.size() > 0)
    {
        int id = mTasks.back()->getId();
        mTasks.pop_back();
        emit taskRemoved(mTasks.size(), id);
        emit taskListChanged();
    }
}

bool Simulation::addTask(boost::shared_ptr<TaskDefinition> taskDefinition)
{
    shared_ptr<SimulationTask> task = shared_ptr<SimulationTask>();
    SimulationManager* manager = Carbon::get()->getSimulationManager();
    shared_ptr<ServerThread> server = manager->getSparkServerThread();

    bool createNew = true;
    if (taskDefinition->getType() == TaskDefinition::TT_SERVERTHREAD)
    {
        //Only one server can be initialized. If there already is one, use it
        if (server.get() != 0)
        {
            //Reinitialize this thread and use it again
            bool success = server->restartServer(*taskDefinition); //Data of the task definition will be changed (changes SimulationSetup)
            if (!success)
            {
                LOG_ERROR() << "Could not reinitialize server thread " <<  server->getTaskDefinition().getName();
                return false;
            }
            
            createNew = false;
        }
        else
        {
            //Its the first time for the server to be initialized. Start it like any other thread.
        }

        if (createNew)
        {
            //Try to create thread
            ServerThread* ptr = dynamic_cast<ServerThread*>(SimulationTask::createSimulationTask(taskDefinition));

            if (ptr == 0)
            {
                LOG_ERROR() << "Could not create thread " << taskDefinition->getName() << " (" << TaskDefinition::getTypeString(taskDefinition->getType()) << ")";
                return false;
            }

            //Register
            server = manager->initSparkServerThread(ptr);
            if (server.get() == 0)
            {
                LOG_ERROR() << "Could not create thread " << taskDefinition->getName() << " (" << TaskDefinition::getTypeString(taskDefinition->getType()) << 
                    "). Failed to register server thread in SimulationManager.";
                return false;
            }

        }

        task = shared_ptr<SimulationTask>(shared_dynamic_cast<SimulationTask>(server));
    }
    else
    {
        //Non-server tasks

        //Try to create task
        SimulationTask* ptr = SimulationTask::createSimulationTask(taskDefinition);

        if (ptr == 0)
        {
            LOG_ERROR() << "Could not create task " << taskDefinition->getName() << " (" << TaskDefinition::getTypeString(taskDefinition->getType()) << ")";
            return false;
        }

        //Success
        task = shared_ptr<SimulationTask>(ptr);
    }

    //Add to list
    int index = mTasks.size();
    mTasks.push_back(task);

    //Connect task signals
    const TaskDefinition& definition = task->getTaskDefinition();
    const SimulationTaskMessenger* messenger = task->getMessenger();
    connect(messenger, SIGNAL(executionStateChanged(int, SimulationTask::ETaskExecutionState)), this, SLOT(onTaskStateChanged(int, SimulationTask::ETaskExecutionState)));

    //Connect threaddefinition signals
    connect(messenger, SIGNAL(taskDefinitionChanged(int)), this, SLOT(onTaskDefinitionChanged(int)));

    emit taskAdded(index, task->getId());
    emit taskListChanged();

    if (taskDefinition->getType() == TaskDefinition::TT_SERVERTHREAD)
    {
        emit serverCreated(shared_static_cast<ServerThread>(task));
    }

    //If a existing spark server reinitialized, leave it paused. Otherwise initialize the task.
    if (createNew || 
        taskDefinition->getType() != TaskDefinition::TT_SERVERTHREAD)
        task->updateInitialized(); //emits initialized

    return true;
}

bool Simulation::initializeTasks()
{
    //Clear current list
    clearTasks();

    //Has server?
    if (!mSetup->hasTaskDefinition(TaskDefinition::TT_SERVERTHREAD))
    {
        LOG_DEBUG() << "Starting Simulation without server definition.";
    }

    bool allgood = true;
    for (auto it = mSetup->getTaskDefinitions().begin(); it != mSetup->getTaskDefinitions().end(); it++)
    {
        //Create new task with the stored task definition (so both share the same connections and changes in the Task itself will affect the SimulationSetup too)
        bool success = addTask(*it); 
        if (!success)
        {
            allgood = false;
            LOG_ERROR() << "Could not initialize task " << it->get()->getName() << " (" << TaskDefinition::getTypeString(it->get()->getType()) << ")";
        }
        else
            LOG_DEBUG() << "Initialized task " << it->get()->getName();
    }

    return allgood;
}

void Simulation::startTask(boost::shared_ptr<SimulationTask> task, bool restart)
{
    switch(task->getExecutionState())
    {
        case SimulationTask::TES_UNDEFINED:
            LOG_INFO() << "Cant start task " << task->getTaskDefinition().getName() << ". Tasks execution state is 'undefined'.";
        break;
        case SimulationTask::TES_INITIALIZED:
            LOG_DEBUG() << "Starting task " << task->getTaskDefinition().getName() << ".";
            task->runTask();
        break;
        case SimulationTask::TES_RUNNING:
            if (restart)
            {
                LOG_INFO() << "Restarting task " << task->getTaskDefinition().getName() << ".";
                task->restart();
            }
            else
            {
                LOG_DEBUG() << "Task " << task->getTaskDefinition().getName() << " already running.";
            }
        break;
        case SimulationTask::TES_PAUSED:
            if (restart)
            {
                LOG_INFO() << "Restarting paused task " << task->getTaskDefinition().getName() << ".";
                task->restart();
            }
            else
            {
                LOG_DEBUG() << "Unpausing task " << task->getTaskDefinition().getName() << ".";
                task->pause(false);
            }
        break;
        case SimulationTask::TES_STOPPED:
            LOG_INFO() << "Restarting stopped task " << task->getTaskDefinition().getName() << ".";
            task->restart();
        break;
        case SimulationTask::TES_FINISHED:
            LOG_DEBUG() << "Restarting finished task " << task->getTaskDefinition().getName() << ".";
            task->runTask();
        break;
        case SimulationTask::TES_TERMINATED:
            LOG_DEBUG() << "Restarting terminated task " << task->getTaskDefinition().getName() << ".";
            task->runTask();
        break;
    }
}

void Simulation::onTaskStateChanged(int id, SimulationTask::ETaskExecutionState state)
{
    //Search for the thread with this id and emit a task state change at its index

    //Search index
    int index = getTaskIndex(id);
    if (index == -1)
    {
        LOG_WARNING() << "Task with id " << id << " not found.";
        return;
    }

    SimulationTask& task = *mTasks.at(index);
    QString message("");
    switch(state)
    {
    case SimulationTask::TES_INITIALIZED:   message = QString("initialized"); break;
    case SimulationTask::TES_RUNNING:       message = QString("running"); break;
    case SimulationTask::TES_PAUSED:        message = QString("paused"); break;
    case SimulationTask::TES_STOPPED:       message = QString("stopped"); break;
    case SimulationTask::TES_FINISHED:      message = QString("finished"); break;
    case SimulationTask::TES_TERMINATED:    message = QString("terminated"); break;
    default:
        LOG_ERROR() << "SimulationTask reported illegal execution state " << state;
        break;
    }

    LOG_DEBUG() << "Task " << task.getTaskDefinition().getName() << ": " << message.toStdString().c_str();

    //Emit state change at given index
    emit taskStateChanged(index, task.getId(), state);
}

void Simulation::onTaskDefinitionChanged(int id)
{
    int index = getTaskIndex(id);
    if (index == -1)
    {
        LOG_WARNING() << "Task with id " << id << " not found.";
        return;
    }

    emit taskDefinitionChanged(index, id);
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

