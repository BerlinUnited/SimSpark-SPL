/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id$

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
#include <oxygen/physicsserver/hinge2joint.h>
#include <oxygen/physicsserver/ode/odehinge2joint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

Hinge2Joint::Hinge2Joint() : Generic6DOFJoint()
{
    mHinge2JointImp = boost::shared_ptr<ODEHinge2Joint>(new ODEHinge2Joint());
}

Hinge2Joint::~Hinge2Joint()
{
}

void Hinge2Joint::OnLink()
{
    long world = GetWorldID();
    if (world == 0)
        {
            return;
        }
    
    mJointID = mHinge2JointImp->CreateHinge2Joint(world);
}

void Hinge2Joint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);

    // relative universal hinge2 axis 1 points up
    Vector3f up(GetWorldTransform().Rotate(Vector3f(0,0,1)));

    // relative universal hinge2 axis 2 points right
    Vector3f right(GetWorldTransform().Rotate(Vector3f(1,0,0)));
    
    mHinge2JointImp->SetAnchor(gAnchor, up, right, mJointID);
}

Vector3f Hinge2Joint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                pos = mHinge2JointImp->GetAnchor1(mJointID);
            }

        case BI_SECOND:
            {
                pos = mHinge2JointImp->GetAnchor2(mJointID);
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

float Hinge2Joint::GetAngle(EAxisIndex idx)
{
    switch (idx)
        {
        case AI_FIRST:
            return mHinge2JointImp->GetAngle(mJointID);

        case AI_SECOND:
            GetLog()->Warning() <<
                "(Hinge2Joint) WARNING: GetAngle is undefined for EAxisIndex::AI_SECOND, returned zero\n";
            return 0;

        default:
            return 0;
        }
}

float Hinge2Joint::GetAngleRate(EAxisIndex idx)
{
    switch (idx)
        {
        case AI_FIRST:
            return mHinge2JointImp->GetAngleRate1(mJointID);

        case AI_SECOND:
            return mHinge2JointImp->GetAngleRate2(mJointID);

        default:
            return 0;
        }
}

void Hinge2Joint::SetParameter(int parameter, float value)
{
    mHinge2JointImp->SetParameter(parameter, value, mJointID);
}

float Hinge2Joint::GetParameter(int parameter) const
{
    return mHinge2JointImp->GetParameter(parameter, mJointID);
}
