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
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class RigidBody;

class OXYGEN_API RigidBodyInt
{

public:
    virtual RigidBody* GetBodyPointer(long bodyID) = 0;
    
    virtual void Enable(long bodyID) = 0;
    virtual void Disable(long bodyID) = 0;
    virtual bool IsEnabled(long bodyID) const = 0;
    virtual void UseGravity(bool f, long bodyID) = 0;
    virtual bool UsesGravity(long bodyID) const = 0;
    virtual void SetMass(float mass, long bodyID) = 0;
    virtual void SetMassParameters(const GenericMass& mass, long bodyID) = 0;
    virtual float GetMass(long bodyID) const = 0;
    virtual void SetSphere(float density, float radius, long bodyID) = 0;
    virtual salt::Vector3f AddSphere(float density, float radius, const salt::Matrix& matrix,
                                     salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetSphereTotal(float total_mass, float radius, long bodyID) = 0;
    virtual salt::Vector3f AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix,
                                          salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetBox(float density, const salt::Vector3f& size, long bodyID) = 0;
    virtual salt::Vector3f AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix,
                                  salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetBoxTotal(float total_mass, const salt::Vector3f& size, long bodyID) = 0;
    virtual salt::Vector3f AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix,
                                       salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetCylinder(float density, float radius, float length, long bodyID) = 0;
    virtual salt::Vector3f AddCylinder(float density, float radius, float length, const salt::Matrix& matrix,
                                       salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetCylinderTotal(float total_mass, float radius, float length, long bodyID) = 0;
    virtual salt::Vector3f AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                            salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetCapsule(float density, float radius, float length, long bodyID) = 0;
    virtual salt::Vector3f AddCapsule(float density, float radius, float length, const salt::Matrix& matrix,
                                      salt::Vector3f massTrans, long bodyID) = 0;
    virtual void SetCapsuleTotal(float total_mass, float radius, float length, long bodyID) = 0;
    virtual salt::Vector3f AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                           salt::Vector3f massTrans, long bodyID) = 0;
    virtual void TranslateMass(const salt::Vector3f& v, long bodyID) = 0;
    virtual salt::Vector3f GetVelocity(long bodyID) const = 0;
    virtual void SetVelocity(const salt::Vector3f& vel, long bodyID) = 0;
    virtual void SetRotation(const salt::Matrix& rot, long bodyID) = 0;
    virtual salt::Matrix GetRotation(long bodyID) const = 0;
    virtual salt::Vector3f GetLocalAngularVelocity(long bodyID) const = 0;
    virtual salt::Vector3f GetAngularVelocity(long bodyID) const = 0;
    virtual void SetAngularVelocity(const salt::Vector3f& vel, long bodyID) = 0;
    virtual void AddForce(const salt::Vector3f& force, long bodyID) = 0;
    virtual salt::Vector3f GetForce(long bodyID) const = 0;
    virtual void AddTorque(const salt::Vector3f& torque, long bodyID) = 0;
    virtual void SetPosition(const salt::Vector3f& pos, long bodyID) = 0;
    virtual salt::Vector3f GetPosition(long bodyID) const = 0;
    virtual void DestroyPhysicsObject(long bodyID) = 0;
    virtual salt::Matrix GetSynchronisationMatrix(long bodyID) = 0;
    virtual void BodySetData(RigidBody* rb, long bodyID) = 0;
    virtual RigidBody* BodyGetData(long bodyID) = 0;    
    virtual long CreateBody(long world) = 0;
};

} //namespace oxygen

#endif //OXYGEN_RIGIDBODYINT_H
