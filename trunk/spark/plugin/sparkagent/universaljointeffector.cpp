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
#include "universaljointeffector.h"
#include "universaljointaction.h"
#include <oxygen/physicsserver/rigidbody.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

UniversalJointEffector::UniversalJointEffector()
    : JointEffector<UniversalJoint>::JointEffector("universaljoint")
{
}

UniversalJointEffector::~UniversalJointEffector()
{
}

bool UniversalJointEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    boost::shared_ptr<UniversalJointAction> universalAction =
        shared_dynamic_cast<UniversalJointAction>(action);

    if (universalAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (UniversalJointtEffector) cannot realize an "
            << "unknown ActionObject\n";
        return false;
    }

    float finalMotorVel1 = universalAction->GetMotorVelocity(Joint::AI_FIRST);
    float finalMotorVel2 = universalAction->GetMotorVelocity(Joint::AI_SECOND);

    if (mJoint->IsLimitJointMaxSpeed1())
    {
        finalMotorVel1 = (finalMotorVel1 > 0) ? 
                        gMin(finalMotorVel1, mJoint->GetJointMaxSpeed1()) 
                        : 
                        gMax(finalMotorVel1, - mJoint->GetJointMaxSpeed1());
    }

    if (mJoint->IsLimitJointMaxSpeed2())
    {
        finalMotorVel2 = (finalMotorVel2 > 0) ? 
                        gMin(finalMotorVel2, mJoint->GetJointMaxSpeed2()) 
                        : 
                        gMax(finalMotorVel2, - mJoint->GetJointMaxSpeed2());
    }

    mJoint->SetParameter(2 /*value of ODE's dParamVel*/, finalMotorVel1);
    mJoint->SetParameter(258 /*value of ODE's dParamVel2*/, finalMotorVel2);

    if (universalAction->GetMotorVelocity(Joint::AI_FIRST) != 0
            || universalAction->GetMotorVelocity(Joint::AI_SECOND) != 0)
        {
            boost::shared_ptr<RigidBody> body = mJoint->GetBody(Joint::BI_FIRST);
            if (body && !body->IsEnabled())
                {
                    body->Enable();
                }
        }

    return true;
}

boost::shared_ptr<ActionObject> UniversalJointEffector::GetActionObject(const Predicate& predicate)
{
    for(;;)
        {
            if (mJoint.get() == 0)
                {
                    break;
                }

            if (predicate.name != GetPredicate())
                {
                    GetLog()->Error()
                        << "ERROR: (UniversalJointEffector) invalid predicate"
                        << predicate.name << "\n";
                    break;
                }

            Predicate::Iterator iter = predicate.begin();

            float velocity1;
            float velocity2;

            if (! predicate.AdvanceValue(iter, velocity1))
                {
                    GetLog()->Error()
                        << "ERROR: (UniversalJointEffector) motor velocity1 expected\n";
                    break;
                }
            if (! predicate.AdvanceValue(iter, velocity2))
                {
                    GetLog()->Error()
                        << "ERROR: (UniversalJointEffector) motor velocity2 expected\n";
                    break;
                }

            return boost::shared_ptr<UniversalJointAction>(new  UniversalJointAction(GetPredicate(),velocity1,velocity2));
        }

    return boost::shared_ptr<ActionObject>();
}
