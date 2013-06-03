/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2013 RoboCup Soccer Server 3D Maintenance Group

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

#include "gps.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/body.h>
#include <salt/vector.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;

GPS::GPS() : oxygen::Perceptor()
{
}

GPS::~GPS()
{
}

void 
GPS::OnLink()
{
    mTransformParent = shared_static_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());   
}

void 
GPS::OnUnlink()
{
    mTransformParent.reset();
}

bool
GPS::Percept(boost::shared_ptr<PredicateList> predList)
{
    Predicate &predicate = predList->AddPredicate();
    predicate.name = "GPS";
    predicate.parameter.Clear();
    
    ParameterList &nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("n"));
    nameElement.AddValue(GetName());       

    const Matrix& tf = mTransformParent->GetWorldTransform();

    ParameterList &tfElement = predicate.parameter.AddList();
    tfElement.AddValue(std::string("tf"));
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            tfElement.AddValue(tf(i, j));
        }
    }
    
    return true;
}
