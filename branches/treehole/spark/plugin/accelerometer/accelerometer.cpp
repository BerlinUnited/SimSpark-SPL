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


#include "oxygen/physicsserver/body.h"


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
    shared_ptr<Transform> transformParent = shared_static_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());

    mBody = shared_static_cast<const Body>
        (transformParent->GetChildOfClass("Body"));

    mGravity = mBody->GetWorld()->GetGravity();
    mLastVel = mBody->GetVelocity();
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

    Vector3f vel = mBody->GetVelocity();
    Vector3f acc = (vel - mLastVel) / deltaTime;
    acc -= mGravity;

    Matrix invRot = mBody->GetRotation();
    invRot.InvertRotationMatrix();
    acc = invRot * acc;

    float k = 0.9;
    mAcc = k*mAcc + (1-k)*acc;
    mLastVel = vel;
}
