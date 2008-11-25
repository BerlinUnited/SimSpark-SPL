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

   InputSystemSDL

   HISTORY: 21.08.02 - MK - Initial version
*/
#ifndef INPUTSYSTEMSDL_H__
#define INPUTSYSTEMSDL_H__

#include <deque>
#include <kerosin/inputserver/inputsystem.h>
#include <kerosin/inputserver/inputserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <SDL/SDL.h>

struct SDL_mutex;

class TimerSDL;

class InputSystemSDL : public kerosin::InputSystem
{
    //
    // functions
    //
public:
    InputSystemSDL();
    virtual ~InputSystemSDL();

    virtual bool Init(kerosin::InputServer* inputServer);
    virtual bool CreateDevice(const std::string& deviceName);

    SDL_mutex* GetMutex() const;

    int EventFilter(const SDL_Event* event);

    //! add the input to the queue
    virtual void AddInput(const kerosin::Input& input);
    //! retrieve an input from the queue
    virtual bool GetInput(kerosin::Input& input);

protected:
    virtual bool UpdateTimerInput(kerosin::Input& input);

    //
    // members
    //
private:
    SDL_mutex* mMutex;

    boost::shared_ptr<TimerSDL> mTimer;
};

DECLARE_CLASS(InputSystemSDL);

#endif //INPUTSYSTEMSDL_H__
