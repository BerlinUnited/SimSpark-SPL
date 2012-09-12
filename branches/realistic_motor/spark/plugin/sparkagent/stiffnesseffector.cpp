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

#include "stiffnesseffector.h"
#include "stiffnessaction.h"

using namespace std;
using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;


StiffnessEffector::StiffnessEffector()
    : JointEffector<AngularMotor>::JointEffector("amotorStiffness")
{
}

StiffnessEffector::~StiffnessEffector()
{
}

bool StiffnessEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mJoint.get() == 0) return false;

    shared_ptr<StiffnessAction> stiffnessAction = shared_dynamic_cast<StiffnessAction>(action);

    if (stiffnessAction.get() == 0)
    {
        GetLog()->Error()
                << "ERROR: (StiffnessEffector) cannot realize an "
                << "unknown ActionObject\n";
        return false;
    }

    float stiffness = stiffnessAction->GetStiffness();
    stiffness = gClamp(stiffness, 0.0f, 1.0f);
    mJoint->SetStiffness(stiffness);
    return true;
}

shared_ptr<ActionObject> StiffnessEffector::GetActionObject(const Predicate& predicate)
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


        float stiffness;
        if (! predicate.AdvanceValue(iter, stiffness))
        {
            GetLog()->Error()
                    << "ERROR: (HingeEffector) motor stiffness expected\n";
            break;
        }

        return shared_ptr<StiffnessAction>(new StiffnessAction(GetPredicate(),stiffness));
    }

    return shared_ptr<ActionObject>();
}
