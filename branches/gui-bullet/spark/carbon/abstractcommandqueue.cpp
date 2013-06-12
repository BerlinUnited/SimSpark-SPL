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

#include "abstractcommandqueue.h"
#include "abstractcommand.h"
#include "cutelogger/logger.h"

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

AbstractCommandQueue::AbstractCommandQueue()
    : mSemaphore(1)
{
}

AbstractCommandQueue::~AbstractCommandQueue()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

bool AbstractCommandQueue::queueCommand(boost::shared_ptr<AbstractCommand> command, int timeout, bool log_error)
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
            LOG_ERROR() << "Could not queue command " << command->toString() << ". Queue access is locked.";
        return false;
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void AbstractCommandQueue::executeCommands()
{
    for (auto it = mCommands.begin(); it != mCommands.end();)
    {
        boost::shared_ptr<AbstractCommand> command = *it;
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

bool AbstractCommandQueue::lockQueue(int timeout)
{
    return mSemaphore.tryAcquire(1, timeout);
}

void AbstractCommandQueue::unlockQueue()
{
    mSemaphore.release();
}
