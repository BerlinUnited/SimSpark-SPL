/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hinge2joint.cpp,v 1.5 2007/05/31 08:06:31 jboedeck Exp $

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
#include "hinge2joint.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

Hinge2Joint::Hinge2Joint() : Joint()
{
}

Hinge2Joint::~Hinge2Joint()
{
}

void Hinge2Joint::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mODEJoint = dJointCreateHinge2(world, 0);
}

void Hinge2Joint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    dJointSetHinge2Anchor (mODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);

    // relative universal hinge2 axis 1 points up
    Vector3f up(GetWorldTransform().Rotate(Vector3f(0,0,1)));
    dJointSetHinge2Axis1(mODEJoint,up[0],up[1],up[2]);

    // relative universal hinge2 axis 2 points right
    Vector3f right(GetWorldTransform().Rotate(Vector3f(1,0,0)));
    dJointSetHinge2Axis2(mODEJoint,right[0],right[1],right[2]);
}

Vector3f Hinge2Joint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                dReal anchor[3];
                dJointGetHinge2Anchor (mODEJoint, anchor);
                pos = Vector3f(anchor[0],anchor[1],anchor[2]);
                break;
            }

        case BI_SECOND:
            {
                dReal anchor[3];
                dJointGetHinge2Anchor2(mODEJoint, anchor);
                pos = Vector3f(anchor[0],anchor[1],anchor[2]);
                break;
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
            return gRadToDeg(dJointGetHinge2Angle1(mODEJoint));

        case AI_SECOND:
            // dJointGetHinge2Angle2 is undeclared in ODE 0.039
            // return dJointGetHinge2Angle2(mODEJoint);
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
            return gRadToDeg(dJointGetHinge2Angle1Rate(mODEJoint));

        case AI_SECOND:
            return gRadToDeg(dJointGetHinge2Angle2Rate(mODEJoint));

        default:
            return 0;
        }
}

void Hinge2Joint::SetParameter(int parameter, float value)
{
    dJointSetHinge2Param(mODEJoint, parameter, value);
}

float Hinge2Joint::GetParameter(int parameter) const
{
    return dJointGetHinge2Param(mODEJoint, parameter);
}





