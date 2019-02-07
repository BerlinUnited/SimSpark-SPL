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
#ifndef KEROSIN_INPUTDEVICE_H
#define KEROSIN_INPUTDEVICE_H

/*      \class InputDevice

        InputDevice

        An InputDevice is the abstract base class which creates input
        for the inputserver. Common InputDevice would be the Keyboard
        and Mouse, but less common 'devices' such as AI agents (think
        of them pushing the controls) or time are also possible.

        The input devices are managed by the input server. As the
        application only communicates with the input server in general
        this makes adding new input devices transparent to the
        application (Good Thing(TM)).

        HISTORY: 21.08.02 - MK - Initial version
*/

#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>
#include "inputsystem.h"

namespace kerosin
{

class KEROSIN_API InputDevice : public zeitgeist::Leaf
{
    //
    // functions
    //
public:
    InputDevice();
    ~InputDevice();

    //! init the device
    virtual bool Init(kerosin::InputSystem* inputSystem);

    //
    // members
    //
protected:
    InputSystem* mInputSystem;
};

DECLARE_CLASS(InputDevice);

} //namespace kerosin

#endif //KEROSIN_INPUTDEVICE_H
