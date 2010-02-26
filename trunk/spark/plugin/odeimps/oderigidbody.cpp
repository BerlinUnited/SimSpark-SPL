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

#include "oderigidbody.h"

using namespace oxygen;
using namespace boost;
using namespace salt;
using namespace std;

RigidBodyImp::RigidBodyImp() : BodyImp(){

}

RigidBody* RigidBodyImp::GetBodyPointer(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    return static_cast<RigidBody*>(dBodyGetData(ODEBody));
}

void RigidBodyImp::Enable(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodyEnable(ODEBody);
}

void RigidBodyImp::Disable(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodyDisable(ODEBody);
}

bool RigidBodyImp::IsEnabled(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    return (dBodyIsEnabled(ODEBody) != 0);
}

void RigidBodyImp::UseGravity(bool f, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    if (f == true)
        {
            // body is affected by gravity
            dBodySetGravityMode(ODEBody, 1);
        }
    else
        {
            // body is not affected by gravity
            dBodySetGravityMode(ODEBody, 0);
        }
}

bool RigidBodyImp::UsesGravity(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    return (dBodyGetGravityMode(ODEBody) != 0);
}

long RigidBodyImp::CreateBody(long worldID)
{
    // create the managed body
   dBodyID ODEBody = dBodyCreate((dWorldID) worldID);
   return (long) ODEBody; 
}

void RigidBodyImp::DestroyRigidBody(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodyDestroy(ODEBody);
}

void RigidBodyImp::BodySetData(RigidBody* rb, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodySetData(ODEBody, rb);
}

void RigidBodyImp::SetMass(float mass, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    dBodyGetMass(ODEBody, &ODEMass);
    dMassAdjust(&ODEMass, mass);
    dBodySetMass(ODEBody, &ODEMass);
}

float RigidBodyImp::GetMass(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass m;
    dBodyGetMass(ODEBody, &m);
    return m.mass;
}

Vector3f RigidBodyImp::AddMass(const dMass& ODEMass, const Matrix& matrix, Vector3f massTrans, const long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass transMass(ODEMass);

    dMatrix3 ODEMatrix;
    GenericPhysicsMatrix& matrixRef = (GenericPhysicsMatrix&) ODEMatrix;
    ConvertRotationMatrix(matrix, matrixRef);
    dMassRotate(&transMass, ODEMatrix);

    const Vector3f& trans(matrix.Pos());
    dMassTranslate(&transMass,trans[0],trans[1],trans[2]);

    dMassTranslate(&transMass,massTrans[0],massTrans[1],massTrans[2]);
    
    dMass bodyMass;
    dBodyGetMass(ODEBody, &bodyMass);
    dMassAdd(&bodyMass, &transMass);

    /** ODE currently requires that the center mass is always in the
        origin of the body
    */
    Vector3f trans2(bodyMass.c[0], bodyMass.c[1], bodyMass.c[2]);

    dMassTranslate(&bodyMass, -trans2[0], -trans2[1], -trans2[2]);
    bodyMass.c[0] = bodyMass.c[1] = bodyMass.c[2] = 0.0f;
    dBodySetMass(ODEBody, (const dMass*)&bodyMass);

    // Move body so mass is at right position again
    SetPosition(GetPosition(bodyID) + trans2, bodyID);
    
    // Keep track of total translation of mass
    return massTrans - trans2;
}

void RigidBodyImp::SetMassParameters(const GenericMass& mass, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass& ODEMass = (dMass&) mass;
    dBodySetMass(ODEBody, &ODEMass);
}

void RigidBodyImp::PrepareSphere(dMass& mass, float density, float radius) const
{
    dMassSetSphere(&mass, density, radius);
}

void RigidBodyImp::SetSphere(float density, float radius, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddSphere(float density, float radius, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareSphereTotal(dMass& mass, float total_mass, float radius) const
{
    dMassSetSphereTotal(&mass, total_mass, radius);
}

void RigidBodyImp::SetSphereTotal(float total_mass, float radius, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddSphereTotal(float total_mass, float radius, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareBox(dMass& mass, float density, const Vector3f& size) const
{
    dMassSetBox(&mass, density, size[0], size[1], size[2]);
}

void RigidBodyImp::SetBox(float density, const Vector3f& size, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddBox(float density, const Vector3f& size, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareBoxTotal(dMass& mass, float total_mass, const Vector3f& size) const
{
    dMassSetBoxTotal(&mass, total_mass, size[0], size[1], size[2]);
}

void RigidBodyImp::SetBoxTotal(float total_mass, const Vector3f& size, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddBoxTotal(float total_mass, const Vector3f& size, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareCylinder (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinder (&mass, density, direction, radius, length);
}

void RigidBodyImp::SetCylinder (float density, float radius, float length, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddCylinder(float density, float radius, float length, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;
    dMassSetCylinderTotal(&mass, total_mass, direction, radius, length);
}

void RigidBodyImp::SetCylinderTotal(float total_mass, float radius, float length, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddCylinderTotal(float total_mass, float radius, float length, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareCapsule (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsule (&mass, density, direction, radius, length);
}

void RigidBodyImp::SetCapsule (float density, float radius, float length, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddCapsule (float density, float radius, float length, const Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

void RigidBodyImp::PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsuleTotal(&mass, total_mass, direction, radius, length);
}

void RigidBodyImp::SetCapsuleTotal(float total_mass, float radius, float length, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(ODEBody, &ODEMass);
}

salt::Vector3f RigidBodyImp::AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix, salt::Vector3f massTrans, long bodyID)
{
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    return AddMass(ODEMass, matrix, massTrans, bodyID);
}

Vector3f RigidBodyImp::GetVelocity(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* vel = dBodyGetLinearVel(ODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void RigidBodyImp::SetVelocity(const Vector3f& vel, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodySetLinearVel(ODEBody, vel[0], vel[1], vel[2]);
}

void RigidBodyImp::SetRotation(const Matrix& rot, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMatrix3 ODEMatrix;
    GenericPhysicsMatrix& matrixRef = (GenericPhysicsMatrix&) ODEMatrix;
    ConvertRotationMatrix(rot, matrixRef);
    dBodySetRotation(ODEBody, ODEMatrix);
}

salt::Matrix RigidBodyImp::GetRotation(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* ODEMatrix = dBodyGetRotation(ODEBody);
    GenericPhysicsMatrix* matrixPtr = (GenericPhysicsMatrix*) ODEMatrix;
    salt::Matrix rot;
    ConvertRotationMatrix(matrixPtr,rot);
    return rot;
}

Vector3f RigidBodyImp::GetLocalAngularVelocity(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* vel = dBodyGetAngularVel(ODEBody);
    dReal w[3];
    dBodyVectorFromWorld(ODEBody, vel[0], vel[1], vel[2], w);
    return Vector3f(w[0], w[1], w[2]);
}

Vector3f RigidBodyImp::GetAngularVelocity(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* vel = dBodyGetAngularVel(ODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void RigidBodyImp::SetAngularVelocity(const Vector3f& vel, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodySetAngularVel(ODEBody, vel[0], vel[1], vel[2]);
}

salt::Matrix RigidBodyImp::GetSynchronisationMatrix(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* pos = dBodyGetPosition(ODEBody);
    const dReal* rot = dBodyGetRotation(ODEBody);
    
    Matrix mat;
    mat.m[0] = rot[0];
    mat.m[1] = rot[4];
    mat.m[2] = rot[8];
    mat.m[3] = 0;
    mat.m[4] = rot[1];
    mat.m[5] = rot[5];
    mat.m[6] = rot[9];
    mat.m[7] = 0;
    mat.m[8] = rot[2];
    mat.m[9] = rot[6];
    mat.m[10] = rot[10];
    mat.m[11] = 0;
    mat.m[12] = pos[0];
    mat.m[13] = pos[1];
    mat.m[14] = pos[2];
    mat.m[15] = 1;

    return mat;
}

RigidBody* RigidBodyImp::BodyGetData(long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    RigidBody* bodyPtr =
        static_cast<RigidBody*>(dBodyGetData(ODEBody));

    return bodyPtr;
}

void RigidBodyImp::AddForce(const Vector3f& force, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodyAddForce(ODEBody, force.x(), force.y(), force.z());
}

Vector3f RigidBodyImp::GetForce(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* f = dBodyGetForce(ODEBody);
    return Vector3f(f[0], f[1], f[2]);
}

void RigidBodyImp::AddTorque(const Vector3f& torque, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodyAddTorque(ODEBody, torque.x(), torque.y(), torque.z());
}

void RigidBodyImp::SetPosition(const Vector3f& pos, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dBodySetPosition(ODEBody, pos.x(), pos.y(), pos.z());
    // the parent node will be updated in the next physics cycle
}

Vector3f RigidBodyImp::GetPosition(long bodyID) const
{
    dBodyID ODEBody = (dBodyID) bodyID;
    const dReal* pos = dBodyGetPosition(ODEBody);
    return Vector3f(pos[0], pos[1], pos[2]);
}

void RigidBodyImp::TranslateMass(const Vector3f& v, long bodyID)
{
    dBodyID ODEBody = (dBodyID) bodyID;
    dMass m;
    dBodyGetMass(ODEBody, &m);
    dMassTranslate(&m,v[0],v[1],v[2]);
}

GenericMass& RigidBodyImp::CreateMass(float mass, salt::Vector3f cVector)
{
    dMass ODEMass;
    ODEMass.mass = mass;
    ODEMass.c[0] = cVector[0];
    ODEMass.c[1] = cVector[1];
    ODEMass.c[2] = cVector[2];
    
    GenericMass& massRef = (GenericMass&) ODEMass;
    return massRef;
}

void RigidBodyImp::SetInertiaTensorAt(int i, float value, GenericMass& mass)
{
    dMass& ODEMass = (dMass&) mass;
    ODEMass.I[i] = value;
}
