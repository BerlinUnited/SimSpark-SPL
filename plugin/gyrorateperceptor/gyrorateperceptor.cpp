/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gyrorateperceptor.cpp,v 1.2 2007/05/16 14:23:44 jboedeck Exp $

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

bool
GyroRatePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name = "GYR";
    predicate.parameter.Clear();
    
    ParameterList & nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("name"));
    nameElement.AddValue(GetName());

    shared_ptr<Transform> transformParent = shared_static_cast<Transform>
        (GetParentSupportingClass("Transform").lock());

    shared_ptr<Body> body = shared_static_cast<Body>
        (transformParent->GetChildOfClass("Body"));    

    Vector3f rate = body->GetAngularVelocity();

    ParameterList & ratesElement = predicate.parameter.AddList();    
    ratesElement.AddValue(rate.x());
    ratesElement.AddValue(rate.y());
    ratesElement.AddValue(rate.z());

    return true;
}
