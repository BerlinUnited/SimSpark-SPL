/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: keyboardsdl.cpp,v 1.3 2003/12/21 23:36:38 fruit Exp $

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
#include "keyboardsdl.h"
#include <kerosin/inputserver/inputserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

KeyboardSDL::KeyboardSDL() :
InputDeviceSDL()
{
}

KeyboardSDL::~KeyboardSDL()
{
}

bool KeyboardSDL::Init(kerosin::InputSystem *inputSystem)
{
        if (InputDevice::Init(inputSystem) == false) return false;

        return true;
}

int KeyboardSDL::EventFilter(const SDL_Event *event)
{
        // we only want keyboard events
        if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
        {
                mInputSystem->GetInputServer()->mModifierState = 0;
                if (event->key.keysym.mod & KMOD_LSHIFT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eLShift;
                if (event->key.keysym.mod & KMOD_RSHIFT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eRShift;
                if (event->key.keysym.mod & KMOD_LALT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eLAlt;
                if (event->key.keysym.mod & KMOD_RALT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eRAlt;
                if (event->key.keysym.sym != 0)
                {
                        InputServer::Input input(InputServer::eButton, event->key.keysym.scancode);
                        input.data.l = (event->type == SDL_KEYDOWN);
                        mInputSystem->AddInputInternal(input);
                        return 0;
                }
        }
        return 1;
}
