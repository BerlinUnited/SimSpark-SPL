/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: body.h,v 1.6 2004/02/12 14:07:22 fruit Exp $

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
#ifndef OXYGEN_BODY_H
#define OXYGEN_BODY_H

#include "odeobject.h"

namespace oxygen
{
class World;

/** Body encapsulates an ODE rigid body object. A rigid body has
    various properties from the point of view of the simulation. Some
    properties change over time: position, linear velocity,
    orientation and angular velocity. Other body properties are
    usually constant over time: Mass, Center of mass and Inertia
    matrix (mass distribution matrix)
 */
class Body : public ODEObject
{
    //
    // Functions
    //
public:
    Body();
    virtual ~Body();

    /** returns the ID of the mangaged ODE body */
    dBodyID GetODEBody() const;

    /** enables this body. Each body can be enabled or
        disabled. Disabled bodies are effectively turned off and are
        not updated during a simulation step. Disabling bodies is an
        effective way to save computation time when it is known that
        the bodies are motionless or otherwise irrelevent to the
        simulation.
    */
    void Enable();

    /** disables this body */
    void Disable();

    /** returns true if this body is enabled */
    bool IsEnabled() const;

    /** sets whether the body is influenced by the world's gravity or
        not. Bodies are constructed to be influenced by the world's
        gravity by default.
    */
    void UseGravity(bool f);

    /** returns true, if this body is influenced by the world's
        gravity */
    bool UsesGravity() const;

    /** sets the mass of this body */
    void SetMass(float mass);

    /** sets the mass parameters to represent a sphere of the given
        radius and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetSphere(float density, float radius);

    /** sets the maximum speed this body is allowed to move at */
    void SetMaxSpeed(float speed);

    /** returns the current linear velocity vector of this body */
    salt::Vector3f GetVelocity() const;

    /** sets the current linear velocity of this body */
    void SetVelocity(const salt::Vector3f& vel);

    /** returns the current angular velocity of this body */
    salt::Vector3f GetAngularVelocity();

    /** sets the current angular velocity of this body */
    void SetAngularVelocity(const salt::Vector3f& vel);

    /** returns the Body node corresponding to the given ODE body */
    static Body* GetBody(dBodyID id);

    /** applies a force to the managed ODE body */
    void AddForce(const salt::Vector3f& force);

    /** aplies a  force to the managed ODE body */
    void AddTorque(const salt::Vector3f& torque);

    /** sets the position of the managed ODE body */
    void SetPosition(const salt::Vector3f& pos);

protected:
    /** creates the managed ODE body and moves it to the position of
        it's scene-graph parent
    */
    virtual void OnLink();

    /** destroys the managed ODE body */
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
    boost::shared_ptr<World> mWorld;

    /** the managed ode body */
    dBodyID mODEBody;

    /** linear drag (default 0.3) */
    float mLinearDrag;

    /** maximum velocity, this body can attain */
    float mMaxSpeed;

    /** maximum force which can be applied to this body */
    float mMaxForce;
};

DECLARE_CLASS(Body);

} //namespace oxygen

#endif //OXYGEN_BODY_H
