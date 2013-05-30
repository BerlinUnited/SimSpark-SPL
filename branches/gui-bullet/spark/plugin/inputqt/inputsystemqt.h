/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Author: Patrick Geib
   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef INPUTSYSTEMQT_H__
#define INPUTSYSTEMQT_H__

#include <deque>
#include <kerosin/inputserver/inputsystem.h>
#include <kerosin/inputserver/inputserver.h>

class InputSystemQt : public kerosin::InputSystem
{
    //
    // functions
    //
public:
    InputSystemQt();
    virtual ~InputSystemQt();

    virtual bool Init(kerosin::InputServer* inputServer);
    virtual bool CreateDevice(const std::string& deviceName);

protected:

    virtual bool UpdateTimerInput(kerosin::Input& input);

    //
    // members
    //
protected:
};

DECLARE_CLASS(InputSystemQt);

#endif //INPUTSYSTEMQT_H__
