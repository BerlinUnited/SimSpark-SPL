/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: hinge2perceptor.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "hinge2perceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

Hinge2Perceptor::Hinge2Perceptor()
    : JointPerceptor<Hinge2Joint>::JointPerceptor()
{
}

Hinge2Perceptor::~Hinge2Perceptor()
{
}

void Hinge2Perceptor::InsertAxisAngle(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("ax"));
    axisElement.AddValue(static_cast<int>(idx));
    axisElement.AddValue(mJoint->GetAngle(idx));
}

void Hinge2Perceptor::InsertAxisRate(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("rt"));
    axisElement.AddValue(static_cast<int>(idx));
    axisElement.AddValue(mJoint->GetAngleRate(idx));
}

bool Hinge2Perceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "H2J";
    predicate.parameter.Clear();

    ParameterList& nameElement = predicate.parameter.AddList();
    nameElement.AddValue(string("n"));
    nameElement.AddValue(GetName());

    InsertAxisAngle(predicate,Joint::AI_FIRST);
//    InsertAxisRate(predicate,Joint::AI_SECOND);

    return true;
}
