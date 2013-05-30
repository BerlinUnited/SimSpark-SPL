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

#include "carbon.h"
#include "simulationmanager.h"
#include "simulationtask.h"
#include "simulationthread.h"
#include "simulationprocess.h"
#include "serverthread.h"
#include "pluginthread.h"
#include "sparkprocess.h"
#include "process.h"

//--------------------------------------------------------------
// SimulationTaskMessenger
//--------------------------------------------------------------

SimulationTaskMessenger::SimulationTaskMessenger(SimulationTask* task)
{
    mTask = task;
}

SimulationTaskMessenger::~SimulationTaskMessenger()
{
}

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

SimulationTask* SimulationTask::createSimulationTask(boost::shared_ptr<TaskDefinition> definition)
{
    SimulationTask* newThread = 0;

    //Thread constructor might throw SimulationTask::IllegalTaskTypeException
    try
    {
        switch(definition->getType())
        {
        case TaskDefinition::TT_SERVERTHREAD:     newThread = new ServerThread (definition); break;
        case TaskDefinition::TT_PLUGINTHREAD:     newThread = new PluginThread (definition); break;
        case TaskDefinition::TT_PROCESS:          newThread = new Process      (definition); break;
        case TaskDefinition::TT_SPARKPROCESS:     newThread = new SparkProcess (definition); break;
        default:
            LOG_ERROR() << "Unknown task type " << definition->getType();
            break;
        }
    }
    catch (SimulationTask::IllegalTaskTypeException e)
    {
        LOG_ERROR() << e.message() << " : " << e.what();
    }
    catch (...)
    {
        LOG_ERROR() << "Unknown exception.";
    }

    return newThread;
}

int SimulationTask::getNextTaskId()
{
    return Carbon::get()->getSimulationManager()->getNextTaskId();
}

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SimulationTask::SimulationTask(boost::shared_ptr<TaskDefinition> definition) :
    mTaskDefinition(definition), mExecutionSemaphore(1)
{
    mId = getNextTaskId();
    mExecutionState = TES_UNDEFINED;

    //Create messenger to send and receive messages. Automatically connects to the task.
    mMessenger = new SimulationTaskMessenger(this);
}

SimulationTask::~SimulationTask()
{
    delete mMessenger;
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SimulationTaskMessenger* SimulationTask::getMessenger() const
{
    return mMessenger;
}

const TaskDefinition& SimulationTask::getTaskDefinition() const
{
    return *mTaskDefinition;
}

SimulationTask::ETaskExecutionState SimulationTask::getExecutionState() const
{
    return mExecutionState;
}

int SimulationTask::getId() const                               
{
    return mId;
}

SimulationThread* SimulationTask::toThread() const
{
    if (!isThread())
    {
        LOG_ERROR() << "Attempt to access a non-thread SimulationTask as SimulationThread.";
        return 0;
    }

    return (SimulationThread*)this;
}

SimulationProcess* SimulationTask::toProcess() const
{
    if (!isProcess())
    {
        LOG_ERROR() << "Attempt to access a non-process SimulationTask as SimulationProcess.";
        return 0;
    }

    return (SimulationProcess*)this;
}


bool SimulationTask::isThread() const       {return mTaskDefinition->isThread();}
bool SimulationTask::isProcess() const      {return mTaskDefinition->isProcess();}

bool SimulationTask::stateUndefined() const   {return mExecutionState == TES_UNDEFINED;}
bool SimulationTask::stateInitialized() const {return mExecutionState == TES_INITIALIZED;}
bool SimulationTask::stateRunning() const     {return mExecutionState == TES_RUNNING;}
bool SimulationTask::statePaused() const      {return mExecutionState == TES_PAUSED;}
bool SimulationTask::stateStopped() const     {return mExecutionState == TES_STOPPED;}
bool SimulationTask::stateFinished() const    {return mExecutionState == TES_FINISHED;}
bool SimulationTask::stateTerminated() const  {return mExecutionState == TES_TERMINATED;}

void SimulationTask::runTask()
{
    if (isThread())
    {
        //To start task in another thread, run() has to be called by start() of the new thread
        toThread()->start();
    }
    else if (isProcess())
    {
        toProcess()->run();
    }
    else
    {
        LOG_ERROR() << "Illegal simulation task type.";
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool SimulationTask::setTaskDefinition(const TaskDefinition& definition)
{
    *mTaskDefinition = definition;

    return true;
}

//--------------------------------------------------------------
// Slots (Messenger): Calls the respective functions of the SimulationTask
//--------------------------------------------------------------

void SimulationTaskMessenger::updateInitialized()       {mTask->updateInitialized();}
void SimulationTaskMessenger::updateRunning()           {mTask->updateRunning();}
void SimulationTaskMessenger::updatePaused(bool state)  {mTask->updatePaused(state);}
void SimulationTaskMessenger::updatePaused()            {mTask->updatePaused();}
void SimulationTaskMessenger::updateStopped()           {mTask->updateStopped();}
void SimulationTaskMessenger::updateFinished()          {mTask->updateFinished();}
void SimulationTaskMessenger::updateTerminated()        {mTask->updateTerminated();}
void SimulationTaskMessenger::updateDefinitionChanged() {mTask->updateDefinitionChanged();}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationTask::updateInitialized()
{
    if (mExecutionState == TES_INITIALIZED)
        return;

    mExecutionState = TES_INITIALIZED;
    emit mMessenger->initialized(mId);
    emit mMessenger->executionStateChanged(mId, TES_INITIALIZED);
}

void SimulationTask::updateRunning()
{
    if (mExecutionState == TES_RUNNING)
        return;

    mExecutionState = TES_RUNNING;
    emit mMessenger->running(mId);
    emit mMessenger->executionStateChanged(mId, TES_RUNNING);
}

void SimulationTask::updatePaused(bool state)
{
    if (mExecutionState == TES_PAUSED && state == true)
        return;

    if (mExecutionState == TES_RUNNING && state == false)
        return;

    if (state == false)
    {
        updateRunning();
        return;
    }

    mExecutionState = TES_PAUSED;
    emit mMessenger->paused(mId);
    emit mMessenger->executionStateChanged(mId, TES_PAUSED);
}

void SimulationTask::updatePaused()
{
    if (mExecutionState == TES_PAUSED)
        return;

    mExecutionState = TES_PAUSED;
    emit mMessenger->paused(mId);
    emit mMessenger->executionStateChanged(mId, TES_PAUSED);
}

void SimulationTask::updateStopped()
{
    if (mExecutionState == TES_STOPPED)
        return;

    mExecutionState = TES_STOPPED;
    emit mMessenger->stopped(mId);
    emit mMessenger->executionStateChanged(mId, TES_STOPPED);
}

void SimulationTask::updateFinished()
{
    if (mExecutionState == TES_FINISHED)
        return;

    mExecutionState = TES_FINISHED;
    emit mMessenger->finished(mId);
    emit mMessenger->executionStateChanged(mId, TES_FINISHED);
}

void SimulationTask::updateTerminated()
{
    if (mExecutionState == TES_TERMINATED)
        return;

    mExecutionState = TES_TERMINATED;
    emit mMessenger->terminated(mId);
    emit mMessenger->executionStateChanged(mId, TES_TERMINATED);
}

void SimulationTask::updateDefinitionChanged()
{
    emit mMessenger->taskDefinitionChanged(mId);
}
