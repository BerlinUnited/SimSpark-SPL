/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "inputsystem.h"

#ifndef Q_MOC_RUN
#include <boost/thread/locks.hpp>
#endif

using namespace kerosin;
using namespace zeitgeist;

InputSystem::InputSystem()
    : Node(), mInputServer(NULL)
{
}

InputSystem::~InputSystem()
{
}

bool
InputSystem::Init(kerosin::InputServer* inputServer)
{
    mInputServer = inputServer;

    return (mInputServer != 0);
}

void
InputSystem::AddInput(const Input& input)
{
    AddInputInternal(input);
}

bool
InputSystem::GetInput(Input& input)
{
    boost::unique_lock<boost::shared_mutex> lock(mMutex);
    static bool hasDoneTimer = false;
    if (mInputQueue.size() > 0)
    {
        input = mInputQueue.front();
        mInputQueue.pop_front();
        return true;
    }

    if (!hasDoneTimer)
    {
        hasDoneTimer = true;
        return UpdateTimerInput(input);
    }
    else
    {
        hasDoneTimer = false;
        return false;
    }
}

void
InputSystem::AddInputInternal(const Input& input)
{
    boost::unique_lock<boost::shared_mutex> lock(mMutex);
    mInputQueue.push_back(input);
}

bool
InputSystem::UpdateTimerInput(Input &/*input*/)
{
    return false;
}
