/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: universaljoint.cpp,v 1.4 2004/05/01 11:30:31 rollmark Exp $

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
#include "universaljoint.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

UniversalJoint::UniversalJoint() : Joint()
{
}

UniversalJoint::~UniversalJoint()
{
}

void UniversalJoint::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mODEJoint = dJointCreateUniversal(world, 0);
}

void UniversalJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    dJointSetUniversalAnchor (mODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);

    // relative universal axis 1 points up
    Vector3f up(GetWorldTransform().Rotate(Vector3f(0,0,1)));
    dJointSetUniversalAxis1(mODEJoint,up[0],up[1],up[2]);

    // relative universal axis 2 points right
    Vector3f right(GetWorldTransform().Rotate(Vector3f(1,0,0)));
    dJointSetUniversalAxis2(mODEJoint,right[0],right[1],right[2]);
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
            }

        case BI_SECOND:
            {
                dReal anchor[3];
                dJointGetUniversalAnchor2(mODEJoint, anchor);
                pos = Vector3f(anchor[0],anchor[1],anchor[2]);
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

float UniversalJoint::GetAngle(EAxisIndex idx)
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

float UniversalJoint::GetAngleRate(EAxisIndex idx)
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

float UniversalJoint::GetParameter(int parameter)
{
    return dJointGetUniversalParam(mODEJoint, parameter);
}








