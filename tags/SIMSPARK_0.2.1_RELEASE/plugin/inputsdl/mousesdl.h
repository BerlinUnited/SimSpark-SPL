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

   MouseSDL

   HISTORY: 22.08.02 - MK - Initial version

*/
#ifndef MOUSESDL_H__
#define MOUSESDL_H__

#include "inputdevicesdl.h"

class MouseSDL : public InputDeviceSDL
{
    //
    // functions
    //
public:
    MouseSDL();
    virtual ~MouseSDL();

    //! init the device
    virtual bool Init(kerosin::InputSystem* inputSystem);

    //! this filters an SDL specific input
    virtual int EventFilter(const SDL_Event* event);
};

DECLARE_CLASS(MouseSDL);

#endif //MOUSESDL_H__
