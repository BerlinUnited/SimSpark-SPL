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

UniversalJointPerceptor::UniversalJointPerceptor() : Perceptor()
{
}

UniversalJointPerceptor::~UniversalJointPerceptor()
{
}

void UniversalJointPerceptor::OnLink()
{
    mJoint = make_shared(FindParentSupportingClass<UniversalJoint>());

    if (mJoint.get() == 0)
        {
            GetLog()->Error()
                << "(UniversalJointPerceptor) ERROR: found no UniversalJoint parent\n";
        }

}

void UniversalJointPerceptor::OnUnlink()
{
    mJoint.reset();
}

void UniversalJointPerceptor::InsertAxisAngle(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("axis"));
    axisElement.AddValue(mJoint->GetAngle(idx));
}

void UniversalJointPerceptor::InsertAxisRate(Predicate& predicate, Joint::EAxisIndex idx)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("rate"));
    axisElement.AddValue(mJoint->GetAngleRate(idx));
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
    nameElement.AddValue(string("name"));
    nameElement.AddValue(GetName());

    InsertAxisAngle(predicate, Joint::AI_FIRST);
    InsertAxisRate(predicate, Joint::AI_FIRST);
    InsertAxisAngle(predicate, Joint::AI_SECOND);
    InsertAxisRate(predicate, Joint::AI_SECOND);

    return true;
}


