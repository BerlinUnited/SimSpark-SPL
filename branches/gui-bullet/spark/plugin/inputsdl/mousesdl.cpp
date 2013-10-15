/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: mousesdl.cpp 207 2010-06-01 13:04:58Z sgvandijk $

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
#include "mousesdl.h"
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

MouseSDL::MouseSDL()
    : InputDeviceSDL()
{
}

MouseSDL::~MouseSDL()
{
}

bool
MouseSDL::Init(kerosin::InputSystem* inputSystem)
{
    return InputDevice::Init(inputSystem);
}

int
MouseSDL::EventFilter(const SDL_Event* event)
{
    // we only want mouse events
    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        Input input(Input::eButton);

        switch (event->button.button)
        {
        case SDL_BUTTON_LEFT:
            input.mCode = Input::IC_MOUSE_LEFT;
            break;

        case SDL_BUTTON_RIGHT:
            input.mCode = Input::IC_MOUSE_RIGHT;
            break;

        case SDL_BUTTON_MIDDLE:
            input.mCode = Input::IC_MOUSE_MIDDLE;
            break;

        default:
            return 1;
        }

        input.mData.l = (event->type == SDL_MOUSEBUTTONDOWN);
        input.mModState = 0;
        mInputSystem->AddInputInternal(input);
        return 0;
    }
    break;

    case SDL_MOUSEMOTION:
    {
        Input input(Input::eAxis,Input::IC_AXISX);
        input.mData.l = event->motion.xrel;
        mInputSystem->AddInputInternal(input);

        input.mCode = Input::IC_AXISY;
        input.mData.l = event->motion.yrel;
        mInputSystem->AddInputInternal(input);
        return 0;
    }
    break;
    }

    return 1;
}
