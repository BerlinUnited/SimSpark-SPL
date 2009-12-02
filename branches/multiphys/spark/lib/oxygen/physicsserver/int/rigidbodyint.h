/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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
#include <oxygen/physicsserver/ode/odebody.h>
#include <iostream>

namespace oxygen
{

class OXYGEN_API RigidBodyInt : public ODEBody
{

public:
    virtual void Enable() = 0;
    virtual void Disable() = 0;
    virtual bool IsEnabled() const = 0;
    virtual void UseGravity(bool f) = 0;
    virtual bool UsesGravity() const = 0;
    virtual void CreateBody(long world) = 0;
    virtual void SetMass(float mass) = 0;
    virtual void SetMassParameters(const dMass& mass) = 0;
    virtual float GetMass() const = 0;
    virtual void GetMassParameters(dMass& mass) const = 0;
    virtual void SetSphere(float density, float radius) = 0;
    virtual void AddSphere(float density, float radius, const salt::Matrix& matrix) = 0;
    virtual void SetSphereTotal(float total_mass, float radius) = 0;
    virtual void AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix) = 0;
    virtual void SetBox(float density, const salt::Vector3f& size) = 0;
    virtual void AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix) = 0;
    virtual void SetBoxTotal(float total_mass, const salt::Vector3f& size) = 0;
    virtual void AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix) = 0;
    virtual void SetCylinder(float density, float radius, float length) = 0;
    virtual void AddCylinder(float density, float radius, float length, const salt::Matrix& matrix) = 0;
    virtual void SetCylinderTotal(float total_mass, float radius, float length) = 0;
    virtual void AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix) = 0;
    virtual void SetCapsule(float density, float radius, float length) = 0;
    virtual void AddCapsule(float density, float radius, float length, const salt::Matrix& matrix) = 0;
    virtual void SetCapsuleTotal(float total_mass, float radius, float length) = 0;
    virtual void AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix) = 0;
    virtual void TranslateMass(const salt::Vector3f& v) = 0;
    virtual salt::Vector3f GetVelocity() const = 0;
    virtual void SetVelocity(const salt::Vector3f& vel) = 0;
    virtual void SetRotation(const salt::Matrix& rot) = 0;
    virtual salt::Matrix GetRotation() const = 0;
    virtual salt::Vector3f GetAngularVelocity() const = 0;
    virtual void SetAngularVelocity(const salt::Vector3f& vel) = 0;
    virtual void AddForce(const salt::Vector3f& force) = 0;
    virtual void AddTorque(const salt::Vector3f& torque) = 0;
    virtual void SetPosition(const salt::Vector3f& pos) = 0;
    virtual salt::Vector3f GetPosition() const = 0;
    virtual void DestroyPhysicsObject() = 0;
    virtual salt::Matrix GetSynchronisationMatrix() = 0;
    virtual void AddMass(const dMass& mass, const salt::Matrix& matrix) = 0;
    //virtual salt::Vector3f GetMassCenter() const = 0;
    virtual void BodySetData(RigidBody* rb) = 0;
    virtual RigidBody* BodyGetData(long bodyID) = 0;
    
    virtual long GetBodyID() = 0;
    virtual salt::Vector3f GetMassTrans() = 0;
    virtual void SetMassTrans(salt::Vector3f massTrans) = 0; 
    virtual bool GetMassTransformed() = 0;
    virtual void SetMassTransformed(bool f) = 0;
    
protected:
    RigidBodyInt() : ODEBody(), mBodyID(0), mMassTrans(0,0,0), mMassTransformed(false){
    };

    /** sets up an ode mass struct representing a box of the given
        size and total_mass
    */
    virtual void PrepareBoxTotal(dMass& mass, float total_mass, const salt::Vector3f& size) const = 0;
    
    /** sets up an ode mass struct representing a box of the given
        density and size
    */
    virtual void PrepareBox(dMass& mass, float density, const salt::Vector3f& size) const = 0;
    
    /** sets up an ode mass struct representing a sphere of the given
        density and radius
    */
    virtual void PrepareSphere(dMass& mass, float density, float radius) const = 0;   
    
    /** sets up an ode mass struct representing a sphere of the given
        radius and total_mass
    */
    virtual void PrepareSphereTotal(dMass& mass, float total_mass, float radius) const = 0;
    
    /** sets up an ode mass struct representing a flat-ended cylinder
        of the given parameters and density, with the center of mass
        at (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    virtual void PrepareCylinder(dMass& mass, float density, float radius, float length) const = 0;
    
    /** sets up an ode mass struct representing a flat-ended cylinder
        of the given parameters and total mass, with the center of
        mass at (0,0,0) relative to the body. The radius of the
        cylinder is radius. The length of the cylinder is length. The
        cylinder's long axis is oriented along the body's z axis.
     */
    virtual void PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const = 0;
    
    /* sets up an ode mass struct representing a capsule of
       the given parameters and density, with the center of mass at
       (0,0,0) relative to the body. The radius of the capsule (and
       the spherical cap) is radius. The length of the capsule (not
       counting the spherical cap) is length. The capsule's long axis
       is oriented along the body's z axis.
    */
    virtual void PrepareCapsule(dMass& mass, float density, float radius, float length) const = 0;
    
    /* sets up an ode mass struct representing a capsule of
       the given parameters and total mass, with the center of mass at
       (0,0,0) relative to the body. The radius of the capsule (and
       the spherical cap) is radius. The length of the capsule (not
       counting the spherical cap) is length. The capsule's long axis
       is oriented along the body's z axis.
    */
    virtual void PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const = 0;
    
protected:
    long mBodyID;
    salt::Vector3f mMassTrans;
    bool mMassTransformed;
};

} //namespace oxygen

#endif //OXYGEN_RIGIDBODYINT_H
