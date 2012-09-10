/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "timersdl.h"
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

TimerSDL::TimerSDL()
    : InputDeviceSDL(), mLastTicks(0)
{
}

TimerSDL::~TimerSDL()
{
}

bool
TimerSDL::Init(kerosin::InputSystem *inputSystem)
{
    if (InputDevice::Init(inputSystem) == false)
    {
        return false;
    }

    mLastTicks = SDL_GetTicks();

    return true;
}

int
TimerSDL::EventFilter(const SDL_Event* event)
{
    return 1;
}

void
TimerSDL::GetInput(Input& input)
{
    input.mType = Input::eAxis;
    input.mCode = Input::IC_AXIST;

    unsigned int ticks = SDL_GetTicks();

    // FIXME: ticks might wrap
    input.mData.l = ticks - mLastTicks;
    mLastTicks = ticks;
}
