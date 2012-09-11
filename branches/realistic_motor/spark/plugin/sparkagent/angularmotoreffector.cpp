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
#include "angularmotoreffector.h"
#include "hingeaction.h"
#include <oxygen/physicsserver/rigidbody.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;


AngularMotorEffector::AngularMotorEffector()
    : JointEffector<AngularMotor>::JointEffector("amotor")
{
}

AngularMotorEffector::~AngularMotorEffector()
{
}

bool AngularMotorEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    // disabled joint?
    if ( mJoint->Disabled() )
    {
      boost::shared_ptr<RigidBody> body = mJoint->GetBody(Joint::BI_FIRST);
      if (body && !body->IsEnabled() )
      {
        body->Enable();
      }
      mJoint->SetParameter(2 /*value of dParamVel in ODE*/, 0);
      return false;
    }

    shared_ptr<HingeAction> motorAction = shared_dynamic_cast<HingeAction>(action);

    if (motorAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (HingeEffector) cannot realize an "
            << "unknown ActionObject\n";
        return false;
    }

    float finalMotorVel = motorAction->GetMotorVelocity();


    if (mJoint->IsLimitJointMaxSpeed1())
    {
        finalMotorVel = (finalMotorVel > 0) ? 
                        gMin(finalMotorVel, mJoint->GetJointMaxSpeed1()) 
                        : 
                        gMax(finalMotorVel, - mJoint->GetJointMaxSpeed1());
    }

    mJoint->SetParameter(2 /*value of dParamVel in ODE*/, finalMotorVel);

    if (motorAction->GetMotorVelocity() != 0)
        {
            boost::shared_ptr<RigidBody> body = mJoint->GetBody(Joint::BI_FIRST);
            if (body && !body->IsEnabled())
                {
                    body->Enable();
                }
        }
    return true;
}

shared_ptr<ActionObject> AngularMotorEffector::GetActionObject(const Predicate& predicate)
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

            /*
            float stiffness;
            if (! predicate.AdvanceValue(iter, stiffness))
                {
                    GetLog()->Error()
                        << "ERROR: (HingeEffector) motor stiffness expected\n";
                    break;
                }*/

            return shared_ptr<HingeAction>(new HingeAction(GetPredicate(),velocity));
        }

    return shared_ptr<ActionObject>();
}
