/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.cpp,v 1.1.2.6 2004/02/10 19:45:21 rollmark Exp $

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
#include "soccerruleaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/sceneserver/scene.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/ballstateaspect/ballstateaspect.h>
#include <soccer/agentstate/agentstate.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

SoccerRuleAspect::SoccerRuleAspect() : SoccerControlAspect()
{
}

SoccerRuleAspect::~SoccerRuleAspect()
{
}

void SoccerRuleAspect::MoveBall(const salt::Vector3f& pos)
{
    mBallBody->SetPosition(pos);
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
}

void SoccerRuleAspect::UpdateBeforeKickOff()
{
    // before the game starts the ball should stay in the middle of
    // the playing field
    salt::Vector3f pos(0,mBallRadius,0);
    MoveBall(pos);
}

void SoccerRuleAspect::UpdateKickOff()
{
    // after the first agent touches the ball move to PM_PLAYON
    shared_ptr<AgentAspect> agent;
    TTime time;
    if (mBallState->GetLastCollidingAgent(agent,time))
        {
            mGameState->SetPlayMode(PM_PlayOn);
        }
}

void SoccerRuleAspect::UpdateKickIn()
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
        {
            return;
        }

    // after the first agent touches the ball move to PM_PLAYON. the
    // time when the agent last touches the ball must be after the
    // change to the KickIn mode
    shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
        {
            return;
        }

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange)
        {
            mGameState->SetPlayMode(PM_PlayOn);
        } else
            {
                // move the ball back on the ground where it left the playing
                // field
                MoveBall(mLastValidBallPos);
            }
}

bool SoccerRuleAspect::CheckBallLeftField()
{
    if (mBallState->GetBallOnField())
        {
            return false;
        }

    // get the team of the last agent touching the ball and set the
    // correct kick in playmode
    shared_ptr<AgentAspect> agent;
    TTime time;

    if (mBallState->GetLastCollidingAgent(agent,time))
        {
            shared_ptr<AgentState> agentState;
            if (SoccerBase::GetAgentState(agent,agentState))
                {
                    mGameState->SetPlayMode
                        (
                         (agentState->GetTeamIndex() == TI_LEFT) ?
                         PM_KickIn_Right : PM_KickIn_Left
                         );

                    mLastValidBallPos =
                        mBallState->GetLastValidBallPosition();
                    mLastValidBallPos[1] = mBallRadius;
                }
        }

    return true;
}

bool SoccerRuleAspect::CheckGoal()
{
    // check if the ball is in one of the goals
    TTeamIndex idx = mBallState->GetGoalState();
    if (idx == TI_NONE)
        {
            return false;
        }

    // score the lucky team
    mGameState->ScoreTeam((idx == TI_LEFT) ? TI_RIGHT : TI_LEFT);
    mGameState->SetPlayMode((idx == TI_LEFT) ? PM_Goal_Right : PM_Goal_Left);

    return true;
}

void SoccerRuleAspect::UpdatePlayOn()
{
    // check if the ball is in one of the goals
    if (CheckGoal())
        {
            return;
        }

    // check if the ball is otherwise not on the playing field
    if (CheckBallLeftField())
        {
            return;
        }

    // other checks go here...
}

void SoccerRuleAspect::UpdateGoal()
{
    // check if the pause time after the goal has elapsed
    if (mGameState->GetModeTime() < mGoalPauseTime)
        {
            return;
        }

    // put the ball back in the middle of the playing field
    salt::Vector3f pos(0,0,0);
    SoccerBase::GetSoccerVar(*this,"BallRadius",pos[1]);
    MoveBall(pos);

    // kick off for the opposite team
    mGameState->SetPlayMode(
                            mGameState->GetPlayMode() == PM_Goal_Left ?
                            PM_KickOff_Right : PM_KickOff_Left
                            );
}

void SoccerRuleAspect::Update(float deltaTime)
{
    if (
        (mGameState.get() == 0) ||
        (mBallState.get() == 0) ||
        (mBallBody.get() == 0)
        )
        {
            return;
        }

    TPlayMode playMode = mGameState->GetPlayMode();

    switch (playMode)
        {
        case PM_BeforeKickOff:
            UpdateBeforeKickOff();
            break;

        case PM_PlayOn:
            UpdatePlayOn();
            break;

        case PM_KickOff_Left:
        case PM_KickOff_Right:
            UpdateKickOff();
            break;

        case PM_KickIn_Left:
        case PM_KickIn_Right:
            UpdateKickIn();
            break;

        case PM_Goal_Left:
        case PM_Goal_Right:
            UpdateGoal();
            break;

        default:
            GetLog()->Error()
                << "ERROR: (SoccerRuleAspect) unknown play mode "
                << playMode << "\n";
            break;
        }
}

void SoccerRuleAspect::OnLink()
{
    SoccerControlAspect::OnLink();

    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetControlAspect("GameStateAspect"));

    mBallState = shared_dynamic_cast<BallStateAspect>
        (GetControlAspect("BallStateAspect"));

    SoccerBase::GetBallBody(*this,mBallBody);

    mBallRadius = 0.111f;
    SoccerBase::GetSoccerVar(*this,"BallRadius",mBallRadius);

    mGoalPauseTime = 3;
    SoccerBase::GetSoccerVar(*this,"RuleGoalPauseTime",mGoalPauseTime);

    mKickInPauseTime = 1;
    SoccerBase::GetSoccerVar(*this,"RuleKickingPauseTime",mKickInPauseTime);
}

void SoccerRuleAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mGameState.reset();
    mBallState.reset();
    mBallBody.reset();
}


