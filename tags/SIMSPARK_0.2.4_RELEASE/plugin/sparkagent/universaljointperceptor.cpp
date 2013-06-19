/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Jan 4 2006
   Copyright (C) 2006 RoboCup Soccer Server 3D Maintenance Group

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
#include "universaljointperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

UniversalJointPerceptor::UniversalJointPerceptor()
    : JointPerceptor<UniversalJoint>::JointPerceptor()
{
}

UniversalJointPerceptor::~UniversalJointPerceptor()
{
}

void UniversalJointPerceptor::InsertAxisAngle(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    if (idx == Joint::AI_FIRST)
    {
        axisElement.AddValue(string("ax1"));
        axisElement.AddValue(mJoint->GetAngle(Joint::AI_FIRST));
    }
    else
    {
        axisElement.AddValue(string("ax2"));
        axisElement.AddValue(mJoint->GetAngle(Joint::AI_SECOND));
    }
}

void UniversalJointPerceptor::InsertAxisRate(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    if (idx == Joint::AI_FIRST)
    {
        axisElement.AddValue(string("rt1"));
        axisElement.AddValue(mJoint->GetAngleRate(Joint::AI_FIRST));
    }
    else
    {
        axisElement.AddValue(string("rt2"));
        axisElement.AddValue(mJoint->GetAngleRate(Joint::AI_SECOND));
    }
}

bool UniversalJointPerceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "UJ";
    predicate.parameter.Clear();

    ParameterList& nameElement = predicate.parameter.AddList();
    nameElement.AddValue(string("n"));
    nameElement.AddValue(GetName());

    InsertAxisAngle(predicate, Joint::AI_FIRST);
//    InsertAxisRate(predicate, Joint::AI_FIRST);
    InsertAxisAngle(predicate, Joint::AI_SECOND);
//    InsertAxisRate(predicate, Joint::AI_SECOND);

    return true;
}
