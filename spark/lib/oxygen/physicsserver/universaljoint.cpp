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

    mODEJoint = dJointCreateUniversal((dWorldID) world, 0);
}

void UniversalJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    dJointSetUniversalAnchor (mODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);
}

Vector3f UniversalJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
    {
    case BI_FIRST:
    {
        dReal anchor[3];
        dJointGetUniversalAnchor (mODEJoint, anchor);
        pos = Vector3f(anchor[0],anchor[1],anchor[2]);
        break;
    }

    case BI_SECOND:
    {
        dReal anchor[3];
        dJointGetUniversalAnchor2(mODEJoint, anchor);
        pos = Vector3f(anchor[0],anchor[1],anchor[2]);
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
    dJointSetUniversalAxis1(mODEJoint,first[0],first[1],first[2]);
}

void UniversalJoint::SetAxis2(const Vector3f & axis)
{
    Vector3f second(GetWorldTransform().Rotate(axis));
    dJointSetUniversalAxis2(mODEJoint,second[0],second[1],second[2]);
}

Vector3f UniversalJoint::GetAxis(EAxisIndex idx) const
{
    Vector3f vec(0,0,0);

    switch (idx)
    {
    case AI_FIRST:
    {
        dReal axis[3];
        dJointGetUniversalAxis1(mODEJoint, axis);
        vec = Vector3f(axis[0],axis[1],axis[2]);
        break;
    }

    case AI_SECOND:
    {
        dReal axis[3];
        dJointGetUniversalAxis2(mODEJoint, axis);
        vec = Vector3f(axis[0],axis[1],axis[2]);
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
        return gRadToDeg(dJointGetUniversalAngle1(mODEJoint));

    case AI_SECOND:
        return gRadToDeg(dJointGetUniversalAngle2(mODEJoint));

    default:
        return 0;
    }
}

float UniversalJoint::GetAngleRate(EAxisIndex idx) const
{
    switch (idx)
        {
        case AI_FIRST:
            return gRadToDeg(dJointGetUniversalAngle1Rate(mODEJoint));

        case AI_SECOND:
            return gRadToDeg(dJointGetUniversalAngle2Rate(mODEJoint));

        default:
            return 0;
        }
}

void UniversalJoint::SetParameter(int parameter, float value)
{
    dJointSetUniversalParam(mODEJoint, parameter, value);
}

float UniversalJoint::GetParameter(int parameter) const
{
    return dJointGetUniversalParam(mODEJoint, parameter);
}
