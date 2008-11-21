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

#include "gyrorateperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/body.h>
#include <salt/vector.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;

GyroRatePerceptor::GyroRatePerceptor() : oxygen::Perceptor()
{
}

GyroRatePerceptor::~GyroRatePerceptor()
{
}

void 
GyroRatePerceptor::OnLink()
{
    shared_ptr<Transform> transformParent = shared_static_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());

    mBody = shared_static_cast<Body>
        (transformParent->GetChildOfClass("Body"));     
}

void 
GyroRatePerceptor::OnUnlink()
{
    mBody.reset();
}

bool
GyroRatePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    Predicate &predicate = predList->AddPredicate();
    predicate.name = "GYR";
    predicate.parameter.Clear();
    
    ParameterList &nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("n"));
    nameElement.AddValue(GetName());       

    Matrix invRot = mBody->GetRotation();
    invRot.InvertRotationMatrix();
    Vector3f rate = invRot * mBody->GetAngularVelocity();

    ParameterList &ratesElement = predicate.parameter.AddList();    
    ratesElement.AddValue(std::string("rt"));
    ratesElement.AddValue(gRadToDeg(rate.x()));
    ratesElement.AddValue(gRadToDeg(rate.y()));
    ratesElement.AddValue(gRadToDeg(rate.z()));

   // What should be done when yrotate is around 90? in that case, the parameters of the atan2 are 0!
//    const dReal* q = dBodyGetQuaternion(mBody->GetODEBody());
//    float xrotate = gArcTan2(2*(q[0]*q[1]+q[2]*q[3]), 1-2*(q[1]*q[1]+q[2]*q[2]));
//    float yrotate = gArcSin(2*(q[0]*q[2] - q[3]*q[1]));
//    float zrotate = gArcTan2(2*(q[0]*q[3] + q[1]*q[2]), 1-2*(q[2]*q[2]+q[3]*q[3]));
//    
//    ParameterList & anglesElement = predicate.parameter.AddList();    
//    anglesElement.AddValue(std::string("ang"));
//    anglesElement.AddValue(gRadToDeg(xrotate));
//    anglesElement.AddValue(gRadToDeg(yrotate));
//    anglesElement.AddValue(gRadToDeg(zrotate));
    
    return true;
}
