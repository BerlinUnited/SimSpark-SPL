/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body.h,v 1.4 2003/08/31 21:53:45 fruit Exp $

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

   Body
*/
#ifndef OXYGEN_BODY_H
#define OXYGEN_BODY_H

#include "odeobject.h"

namespace oxygen
{

class World;

/** Body encapsulates an ODE rigid body object. A rigid body has various
    properties from the point of view of the simulation. Some properties
    change over time: Position, Linear velocity, Orientation, Angular
    velocity. Other body properties are usually constant over time: Mass,
    Center of mass and Inertia matrix (mass distribution matrix)
 */
class Body : public ODEObject
{
    //
    // Functions
    //
public:
    Body();
    virtual ~Body();

    /** returns the ID of the ODE body */
    dBodyID GetODEBody() const;

    /** enables this body. Each body can be enabled or disabled. Disabled
        bodies are effectively `turned off' and are not updated during a
        simulation step. Disabling bodies is an effective way to save
        computation time when it is known that the bodies are motionless
        or otherwise irrelevent to the simulation.
    */
    void Enable();

    /** disables this body */
    void Disable();

    /** returns true if this body is enabled */
    bool IsEnabled() const;

    /** sets whether the body is influenced by the world's gravity
        or not. Newly created bodies are always influenced by the
        world's gravity.
    */
    void UseGravity(bool f);

    /** returns true, if this body is influenced by the world's gravity */
    bool UsesGravity() const;

    /** sets the mass of this body */
    void SetMass(float mass);

    /** sets the mass parameters to represent a sphere of the
        given radius and density, with the center of mass at
        (0,0,0) relative to the body.
    */
    void SetSphere(float density, float radius);

    /** sets the maximum speed this body is allowed to move at */
    void SetMaxSpeed(float speed)   {       mMaxSpeed = speed;      }

    /** returns the current velocity vector of this body */
    salt::Vector3f GetVelocity() const;

protected:
    /** creates the corresponding ODE body and moves it to the
        position of it's scene-graph parent
    */
    virtual void OnLink();

    /** destroys the corresponding ODE body */
    virtual void OnUnlink();

private:
    /** updates the the internal state after physics calculation,
        i.e. synchronises this scene graph node and the corresponding
        ODE body.
    */
    virtual void PostPhysicsUpdateInternal();

    //
    // Members
    //
protected:
    /** the world this body is contained in */
    boost::shared_ptr<World>        mWorld;

    /** the managed ode body */
    dBodyID                                         mODEBody;

    /** linear drag (default 0.3) */
    float                                           mLinearDrag;

    /** maximum velocity, this body can attain */
    float                                           mMaxSpeed;

    /** maximum force which can be applied to this body */
    float                                           mMaxForce;
};

    DECLARE_CLASS(Body);

} //namespace oxygen

#endif //OXYGEN_BODY_H
