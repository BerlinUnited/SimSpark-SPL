/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

#include "oxygen/physicsserver/world.h"
#include "oxygen/physicsserver/rigidbody.h"
#include "accelerometer.h"
#include <oxygen/sceneserver/transform.h>

// using namespace kerosin;
using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

Accelerometer::Accelerometer() : oxygen::Perceptor()
{
}

Accelerometer::~Accelerometer()
{
}

void Accelerometer::OnLink()
{
    boost::shared_ptr<Transform> transformParent = static_pointer_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());

    mBody = static_pointer_cast<const RigidBody>
        (transformParent->GetChildOfClass("RigidBody"));

    mGravity = mBody->GetWorld()->GetGravity();
    mLastVel = mBody->GetVelocity();
    mAcc.Zero();
}

bool Accelerometer::Percept(boost::shared_ptr<PredicateList> predList)
{
    Predicate &predicate = predList->AddPredicate();
    predicate.name = "ACC";
    predicate.parameter.Clear();

    ParameterList &nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("n"));
    nameElement.AddValue(GetName());

    ParameterList &ratesElement = predicate.parameter.AddList();
    ratesElement.AddValue(std::string("a"));
    ratesElement.AddValue(mAcc.x());
    ratesElement.AddValue(mAcc.y());
    ratesElement.AddValue(mAcc.z());

    return true;
}

void Accelerometer::PrePhysicsUpdateInternal(float deltaTime)
{
//    Vector3f F = mBody->GetForce();
//    float mass = mBody->GetMass();
//    mAcc = F / mass - mGravity;

    // calculate the acceleration according to velocity, it is a bit noise
    Vector3f vel = mBody->GetVelocity();
    Vector3f acc = (vel - mLastVel) / deltaTime;
    mLastVel = vel;
    acc -= mGravity;

    Matrix invRot = mBody->GetRotation();
    invRot.InvertRotationMatrix();
    mAcc = invRot * acc;
}
