/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: body.h,v 1.16 2007/06/16 11:01:35 yxu Exp $

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

    /** set the ODE mass parameters of this body */
    void SetMassParameters(const dMass& mass);

    /** returns the mass of this body */
    float GetMass() const;

    /** returns the ODE mass parameters of this body */
    void GetMassParameters(dMass& mass) const;

    /** sets the mass parameters to represent a sphere of the given
        radius and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetSphere(float density, float radius);

    /** sets the mass parameters to represent a sphere of the given
        radius and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetSphereTotal(float total_mass, float radius);

    /** Set the mass parameters to represent a box of the given
        dimensions and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetBox(float density, const salt::Vector3f& size);

    /** Set the mass parameters to represent a box of the given
        dimensions and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetBoxTotal(float total_mass, const salt::Vector3f& size);

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and density, with the center of mass at
        (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    void SetCylinder (float density, float radius, float length);

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and total mass, with the center of mass
        at (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    void SetCylinderTotal(float total_mass, float radius, float length);

    /* Set the mass parameters to represent a capped cylinder of the
       given parameters and density, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    void SetCappedCylinder (float density, float radius, float length);

    /* Set the mass parameters to represent a capped cylinder of the
       given parameters and total mass, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    void SetCappedCylinderTotal(float total_mass, float radius, float length);

    /** displace the mass center relative to the body frame */
    void TranslateMass(const salt::Vector3f& v);

    /** returns the current linear velocity vector of this body */
    salt::Vector3f GetVelocity() const;

    /** sets the current linear velocity of this body */
    void SetVelocity(const salt::Vector3f& vel);

    /** sets the roation of this body */
    void SetRotation(const salt::Matrix& rot);

    /** gets the rotation of this body */
    salt::Matrix GetRotation() const;

    /** returns the current angular velocity of this body */
    salt::Vector3f GetAngularVelocity() const;

    /** sets the current angular velocity of this body */
    void SetAngularVelocity(const salt::Vector3f& vel);

    /** returns the Body node corresponding to the given ODE body */
    static boost::shared_ptr<Body> GetBody(dBodyID id);

    /** applies a force to the managed ODE body */
    void AddForce(const salt::Vector3f& force);

    /** aplies a  force to the managed ODE body */
    void AddTorque(const salt::Vector3f& torque);

    /** sets the position of the managed ODE body */
    void SetPosition(const salt::Vector3f& pos);

    /** returns the current poosition of this body */
    salt::Vector3f GetPosition() const;

protected:
    /** creates the managed ODE body and moves it to the position of
        it's scene-graph parent
    */
    virtual void OnLink();

    /** create the managed ODE body; returns true on success */
    bool CreateBody();

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
    /** the managed ode body */
    dBodyID mODEBody;
};

DECLARE_CLASS(Body);

} //namespace oxygen

#endif //OXYGEN_BODY_H
