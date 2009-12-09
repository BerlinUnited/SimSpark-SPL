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
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/ode/oderigidbody.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;
using namespace std;

ODERigidBody::ODERigidBody() : ODEBody(), mODEBody(0){
    mBodyID = 0;
    mMassTrans = Vector3f(0,0,0);
    mMassTransformed = false;
}

long ODERigidBody::GetBodyID(){
    return mBodyID;
}  

salt::Vector3f ODERigidBody::GetMassTrans(){
    return mMassTrans;
}

void ODERigidBody::SetMassTrans(Vector3f massTrans){
    mMassTrans = massTrans;
}

bool ODERigidBody::GetMassTransformed(){
    return mMassTransformed;
}

void ODERigidBody::SetMassTransformed(bool f){
    mMassTransformed = f;
}

void ODERigidBody::Enable()
{
    dBodyEnable(mODEBody);
}

void ODERigidBody::Disable()
{
    dBodyDisable(mODEBody);
}

bool ODERigidBody::IsEnabled() const
{
    return (dBodyIsEnabled(mODEBody) != 0);
}

void ODERigidBody::UseGravity(bool f)
{
    if (f == true)
        {
            // body is affected by gravity
            dBodySetGravityMode(mODEBody, 1);
        }
    else
        {
            // body is not affected by gravity
            dBodySetGravityMode(mODEBody, 0);
        }
}

bool ODERigidBody::UsesGravity() const{
    return (dBodyGetGravityMode(mODEBody) != 0);
}

void ODERigidBody::CreateBody(long world)
{
    // create the managed body
    mODEBody = dBodyCreate((dWorldID) world);
    mBodyID = (long) mODEBody; 
}

void ODERigidBody::DestroyPhysicsObject(){
    if (mBodyID == 0)
        {
            return;
        }

    dBodyDestroy(mODEBody);
    mBodyID = 0;
}

void ODERigidBody::BodySetData(RigidBody* rb)
{
    dBodySetData(mODEBody, rb);
}

void ODERigidBody::SetMass(float mass)
{
    dMass ODEMass;
    dBodyGetMass(mODEBody, &ODEMass);
    dMassAdjust(&ODEMass, mass);
    dBodySetMass(mODEBody, &ODEMass);
}

float ODERigidBody::GetMass() const
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    return m.mass;
}

void ODERigidBody::AddMass(const dMass& ODEMass, const Matrix& matrix)
{
    dMass transMass(ODEMass);

    dMatrix3 ODEMatrix;
    void* matrixPtr = (void*) &ODEMatrix;
    ConvertRotationMatrix(matrix, matrixPtr);
    dMassRotate(&transMass, ODEMatrix);

    const Vector3f& trans(matrix.Pos());
    dMassTranslate(&transMass,trans[0],trans[1],trans[2]);

    dMassTranslate(&transMass,mMassTrans[0],mMassTrans[1],mMassTrans[2]);
    
    dMass bodyMass;
    dBodyGetMass(mODEBody, &bodyMass);
    dMassAdd(&bodyMass, &transMass);

    /** ODE currently requires that the center mass is always in the
        origin of the body
    */
    Vector3f trans2(bodyMass.c[0], bodyMass.c[1], bodyMass.c[2]);

    dMassTranslate(&bodyMass, -trans2[0], -trans2[1], -trans2[2]);
    bodyMass.c[0] = bodyMass.c[1] = bodyMass.c[2] = 0.0f;
    dBodySetMass(mODEBody, (const dMass*)&bodyMass);

    // Move body so mass is at right position again
    SetPosition(GetPosition() + trans2);
    
    // Keep track of total translation of mass
    mMassTrans = mMassTrans - trans2;
    
    mMassTransformed = true;
}

void ODERigidBody::SetMassParameters(const float& mass)
{
    dMass& ODEMass = (dMass&) mass;
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::PrepareSphere(dMass& mass, float density, float radius) const
{
    dMassSetSphere(&mass, density, radius);
}

void ODERigidBody::SetSphere(float density, float radius)
{
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddSphere(float density, float radius, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareSphere(ODEMass, density, radius);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareSphereTotal(dMass& mass, float total_mass, float radius) const
{
    dMassSetSphereTotal(&mass, total_mass, radius);
}

void ODERigidBody::SetSphereTotal(float total_mass, float radius)
{
    dMass ODEMass;;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddSphereTotal(float total_mass, float radius, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareSphereTotal(ODEMass, total_mass, radius);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareBox(dMass& mass, float density, const Vector3f& size) const
{
    dMassSetBox(&mass, density, size[0], size[1], size[2]);
}

void ODERigidBody::SetBox(float density, const Vector3f& size)
{
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddBox(float density, const Vector3f& size, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareBox(ODEMass, density, size);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareBoxTotal(dMass& mass, float total_mass, const Vector3f& size) const
{
    dMassSetBoxTotal(&mass, total_mass, size[0], size[1], size[2]);
}

void ODERigidBody::SetBoxTotal(float total_mass, const Vector3f& size)
{
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddBoxTotal(float total_mass, const Vector3f& size, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareBoxTotal(ODEMass, total_mass, size);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareCylinder (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCylinder (&mass, density, direction, radius, length);
}

void ODERigidBody::SetCylinder (float density, float radius, float length)
{
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddCylinder (float density, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCylinder(ODEMass, density, radius, length);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareCylinderTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;
;
    dMassSetCylinderTotal(&mass, total_mass, direction, radius, length);
}

void ODERigidBody::SetCylinderTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddCylinderTotal(float total_mass, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCylinderTotal(ODEMass, total_mass, radius, length);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareCapsule (dMass& mass, float density, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsule (&mass, density, direction, radius, length);
}

void ODERigidBody::SetCapsule (float density, float radius, float length)
{
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddCapsule (float density, float radius, float length, const Matrix& matrix)
{
    dMass ODEMass;
    PrepareCapsule(ODEMass, density, radius, length);
    AddMass(ODEMass, matrix);
}

void ODERigidBody::PrepareCapsuleTotal(dMass& mass, float total_mass, float radius, float length) const
{
    // direction: (1=x, 2=y, 3=z)
    int direction = 3;

    dMassSetCapsuleTotal(&mass, total_mass, direction, radius, length);
}

void ODERigidBody::SetCapsuleTotal(float total_mass, float radius, float length)
{
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    dBodySetMass(mODEBody, &ODEMass);
}

void ODERigidBody::AddCapsuleTotal(float total_mass, float radius, float length, const salt::Matrix& matrix)
{
    dMass ODEMass;
    PrepareCapsuleTotal(ODEMass, total_mass, radius, length);
    AddMass(ODEMass, matrix);
}

Vector3f ODERigidBody::GetVelocity() const
{
    const dReal* vel = dBodyGetLinearVel(mODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void ODERigidBody::SetVelocity(const Vector3f& vel)
{
    dBodySetLinearVel(mODEBody, vel[0], vel[1], vel[2]);
}

void ODERigidBody::SetRotation(const Matrix& rot)
{
    dMatrix3 ODEMatrix;
    void* matrixPtr = (void*) &ODEMatrix;
    ConvertRotationMatrix(rot, matrixPtr);
    dBodySetRotation(mODEBody, ODEMatrix);
}

salt::Matrix ODERigidBody::GetRotation() const
{
    const dReal* m = dBodyGetRotation(mODEBody);
    salt::Matrix rot;
    ConvertRotationMatrix(m,rot);
    return rot;
}

Vector3f ODERigidBody::GetAngularVelocity() const
{
    const dReal* vel = dBodyGetAngularVel(mODEBody);
    return Vector3f(vel[0], vel[1], vel[2]);
}

void ODERigidBody::SetAngularVelocity(const Vector3f& vel)
{
    dBodySetAngularVel(mODEBody, vel[0], vel[1], vel[2]);
}

salt::Matrix ODERigidBody::GetSynchronisationMatrix()
{
    const dReal* pos = dBodyGetPosition(mODEBody);
    const dReal* rot = dBodyGetRotation(mODEBody);
    
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

RigidBody* ODERigidBody::BodyGetData(long bodyID)
{
    dBodyID ODEBodyID = (dBodyID) bodyID;
    RigidBody* bodyPtr =
        static_cast<RigidBody*>(dBodyGetData(ODEBodyID));

    return bodyPtr;
}

void ODERigidBody::AddForce(const Vector3f& force)
{
    dBodyAddForce(mODEBody, force.x(), force.y(), force.z());
}

void ODERigidBody::AddTorque(const Vector3f& torque)
{
    dBodyAddTorque(mODEBody, torque.x(), torque.y(), torque.z());
}

void ODERigidBody::SetPosition(const Vector3f& pos)
{
    dBodySetPosition(mODEBody, pos.x(), pos.y(), pos.z());
    // the parent node will be updated in the next physics cycle
}

Vector3f ODERigidBody::GetPosition() const
{
    const dReal* pos = dBodyGetPosition(mODEBody);
    return Vector3f(pos[0], pos[1], pos[2]);
}

void ODERigidBody::TranslateMass(const Vector3f& v)
{
    dMass m;
    dBodyGetMass(mODEBody, &m);
    dMassTranslate(&m,v[0],v[1],v[2]);
}
