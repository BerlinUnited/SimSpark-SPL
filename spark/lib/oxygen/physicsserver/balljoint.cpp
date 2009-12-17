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
#include <oxygen/physicsserver/ode/odeballjoint.h>
#include <oxygen/physicsserver/balljoint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

BallJoint::BallJoint() : Generic6DOFJoint()
{
    mBallJointImp = boost::shared_ptr<ODEBallJoint>(new ODEBallJoint());
}

BallJoint::~BallJoint()
{
}

void BallJoint::OnLink()
{
    long world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mBallJointImp->CreateBallJoint(world);
    mODEJoint = dJointCreateBall((dWorldID) world, 0);
}

void BallJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor = GetWorldTransform() * anchor;
    mBallJointImp->SetAnchor(gAnchor);
}

Vector3f BallJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                pos = mBallJointImp->GetAnchor1();
                break;
            }

        case BI_SECOND:
            {
                pos = mBallJointImp->GetAnchor2();
                break;
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

void BallJoint::SetParameter(int /*parameter*/, float /*value*/)
{
    GetLog()->Error() <<
        "(BallJoint) WARNING: SetParameter function undefined for BallJoint, ignored\n";
}

float BallJoint::GetParameter(int /*parameter*/) const
{
    GetLog()->Error() <<
        "(BallJoint) WARNING: GetParameter function undefined for BallJoint, returned zero\n";
    return 0;
}
