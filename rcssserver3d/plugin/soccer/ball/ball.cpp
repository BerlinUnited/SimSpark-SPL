/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/body.h>
#include <ballstateaspect/ballstateaspect.h>
#include "ball.h"

using namespace boost;
using namespace oxygen;

Ball::Ball() : Transform(), mForceTTL(0)
{
}

void
Ball::SetAcceleration(int steps, const salt::Vector3f& force,
                      const salt::Vector3f& torque,
                      boost::shared_ptr<oxygen::AgentAspect> agent)
{
    if (mForceTTL > 0 && mKickedLast == agent) return;

    mForceTTL = steps;

    mForce = force;
    mTorque = torque;
    mKickedLast = agent;

    if (mBody.get() == 0)
    {
        mBody = dynamic_pointer_cast<RigidBody>(GetChildOfClass("RigidBody"));
    }
}

void
Ball::PrePhysicsUpdateInternal(float deltaTime)
{
    Transform::PrePhysicsUpdateInternal(deltaTime);
    if (mBody.get() == 0 || mForceTTL <= 0) return;

    // the BallStateAspect is created after the ball, so we cannot set
    // mBallStateAspect during OnLink
    if (mBallStateAspect.get() == 0)
    {
        mBallStateAspect = dynamic_pointer_cast<BallStateAspect>
            (GetCore()->Get("/sys/server/gamecontrol/BallStateAspect"));
        if (mBallStateAspect.get() == 0) return;
    }

    mBody->AddForce(mForce);
    mBody->AddTorque(mTorque);
    mBallStateAspect->UpdateLastCollidingAgent(mKickedLast);

    --mForceTTL;
}
