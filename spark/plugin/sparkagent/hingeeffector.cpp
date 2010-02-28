/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Wed Nov 9 2005
   Copyright (C) 2005 RoboCup Soccer Server 3D Maintenance Group

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
#include "hingeeffector.h"
#include "hingeaction.h"
#include <oxygen/physicsserver/rigidbody.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

HingeEffector::HingeEffector()
    : JointEffector<HingeJoint>::JointEffector("hinge")
{
}

HingeEffector::~HingeEffector()
{
}

bool HingeEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    boost::shared_ptr<HingeAction> hingeAction =
        shared_dynamic_cast<HingeAction>(action);

    if (hingeAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (HingeEffector) cannot realize an "
            << "unknown ActionObject\n";
        return false;
    }

    float finalMotorVel = hingeAction->GetMotorVelocity();

    if (mJoint->IsLimitJointMaxSpeed1())
    {
        finalMotorVel = (finalMotorVel > 0) ? 
                        gMin(finalMotorVel, mJoint->GetJointMaxSpeed1()) 
                        : 
                        gMax(finalMotorVel, - mJoint->GetJointMaxSpeed1());
    }

    mJoint->SetParameter(2 /*value of dParamVel in ODE*/, finalMotorVel);

    if (hingeAction->GetMotorVelocity() != 0)
        {
            boost::shared_ptr<RigidBody> body = mJoint->GetBody(Joint::BI_FIRST);
            if (body && !body->IsEnabled())
                {
                    body->Enable();
                }
        }

    return true;
}

boost::shared_ptr<ActionObject> HingeEffector::GetActionObject(const Predicate& predicate)
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
                        << "ERROR: (HingeEffector) invalid predicate"
                        << predicate.name << "\n";
                    break;
                }

            Predicate::Iterator iter = predicate.begin();

            float velocity;
            if (! predicate.AdvanceValue(iter, velocity))
                {
                    GetLog()->Error()
                        << "ERROR: (HingeEffector) motor velocity expected\n";
                    break;
                }

            return boost::shared_ptr<HingeAction>(new HingeAction(GetPredicate(),velocity));
        }

    return boost::shared_ptr<ActionObject>();
}
