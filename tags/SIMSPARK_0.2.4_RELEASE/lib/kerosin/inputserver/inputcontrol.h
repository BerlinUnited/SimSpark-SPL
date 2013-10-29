/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef KEROSIN_INPUTCONTROL_H
#define KEROSIN_INPUTCONTROL_H

#include <oxygen/simulationserver/simcontrolnode.h>
#include <oxygen/sceneserver/fpscontroller.h>
#include <kerosin/kerosin_defines.h>
#include <kerosin/inputserver/inputserver.h>

namespace oxygen
{
class FPSController;
}

namespace kerosin
{
/** \class InputControl is a SimControlNode that processes any pending
    input events from the InputServer at the begin of each simulation
    cycle. It defines and handles a set of default events and can be
    customized by registering InputItems to it.

    If set, InputControl disables the automatic time stepping of the
    SimulationServer and steps the simulation using the real time
    passed.
 */
class KEROSIN_API InputControl : public oxygen::SimControlNode
{
public:
    // define common input constants
    enum ECmds
        {
            CmdTimer    = 1,
            CmdMouseX   = 2,
            CmdMouseY   = 3,
            CmdUp       = 4,
            CmdDown     = 5,
            CmdLeft     = 6,
            CmdRight    = 7,
            CmdForward  = 8,
            CmdBackward = 9,
            CmdMouseLook = 10,
            CmdQuit     = 11,

            // first user defined input constant
            CmdUser = 12
        };

public:
    InputControl();
    virtual ~InputControl();

    /** sets the location of the FPS Controller to be used */
    bool SetFPSController(const std::string& path);

    /** returns the cached location of the FPS controller */
    boost::shared_ptr<oxygen::FPSController> GetFPSController();

    /** sets the horizontal mouse sensitivity */
    void SetHorizontalSensitivity(float s);

    /** returns the horizontal mouse sensitivity */
    float GetHorizontalSensitivity();

    /** sets the horizontal mouse sensitivity */
    void SetVerticalSensitivity(float s);

    /** returns the vertical mouse sensitivity */
    float GetVerticalSensitivity();

    /** returns the size of the last time step */
    float GetDeltaTime();

    /** if set the deltatime read from the InputServer is
        automatically passed to the SimulationServer. The automatic
        time stepping of the SimulationServer is disabled and this
        ControlNode takes the responsibility to advance the time,
        default true. This useful if the simulation speed should be
        locked to the real time passed.
    */
    void SetAdvanceTime(bool advance);

    /** returns the advance time mode */
    bool GetAdvanceTime();

    /** called once when the simulation is started */
    virtual void InitSimulation();

    /** processes pending input events at the start of each simulation
        cycle */
    virtual void StartCycle();

    /** This function creates an instance of class 'inputItem' and
        adds it as a child node below this control node
     */
    bool RegisterInputItem(const std::string& inputItem, const std::string& name);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

protected:
    /** the controlled FPSController */
    CachedPath<oxygen::FPSController> mFPSController;

    /** cached reference to the InputServer */
    CachedPath<kerosin::InputServer> mInputServer;

    /** the size of the last time step */
    float mDeltaTime;

    /** horizontal mouse sensitivity */
    float mHorSens;

    /** vertical mouse sensitivity */
    float mVertSens;

    /** true, if the deltatime read from the InputServer is
        automatically passed to the SimulationServer.
    */
    bool mAdvanceTime;

    /** true, if mouse look is currently activated */
    bool mMouseLook;
};

DECLARE_CLASS(InputControl);

} // namespace kerosin

#endif // KEROSIN_INPUTCONTROL_H
