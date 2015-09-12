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

#include "serverthread.h"
#include "sparkcontroller.h"

#include <QFileInfo>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

ServerThread::ServerThread(boost::shared_ptr<TaskDefinition> definition) :
    SparkSimulationThread(definition)
{
    //Check definition type
    if (definition->getType() != TaskDefinition::TT_SERVERTHREAD)
        throw SimulationThread::IllegalTaskTypeException("ServerThread initialization error.", definition->getType(), TaskDefinition::TT_SERVERTHREAD);

    mAwaitRestart = false;

    //Create a messenger in this thread to communicate with the main thread
    mServerMessenger = new ServerThreadMessenger(this);
    mServerMessenger->moveToThread(this);
}

ServerThread::~ServerThread()
{
    delete mServerMessenger;
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

bool ServerThread::restartServer(const TaskDefinition& newTaskDefinition)
{
    if (newTaskDefinition.getType() != TaskDefinition::TT_SERVERTHREAD)
    {
        LOG_ERROR() << "Thread definition has illegal type " << newTaskDefinition.getTypeString() << 
            ". It has to be " << TaskDefinition::getTypeString(TaskDefinition::TT_SERVERTHREAD) << ".";
        return false;
    }

    setTaskDefinition(newTaskDefinition);

    //Restart Server
    int error = 0;
    if (mController->getSpark())
    {
        error = initSpark(true);
    }
    else
    {
        //Has not been initialized yet, leave it as it is
    }

    if (error == 1)
        LOG_ERROR() << "Spark initialization error";

    return error == 0;
}

//--------------------------------------------------------------
// Public Virtual
//--------------------------------------------------------------

void ServerThread::run()
{
    //Initialize Spark Server (returns 0 if successful or already initialized)
    int returning = initSpark(true);
    if (returning == 1)
    {
        LOG_ERROR() << "Spark initialization error.";
        return;
    }
    if (returning == 2)
    {
        LOG_ERROR() << "Spark controller not initialized.";
        return;
    }

    //Init logging
    //initSparkLogging();

    //Run server
    returning = runSpark();
    if (returning != 0)
    {
        LOG_ERROR() << "Spark execution error";

        LOG_INFO() << "After execution error: Release Execution Semaphore.";
        mExecutionSemaphore.release();
    }

    if (returning == 0 && mShutdown == false)
    {
        //Await restart or quit
        mAwaitRestart = true;
        getMessenger()->updateStopped();
        exec();
        mAwaitRestart = false;
    }
}

void ServerThread::stop()
{
    stopSpark();
}

void ServerThread::restart()
{
    //Only restart if finished (behaviour is different from other tasks, which are stopped and started again because spark cant really be resetted)
    if (mAwaitRestart)
        mServerMessenger->emitRestart();
    else
    {
        //Unpause if paused (from main thread)
        pauseSpark(false);
    }
}

void ServerThread::pause(bool pause)
{
    pauseSpark(pause);
}

void ServerThread::serverRestart()
{
    if (!mAwaitRestart)
    {
        LOG_ERROR() << "ServerThread not ready to be restarted.";
        return;
    }
    mAwaitRestart = false;

    int returning = initSpark(true);
    if (returning == 1)
    {
        LOG_ERROR() << "Spark initialization error.";
        return;
    }
    else if (returning == 2)
    {
        LOG_ERROR() << "Spark controller not initialized.";
        return;
    }

    //Init logging
    initSparkLogging();

    //Run server
    returning = runSpark();
    if (returning == 1)
    {
        LOG_ERROR() << "Spark execution error";
        return;
    }
    getMessenger()->updateStopped();

    mAwaitRestart = true;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------
