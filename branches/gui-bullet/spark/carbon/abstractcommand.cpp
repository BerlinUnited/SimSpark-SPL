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

#include "abstractcommand.h"
#include "abstractcommandqueue.h"
#include "cutelogger/logger.h"

using namespace boost;

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------
AbstractCommand::~AbstractCommand()
{
}

int AbstractCommand::getId() 
{
    return mId;
}

void AbstractCommand::setId(int id) 
{
    mId = id;
}

bool AbstractCommand::isProlonged() const
{
    return mProlonged;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

AbstractCommand::AbstractCommand(int type) :
     mType(type), mId(0), mParentQueue(0) 
{
}

void AbstractCommand::emitExecuted(bool success)
{
    emit executed(mId, success);
}


void AbstractCommand::setQueue(AbstractCommandQueue* queue) 
{
    mParentQueue = queue;
}

void AbstractCommand::prolong(bool state)
{
    mProlonged = state;
}
