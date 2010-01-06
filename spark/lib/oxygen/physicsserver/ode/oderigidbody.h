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

#include <oxygen/physicsserver/ode/odebody.h>
#include <oxygen/physicsserver/int/rigidbodyint.h>

namespace oxygen
{
class RigidBody;

class OXYGEN_API ODERigidBody : public RigidBodyInt, public ODEBody
{
public:
    ODERigidBody();
    
    void Enable(long bodyID);
    void Disable(long bodyID);
    bool IsEnabled(long bodyID) const;
    void UseGravity(bool f, long bodyID);
    bool UsesGravity(long bodyID) const;
    void SetMass(float mass, long bodyID);
    void SetMassParameters(const GenericMass& mass, long bodyID);
    float GetMass(long bodyID) const;
    void SetSphere(float density, float radius, long bodyID);
    salt::Vector3f AddSphere(float density, float radius, const salt::Matrix& matrix,
                             salt::Vector3f massTrans, long bodyID);
    void SetSphereTotal(float total_mass, float radius, long bodyID);
    salt::Vector3f AddSphereTotal(float total_mass, float radius, const salt::Matrix& matrix,
                                  salt::Vector3f massTrans, long bodyID);
    void SetBox(float density, const salt::Vector3f& size, long bodyID);
    salt::Vector3f AddBox(float density, const salt::Vector3f& size, const salt::Matrix& matrix,
                          salt::Vector3f massTrans, long bodyID);
    void SetBoxTotal(float total_mass, const salt::Vector3f& size, long bodyID);
    salt::Vector3f AddBoxTotal(float total_mass, const salt::Vector3f& size, const salt::Matrix& matrix,
                               salt::Vector3f massTrans, long bodyID);
    void SetCylinder(float density, float radius, float length, long bodyID);
    salt::Vector3f AddCylinder(float density, float radius, float length, const salt::Matrix& matrix,
                               salt::Vector3f massTrans, long bodyID);
    void SetCylinderTotal(float total_mass, float radius, float length, long bodyID);
    salt::Vector3f AddCylinderTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                    salt::Vector3f massTrans, long bodyID);
    void SetCapsule(float density, float radius, float length, long bodyID);
    salt::Vector3f AddCapsule(float density, float radius, float length, const salt::Matrix& matrix,
                              salt::Vector3f massTrans, long bodyID);
    void SetCapsuleTotal(float total_mass, float radius, float length, long bodyID);
    salt::Vector3f AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix,
                                   salt::Vector3f massTrans, long bodyID);
    void TranslateMass(const salt::Vector3f& v, long bodyID);
    salt::Vector3f GetVelocity(long bodyID) const;
    void SetVelocity(const salt::Vector3f& vel, long bodyID);
    void SetRotation(const salt::Matrix& rot, long bodyID);
    salt::Matrix GetRotation(long bodyID) const;
    salt::Vector3f GetLocalAngularVelocity(long bodyID) const;
    salt::Vector3f GetAngularVelocity(long bodyID) const;
    void SetAngularVelocity(const salt::Vector3f& vel, long bodyID);
    void AddForce(const salt::Vector3f& force, long bodyID);
    salt::Vector3f GetForce(long bodyID) const;
    void AddTorque(const salt::Vector3f& torque, long bodyID);
    void SetPosition(const salt::Vector3f& pos, long bodyID);
    salt::Vector3f GetPosition(long bodyID) const;
    void DestroyPhysicsObject(long bodyID);
    salt::Matrix GetSynchronisationMatrix(long bodyID);
    void BodySetData(RigidBody* rb, long bodyID);
    RigidBody* BodyGetData(long bodyID);    
    long CreateBody(long world);
    
protected:
    //These methods are only called internally and are not declared in the interface.
    salt::Vector3f AddMass(const dMass& mass, const salt::Matrix& matrix, salt::Vector3f massTrans, const long ODEBody);
    
    /** sets up an ode mass struct representing a box of the given
        size and total_mass
    */
    void PrepareBoxTotal(dMass& mass, float total_mass, const salt::Vector3f& size) const;
    
    /** sets up an ode mass struct representing a box of the given
        density and size
    */
    void PrepareBox(dMass& mass, float density, const salt::Vector3f& size) const;
    
    /** sets up an ode mass struct representing a sphere of the given
        density and radius
    */
    void PrepareSphere(dMass& mass, float density, float radius) const;   
    
    /** sets up an ode mass struct representing a sphere of the given
        radius and total_mass
    */
    void PrepareSphereTotal(dMass& mass, float total_mass, float radius) const;
    
    /** sets up an ode mass struct representing a flat-ended cylinder
        of the given parameters and density, with the center of mass
        at (0,0,0) relative to the body. The radius of the cylinder is
        radius. The length of the cylinder is length. The cylinder's
        long axis is oriented along the body's z axis.
     */
    void PrepareCylinder(dMass& mass, float density, float radius, float length) const;
    
    /** sets up an ode mass struct representing a flat-ended cylinder
        of the given parameters and total mass, with the center of
        mass at (0,0,0) relative to the body. The radius of the
        cylinder is radius. The length of the cylinder is length. The
        cylinder's long axis is oriented along the body's z axis.
     */
    void PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const;
    
    /** sets up an ode mass struct representing a capsule of
        the given parameters and density, with the center of mass at
        (0,0,0) relative to the body. The radius of the capsule (and
        the spherical cap) is radius. The length of the capsule (not
        counting the spherical cap) is length. The capsule's long axis
        is oriented along the body's z axis.
    */
    void PrepareCapsule(dMass& mass, float density, float radius, float length) const;
    
    /** sets up an ode mass struct representing a capsule of
        the given parameters and total mass, with the center of mass at
        (0,0,0) relative to the body. The radius of the capsule (and
        the spherical cap) is radius. The length of the capsule (not
        counting the spherical cap) is length. The capsule's long axis
        is oriented along the body's z axis.
    */
    void PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const;
};

} //namespace oxygen

#endif //OXYGEN_ODERIGIDBODY_H
