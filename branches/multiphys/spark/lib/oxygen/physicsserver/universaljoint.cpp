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
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/physicsserver/ode/odeuniversaljoint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

UniversalJoint::UniversalJoint() : Generic6DOFJoint()
{
    mUniversalJointImp = boost::shared_ptr<ODEUniversalJoint>(new ODEUniversalJoint());
}

UniversalJoint::~UniversalJoint()
{
}

void UniversalJoint::OnLink()
{
    long world = GetWorldID();

    if (world == 0)
    {
        return;
    }

    mJointID = mUniversalJointImp->CreateUniversalJoint(world);
}

void UniversalJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    mUniversalJointImp->SetAnchor(gAnchor, mJointID);
}

Vector3f UniversalJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
    {
    case BI_FIRST:
    {
        pos = mUniversalJointImp->GetAnchor1(mJointID);
        break;
    }

    case BI_SECOND:
    {
        pos = mUniversalJointImp->GetAnchor2(mJointID);
        break;
    }

    default:
        break;
    }

    return GetLocalPos(pos);
}

void UniversalJoint::SetAxis1(const Vector3f & axis)
{
    Vector3f first(GetWorldTransform().Rotate(axis));
    mUniversalJointImp->SetAxis1(first, mJointID);
}

void UniversalJoint::SetAxis2(const Vector3f & axis)
{
    Vector3f second(GetWorldTransform().Rotate(axis));
    mUniversalJointImp->SetAxis2(second, mJointID);
}

Vector3f UniversalJoint::GetAxis(EAxisIndex idx) const
{
    Vector3f vec(0,0,0);

    switch (idx)
    {
    case AI_FIRST:
    {
        vec = mUniversalJointImp->GetAxis1(mJointID);
        break;
    }

    case AI_SECOND:
    {
        vec = mUniversalJointImp->GetAxis2(mJointID);
        break;
    }

    default:
        break;
    }

    return GetLocalPos(vec);
}

float UniversalJoint::GetAngle(EAxisIndex idx) const
{
    switch (idx)
    {
    case AI_FIRST:
        return mUniversalJointImp->GetAngle1(mJointID);

    case AI_SECOND:
        return mUniversalJointImp->GetAngle2(mJointID);

    default:
        return 0;
    }
}

float UniversalJoint::GetAngleRate(EAxisIndex idx) const
{
    switch (idx)
        {
        case AI_FIRST:
            return mUniversalJointImp->GetAngleRate1(mJointID);

        case AI_SECOND:
            return mUniversalJointImp->GetAngleRate2(mJointID);

        default:
            return 0;
        }
}

void UniversalJoint::SetParameter(int parameter, float value){
    mJointImp->SetParameter(parameter, value, mJointID);
}
