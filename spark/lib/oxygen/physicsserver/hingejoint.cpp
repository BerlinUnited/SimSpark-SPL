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
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/ode/odehingejoint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

HingeJoint::HingeJoint() : Generic6DOFJoint()
{
    mHingeJointImp = boost::shared_ptr<ODEHingeJoint>(new ODEHingeJoint());
}

HingeJoint::~HingeJoint()
{
}

void HingeJoint::OnLink()
{
    long world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mJointID = mHingeJointImp->CreateHingeJoint(world);
}

void HingeJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    mHingeJointImp->SetAnchor(gAnchor, mJointID);
}

Vector3f HingeJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                pos = mHingeJointImp->GetAnchor1(mJointID);
                break;
            }

        case BI_SECOND:
            {
                pos = mHingeJointImp->GetAnchor2(mJointID);
                break;
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

void HingeJoint::SetAxis(EAxisIndex idx)
{
    switch(idx)
    {
    case AI_FIRST:
    {
        // calculate hinge axis (pos. x, relative to world transform)
        SetAxis(Vector3f(1,0,0));
        break;
    }
    case AI_SECOND:
    {
        // calculate hinge axis (pos. y, relative to world transform)
        SetAxis(Vector3f(0,1,0));
        break;
    }
    case AI_THIRD:
    {
        // calculate hinge axis (pos. z, relative to world transform)
        SetAxis(Vector3f(0,0,1));
        break;
    }
    default:
        break;
    }
}

void HingeJoint::SetAxis(const Vector3f& axis)
{
    Vector3f absAxis(GetWorldTransform().Rotate(axis));
    mHingeJointImp->SetAxis(absAxis, mJointID);
}

Vector3f HingeJoint::GetAxis()
{
    return mHingeJointImp->GetAxis(mJointID);
}

float HingeJoint::GetAngle() const
{
    return mHingeJointImp->GetAngle(mJointID);
}

float HingeJoint::GetAngleRate() const
{
    return mHingeJointImp->GetAngleRate(mJointID);
}

void HingeJoint::SetParameter(int parameter, float value){
    mJointImp->SetParameter(parameter, value, mJointID);
}
