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

#ifndef SIMULATIONTASK_H
#define SIMULATIONTASK_H
/**
 * \file simulationtask.h
 * \brief Contains SimulationTask and SimulationTaskMessenger classes.
 */

#include <QThread>
#include <QSemaphore>
#include <exception>
#include "taskdefinition.h"
#include "carbon.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class ThreadState;
class SimulationThread;
class SimulationProcess;
class SimulationTask;
class SimulationTaskMessenger;
class Simulation;
class SparkController;

/*!
 \brief Abstract class for a single task (thread or process) in a Simulation.

 SimulationTask is the base class for all threads and processes managed by the SimulationManager and contained in a Simulation.
 They are defined by a TaskDefinition and stored in a SimulationSetup.
 The concrete classes are either QThreads or QProcesses. Therefore the SimulationTask is not derived from QObject but uses a messager
 object of SimulationTaskMessager to receive and send messages.

 The SimulationTask is executed using runTask().
 Depending on its derived class this either executes run() in new Thread or uses run() to start a new Process.\n

 The SimulationTask keeps track of its actual state. It receives update calls from the threads and processes and notfies the
 SimulationManager if something changed.
 Since the SimulationTasks run() methods may all run in different threads, and thus notify any change via queued signals, the state is not
 always perfectly synced with the real state of the task. Therefore all the other gui elements and plugins should access the state the state
 via the SimulationManager or by requesting the ETaskExecutionState instead of asking the derived classes for their state.

 The possible states of a SimulationTask are defined in ETaskExecutionState.
*/
class SimulationTask
{
    friend Simulation;
    friend SimulationTaskMessenger;
    friend SparkController;

public:
    //public types

    /*!
     \brief State of execution of the task. Not every task can have all these states.
    */
    enum ETaskExecutionState
    {
        TES_UNDEFINED,    /*!< State before initialization or when reaching an illegal state. Both threads and processes start as undefined.*/
        TES_INITIALIZED,  /*!< Default value after initialization for threads and processes. */
        TES_RUNNING,      /*!< The task has been activated. Threads are running as long as they are in the run() routine. Process tasks are running as long as the process is running.*/
        TES_PAUSED,       /*!< The task is currently paused. For spark threads this means they are running and waiting, Plugin threads are halted, and processes are stopped to be observed.*/
        TES_STOPPED,      /*!< The task is currently stopped. For spark threads this means they left their spark Run() routine and are waiting in an exec loop. Plugin threads and processes are never stopped (only paused, finished or terminated).*/
        TES_FINISHED,     /*!< State after the thread finished its run() routine or the process returned finished. */
        TES_TERMINATED    /*!< State after the thread was forcefully terminated or the process crashed. */
    };

protected:
    // protected types

    /*!
     \brief Exception for SimulationTask derived constructors.

     The exception is thrown when a base SimulationTask object is initialized with a TaskDefinition containing a ETaskType
     that does not match with the the Class.
    */
    class IllegalTaskTypeException : public std::exception
    {
    public:
        /*!
         \brief Complete constructor.

         \param message message to throw
         \param got_type type found
         \param expected_type type expected
        */
        IllegalTaskTypeException(const QString& message, TaskDefinition::ETaskType got_type, TaskDefinition::ETaskType expected_type) :
          mMessage(message), mType(got_type), mExpectedType(expected_type)
          { hasMessage = mMessage.compare("") != 0; }

        ~IllegalTaskTypeException() throw() {}

        /*!
           \brief Returns the error message.
         */
        virtual QString message()
        {
            return (hasMessage? mMessage + " " : "") + QString("Illegal thread definition: ") + TaskDefinition::getTypeString(mType) + QString(" Expected type: ") +
                    TaskDefinition::getTypeString(mExpectedType);
        }

    private:
        const TaskDefinition::ETaskType mType;         /*!< Type received. */
        const TaskDefinition::ETaskType mExpectedType; /*!< Type expected. */
        const QString mMessage; /*!< Error message. */
        bool hasMessage;        /*!< True if there is a message. */
    };

public:
    //static public functions & members

    /*!
     \brief Creates a new SimulationTask with the type specified in the given TaskDefinition.

     The definition is used as constructor argument and stored in the task.

     \param definition definition to create task from
    */
    static SimulationTask* createSimulationTask(boost::shared_ptr<TaskDefinition> definition);

    /*!
     \brief Returns the next unique task Id.

     Calls the current simulation managers getNextTaskId() function.
    */
    static int getNextTaskId();

public:
    // public functions

    /*!
     \brief Destructor.
    */
    virtual ~SimulationTask();

    /*!
     \brief Returns the messenger for the SimulationTask.

     The messenger emits all signals and offers all slots for SimulationTask.
    */
    SimulationTaskMessenger* getMessenger() const;

    /*!
     \brief Returns the task definition.
    */
    const TaskDefinition& getTaskDefinition() const;

    /*!
     \brief Returns the execution state.
    */
    ETaskExecutionState getExecutionState() const;
    /*!
     \brief Returns the task as SimulationThread. Returns 0 if it actually is no thread.
    */
    SimulationThread* toThread() const;
    /*!
     \brief Returns the task as SimulationProcess. Returns 0 if it actually is no process.
    */
    SimulationProcess* toProcess() const;

    /*!
     \brief Returns the task id.
    */
    int getId() const;

    bool isThread() const;         /*!< \brief Returns true if the task is a SimulationThread.*/
    bool isProcess() const;        /*!< \brief Returns true if the task is a SimulationProcess.*/

    bool stateUndefined() const;   /*!< \brief Returns true if the task is in state TES_UNDEFINED.*/
    bool stateInitialized() const; /*!< \brief Returns true if the task is in state TES_INITIALIZED.*/
    bool stateRunning() const;     /*!< \brief Returns true if the task is in state TES_RUNNING.*/
    bool statePaused() const;      /*!< \brief Returns true if the task is in state TES_PAUSED.*/
    bool stateStopped() const;     /*!< \brief Returns true if the task is in state TES_STOPPED.*/
    bool stateFinished() const;    /*!< \brief Returns true if the task is in state TES_FINISHED.*/
    bool stateTerminated() const;  /*!< \brief Returns true if the task is in state TES_TERMINATED.*/

protected: //slots
    //protected slot functions (called indirectly via SimulationTaskMessenger)

    /*!
     \brief Informs the Task, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadInitialized().
     Originates from Simulation or the derived classes.
    */
    void updateInitialized();
    /*!
     \brief Informs the Task internally, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadRunning().
     Originates from the derived classes.
    */
    void updateRunning();
    /*!
     \brief Informs the Task internally, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadPaused() if the pause state is true, or threadRunning() otherwise.
     Originates from the derived classes.
    */
    void updatePaused(bool state);
    /*!
     \brief Informs the Task internally, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadPaused().
     Originates from the derived classes.
    */
    void updatePaused();
    /*!
     \brief Informs the Task internally, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadStopped().
     Originates from the derived classes.
    */
    void updateStopped();
    /*!
     \brief Informs the ThreadState, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadFinished().
     Originates from the derived classes.
    */
    void updateFinished();
    /*!
     \brief Informs the ThreadState, that the execution state changed.

     Emits the SimulationTaskMessengers respective signal threadTerminated().
     Originates from the derived classes.
    */
    void updateTerminated();
    /*!
     \brief Informs the ThreadState, that the thread definition has changed.

     Emits the SimulationTaskMessengers respective signal threadDefinitionChanged().
     Originates from the derived classes.
    */
    void updateDefinitionChanged();

public:
   // virtual public functions

   /*!
    \brief Run the task by initializing the base thread or process and executing it.

    A SimulationThreads run() function is called in a new thread by executing start().
    A SimulationProcess' run() function is used to start a new process.
   */
   virtual void runTask();
   /*!
    \brief Stops the task.

    Stops threads. Terminates processes.
   */
   virtual void stop() = 0;
   /*!
    \brief Pauses the task.

    Tries to pause threads. Tries to pause processes. They are not guaranteed to be pausable.

    \param pause wished state
   */
   virtual void pause(bool pause = true) = 0;
   /*!
    \brief Restarts the task.

    Restarts thread content. Terminates and restarts processes.
   */
   virtual void restart() = 0;

protected:
    //virtual protected functions

   /*!
    \brief Runs the task.

    Threads run() function. Starts processes.
   */
   virtual void run() = 0;

protected:
    // protected functions

    /*!
     \brief Cosntructor for derived classes.

     \param definition task definition to store
    */
    SimulationTask(boost::shared_ptr<TaskDefinition> definition);

    /*!
     \brief Changes the task definition.

     Used for reinitialization of processes or threads after the taskdefinition was changed.
     The data of the old definition will be changed to the reference definitions data.
     Connections of both definition remain as they are.

     \param definition new definition
    */
    bool setTaskDefinition(const TaskDefinition& definition);

protected:
    //protected members

    QSemaphore mExecutionSemaphore; /*!< Semaphore to control execution externally. Release after initialization, as soon as it is safe for other tasks to start. */

private:
    // private members

    SimulationTaskMessenger* mMessenger;                /*!< Messenger who receives and sends signals for the task. */
    boost::shared_ptr<TaskDefinition> mTaskDefinition;  /*!< Definition of the task. */
    ETaskExecutionState mExecutionState;                /*!< Current execution state of the task. */
    int mId;                                            /*!< Id of the task, specified by the owning Simulation. */
};


/*!
 \brief Messenger class for SimulationTask.

 Every SimulationTask possesses one SimulationTaskMessenger.
 Since all derived classes from SimulationTask are also derived from QObject, SimualtionTask cant be derived from QObject too. Therefore it uses
 a messenger object to emit signals and receive them.
*/
class SimulationTaskMessenger : public QObject
{
    Q_OBJECT

    friend SimulationTask;

public:
    //public functions

    /*!
     \brief Constructor.

     \param task pointer to parent
    */
    SimulationTaskMessenger(SimulationTask* task);
    /*!
     \brief Destructor.
    */
    virtual ~SimulationTaskMessenger();

public: signals:

    /*!
     \brief Emitted when the thread has been initialized.

     \param id id of the thread
    */
    void initialized(int id);
    /*!
     \brief Emitted when the thread has been set to running.

     \param id id of the thread
    */
    void running(int id);
    /*!
     \brief Emitted when the thread has been paused.

     \param id id of the thread
    */
    void paused(int id);
    /*!
     \brief Emitted when the thread has finished running.

     \param id id of the thread
    */
    void finished(int id);
    /*!
     \brief Emitted when the thread has stopped and is waiting to be run again.

     \param id id of the thread
    */
    void stopped(int id);
    /*!
     \brief Emitted when the thread has been terminated.

     \param id id of the thread
    */
    void terminated(int id);
    /*!
     \brief Emitted when the thread state has changed.

     \param id id of the thread
     \param state state of the thread
    */
    void executionStateChanged(int id, SimulationTask::ETaskExecutionState state);

    /*!
     \brief Emitted when the thread definition has changed.

     \param id id of the thread
    */
    void taskDefinitionChanged(int id);

public slots:

    void updateInitialized();       /*!< \brief Calls SimulationTask::updateInitialized(). */
    void updateRunning();           /*!< \brief Calls SimulationTask::updateRunning(). */
    void updatePaused(bool state);  /*!< \brief Calls SimulationTask::updatePaused(). */
    void updatePaused();            /*!< \brief Calls SimulationTask::updatePaused(). */
    void updateStopped();           /*!< \brief Calls SimulationTask::updateStopped(). */
    void updateFinished();          /*!< \brief Calls SimulationTask::updateFinished(). */
    void updateTerminated();        /*!< \brief Calls SimulationTask::updateTerminated(). */
    void updateDefinitionChanged(); /*!< \brief Calls SimulationTask::updateDefinitionChanged(). */

private:
    //private members

    SimulationTask* mTask; /*!< Pointer to parent. */

};

#endif //SIMULATIONTASK_H
