/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: body.h 106 2009-11-19 10:10:50Z a-held $

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
#ifndef OXYGEN_RIGIDBODY_H
#define OXYGEN_RIGIDBODY_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/body.h>

namespace oxygen
{
class World;
class RigidBodyInt;

/** Body encapsulates an ODE rigid body object. A rigid body has
    various properties from the point of view of the simulation. Some
    properties change over time: position, linear velocity,
    orientation and angular velocity. Other body properties are
    usually constant over time: Mass, Center of mass and Inertia
    matrix (mass distribution matrix)
 */
class OXYGEN_API RigidBody : public Body
{
    //
    // Functions
    //
public:
    RigidBody();
    virtual ~RigidBody();

    /** returns the ID of the mangaged ODE body */
    long GetBodyID() const;

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
    void GetMassParameters(float& mass) const;
    
    /** adds the given ode mass to this body. The given matrix is
        applied to the mass center
    */
    void AddMass(const dMass& mass, const salt::Matrix& matrix);

    /** sets the mass parameters to represent a sphere of the given
        radius and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetSphere(float density, float radius);

    /** adds a mass representing a sphere of the given radius and
        density, with the matrix determining its center and
        orientation
    */
    void AddSphere(float density, float radius, const salt::Matrix& matrix);

    /** sets the mass parameters to represent a sphere of the given
        radius and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetSphereTotal(float total_mass, float radius);

    /** add a mass representing a sphere of the given radius and total
        mass, with the matrix determining its center and orientation
    */
    void AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix);

    /** Set the mass parameters to represent a box of the given
        dimensions and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetBox(float density, const salt::Vector3f& size);

    /** Add a mass representing a box of the given dimensions and
        density, with the matrix determining its center and
        orientation
    */
    void AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix);

    /** Set the mass parameters to represent a box of the given
        dimensions and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    void SetBoxTotal(float total_mass, const salt::Vector3f& size);

    /** Add a mass representing a box of the given dimensions and
        total mass, with the matrix determining its center and
        orientation
    */
    void AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix);

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and density, with the center of mass at
        (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    void SetCylinder(float density, float radius, float length);

    /** Add a mass representing a flat-ended cylinder of the given
        parameters and density, with the matrix determining its center
        and orientation
     */
    void AddCylinder(float density, float radius, float length, const salt::Matrix& matrix);

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and total mass, with the center of mass
        at (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    void SetCylinderTotal(float total_mass, float radius, float length);

    /** Add a mass representing a flat-ended cylinder of the given
        parameters and total mass, with the matrix determining its
        center and orientation
     */
    void AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix);

    /* Set the mass parameters to represent a capped cylinder of the
       given parameters and density, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    void SetCappedCylinder (float density, float radius, float length);

    /* Add a mass representing a capped cylinder of the given
       parameters and density, with the matrix determining its center
       and orientation
    */
    void AddCappedCylinder (float density, float radius, float length, const salt::Matrix& matrix);

    /* Set the mass parameters to represent a capped cylinder of the
       given parameters and total mass, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    void SetCappedCylinderTotal(float total_mass, float radius, float length);

    /* Add a mass representing a capped cylinder of the given
       parameters and total mass, with the matrix determining its
       center and orientation
    */
    void AddCappedCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix);

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
    static boost::shared_ptr<RigidBody> GetBody(long id);

    /** applies a force to the managed ODE body */
    void AddForce(const salt::Vector3f& force);

    /** aplies a  force to the managed ODE body */
    void AddTorque(const salt::Vector3f& torque);

    /** sets the position of the managed ODE body */
    void SetPosition(const salt::Vector3f& pos);

    /** returns the current poosition of this body */
    salt::Vector3f GetPosition() const;

    /** destroy the managed physicsobject */
    virtual void DestroyPhysicsObject();

    /** synchronize parent node with the bodies position and
        orientation
    */
    void SynchronizeParent() const;

    salt::Vector3f GetMassCenter() const;

protected:
    /** creates the managed ODE body and moves it to the position of
        it's scene-graph parent
    */
    virtual void OnLink();

    /** create the managed ODE body; returns true on success */
    bool CreateBody();

private:
    /** updates internal state before physics calculation */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** updates the the internal state after physics calculation,
        i.e. synchronises this scene graph node and the corresponding
        ODE body.
    */
    virtual void PostPhysicsUpdateInternal();

    //
    // Members
    //
protected:
    boost::shared_ptr<RigidBodyInt> mRigidBodyImp;
};

DECLARE_CLASS(RigidBody);

} //namespace oxygen

#endif //OXYGEN_RIGIDBODY_H
