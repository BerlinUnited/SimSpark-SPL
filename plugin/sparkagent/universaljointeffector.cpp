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

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

UniversalJointEffector::UniversalJointEffector() : Effector()
{
    SetName("universaljoint");
}

UniversalJointEffector::~UniversalJointEffector()
{
}

void UniversalJointEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (mAction.get() == 0 || mJoint.get() == 0)
        {
            return;
        }

    shared_ptr<UniversalJointAction> universalAction =
        shared_dynamic_cast<UniversalJointAction>(mAction);
    mAction.reset();

    if (universalAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (UniversalJointtEffector) cannot realize an "
            << "unknown ActionObject\n";
        return;
    }

    // check if the angle out of range
    float minAng = mJoint->GetLowStopDeg(Joint::AI_FIRST);
    float maxAng = mJoint->GetHighStopDeg(Joint::AI_FIRST);
    float currentAng = mJoint->GetAngle(Joint::AI_FIRST);
    float vel = universalAction->GetMotorVelocity(Joint::AI_FIRST);
    if ( gInRange(currentAng, minAng, maxAng) && gInRange(vel,-9000.0f,9000.0f) )
    {
        mJoint->SetParameter(dParamVel, vel);
    }
    else
    {
        mJoint->SetParameter(dParamVel, 0);
    }

    minAng = mJoint->GetLowStopDeg(Joint::AI_SECOND);
    maxAng = mJoint->GetHighStopDeg(Joint::AI_SECOND);
    currentAng = mJoint->GetAngle(Joint::AI_SECOND);
     vel = universalAction->GetMotorVelocity(Joint::AI_SECOND);
    if ( gInRange(currentAng, minAng, maxAng) && gInRange(vel,-9000.0f,9000.0f) )
    {
        mJoint->SetParameter(dParamVel2, vel );
    }
    else
    {
        mJoint->SetParameter(dParamVel2, 0);
    }
}

shared_ptr<ActionObject> UniversalJointEffector::GetActionObject(const Predicate& predicate)
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

            return shared_ptr<UniversalJointAction>(new  UniversalJointAction(GetPredicate(),velocity1,velocity2));
        }

    return shared_ptr<ActionObject>();
}

void UniversalJointEffector::OnLink()
{
    mJoint = make_shared(FindParentSupportingClass<UniversalJoint>());

    if (mJoint.get() == 0)
        {
            GetLog()->Error()
                << "(UniversalJointEffector) ERROR: found no UniversalJoint parent\n";
        } else {
            mJoint->SetParameter(dParamFudgeFactor, 0.8);
            mJoint->SetParameter(dParamFudgeFactor2, 0.8);
        }
}

void UniversalJointEffector::OnUnlink()
{
    mJoint.reset();
}
