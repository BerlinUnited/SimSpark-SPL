/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: timersdl.h 3 2008-11-21 02:38:08Z hedayat $

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

   TimerSDL

   HISTORY: 24.10.02 - MK - Initial version

*/
#ifndef TIMERSDL_H__
#define TIMERSDL_H__

#include "inputdevicesdl.h"

/**     \class TimerSDL returns the elapsed time since the last call
        in milliseconds.
*/
class TimerSDL : public InputDeviceSDL
{
    //
    // functions
    //
public:
    TimerSDL();
    virtual ~TimerSDL();

    //! init the device
    virtual bool Init(kerosin::InputSystem* inputSystem);

    //! this filters an SDL specific input
    virtual int EventFilter(const SDL_Event* event);

    //! return an Input
    void GetInput(kerosin::Input &input);

protected:
    unsigned long mLastTicks;
};

DECLARE_CLASS(TimerSDL);

#endif //MOUSESDL_H__
