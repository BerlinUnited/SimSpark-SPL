/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ballstateaspect.cpp,v 1.1.2.5 2004/02/06 10:17:23 rollmark Exp $

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
#include "ballstateaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/ball/ball.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

BallStateAspect::BallStateAspect() : SoccerControlAspect()
{
    mBallOnField = false;
    mLastValidBallPos = Vector3f(0,0,0);
    mGoalState = TI_NONE;
}

BallStateAspect::~BallStateAspect()
{
}

shared_ptr<AgentAspect> BallStateAspect::GetLastCollidingAgent()
{
    return mLastCollidingAgent;
}

void BallStateAspect::UpdateLastCollidingAgent()
{
    if (mBallRecorder.get() == 0)
        {
            return;
        }

    // get a list of agents that collided with the ball since the last
    // update of the recorder and remember the first returned node as
    // the last agent that collided with the ball.
    RecorderHandler::TParentList agents;
    mBallRecorder->GetParentsSupportingClass("AgentAspect",agents);

    if (agents.size() > 0)
        {
            mLastCollidingAgent = shared_static_cast<AgentAspect>
                (make_shared(agents.front()));
        }

    // empty the recorder buffer
    mBallRecorder->Clear();
}

void BallStateAspect::UpdateBallOnField()
{
    if (mFieldRecorder.get() == 0)
        {
            return;
        }

    // get a list of Ball nodes that collided with the field
    RecorderHandler::TParentList ball;
    mFieldRecorder->GetParentsSupportingClass("Ball",ball);

    // the ball is on or above the playing field iff it collided with
    // the box collider around the playing field
    mBallOnField = (ball.size() > 0);

    // empty the recorder buffer
    mFieldRecorder->Clear();
}

void BallStateAspect::UpdateLastValidBallPos()
{
    if (! mBallOnField)
        {
            return;
        }

    mLastValidBallPos = mBall->GetWorldTransform().Pos();
}

void BallStateAspect::UpdateGoalState()
{
    if (
        (mBallOnField) ||
        (mLeftGoalRecorder.get() == 0) ||
        (mRightGoalRecorder.get() == 0)
        )
        {
            mGoalState = TI_NONE;
            return;
        }

    // check both goal box collider
    RecorderHandler::TParentList agents;
    mLeftGoalRecorder->GetParentsSupportingClass("AgentAspect",agents);

    if (agents.size())
        {
            mGoalState = TI_LEFT;
        } else
            {
                agents.clear();
                mRightGoalRecorder->GetParentsSupportingClass("AgentAspect",agents);

                if (agents.size())
                    {
                        mGoalState = TI_RIGHT;
                    }
            }

    mLeftGoalRecorder->Clear();
    mRightGoalRecorder->Clear();
}

TTeamIndex BallStateAspect::GetGoalState()
{
    return mGoalState;
}

void BallStateAspect::Update(float deltaTime)
{
    UpdateLastCollidingAgent();
    UpdateBallOnField();
    UpdateLastValidBallPos();
    UpdateGoalState();
}

void BallStateAspect::OnLink()
{
    SoccerControlAspect::OnLink();

    mFieldRecorder = GetFieldRecorder();
    SoccerBase::GetBall(*this,mBall);
    mBallRecorder = GetBallRecorder();
    mLeftGoalRecorder = GetLeftGoalRecorder();
    mRightGoalRecorder = GetRightGoalRecorder();
}

void BallStateAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mBallRecorder.reset();
    mFieldRecorder.reset();
    mLastCollidingAgent.reset();
    mLeftGoalRecorder.reset();
    mRightGoalRecorder.reset();
}

bool BallStateAspect::GetBallOnField()
{
    return mBallOnField;
}

salt::Vector3f BallStateAspect::GetLastValidBallPosition()
{
    return mLastValidBallPos;
}


