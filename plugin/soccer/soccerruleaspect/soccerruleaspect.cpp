/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.cpp,v 1.5 2004/04/21 09:41:29 fruit Exp $

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
#include <salt/random.h>
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
using salt::Vector2f;
using salt::Vector3f;

SoccerRuleAspect::SoccerRuleAspect() :
    mBallRadius(0.111),
    mGoalPauseTime(3),
    mKickInPauseTime(1),
    mHalfTime(2.25 * 60),
    mFreeKickDist(9.15),
    mFreeKickMoveDist(15.15)
{
}

SoccerRuleAspect::~SoccerRuleAspect()
{
}

void
SoccerRuleAspect::MoveBall(const Vector3f& pos)
{
    mBallBody->SetPosition(pos);
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
}

void
SoccerRuleAspect::MoveAgent(shared_ptr<Body> agent_body, const Vector3f& pos)
{
    agent_body->SetPosition(pos);
    agent_body->SetVelocity(Vector3f(0,0,0));
    agent_body->SetAngularVelocity(Vector3f(0,0,0));
}

void
SoccerRuleAspect::MoveAwayPlayers(const salt::Vector3f& pos, float radius,
                                  float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    std::list<boost::shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
        return;

    salt::BoundingSphere sphere(pos, radius);
    boost::shared_ptr<oxygen::Transform> transform_parent;
    boost::shared_ptr<oxygen::Body> agent_body;
    std::list<boost::shared_ptr<AgentState> >::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(*(*i), transform_parent);
        // call GetAgentBody with matching AgentAspect
        SoccerBase::GetAgentBody(transform_parent, agent_body);
        // if agent is too close, move it away
        Vector3f new_pos = agent_body->GetPosition();
        if (sphere.Contains(new_pos))
        {
            float dist = salt::UniformRNG<>(min_dist, min_dist + 2.0)();

            if (idx == TI_LEFT)
            {
                if (pos[0] - dist < -mFieldLength/2.0)
                {
                    new_pos[1] = pos[1] < 0 ? pos[1] + dist : pos[1] - dist;
                } else {
                    new_pos[0] = pos[0] - dist;
                }
            } else {
                if (pos[0] + dist > mFieldLength/2.0)
                {
                    new_pos[1] = pos[1] < 0 ? pos[1] + dist : pos[1] - dist;
                } else {
                    new_pos[0] = pos[0] + dist;
                }
            }
            new_pos[2] = 1.0;
            MoveAgent(agent_body, new_pos);
        }
    }
}

void
SoccerRuleAspect::MoveAwayPlayers(const salt::AABB2& box,
                                  float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    std::list<boost::shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
        return;

    boost::shared_ptr<oxygen::Transform> transform_parent;
    boost::shared_ptr<oxygen::Body> agent_body;

    std::list<boost::shared_ptr<AgentState> >::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(*(*i), transform_parent);
        // call GetAgentBody with matching AgentAspect
        SoccerBase::GetAgentBody(transform_parent, agent_body);
        // if agent is too close, move it away
        Vector3f new_pos = agent_body->GetPosition();
        if (box.Contains(Vector2f(new_pos[0], new_pos[1])))
        {
            if (idx == TI_LEFT)
            {
                new_pos[0] = box.minVec[0] -
                    salt::UniformRNG<>(min_dist, min_dist + 2.0)();
            } else {
                new_pos[0] = box.maxVec[0] +
                    salt::UniformRNG<>(min_dist, min_dist + 2.0)();
            }
            new_pos[2] = 1.0;
            MoveAgent(agent_body, new_pos);
        }
    }
}

void
SoccerRuleAspect::UpdateBeforeKickOff()
{
    // before the game starts the ball should stay in the middle of
    // the playing field
    Vector3f pos(0,0,mBallRadius);
    MoveBall(pos);
    MoveAwayPlayers(mRightHalf, 1.0, TI_LEFT);
    MoveAwayPlayers(mLeftHalf, 1.0, TI_RIGHT);
}

void
SoccerRuleAspect::UpdateKickOff(TTeamIndex idx)
{
    MoveAwayPlayers(mRightHalf, 1.0, TI_LEFT);
    MoveAwayPlayers(mLeftHalf, 1.0, TI_RIGHT);
    MoveAwayPlayers(Vector3f(0,0,0), mFreeKickDist, mFreeKickMoveDist,
                    SoccerBase::OpponentTeam(idx));
    // after the first agent touches the ball move to PM_PLAYON
    shared_ptr<AgentAspect> agent;
    TTime time;
    if (mBallState->GetLastCollidingAgent(agent,time))
    {
        mGameState->SetPlayMode(PM_PlayOn);
    }
}

void
SoccerRuleAspect::UpdateKickIn(TTeamIndex idx)
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    MoveAwayPlayers(mFreeKickPos, mFreeKickDist, mFreeKickMoveDist,
                    SoccerBase::OpponentTeam(idx));

    // after the first agent touches the ball move to PM_PLAY_ON. the
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
        MoveBall(mFreeKickPos);
    }
}

void
SoccerRuleAspect::UpdateGoalKick(TTeamIndex idx)
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    MoveAwayPlayers(idx == TI_LEFT ? mRightPenaltyArea : mLeftPenaltyArea,
                    1.0, SoccerBase::OpponentTeam(idx));

    // after the first agent touches the ball, we do nothing until
    // the ball leaves the penalty area.

    shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    // if the team with the goal kick touched the ball and the ball is
    // outside the penalty area, we switch to play on.
    if (time > lastChange)
    {
        Vector2f pos(mBallBody->GetPosition().x(),
                     mBallBody->GetPosition().y());
        if ((idx == TI_RIGHT && !mRightPenaltyArea.Contains(pos)) ||
            (idx == TI_LEFT && !mLeftPenaltyArea.Contains(pos)) ||
            (idx == TI_NONE))
        {
            // we have to handle the case where the team with the goal kick
            // scores a self goal, because the penalty areas contain the goal.
            if (!CheckBallLeftField())
                mGameState->SetPlayMode(PM_PlayOn);
        }
        return;
    }
    // the ball was not touched yet
    else
    {
        // move the ball back on the free kick position
        MoveBall(mFreeKickPos);
    }
}

void
SoccerRuleAspect::UpdateCornerKick(TTeamIndex idx)
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    MoveAwayPlayers(mFreeKickPos, mFreeKickDist, mFreeKickMoveDist,
                    SoccerBase::OpponentTeam(idx));

    // after the first agent touches the ball move to PM_PLAY_ON. the
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
        MoveBall(mFreeKickPos);
    }
}

bool
SoccerRuleAspect::CheckBallLeftField()
{
    if (mBallState->GetBallOnField())
    {
        return false;
    }

    // get the team of the last agent touching the ball and set the
    // correct kick in playmode
    shared_ptr<AgentAspect> agent;
    shared_ptr<AgentState> agentState;
    TTime time;

    if (mBallState->GetLastCollidingAgent(agent,time) &&
        SoccerBase::GetAgentState(agent,agentState))
    {
        /* Possibilities are:
           - Ball left field on the long sides: kick in for the other team
           - Ball left field on the short sides:
             - corner kick if the a team kicked the ball over its own side
             - goal kick if the a team kicked the ball over opponent side
           - [ The ball left the field through a hole in the floor or ceiling
               or some aliens have taken it away =:-]
               In this case put the ball back to the last valid position and
               we hope nobody sees.
               (this should not happen. really.  But it CAN happen depending
               on the ODE parameters you choose or if somebody is going to
               simulate the first contact (stardate 50893.5)).
        */
        Vector3f ball_pos = mBallBody->GetPosition();
        bool last_touch_left = (agentState->GetTeamIndex() == TI_LEFT);
        bool ball_left = (ball_pos[0] < 0);
        bool ball_up = (ball_pos[1] < 0);

        // handle corner / goal kick
        if (salt::gAbs(ball_pos.x()) >= mFieldLength / 2)
        {
            // check goal kick right team
            if (last_touch_left && !ball_left)
            {
                mFreeKickPos[0] = mFieldLength / 2 - 5.0;
                mFreeKickPos[1] = 0.0;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_GOAL_KICK_RIGHT);
            }
            // check goal kick left team
            else if (!last_touch_left && ball_left)
            {
                mFreeKickPos[0] = -mFieldLength / 2 + 5.0;
                mFreeKickPos[1] = 0.0;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_GOAL_KICK_LEFT);
            }
            // check corner kick right team
            else if (last_touch_left && ball_left)
            {
                mFreeKickPos[0] = -mFieldLength / 2 + 0.05;
                mFreeKickPos[1] = ball_pos[1] > 0 ?
                    mFieldWidth / 2 - 0.05 : -mFieldWidth / 2 + 0.05;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_CORNER_KICK_RIGHT);
            }
            // check corner kick left team
            else
            {
                mFreeKickPos[0] = mFieldLength / 2 - 0.05;
                mFreeKickPos[1] = ball_pos[1] > 0 ?
                    mFieldWidth / 2 - 0.05 : -mFieldWidth / 2 + 0.05;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_CORNER_KICK_LEFT);
            }
        }
        // handle kick in
        else if (salt::gAbs(ball_pos.y()) >= mFieldWidth / 2)
        {
            mFreeKickPos = mBallState->GetLastValidBallPosition();
            mFreeKickPos[1] = mFreeKickPos[1] > 0 ?
                mFieldWidth / 2 : -mFieldWidth / 2;
            mFreeKickPos[2] = mBallRadius;
            mGameState->SetPlayMode((agentState->GetTeamIndex() == TI_LEFT) ?
                                    PM_KickIn_Right : PM_KickIn_Left);
        }
        // we've got a situation here
        else {
            // this will stop the ball, but better than losing it away.
            MoveBall(mBallState->GetLastValidBallPosition());
            // no need to change the game state, we just don't know what
            // has been going on.
            return false;
        }
    }

    return true;
}

bool
SoccerRuleAspect::CheckGoal()
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

void
SoccerRuleAspect::UpdatePlayOn()
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

void
SoccerRuleAspect::UpdateGoal()
{
    // check if the pause time after the goal has elapsed
    if (mGameState->GetModeTime() < mGoalPauseTime)
    {
        return;
    }

    // put the ball back in the middle of the playing field
    Vector3f pos(0,0,mBallRadius);
    MoveBall(pos);

    // kick off for the opposite team
    mGameState->SetPlayMode(
        mGameState->GetPlayMode() == PM_Goal_Left ?
        PM_KickOff_Right : PM_KickOff_Left
        );
}

void
SoccerRuleAspect::CheckTime()
{
    TTime now = mGameState->GetTime();
    TGameHalf half = mGameState->GetGameHalf();

    if (
        (half == GH_FIRST) &&
        (now >= mHalfTime)
        )
    {
        // the first game half is over
        mGameState->SetPlayMode(PM_BeforeKickOff);
        mGameState->SetGameHalf(GH_SECOND);
    } else if (
        (half == GH_SECOND) &&
        (now >= 2 * mHalfTime)
        )
    {
        // the game is over
        mGameState->SetPlayMode(PM_GameOver);
    }
}

void
SoccerRuleAspect::Update(float deltaTime)
{
    if (
        (mGameState.get() == 0) ||
        (mBallState.get() == 0) ||
        (mBallBody.get() == 0)
        )
    {
        return;
    }

    CheckTime();

    TPlayMode playMode = mGameState->GetPlayMode();

    switch (playMode)
    {
    case PM_BeforeKickOff:
        // At the beginning of the match, we update the member variables
        // with the values from the ruby script (once). At this point in time,
        // the ruby script has definitely been processed.
        UpdateCached();
        // Below is the check we do during before kick off mode.
        UpdateBeforeKickOff();
        break;

    case PM_PlayOn:
        UpdatePlayOn();
        break;

    case PM_KickOff_Left:
        UpdateKickOff(TI_LEFT);
        break;
    case PM_KickOff_Right:
        UpdateKickOff(TI_RIGHT);
        break;

    case PM_KickIn_Left:
        UpdateKickIn(TI_LEFT);
        break;
    case PM_KickIn_Right:
        UpdateKickIn(TI_RIGHT);
        break;

    case PM_GOAL_KICK_LEFT:
        UpdateGoalKick(TI_LEFT);
        break;
    case PM_GOAL_KICK_RIGHT:
        UpdateGoalKick(TI_RIGHT);
        break;

    case PM_CORNER_KICK_LEFT:
        UpdateCornerKick(TI_LEFT);
        break;

    case PM_CORNER_KICK_RIGHT:
        UpdateCornerKick(TI_RIGHT);
        break;

    case PM_Goal_Left:
    case PM_Goal_Right:
        UpdateGoal();
        break;

    case PM_GameOver:
        break;

    default:
        GetLog()->Error()
            << "ERROR: (SoccerRuleAspect) unknown play mode "
            << playMode << "\n";
        break;
    }
}

void
SoccerRuleAspect::OnLink()
{
    SoccerControlAspect::OnLink();

    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetControlAspect("GameStateAspect"));

    mBallState = shared_dynamic_cast<BallStateAspect>
        (GetControlAspect("BallStateAspect"));

    SoccerBase::GetBallBody(*this,mBallBody);
}

void
SoccerRuleAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mGameState.reset();
    mBallState.reset();
    mBallBody.reset();
}


void
SoccerRuleAspect::UpdateCached()
{
    static bool updated = false;

    if (updated) return;

    updated = true;

    SoccerBase::GetSoccerVar(*this,"BallRadius",mBallRadius);
    SoccerBase::GetSoccerVar(*this,"RuleGoalPauseTime",mGoalPauseTime);
    SoccerBase::GetSoccerVar(*this,"RuleKickingPauseTime",mKickInPauseTime);
    SoccerBase::GetSoccerVar(*this,"RuleHalfTime",mHalfTime);
    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);
    SoccerBase::GetSoccerVar(*this,"FieldWidth",mFieldWidth);
    SoccerBase::GetSoccerVar(*this,"GoalWidth",mGoalWidth);
    SoccerBase::GetSoccerVar(*this,"FreeKickDistance",mFreeKickDist);

    // set up bounding boxes for halfs and goal areas

    // the right and the left half are intentionally oversized towards the sides and
    // the end of the field so that no opponents sneak up from behind.
    mRightHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                             Vector2f(mFieldLength/2.0 + 10, mFieldWidth/2.0 + 10.0));
    mLeftHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                            Vector2f(-mFieldLength/2.0 - 10, mFieldWidth/2.0 + 10.0));
    // the penalty areas are oversized towards the end of the field
    mRightPenaltyArea = salt::AABB2(Vector2f(mFieldLength/2.0 - 16.5, -16.5 - mGoalWidth/2.0),
                                    Vector2f(mFieldLength/2.0 + 10, 16.5 + mGoalWidth/2.0));
    mLeftPenaltyArea = salt::AABB2(Vector2f(mFieldLength/2.0 - 16.5, -16.5 - mGoalWidth/2.0),
                                   Vector2f(mFieldLength/2.0 + 10, 16.5 + mGoalWidth/2.0));
}

