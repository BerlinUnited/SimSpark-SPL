/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of rcssserver3D
   Thu Mar 24 2011
   Copyright (C) 2003-1011 RoboCup Soccer Server 3D Maintenance Group
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
#include "timersystemsdl.h"
#include <zeitgeist/logserver/logserver.h>
#include <SDL.h>

#ifdef _MSC_VER
#ifndef Q_MOC_RUN
#include <boost/math/special_functions/round.hpp>
#endif
using boost::math::round;
#endif

using namespace oxygen;

void TimerSystemSDL::Initialize()
{
    if (!SDL_WasInit(SDL_INIT_TIMER))
    {
        if (SDL_Init(SDL_INIT_TIMER) < 0)
        {
            GetLog()->Error() << "ERROR: (TimerSystemSDL) SDL Timer not initialized!\n";
        }
    }

    mLastTicks = SDL_GetTicks();
}

float TimerSystemSDL::GetTimeSinceLastQuery()
{
    unsigned int ticks = SDL_GetTicks();

    // FIXME: ticks might wrap
    unsigned int timeDiff = ticks - mLastTicks;
    mLastTicks = ticks;

    return timeDiff / 1000.0f;
}

void TimerSystemSDL::WaitFromLastQueryUntil(float deadline)
{
    int milliseconds = round(deadline * 1000);
    int expectedTime = mLastTicks + milliseconds;

    int delayTime = expectedTime - SDL_GetTicks();
    while (delayTime > 0)
    {
        SDL_Delay(delayTime);
        // sometimes, even with the above SDL_Delay() call we will not reach
        // the desired delay; so we check if we've actually reched the desired
        // time
        delayTime = expectedTime - SDL_GetTicks();
    }

//    GetLog()->Debug() << "(TimerSystemSDL) Waiting for " << deadline
//            << " seconds or " << milliseconds << " millisecs\n";
//    GetLog()->Debug() << "CURRENT TIME: " << SDL_GetTicks() << '\n';
}

//void TimerSystemSDL::Finalize()
//{
//}
