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

   InputDeviceSDL

   HISTORY: 21.08.02 - MK - Initial version
*/

#ifndef INPUTDEVICESDL_H__
#define INPUTDEVICESDL_H__

#include <kerosin/inputserver/inputdevice.h>
#include <kerosin/inputserver/inputserver.h>

#ifndef WIN32
#include <SDL/SDL.h>
#else
#include <include/SDL.h>
#endif

/**     \class InputDeviceSDL
        This class introduces SDL-specific callback functions for the event filtering.
*/
class InputDeviceSDL : public kerosin::InputDevice
{
    //
    // functions
    //
public:
    //! this filters an SDL specific input
    virtual int EventFilter(const SDL_Event* event) = 0;
};

DECLARE_ABSTRACTCLASS(InputDeviceSDL);

#endif //INPUTDEVICESDL_H__
