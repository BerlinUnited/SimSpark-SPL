/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "ballstateaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <gamestateaspect/gamestateaspect.h>
#include <soccerbase/soccerbase.h>
#include <ball/ball.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

BallStateAspect::BallStateAspect() : SoccerControlAspect()
{
    mBallOnField = false;
    mLastValidBallPos = Vector3f(0,0,0);
    mGoalState = TI_NONE;
    mLastAgentCollisionTime = 0;
}

BallStateAspect::~BallStateAspect()
{
}

bool BallStateAspect::GetLastCollidingAgent(shared_ptr<AgentAspect>& agent,
                                            TTime& time)
{
    agent = mLastCollidingAgent;
    time  = mLastAgentCollisionTime;

    return (agent.get() != 0);
}

bool BallStateAspect::GetLastKickingAgent(shared_ptr<AgentAspect>& agent,
                                            TTime& time)
{
    agent = mLastKickingAgent;
    time  = mLastAgentKickTime;

    return (agent.get() != 0);
}

void BallStateAspect::UpdateLastCollidingAgent()
{
    // get a list of agents that collided with the ball since the last
    // update of the recorder and remember the first returned node as
    // the last agent that collided with the ball.
    RecorderHandler::TParentList agents;
    mBallRecorder->FindParentsSupportingClass<AgentAspect>(agents);

    if (agents.size() > 0)
        {
            mLastCollidingAgent = shared_static_cast<AgentAspect>
                (agents.front().lock());

            mLastAgentCollisionTime = mGameState->GetTime();
        }

    // empty the recorder buffer
    mBallRecorder->Clear();
}

void
BallStateAspect::UpdateLastCollidingAgent(boost::shared_ptr<AgentAspect> agent)
{
    mLastCollidingAgent = agent;
    mLastAgentCollisionTime = mGameState->GetTime();
}

void
BallStateAspect::UpdateLastKickingAgent(boost::shared_ptr<AgentAspect> agent)
{
    mLastKickingAgent = agent;
    mLastAgentKickTime = mGameState->GetTime();
}

void BallStateAspect::UpdateBallOnField()
{
    const Vector3f& posBall = mBall->GetWorldTransform().Pos();
    mBallOnField = ( gAbs(posBall.x()) < mHalfFieldLength + mBallRadius + 0.001 )
        && ( gAbs(posBall.y()) < mHalfFieldWidth + mBallRadius + 0.001 );
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
    // check both goal box collider
    RecorderHandler::TParentList ball;
    mLeftGoalRecorder->FindParentsSupportingClass<Ball>(ball);

    if (! ball.empty())
        {
            mGoalState = TI_LEFT;
        } else
            {
                mRightGoalRecorder->FindParentsSupportingClass<Ball>(ball);

                if (! ball.empty())
                    {
                        mGoalState = TI_RIGHT;
                    } else
                        {
                            mGoalState = TI_NONE;
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
    if (
        (mBall.get() == 0) ||
        (mBallRecorder.get() == 0) ||
        (mLeftGoalRecorder.get() == 0) ||
        (mRightGoalRecorder.get() == 0)
        )
        {
            return;
        }

    UpdateLastCollidingAgent();
    UpdateBallOnField();
    UpdateLastValidBallPos();
    UpdateGoalState();
}

void BallStateAspect::OnLink()
{
    SoccerControlAspect::OnLink();
    
    SoccerBase::GetBall(*this,mBall);
    mBallRecorder = GetBallRecorder();
    mLeftGoalRecorder = GetLeftGoalRecorder();
    mRightGoalRecorder = GetRightGoalRecorder();

    GetControlAspect(mGameState, "GameStateAspect");

    float fieldWidth = 64.0f;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",fieldWidth);
    mHalfFieldWidth = fieldWidth * 0.5f;

    float fieldLength = 100.0f;
    SoccerBase::GetSoccerVar(*this,"FieldLength",fieldLength);
    mHalfFieldLength = fieldLength * 0.5f;
    
    SoccerBase::GetSoccerVar(*this,"BallRadius",mBallRadius);
}

void BallStateAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mBallRecorder.reset();
    mLastCollidingAgent.reset();
    mLeftGoalRecorder.reset();
    mRightGoalRecorder.reset();
    mGameState.reset();
}

bool BallStateAspect::GetBallOnField()
{
    return mBallOnField;
}

salt::Vector3f BallStateAspect::GetLastValidBallPosition()
{
    return mLastValidBallPos;
}
