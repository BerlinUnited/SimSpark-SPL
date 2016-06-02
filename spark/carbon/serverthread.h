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

#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
/**
 * \file serverthread.h
 * \brief Contains ServerThread class, a thread for a general spark server, and ServerThreadMessenger.
 */

#include "sparksimulationthread.h"

class ServerThreadMessenger;
/*!
 \brief Implementation of the abstract SparkSimulationThread.
*/
class ServerThread : public SparkSimulationThread
{   
    Q_OBJECT

    friend class ServerThreadMessenger;
    
public:
    // public functions
    /*!
    The TaskDefinition::ETaskType of the definition has to be TaskDefinition::TT_SERVERTHREAD or a SimulationTask::IllegalTaskTypeException is thrown.

    \param definition definition for the SimulationTask
    \throw SimulationTask::IllegalTaskTypeException thrown if the TaskDefinition has a wrong task type
    */
    ServerThread(boost::shared_ptr<TaskDefinition> definition);
    /*!
     \brief Destructor.
    */
    virtual ~ServerThread();

    /*!
     \brief Restarts the server with a new task definition.

     Resets, reinitializes and runs SimSpark.

     \param newTaskDefinition
    */
    bool restartServer(const TaskDefinition& newTaskDefinition);

public:
   // virtual public functions

   /*!
    \brief Initializes and executes SimSpark.

    Implements the SimulationTasks interface function.\n

    Called by SimulationTask::runTask().
   */
   virtual void run();
    /*!
     \brief Stops SimSpark by pausing and resetting it.

     Implements the SimulationTasks interface function.\n
    */
   virtual void stop();
    /*!
     \brief Pauses or unpauses SimSpark.

     Implements the SimulationTasks interface function.\n
    */
   virtual void pause(bool pause = true);
    /*!
     \brief Restarts SimSpark with the same task definition.

     Resets reinitializes and runs SimSpark. Uses the ServerThreadMessenger to tell the actual thread (running in an exec() loop after first execution)
     to reinitialize and start SimSpark again.

     Implements the SimulationTasks interface function.\n
    */
   virtual void restart();

protected:
    //protected functions

   /*!
    \brief Restarts the server from the thread which executed it for the first time.

    The run() method of the server circles in an exec() loop after first execution. restart() uses the ServerThreadMessenger to tell the
    thread to run reinitialize SimSpark and run it again.
   */
   void serverRestart();
    
private:
    // private members

    ServerThreadMessenger* mServerMessenger; /*!< Messenger to send signals to the thread where run() is executed in. */
    bool mAwaitRestart; /*!< True while being in exec() loop waiting for server restart or shutdown command. */
};

/*!
 \brief Messenger to send signals from the main thread to the thread where the owning ServerThreads run() method is executed in.

 Using the ServerThread's signals and slots is not possible, because the base class SimulationTask has signals and slots which have to
 communicate with objects in the main thread internally. The messages of a given QObject can only be received by one thread.
 Therefore a Messenger is needed to receive messages in another exec() loop that the main threads loop.\n

 The function calls (e.g. emitRestart()) send a queued signal to the messenger himself, but are caught in another thread. The messenger
 then calls the target function of the ServerThread.
*/
class ServerThreadMessenger : public QObject
{
    Q_OBJECT

public: 
    //public functions

    /*!
     \brief Constructor.

     \param target message target
    */
    ServerThreadMessenger(ServerThread* target)
    {
        mTarget = target;
        connect(this, SIGNAL(restart()), this, SLOT(catchRestart()), Qt::QueuedConnection);
    }

public:
    //public functions

    /*!
     \brief Emits restart.
    */
    void emitRestart()
    {
        emit restart();
    }

public: signals:

    /*!
     \brief Emitted after catchRestart() was executed.
    */
    void restart();

public slots: 

    /*!
     \brief Restart server.
    */
    void catchRestart()
    {
        mTarget->serverRestart();
    }

private:
    ServerThread* mTarget; /*!< Owning server. */
};

#endif //SERVERTHREAD_H
