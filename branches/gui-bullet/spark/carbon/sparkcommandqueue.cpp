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

#include "sparkcommandqueue.h"
#include "sparkcommands.h"
#include "sparkcontroller.h"
#include "guisimcontrol.h"
#include "cutelogger/logger.h"

namespace SparkCommand
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

CommandQueue::CommandQueue(SparkController* parent)
    : mController(parent)
{
}

CommandQueue::~CommandQueue()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------
/*
bool CommandQueue::queueCommand(boost::shared_ptr<Command> command, int timeout, bool log_error)
{
    if (lockQueue(timeout))
    {
        command->setQueue(this);
        mCommands.push_back(command);
        unlockQueue();
        return true;
    }
    else
    {
        if (log_error)
            LOG_ERROR() << "Could not queue SimSpark command " << command->toString() << ". Queue access is locked.";
        return false;
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void CommandQueue::executeCommands()
{
    for (auto it = mCommands.begin(); it != mCommands.end();)
    {
        boost::shared_ptr<Command> command = *it;
        bool success = command->execute(); 
        if (!command->isProlonged())
        {
            command->emitExecuted(success);
            it = mCommands.erase(it);
        }
        else
        {
            command->prolong(false);
            it++;
        }
    }
}

bool CommandQueue::lockQueue(int timeout)
{
    return mSemaphore.tryAcquire(1, timeout);
}

void CommandQueue::unlockQueue()
{
    mSemaphore.release();
}
*/
//--------------------------------------------------------------
// ISimControl interface
//--------------------------------------------------------------

void CommandQueue::endCycle()
{
    if (lockQueue())
    {
        executeCommands();
        unlockQueue();
    }
}

void CommandQueue::waitCycle()
{
    if (lockQueue())
    {
        executeCommands();
        unlockQueue();
    }
}

} //namespace SparkCommands
