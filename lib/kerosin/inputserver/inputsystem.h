/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: inputsystem.h,v 1.4 2003/11/14 14:05:51 fruit Exp $

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
#ifndef KEROSIN_INPUTSYSTEM_H
#define KEROSIN_INPUTSYSTEM_H

/*      \class InputSystem
        $Id: inputsystem.h,v 1.4 2003/11/14 14:05:51 fruit Exp $

        InputSystem

        An input system is the basic abstraction for an input API. You would
        want to have a specific input system for SDL, DirectX, Windows API, X,
        etc..

        The job of InputSystem is:
                - Initialize the input API
                - Initialize the inputcodes used in the InputServer
                - Handle the creation of individual devices

        NOTE:

        HISTORY:
                21.08.02 - MK
                        - Initial version

        TODO:

        TOFIX:
*/

#include <deque>
#include <zeitgeist/node.h>
#include "inputserver.h"

namespace kerosin
{

class InputSystem : public zeitgeist::Node
{
    //
    // functions
    //
public:
    InputSystem();
    virtual ~InputSystem();

    //! init the subsystem
    virtual bool Init(kerosin::InputServer *inputServer);

    //! creates an instance of a device via zeitgeist object creation. Should use name mangling.
    virtual bool CreateDevice(const std::string &deviceName) = 0;

    /*! Add the input to the queue. Uses AddInputInternal. This was necessary to
      allow derived classes to wrap this call in a mutex and use the definitely
      unwrapped addition via AddInputInternal().
    */
    virtual void AddInput(InputServer::Input &input);
    //! this is the actual addition of input to the queue. It should only be used by InputDevices!
    void AddInputInternal(InputServer::Input &input);
    //! retrieve an input from the queue
    virtual bool GetInput(InputServer::Input &input);

    InputServer*    GetInputServer()        {       return mInputServer;    }
protected:
    virtual bool UpdateTimerInput(InputServer::Input &input);

    //
    // members
    //
protected:
    InputServer                                             *mInputServer;
private:
    std::deque<InputServer::Input>  mInputQueue;
};

} // namespace kerosin

#endif //KEROSIN_INPUTSYSTEM_H
