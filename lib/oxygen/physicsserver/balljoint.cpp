/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: balljoint.cpp,v 1.2 2004/04/07 13:30:44 rollmark Exp $

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
#include "balljoint.h"

using namespace oxygen;
using namespace boost;
using namespace salt;

BallJoint::BallJoint() : Joint()
{
}

BallJoint::~BallJoint()
{
}

void BallJoint::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    dJointCreateBall(world, 0);
}

void BallJoint::SetBallAnchor(salt::Vector3f anchor)
{
    dJointSetBallAnchor (mODEJoint, anchor[0], anchor[1], anchor[2]);
}

Vector3f BallJoint::GetBallAnchor(EBodyIndex idx)
{
    switch (idx)
        {
        case BI_FIRST:
            {
                dReal anchor[3];
                dJointGetBallAnchor (mODEJoint, anchor);
                return Vector3f(anchor[0],anchor[1],anchor[2]);
            }

        case BI_SECOND:
            {
                dReal anchor[3];
                dJointGetBallAnchor2(mODEJoint, anchor);
                return Vector3f(anchor[0],anchor[1],anchor[2]);
            }

        default:
            return Vector3f(0,0,0);
        }
}



