/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef OXYGEN_ODERIGIDBODY_H
#define OXYGEN_ODERIGIDBODY_H

#include <oxygen/physicsserver/int/rigidbodyint.h>

namespace oxygen
{

class OXYGEN_API ODERigidBody : public RigidBodyInt
{
public:
    ODERigidBody();
    virtual ~ODERigidBody();
    
    dBodyID GetODEBody() const;
    void Enable();
    void Disable();
    bool IsEnabled() const;
    void UseGravity(bool f);
    bool UsesGravity() const;
    void SetMass(float mass);
    void SetMassParameters(const dMass& mass);
    float GetMass() const;
    void GetMassParameters(dMass& mass) const;
    void AddMass(const dMass& mass, const salt::Matrix& matrix);
    void SetSphere(float density, float radius);
    void AddSphere(float density, float radius, const salt::Matrix& matrix);
    void SetSphereTotal(float total_mass, float radius);
    void AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix);
    void SetBox(float density, const salt::Vector3f& size);
    void AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix);
    void SetBoxTotal(float total_mass, const salt::Vector3f& size);
    void AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix);
    void SetCylinder(float density, float radius, float length);
    void AddCylinder(float density, float radius, float length, const salt::Matrix& matrix);
    void SetCylinderTotal(float total_mass, float radius, float length);
    void AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix);
    void SetCapsule(float density, float radius, float length);
    void AddCapsule(float density, float radius, float length, const salt::Matrix& matrix);
    void SetCapsuleTotal(float total_mass, float radius, float length);
    void AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix);
    void TranslateMass(const salt::Vector3f& v);
    salt::Vector3f GetVelocity() const;
    void SetVelocity(const salt::Vector3f& vel);
    void SetRotation(const salt::Matrix& rot);
    salt::Matrix GetRotation() const;
    salt::Vector3f GetAngularVelocity() const;
    void SetAngularVelocity(const salt::Vector3f& vel);
    void AddForce(const salt::Vector3f& force);
    void AddTorque(const salt::Vector3f& torque);
    void SetPosition(const salt::Vector3f& pos);
    salt::Vector3f GetPosition() const;
    void DestroyPhysicsObject();
    salt::Matrix GetSynchronisationMatrix();
    //salt::Vector3f GetMassCenter() const;
    void BodySetData(RigidBody* rb);
    RigidBody* BodyGetData(long bodyID);
    
protected:
    void CreateBody(long world);
    void PrepareBoxTotal(dMass& mass, float total_mass, const salt::Vector3f& size) const;
    void PrepareBox(dMass& mass, float density, const salt::Vector3f& size) const;
    void PrepareSphere(dMass& mass, float density, float radius) const;   
    void PrepareSphereTotal(dMass& mass, float total_mass, float radius) const;
    void PrepareCylinder(dMass& mass, float density, float radius, float length) const;
    void PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const;
    void PrepareCapsule(dMass& mass, float density, float radius, float length) const;
    void PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const;
    
private:
    void PrePhysicsUpdateInternal(float deltaTime);
    
protected:
    salt::Vector3f mMassTrans;
    bool mMassTransformed;
};

} //namespace oxygen

#endif //OXYGEN_ODERIGIDBODY_H
