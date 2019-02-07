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

#include "sparkprocess.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SparkProcess::SparkProcess(boost::shared_ptr<TaskDefinition> definition) :
    SimulationProcess(definition)
{
    //Check definition type
    if (definition->getType() != TaskDefinition::TT_SPARKPROCESS)
        throw SimulationTask::IllegalTaskTypeException("SparkProcess initialization error.", definition->getType(), TaskDefinition::TT_SPARKPROCESS);
}

SparkProcess::~SparkProcess()
{

}

//--------------------------------------------------------------
// Public Virtual
//--------------------------------------------------------------

void SparkProcess::run()
{
    //Get log id
    bool success = initProcessLogging(getTaskDefinition().getFirst());
    if (!success)
    {
        LOG_ERROR() << "Error while initializing process log for process " << getTaskDefinition().getFirst() << ".";
        return;
    }
    
    //Release access semaphore
    mExecutionSemaphore.release();

    //Call process execution
    success = startProcess(getTaskDefinition().getFirst(), getTaskDefinition().getSecondList());
    if (!success)
    {
        LOG_ERROR() << "Error while starting process " << getTaskDefinition().getFirst() << ".";
    }
}

void SparkProcess::stop()
{
    stopProcess();
}

void SparkProcess::pause(bool pause)
{
    pauseProcess(pause);
}

void SparkProcess::restart()
{
    restartProcess();
}


//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

