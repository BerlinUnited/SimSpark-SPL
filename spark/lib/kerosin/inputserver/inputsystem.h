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
#ifndef KEROSIN_INPUTSYSTEM_H
#define KEROSIN_INPUTSYSTEM_H

/*      \class InputSystem

        An input system is the basic abstraction for an input API. You would
        want to have a specific input system for SDL, DirectX, Windows API, X,
        etc..

        The job of InputSystem is:
                - Initialize the input API
                - Initialize the inputcodes used in the InputServer
                - Handle the creation of individual devices

        HISTORY: 21.08.02 - MK - Initial version
*/

#include "inputserver.h"

#include <deque>
#include <zeitgeist/node.h>
#include <kerosin/kerosin_defines.h>
#ifndef Q_MOC_RUN
#include <boost/thread/shared_mutex.hpp>
#endif

namespace kerosin
{

class KEROSIN_API InputSystem : public zeitgeist::Node
{
    //
    // functions
    //
public:
    InputSystem();
    virtual ~InputSystem();

    //! init the subsystem
    virtual bool Init(kerosin::InputServer* inputServer);

    /** creates an instance of a device via zeitgeist object
        creation. Should use name mangling.
    */
    virtual bool CreateDevice(const std::string& deviceName) = 0;

    /** Add the input to the queue. Uses AddInputInternal. This was
        necessary to allow derived classes to wrap this call in a
        mutex and use the definitely unwrapped addition via
        AddInputInternal().
    */
    virtual void AddInput(const Input& input);

    /** this is the actual addition of input to the queue. It should
        only be used by InputDevices!
     */
    void AddInputInternal(const Input& input);

    //! retrieve an input from the queue
    virtual bool GetInput(Input& input);

    InputServer* GetInputServer() { return mInputServer; }

protected:
    virtual bool UpdateTimerInput(Input &input);

    //
    // members
    //
protected:
    InputServer* mInputServer;

private:
    std::deque<Input>  mInputQueue;

    boost::shared_mutex mMutex;
};

DECLARE_ABSTRACTCLASS(InputSystem);

} // namespace kerosin

#endif //KEROSIN_INPUTSYSTEM_H
