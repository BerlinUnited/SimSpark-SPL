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

#include "inputsystemsdl.h"
#include "inputdevicesdl.h"
#include <kerosin/inputserver/inputserver.h>
#include <zeitgeist/logserver/logserver.h>
#ifndef WIN32
#include <SDL/SDL_thread.h>
#include <SDL/SDL_syswm.h>
#else
#include <SDL_thread.h>
#include <SDL_syswm.h>
#endif

#include "timersdl.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

/*!
  This function is used to filter SDL Events and delegate them to the input
  device which registered for the event type.

  NOTE: This function may run in a different thread, so care has to be taken
  when accessing the global object. Things to watch out for:

  - gDeviceList changes (done by individual devices (construction/destruction)) handled
  - Events are pushed into a device (done by EventFilter)
  - Events are taken out of a device (this is done by the inputserver)
*/

InputSystemSDL *gInputSystem;

static int EventFilterCallback(const SDL_Event *event)
{
    if (gInputSystem)
        {
            return gInputSystem->EventFilter(event);
        } else
            {
                return 1;
            }
}

InputSystemSDL::InputSystemSDL() : InputSystem(), mMutex(NULL)
{
    gInputSystem = this;
}

InputSystemSDL::~InputSystemSDL()
{
    SDL_LockMutex(mMutex);
    SDL_SetEventFilter(NULL);
    gInputSystem = NULL;
    SDL_UnlockMutex(mMutex);

    SDL_DestroyMutex(mMutex);
    mMutex = NULL;
}

bool InputSystemSDL::Init(InputServer *inputServer)
{
    if (InputSystem::Init(inputServer) == false) return false;

    // here we check whether SDL has been initialized prior to adding this
    // input is part of the video subsystem (because of event loops, etc..)
    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
        {
            GetLog()->Error() << "ERROR: (InputSystemSDL) SDL not initialized!\n";
            return false;
        }
//        SDL_SetVideoMode(0,0,0,0);
//        static SDL_SysWMinfo pInfo;
//        SDL_VERSION(&pInfo.version);
//        SDL_GetWMInfo(&pInfo);

   // Also, SDL keeps an internal record of the window size
   //  and position. Because SDL does not own the window, it
   //  missed the WM_POSCHANGED message and has no record of
   //  either size or position. It defaults to {0, 0, 0, 0},
   //  which is then used to trap the mouse "inside the
   //  window". We have to fake a window-move to allow SDL
   //  to catch up, after which we can safely grab input.
//         RECT r;
//         GetWindowRect(pInfo.window, &r);
//         SetWindowPos(pInfo.window, 0, r.left, r.top, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//        SDL_WM_GrabInput(SDL_GRAB_ON);
    }

    if (!SDL_WasInit(SDL_INIT_TIMER))
    {
        if (SDL_Init(SDL_INIT_TIMER) < 0)
        {
            GetLog()->Error() << "ERROR: (InputSystemSDL) SDL Timer not initialized!\n";
            return false;
        }
    }

    // we need a mutex object
    mMutex = SDL_CreateMutex();

    // setup the filtering function
    SDL_SetEventFilter(EventFilterCallback);

    return true;
}

bool InputSystemSDL::CreateDevice(const std::string &deviceName)
{
    // first, we mangle the deviceName to avoid nameclashes
    std::string mangledName = deviceName + "SDL";

    boost::shared_ptr<InputDevice> device =
        shared_static_cast<InputDevice>(GetCore()->New(mangledName));

    if (device.get() == NULL)
        {
            GetLog()->Error()
                << "ERROR: (InputSystemSDL) Creating device '"
                << mangledName << "'" << std::endl;
            return false;
        }

    // initialize the device
    if (device->Init(this) == false)
        {
            GetLog()->Error()
                << "ERROR: (InputSystemSDL) Initializing device '"
                << mangledName << "'" << std::endl;
            return false;
        }

    // some special case handling for the timer (FIXME)
    if (mangledName.compare("TimerSDL") == 0)
        {
            mTimer = shared_static_cast<TimerSDL>(device);
        }
    else
        {
            SDL_LockMutex(mMutex);
            // try to link the device into the inputserver
            if (AddChildReference(device) == false)
                {
                    GetLog()->Error()
                        << "ERROR: (InputSystemSDL) Linking device '"
                        << mangledName << "'" << std::endl;
                    SDL_UnlockMutex(mMutex);
                    return false;
                }
            SDL_UnlockMutex(mMutex);
        }

    return true;
}

int InputSystemSDL::EventFilter(const SDL_Event *event)
{
    int ret = 1;
    SDL_LockMutex(mMutex);
    // loop through all children
    for (TLeafList::iterator i = mChildren.begin(); i!=mChildren.end(); ++i)
        {
            boost::shared_ptr<InputDeviceSDL> device = shared_static_cast<InputDeviceSDL>(*i);
            // every device gets a chance to filter the event, the first one
            // who claims it, gets it
            if (device->EventFilter(event) == 0)
                {
                    ret = 0;
                    break;
                }
        }
    SDL_UnlockMutex(mMutex);

    return ret;
}

void InputSystemSDL::AddInput(const Input &input)
{
    SDL_LockMutex(mMutex);
    InputSystem::AddInput(input);
    SDL_UnlockMutex(mMutex);
}

bool InputSystemSDL::GetInput(Input &input)
{
    SDL_LockMutex(mMutex);
    bool ret = InputSystem::GetInput(input);
    SDL_UnlockMutex(mMutex);
    return ret;
}

bool InputSystemSDL::UpdateTimerInput(Input &input)
{
    if (mTimer.get() == NULL)
        {
            return false;
        } else
            {
                mTimer->GetInput(input);
                return true;
            }
}
