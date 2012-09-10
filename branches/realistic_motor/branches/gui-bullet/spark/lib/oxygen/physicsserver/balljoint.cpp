/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: balljoint.cpp 180 2010-02-28 07:05:31Z a-held $

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

#include <oxygen/physicsserver/int/balljointint.h>
#include <oxygen/physicsserver/balljoint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

boost::shared_ptr<BallJointInt> BallJoint::mBallJointImp;

BallJoint::BallJoint() : Generic6DOFJoint()
{

}

BallJoint::~BallJoint()
{
}

void BallJoint::OnLink()
{
    Joint::OnLink();
    
    if (mBallJointImp.get() == 0)
        mBallJointImp = shared_dynamic_cast<BallJointInt>
            (GetCore()->New("BallJointImp"));

    long world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mJointID = mBallJointImp->CreateBallJoint(world);
}

void BallJoint::SetAnchor(const Vector3f& anchor)
{
    // calculate anchor position in world coordinates
    Vector3f gAnchor = GetWorldTransform() * anchor;
    mBallJointImp->SetAnchor(gAnchor, mJointID);
}

Vector3f BallJoint::GetAnchor(EBodyIndex idx)
{
    Vector3f pos(0,0,0);

    switch (idx)
        {
        case BI_FIRST:
            {
                pos = mBallJointImp->GetAnchor1(mJointID);
                break;
            }

        case BI_SECOND:
            {
                pos = mBallJointImp->GetAnchor2(mJointID);
                break;
            }

        default:
            break;
        }

    return GetLocalPos(pos);
}

void BallJoint::SetParameter(int parameter, float value){
    GetLog()->Warning()
        << "(BallJoint) WARNING: SetParameter undefined for BallJoint, call ignored";
}

float BallJoint::GetParameter(int parameter) const{
    GetLog()->Warning()
        << "(BallJoint) WARNING: SetParameter undefined for BallJoint, call ignored";
        
    return 0.0f;
}
