/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hingejoint.cpp,v 1.5 2004/05/01 11:30:31 rollmark Exp $

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
#include "hingejoint.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

HingeJoint::HingeJoint() : Joint()
{
}

HingeJoint::~HingeJoint()
{
}

void HingeJoint::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mODEJoint = dJointCreateHinge(world, 0);
}

void HingeJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor(GetWorldTransform() * anchor);
    dJointSetHingeAnchor (mODEJoint, gAnchor[0], gAnchor[1], gAnchor[2]);

    // calculate hinge axis (pos. z, relative to world transform)
    Vector3f up(GetWorldTransform().Rotate(Vector3f(0,0,1)));
    dJointSetHingeAxis(mODEJoint, up[0], up[1], up[2]);
}

Vector3f HingeJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                dReal anchor[3];
                dJointGetHingeAnchor (mODEJoint, anchor);
                pos = Vector3f(anchor[0],anchor[1],anchor[2]);
            }

        case BI_SECOND:
            {
                dReal anchor[3];
                dJointGetHingeAnchor2(mODEJoint, anchor);
                pos = Vector3f(anchor[0],anchor[1],anchor[2]);
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

float HingeJoint::GetAngle()
{
    return gRadToDeg(dJointGetHingeAngle(mODEJoint));
}

float HingeJoint::GetAngleRate()
{
    return gRadToDeg(dJointGetHingeAngleRate(mODEJoint));
}

void HingeJoint::SetParameter(int parameter, float value)
{
    dJointSetHingeParam(mODEJoint, parameter, value);
}

float HingeJoint::GetParameter(int parameter)
{
    return dJointGetHingeParam(mODEJoint, parameter);
}






