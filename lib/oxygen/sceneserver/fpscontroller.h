/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: fpscontroller.h,v 1.6 2004/03/09 12:14:33 rollmark Exp $

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
#ifndef OXYGEN_FPSCONTROLLER_H
#define OXYGEN_FPSCONTROLLER_H

#include "basenode.h"

namespace oxygen
{
class Body;

/** FPSController is a node, that provides first person shooter (FPS)
 * movement, i.e. moving forward, backward, strafing etc. It's
 * intended usage is as a child node of an ODE body. The node
 * maintains a state for each movement. This state can either be
 * activated or deactivated with some accessor functions. As long as a
 * state is activated the node performs the associated action during
 * each time step.
 */
class FPSController : public BaseNode
{
    //
    // Functions
    //
public:
    FPSController();
    ~FPSController();

    /** adds a delta increment to the current horizontal angle */
    void AdjustHAngle(const float delta);

    /** adds a delta increment to the current vertical angle */
    void AdjustVAngle(const float delta);

    /** enables or disables forward movement */
    void Forward(const bool state);

    /** enables or disables backwad movement */
    void Backward(const bool state);

    /** enables or disables left strafing */
    void StrafeLeft(const bool state);

    /** enables or disables right strafing */
    void StrafeRight(const bool state);

    /** enables or disables up movement */
    void Up(const bool state);

    /** enables or disables down movement */
    void Down(const bool state);

protected:
    /** calculates and applies the force needed to perfom the
     * activated movements */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** set up the parent body reference */
    virtual void OnLink();

    /** reset the parent body reference */
    virtual void OnUnlink();

    //
    // Members
    //
protected:
    /** reference to the parent body node */
    boost::shared_ptr<Body> mBody;

    /** the current horizontal angle */
    float   mHAngle;

    /** the current vertical angle */
    float   mVAngle;

    // event states

    /** event state 'forward' */
    bool    mForward;

    /** event state 'backward' */
    bool    mBackward;

    /** event state 'left' */
    bool    mLeft;

    /** event state 'right' */
    bool    mRight;

    /** event state 'up' */
    bool    mUp;

    /** event state 'down' */
    bool    mDown;
};

DECLARE_CLASS(FPSController);

} //namespace oxygen

#endif //OXYGEN_FPSCONTROLLER_H
