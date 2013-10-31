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
#ifndef OXYGEN_RIGIDBODY_H
#define OXYGEN_RIGIDBODY_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class World;
class RigidBodyInt;

/** RigidBody encapsulates a rigid body object. A rigid body has
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

    /** returns the ID of the mangaged body */
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
        gravity 
    */
    bool UsesGravity() const;

    /** sets the mass of this body */
    void SetMass(float mass);

    /** returns the mass of this body */
    float GetMass() const;
    
    /** Sets the mass parameters of this body */
    void SetMassParameters(const GenericMass* mass);

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

    /** Set the mass parameters to represent a capsule of the
       given parameters and density, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    void SetCapsule (float density, float radius, float length);

    /* Add a mass representing a capsule of the given
       parameters and density, with the matrix determining its center
       and orientation
    */
    void AddCapsule (float density, float radius, float length, const salt::Matrix& matrix);

    /** Set the mass parameters to represent a capsule of the
        given parameters and total mass, with the center of mass at
        (0,0,0) relative to the body. The radius of the cylinder (and
        the spherical cap) is radius. The length of the cylinder (not
        counting the spherical cap) is length. The cylinder's long axis
        is oriented along the body's z axis.
    */
    void SetCapsuleTotal(float total_mass, float radius, float length);

    /** Add a mass representing a capsule of the given
        parameters and total mass, with the matrix determining its
        center and orientation
    */
    void AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix);

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
    
    /** returns the current angular velocity of this body in local coordinates */
    salt::Vector3f GetLocalAngularVelocity() const;

    /** returns the current angular velocity of this body */
    salt::Vector3f GetAngularVelocity() const;

    /** sets the current angular velocity of this body */
    void SetAngularVelocity(const salt::Vector3f& vel);

    /** returns the Body node corresponding to the given body */
    static boost::shared_ptr<RigidBody> GetBody(long id);

    /** applies a force to the managed body */
    void AddForce(const salt::Vector3f& force);
    
    /** returns the total force applied to this body */
    salt::Vector3f GetForce() const;

    /** aplies a  force to the managed body */
    void AddTorque(const salt::Vector3f& torque);

    /** sets the position of the managed body */
    void SetPosition(const salt::Vector3f& pos);

    /** returns the current poosition of this body */
    salt::Vector3f GetPosition() const;

    /** destroy the managed physicsobject */
    virtual void DestroyPhysicsObject();

    /** synchronize parent node with the bodies position and
        orientation
    */
    void SynchronizeParent() const;
    
    /** Creates a new mass object with the information passed on to this
        method via \param mass and \param cVector
    */
    GenericMass* CreateMass(float mass, salt::Vector3f cVector);

    GenericMass* CreateMass(float mass, const salt::Vector3f& cg, const salt::Vector3f& Ixx, const salt::Vector3f& Ixy ) const;

    /** Sets the value of the inertia tensor of this body. \param i is the
        index of the \param value 's position in the inertia tensor.
        (It's a 3x3 matrix)
    */
    void SetInertiaTensorAt(int i, float value, GenericMass& mass); 

protected:
    /** creates the managed body and moves it to the position of
        it's scene-graph parent
    */
    virtual void OnLink();

    /** create the managed body; returns true on success */
    bool CreateBody();

private:
    /** updates internal state before physics calculation */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** updates the the internal state after physics calculation,
        i.e. synchronises this scene graph node and the corresponding
        body.
    */
    virtual void PostPhysicsUpdateInternal();

    //
    // Members
    //
protected:
    static boost::shared_ptr<RigidBodyInt> mRigidBodyImp;
};

DECLARE_CLASS(RigidBody);

} //namespace oxygen

#endif //OXYGEN_RIGIDBODY_H
