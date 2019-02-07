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

#ifndef OXYGEN_RIGIDBODYINT_H
#define OXYGEN_RIGIDBODYINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class RigidBody;

class OXYGEN_API RigidBodyInt
{

public:
    virtual ~RigidBodyInt() {}

    /** Returns a pointer to the abstract RigidBody object that
        manages the rigid body specified by \param bodyID
    */
    virtual RigidBody* GetBodyPointer(long bodyID) = 0;

    /** enables this body. Each body can be enabled or
        disabled. Disabled bodies are effectively turned off and are
        not updated during a simulation step. Disabling bodies is an
        effective way to save computation time when it is known that
        the bodies are motionless or otherwise irrelevent to the
        simulation.
    */
    virtual void Enable(long bodyID) = 0;

    /** disables this body */
    virtual void Disable(long bodyID) = 0;

    /** returns true if this body is enabled */
    virtual bool IsEnabled(long bodyID) const = 0;

    /** sets whether the body is influenced by the world's gravity or
        not. Bodies are constructed to be influenced by the world's
        gravity by default.
    */
    virtual void UseGravity(bool f, long bodyID) = 0;

    /** returns true, if this body is influenced by the world's
        gravity
    */
    virtual bool UsesGravity(long bodyID) const = 0;

    /** sets the mass of this body */
    virtual void SetMass(float mass, long bodyID) = 0;

    /** Sets the mass parameters of this body */
    virtual void SetMassParameters(const GenericMass* mass, long bodyID) = 0;

    /** returns the mass of this body */
    virtual float GetMass(long bodyID) const = 0;

    /** sets the mass parameters to represent a sphere of the given
        radius and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    virtual void SetSphere(float density, float radius, long bodyID) = 0;

    /** adds a mass representing a sphere of the given radius and
        density, with the matrix determining its center and
        orientation
    */
    virtual salt::Vector3f AddSphere(float density, float radius, const salt::Matrix& matrix,
                                     salt::Vector3f massTrans, long bodyID) = 0;

    /** sets the mass parameters to represent a sphere of the given
        radius and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    virtual void SetSphereTotal(float total_mass, float radius, long bodyID) = 0;

    /** add a mass representing a sphere of the given radius and total
        mass, with the matrix determining its center and orientation
    */
    virtual salt::Vector3f AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix,
                                          salt::Vector3f massTrans, long bodyID) = 0;

    /** Set the mass parameters to represent a box of the given
        dimensions and density, with the center of mass at (0,0,0)
        relative to the body.
    */
    virtual void SetBox(float density, const salt::Vector3f& size, long bodyID) = 0;

    /** Add a mass representing a box of the given dimensions and
        density, with the matrix determining its center and
        orientation
    */
    virtual salt::Vector3f AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix,
                                  salt::Vector3f massTrans, long bodyID) = 0;

    /** Set the mass parameters to represent a box of the given
        dimensions and total mass, with the center of mass at (0,0,0)
        relative to the body.
    */
    virtual void SetBoxTotal(float total_mass, const salt::Vector3f& size, long bodyID) = 0;

    /** Add a mass representing a box of the given dimensions and
        total mass, with the matrix determining its center and
        orientation
    */
    virtual salt::Vector3f AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix,
                                       salt::Vector3f massTrans, long bodyID) = 0;

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and density, with the center of mass at
        (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    virtual void SetCylinder(float density, float radius, float length, long bodyID) = 0;

    /** Add a mass representing a flat-ended cylinder of the given
        parameters and density, with the matrix determining its center
        and orientation
     */
    virtual salt::Vector3f AddCylinder(float density, float radius, float length, const salt::Matrix& matrix,
                                       salt::Vector3f massTrans, long bodyID) = 0;

    /** Set the mass parameters to represent a flat-ended cylinder of
        the given parameters and total mass, with the center of mass
        at (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    virtual void SetCylinderTotal(float total_mass, float radius, float length, long bodyID) = 0;

    /** Add a mass representing a flat-ended cylinder of the given
        parameters and total mass, with the matrix determining its
        center and orientation
     */
    virtual salt::Vector3f AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                            salt::Vector3f massTrans, long bodyID) = 0;

    /* Set the mass parameters to represent a capsule of the
       given parameters and density, with the center of mass at
       (0,0,0) relative to the body. The radius of the cylinder (and
       the spherical cap) is radius. The length of the cylinder (not
       counting the spherical cap) is length. The cylinder's long axis
       is oriented along the body's z axis.
    */
    virtual void SetCapsule(float density, float radius, float length, long bodyID) = 0;

    /** Add a mass representing a capsule of the given
        parameters and density, with the matrix determining its center
        and orientation
    */
    virtual salt::Vector3f AddCapsule(float density, float radius, float length, const salt::Matrix& matrix,
                                      salt::Vector3f massTrans, long bodyID) = 0;

    /** Set the mass parameters to represent a capsule of the
        given parameters and total mass, with the center of mass at
        (0,0,0) relative to the body. The radius of the cylinder (and
        the spherical cap) is radius. The length of the cylinder (not
        counting the spherical cap) is length. The cylinder's long axis
        is oriented along the body's z axis.
    */
    virtual void SetCapsuleTotal(float total_mass, float radius, float length, long bodyID) = 0;

    /** Add a mass representing a capsule of the given
        parameters and total mass, with the matrix determining its
        center and orientation
    */
    virtual salt::Vector3f AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                           salt::Vector3f massTrans, long bodyID) = 0;

    /** displace the mass center relative to the body frame */
    virtual void TranslateMass(const salt::Vector3f& v, long bodyID) = 0;

    /** returns the current linear velocity vector of this body */
    virtual salt::Vector3f GetVelocity(long bodyID) const = 0;

    /** sets the current linear velocity of this body */
    virtual void SetVelocity(const salt::Vector3f& vel, long bodyID) = 0;

    /** sets the roation of this body */
    virtual void SetRotation(const salt::Matrix& rot, long bodyID) = 0;

    /** gets the rotation of this body */
    virtual salt::Matrix GetRotation(long bodyID) const = 0;

    /** returns the current angular velocity of this body in local coordinates */
    virtual salt::Vector3f GetLocalAngularVelocity(long bodyID) const = 0;

    /** returns the current angular velocity of this body */
    virtual salt::Vector3f GetAngularVelocity(long bodyID) const = 0;

    /** sets the current angular velocity of this body */
    virtual void SetAngularVelocity(const salt::Vector3f& vel, long bodyID) = 0;

    /** applies a force to the managed body */
    virtual void AddForce(const salt::Vector3f& force, long bodyID) = 0;

    /** returns the total force applied to this body */
    virtual salt::Vector3f GetForce(long bodyID) const = 0;

    /** aplies a  force to the managed body */
    virtual void AddTorque(const salt::Vector3f& torque, long bodyID) = 0;

    /** sets the position of the managed body */
    virtual void SetPosition(const salt::Vector3f& pos, long bodyID) = 0;

    /** returns the current poosition of this body */
    virtual salt::Vector3f GetPosition(long bodyID) const = 0;

    /** destroys the rigid body specified by \param bodyID */
    virtual void DestroyRigidBody(long bodyID) = 0;

    /** Gets the matrix needed by the SynchronizeParent method */
    virtual salt::Matrix GetSynchronisationMatrix(long bodyID) = 0;

    /** Links the rigid body specified by \param bodyID with the
        abstract RigidBody object managing it. This allows retrieving
        it at a later point.
    */
    virtual void BodySetData(RigidBody* rb, long bodyID) = 0;

    /** Returns a pointer to the abstract RigidBody object managing
        the rigid body specified by \param bodyID
    */
    virtual RigidBody* BodyGetData(long bodyID) = 0;

    /** Creates a new rigid body within the physics world specified
        by \param worldID
    */
    virtual long CreateBody(long worldID) = 0;

    /** Creates a new mass object with the information passed on to this
        method via \param mass and \param cVector
    */
    virtual GenericMass* CreateMass(float mass, salt::Vector3f cVector) = 0;

    virtual GenericMass* CreateMass(float mass, const salt::Vector3f& cg, const salt::Vector3f& Ixx, const salt::Vector3f& Ixy ) const = 0;

    /** Sets the value of the inertia tensor of this body. \param i is the
        index of the \param value 's position in the inertia tensor.
        (It's a 3x3 matrix)
    */
    virtual void SetInertiaTensorAt(int i, float value, GenericMass& mass) = 0;
};

} //namespace oxygen

#endif //OXYGEN_RIGIDBODYINT_H
