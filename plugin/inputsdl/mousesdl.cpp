/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: mousesdl.cpp,v 1.4 2004/03/09 12:47:37 rollmark Exp $

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

MouseSDL::MouseSDL() : InputDeviceSDL()
{
}

MouseSDL::~MouseSDL()
{
}

bool MouseSDL::Init(kerosin::InputSystem *inputSystem)
{
    return InputDevice::Init(inputSystem);
}

int MouseSDL::EventFilter(const SDL_Event *event)
{
    // we only want mouse events
    switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            {
                InputServer::Input input(InputServer::eButton);

                switch (event->button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        input.code =
                            mInputSystem->GetInputServer()->IC_MOUSE_LEFT;
                        break;

                    case SDL_BUTTON_RIGHT:
                        input.code =
                            mInputSystem->GetInputServer()->IC_MOUSE_RIGHT;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        input.code =
                            mInputSystem->GetInputServer()->IC_MOUSE_MIDDLE;
                        break;

                    default:
                        return 1;
                    }

                input.data.l = (event->type == SDL_MOUSEBUTTONDOWN);
                mInputSystem->AddInputInternal(input);
                return 0;
            }
            break;

        case SDL_MOUSEMOTION:
            {
                InputServer::Input input
                    (
                     InputServer::eAxis,
                     mInputSystem->GetInputServer()->IC_AXISX
                     );
                input.data.l = event->motion.xrel;
                mInputSystem->AddInputInternal(input);

                input.code = mInputSystem->GetInputServer()->IC_AXISY;
                input.data.l = event->motion.yrel;
                mInputSystem->AddInputInternal(input);
                return 0;
            }
            break;
        }

    return 1;
}
