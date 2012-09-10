/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group

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
#include "angularmotorperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

AngularMotorPerceptor::AngularMotorPerceptor()
    : JointPerceptor<AngularMotor>::JointPerceptor(),
      mAngleRng(-0.005, 0.005)//-0.04395
{
}

AngularMotorPerceptor::~AngularMotorPerceptor()
{
}

void AngularMotorPerceptor::InsertAxisAngle(Predicate& predicate)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("ax"));
    float noise = mAngleRng();
    float ang = mJoint->GetAxisAngle(Joint::AI_FIRST);// + noise;
    axisElement.AddValue(ang);
}

void AngularMotorPerceptor::InsertAxisRate(Predicate& predicate)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("rt"));
    axisElement.AddValue(mJoint->GetAxisAngleRate(Joint::AI_FIRST));
}

void AngularMotorPerceptor::InsertAxisTorque(Predicate& predicate)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("tq"));
    axisElement.AddValue(mJoint->GetTorque());
}

void AngularMotorPerceptor::InsertCurrent(Predicate& predicate)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("ct"));
    axisElement.AddValue(mJoint->GetCurrent() * 1000);
}

void AngularMotorPerceptor::InsertTempeature(Predicate& predicate)
{
    ParameterList& axisElement = predicate.parameter.AddList();
    axisElement.AddValue(string("tp"));
    axisElement.AddValue(mJoint->GetTempeature());
}

bool AngularMotorPerceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "HJ";
    predicate.parameter.Clear();

    ParameterList& nameElement = predicate.parameter.AddList();
    nameElement.AddValue(string("n"));
    nameElement.AddValue(GetName());

    InsertAxisAngle(predicate);
    //InsertAxisRate(predicate);
    //InsertAxisTorque(predicate);
    InsertTempeature(predicate);
    InsertCurrent(predicate);
    
    return true;
}
