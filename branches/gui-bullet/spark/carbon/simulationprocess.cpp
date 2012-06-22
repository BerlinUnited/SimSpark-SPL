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

#include "simulationprocess.h"
#include "carbon.h"
#include "simulationmanager.h"
#include "cutelogger\logstream.h"

#include <QFileInfo>
#include <QDir>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SimulationProcess::SimulationProcess(boost::shared_ptr<TaskDefinition> definition) :
    SimulationTask(definition)
{
    mLogSourceId = -1;
    mPath = "";

    //Update signals for the base task
    SimulationTaskMessenger* messenger = getMessenger(); //messenger notifies SimulationTask

    //finished(id) and terminated(id) are emitted by invoking updateFinished() and updateTerminated() from the QProcess' finished(int, QProcess::ExitStatus) signal
    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(updateFinished(int, QProcess::ExitStatus))); 
    connect(this, SIGNAL(started()), messenger, SLOT(updateRunning())); //running(id) is emitted by invoking updateRunning() from the QProcess' started() signal. Sent after state() returned "Running"
    //paused is never emitted

    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(logError(QProcess::ProcessError)));
}

SimulationProcess::~SimulationProcess()
{

}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool SimulationProcess::startProcess(const QString& program, const QStringList& arguments)
{
    QString path = program;
    if (!QFileInfo(path).exists())
    {
        //Search executable in include paths
        path = Carbon::get()->getSimulationManager()->findExecutable(program);

        if (path.compare("") == 0)
        {
            LOG_ERROR() << "Program " << program << " could not be found. If using a relative path, make sure to add it to the executable include paths.";
            return false;
        }
    }

    mPath = path;
    mError = false;
    QDir workingDir(path);
    workingDir.cdUp();
    QString workingDirPath = workingDir.absolutePath();
    
    //Start the process
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    //env.insert("PATH", env.value("Path") + path);
    setProcessEnvironment(env);
    setWorkingDirectory(workingDirPath);
    start(path, arguments); //Errors are sent via signal

    if (mError && error() == QProcess::FailedToStart) 
        return false;

    return true;
}

bool SimulationProcess::startProcess(const QString& program, const std::vector<QString>& arguments)
{
    QStringList list;
    for (auto it = arguments.begin(); it != arguments.end(); it++)
    {
        list.append(*it);
    }

    return startProcess(program, list);
}

bool SimulationProcess::stopProcess()
{
    if (state() == QProcess::NotRunning)
        return true;

    //close
    close();

    //wait for termination
    int waited = 0;
    while (waited < 30)
    {
        waitForFinished(1000);
        if (state() == QProcess::Running)
        {
            LOG_DEBUG() << "Waiting for process to terminate ... " << waited;
            if (waited == 10)
            {
                LOG_ERROR() << "Process did not shutdown in time. Killing.";
                kill();
                waitForFinished(2000);
                break;
            }
            waited++;
        }
        else
            break;
    }

    if (state() == QProcess::NotRunning)
        return true;

    return false;
}

bool SimulationProcess::pauseProcess(bool state)
{
    LOG_INFO() << "Cant pause external process.";
    
    return false;
}

bool SimulationProcess::restartProcess()
{
    bool stopped = stopProcess();

    if (stopped)
    {
        LOG_DEBUG() << "Restarting process...";
        return startProcess(mPath, mArguments);
    }
    else
    {
        LOG_ERROR() << "Could not restart process.";
    }

    return false;
}

bool SimulationProcess::initProcessLogging(const QString& sourceName)
{
    mLogSourceId = Logger::getLogSource(sourceName.toStdString());

    connect(this, SIGNAL(readyReadStandardError()), SLOT(fetchErrorMessage()));
    connect(this, SIGNAL(readyReadStandardOutput()), SLOT(fetchOutputMessage()));

    return true;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationProcess::fetchErrorMessage()
{
    //Log info messages from std::err
    setReadChannel(QProcess::StandardError);

    while (canReadLine())
    {
        QString message(readLine());
        std::string msg = message.toStdString();
        
        while (msg.size() != 0 && msg.back() == ('\n'))
            msg = msg.substr(0, msg.size()-2);
    
        if (msg.size() == 0)
            continue;
    
        if (mLogSourceId != -1)
            LOG_S_ERROR(mLogSourceId) << msg.c_str();
        else
            LOG_ERROR() << msg.c_str();
    }
}

void SimulationProcess::fetchOutputMessage()
{
    //Log info messages from std::out
    setReadChannel(QProcess::StandardOutput);

    while (canReadLine())
    {
        QString message(readLine());
        std::string msg = message.toStdString();

        while (msg.size() != 0 && msg.back() == ('\n'))
            msg = msg.substr(0, msg.size()-2);
    
        if (msg.size() == 0)
            continue;

        if (mLogSourceId != -1)
            LOG_S_INFO(mLogSourceId) << msg.c_str();
        else
            LOG_INFO() <<msg.c_str();
    }
}

void SimulationProcess::updateFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        LOG_DEBUG() << "Process " << mPath << " finished: Exit code: " << exitCode;
        SimulationTask::updateFinished();
    }
    else if (exitStatus == QProcess::CrashExit)
    {
        LOG_INFO() << "Process " << mPath << " crashed. Exit code: " << exitCode;
        SimulationTask::updateTerminated();
    }
}

void SimulationProcess::logError(QProcess::ProcessError)
{
    ProcessError err = error();
    LOG_ERROR() << "Process error: " << errorString();
    mError = true;
}