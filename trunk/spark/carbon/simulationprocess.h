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

#ifndef SIMULATIONPROCESS_H
#define SIMULATIONPROCESS_H
/**
 * \file simulationprocess.h
 * \brief Contains SimulationProcess class, a SimulationTask that contains tools to communicate with external processes.
 */

#include "simulationtask.h"

#include <QProcess>
#include <vector>

/*!
 \brief SimulationProcess is a class to start an external process from a TaskDefinition and communicate with it or observe it using QProcess signals.

 SimulationProcesses are part of a Simulation in the SimulationManager and are defined in a SimulationSetup using a TaskDefinition.
 The SimulationProcess is initialized with the information given in the TaskDefinition.
 When SimulationTask executes runTask() the derived classes run() method is called, which starts a process with startProcess.

 The SimulationProcess can observe the created Process' execution state and fetch its standard output and standard error messages and log them to
 the Logger. It receives changes in the process' execution state and uses them to update the ETaskExecutionState.\n

 \note
 Process message logging has an huge issue. It is not possible to receive std::out messages from a started process in real time if the process does
 not regularly flush its std::out stream. std::err is always flushed when a line is logged, and thus automatically sent to QProcess. But the std::out waits
 for a flush command (usually meaning its waiting for a buffer overflow or the process termination). 
 On Unix it would be possible to emulate a console-like behaviour causing the process to stream its data to a mid-buffer that automatically flushes when receiving
 a newline. But on Windows we totally at the process' mercy to regularly flush its buffer. 
 So, to receive up-to-date log messages from a spark process, they have to initialize the logging with the sync-flag. When adding a new log stream to the log server
 the sync-flag tells the log server to automatically sync the stream when a new log message is sent.
 Alternatively, the SimulationProcess could be started detached, opening a new console but preventing connections to std::out and std::err completely.
*/
class SimulationProcess : public QProcess, public SimulationTask
{
    Q_OBJECT

public:
    // public functions
    /*!
     \brief Destructor.
    */
    virtual ~SimulationProcess();

protected:
    // protected functions

    /*!
     \brief Protected constructor.

     \param definition definition for the task
    */
    SimulationProcess(boost::shared_ptr<TaskDefinition> definition);

    /*!
     \brief Start the eternal process.

     \param program path to program to start
     \param arguments list of command line arguments
    */
    bool startProcess(const QString& program, const QStringList& arguments);
    /*!
     \brief Start the eternal process.

     \param program path to program to start
     \param arguments list of command line arguments
    */
    bool startProcess(const QString& program, const std::vector<QString>& arguments);
 
    /*!
     \brief Terminates the process.

     On windows a return value of 0 after termination still flags the Process as "finished" instead of "crashed".
    */
    bool stopProcess();
    /*!
     \brief Pausing a process (not implemented yet).

     \todo Pausing a process relies on communication with it. This is not possible by default.
     Derived functions or a communication interface could add the possibility to pause a known process.

     \param state true to pause, false to unpause
    */
    bool pauseProcess(bool state);
    /*!
     \brief Stops and restarts the process.

     Reuses the same executable path and the same argument list used for the last call, even if the TaskDefinition changed.
    */
    bool restartProcess();

    /*!
     \brief Initializes process logging with a log source name.

     Standard out messages are sent as Logger::Info messages.
     Standard err messages are sent as Logger::Error messages.
     All messages are sent with the log source id as received by the Logger for the source name sourceName;

     \param sourceName name of the log source
    */
    bool initProcessLogging(const QString& sourceName);

public slots:
    //public slots
    
    /*!
     \brief Read and log standard error messages of the process.

     Called when new error messages can be fetched from the process.
    */
    void fetchErrorMessage();
    /*!
     \brief Read and log standard out messages of the process.

     Called when new messages can be fetched from the process.
    */
    void fetchOutputMessage();

    /*!
     \brief Update the process state when the process finished.

     Depending on the exitStatus the SimulationTasks execution state is set to TES_FINISHED or TES_TERMINATED.

     \param exitCode exitCode of the process
     \param exitStatus exitStatus of the process
    */
    void updateFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected slots:
    //protected slots

    /*!
     \brief Logs information on an process error that occurred.

     \param type type of the error
    */
    void logError(QProcess::ProcessError type);

protected:
    // protected members

    QString mPath; /*!< Path to process. */
    QStringList mArguments; /*!< List of arguments for process call. */

private:
    // private members

    int mLogSourceId; /*!< Log source id as received from the Logger when calling initProcessLogging(). Default is 0 (Gui). */
    bool mError; /*!< True if an error occured during process startup. */
};

#endif //IPROCESSINTERFACE_H
