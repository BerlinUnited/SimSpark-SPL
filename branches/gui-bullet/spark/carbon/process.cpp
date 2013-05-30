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

#include "process.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

Process::Process(boost::shared_ptr<TaskDefinition> definition) :
    SimulationProcess(definition)
{
    //Check definition type
    if (definition->getType() != TaskDefinition::TT_PROCESS)
        throw SimulationTask::IllegalTaskTypeException("Process initialization error.", definition->getType(), TaskDefinition::TT_PROCESS);
}

Process::~Process()
{
    stopProcess();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public Virtual
//--------------------------------------------------------------

void Process::run()
{
    //Get log id
    bool success = initProcessLogging(getTaskDefinition().getFirst());
    if (!success)
    {
        LOG_ERROR() << "Error while initializing process log for process " << getTaskDefinition().getFirst() << ".";
        return;
    }
    
    //Call process execution
    success = startProcess(getTaskDefinition().getFirst(), getTaskDefinition().getSecondList());
    if (!success)
    {
        LOG_ERROR() << "Error while starting process " << getTaskDefinition().getFirst() << ".";
    }
}

void Process::stop()
{
    stopProcess();
}

void Process::pause(bool pause)
{
    pauseProcess(pause);
}

void Process::restart()
{
    bool changed = false;
    if (mPath.compare(getTaskDefinition().getFirst()) != 0)
        changed = true;

    if (getTaskDefinition().getSecondList().size() != mArguments.size())
        changed = true;

    if (!changed)
    {
        int index = 0;
        for (QStringList::Iterator it = mArguments.begin(); it != mArguments.end(); it++)
        {
            if (it->compare(getTaskDefinition().getSecondList().at(index)) != 0)
            {
                changed = true;
                break;
            }
            index++;
        }
    }

    if (changed)
    {
        LOG_WARNING() << "Task definition for the process " << getTaskDefinition().getName() << " changed since the last execution. Restarting with the previous "
                      << "path and arguments. To use the new Values, stop and run the Simulation.";
    }

    restartProcess();
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
