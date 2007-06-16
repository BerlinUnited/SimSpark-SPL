/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.cpp,v 1.29 2007/06/16 15:10:14 jboedeck Exp $

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
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/ballstateaspect/ballstateaspect.h>
#include <soccer/agentstate/agentstate.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using salt::Vector2f;
using salt::Vector3f;

SoccerRuleAspect::SoccerRuleAspect() :
    mBallRadius(0.111),
    mGoalPauseTime(3),
    mKickInPauseTime(1),
    mHalfTime(2.25 * 60),
    mFreeKickDist(9.15),
    mFreeKickMoveDist(15.15),
    mAutomaticKickOff(false),
    mWaitBeforeKickOff(1.0),
    mSingleHalfTime(false),
    mSayMsgSize(20),
    mAudioCutDist(50.0),
    mUseOffside(true),
    mFirstCollidingAgent(true),
    mNotOffside(false),
    mLastModeWasPlayOn(false)
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
SoccerRuleAspect::MoveAgent(shared_ptr<Transform> agent_aspect, const Vector3f& pos)
{
    SoccerBase::MoveAgent(agent_aspect, pos);
}

void
SoccerRuleAspect::ClearPlayers(const salt::Vector3f& pos, float radius,
                               float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;

    std::list<boost::shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
        return;

    salt::BoundingSphere sphere(pos, radius);
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    std::list<boost::shared_ptr<AgentState> >::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);

        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();

        Vector3f test_pos = new_pos;
        test_pos[2] = pos[2];   
    
        // DEBUG
        //cerr << "testing position (" << test_pos[0] << "," << test_pos[1] << ")" << endl;    

        if (sphere.Contains(test_pos))
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
            MoveAgent(agent_aspect, new_pos);
        }
    }
#if 0
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
#endif
}

void
SoccerRuleAspect::ClearPlayers(const salt::AABB2& box,
                               float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    std::list<boost::shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
        return;

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    std::list<boost::shared_ptr<AgentState> >::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);

        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();

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
            MoveAgent(agent_aspect, new_pos);
        }
    }
#if 0
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
#endif
}

void
SoccerRuleAspect::DropBall()
{
    DropBall(mBallBody->GetPosition());
}

void
SoccerRuleAspect::DropBall(Vector3f pos)
{
    salt::Vector2f ball_pos(pos.x(), pos.y());

    // we do not drop the ball within the penalty area
    if (mLeftPenaltyArea.Contains(ball_pos))
    {
        pos[0] = mLeftPenaltyArea.maxVec[0];
        pos[1] = pos.y() < 0 ?
            mLeftPenaltyArea.minVec[1] : mLeftPenaltyArea.maxVec[1];
    }
    else if (mRightPenaltyArea.Contains(ball_pos))
    {
        pos[0] = mRightPenaltyArea.minVec[0];
        pos[1] = pos.y() < 0 ?
            mRightPenaltyArea.minVec[1] : mRightPenaltyArea.maxVec[1];
    }

    MoveBall(pos);

    ClearPlayers(pos, mFreeKickDist, mFreeKickMoveDist, TI_LEFT);
    ClearPlayers(pos, mFreeKickDist, mFreeKickMoveDist, TI_RIGHT);

    mGameState->SetPlayMode(PM_PlayOn);
}

void
SoccerRuleAspect::UpdateBeforeKickOff()
{
    // get game control server to check agent count
    shared_ptr<GameControlServer> game_control = shared_dynamic_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));

    if (game_control.get() == 0)
    {
        GetLog()->Error() << "(SoccerRuleAspect) Error: can't get GameControlServer.\n";
        return;
    }
    
    // if no players are connected, just return
    if (! game_control->GetAgentCount()) return;


    // before the game starts the ball should stay in the middle of
    // the playing field
    Vector3f pos(0,0,mBallRadius);
    MoveBall(pos);

    ClearPlayers(mRightHalf, 1.0, TI_LEFT);
    ClearPlayers(mLeftHalf, 1.0, TI_RIGHT);

#if 0
    //
    // TODO: this has to be tested (compiles and no crashes at least)
    mInOffsideLeftPlayers.clear();
    mInOffsideRightPlayers.clear();
#endif

    if (mAutomaticKickOff && mGameState->GetModeTime() > mWaitBeforeKickOff)
    {
        mGameState->KickOff();
    }
}

void
SoccerRuleAspect::UpdateKickOff(TTeamIndex idx)
{
    ClearPlayers(mRightHalf, 1.0, TI_LEFT);
    ClearPlayers(mLeftHalf, 1.0, TI_RIGHT);
    ClearPlayers(Vector3f(0,0,0), mFreeKickDist, mFreeKickMoveDist,
                 SoccerBase::OpponentTeam(idx));

    // if no player touched the ball for mDropBallTime, we move away
    // all players and set the play mode to play on
    if (mDropBallTime > 0 &&
        mGameState->GetModeTime() > mDropBallTime)
    {
        // Drop the ball at its current position.
        // This should always be (0,0) during kickoff.
        DropBall(mBallBody->GetPosition());
        return;
    }

    // after the first agent touches the ball move to PM_PLAYON
    shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        return;
    }
    if (time > mGameState->GetLastModeChange())
    {
        mGameState->SetPlayMode(PM_PlayOn);
    }
}

void
SoccerRuleAspect::UpdateKickIn(TTeamIndex idx)
{
#if 1
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    ClearPlayers(mFreeKickPos, mFreeKickDist, mFreeKickMoveDist,
                 SoccerBase::OpponentTeam(idx));

    // if no player touched the ball for mDropBallTime, we move away
    // all players and set the play mode to play on
    if (mDropBallTime > 0 &&
        mGameState->GetModeTime() > mDropBallTime)
    {
        DropBall(mFreeKickPos);
        return;
    }

    // after the first agent touches the ball move to PM_PLAY_ON. the
    // time when the agent last touches the ball must be after the
    // change to the KickIn mode
    shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "no agent collided yet\n";
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange)
    {
        mGameState->SetPlayMode(PM_PlayOn);
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "Set Playmode to playon\n";
    } else
    {
        // move the ball back on the ground where it left the playing
        // field
        MoveBall(mFreeKickPos);
    }
#endif
}

void
SoccerRuleAspect::UpdateGoalKick(TTeamIndex idx)
{
#if 1
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    ClearPlayers(idx == TI_LEFT ? mLeftPenaltyArea : mRightPenaltyArea,
                 1.0, SoccerBase::OpponentTeam(idx));

    // if no player touched the ball for mDropBallTime, we move away
    // all players and set the play mode to play on
    if (mDropBallTime > 0 &&
        mGameState->GetModeTime() > mDropBallTime)
    {
        DropBall(mFreeKickPos);
        return;
    }

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
//            if (!mBallState->GetBallOnField())
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
#endif
}

void
SoccerRuleAspect::UpdateCornerKick(TTeamIndex idx)
{
#if 1
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    ClearPlayers(mFreeKickPos, mFreeKickDist, mFreeKickMoveDist,
                 SoccerBase::OpponentTeam(idx));

    // if no player touched the ball for mDropBallTime, we move away
    // all players and set the play mode to play on
    if (mDropBallTime > 0 &&
        mGameState->GetModeTime() > mDropBallTime)
    {
        DropBall(mFreeKickPos);
        return;
    }

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
#endif
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

#if 0
    // check if the players are in offside
    if (mUseOffside && CheckOffside())
    {
        return;
    }
#endif

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
    // Original
//     mGameState->SetPlayMode(
//         mGameState->GetPlayMode() == PM_Goal_Left ?
//         PM_KickOff_Right : PM_KickOff_Left
//         );

    // kick off for the opposite team
    mGameState->KickOff(
        mGameState->GetPlayMode() == PM_Goal_Left ?
        TI_RIGHT : TI_LEFT
        );
}

void
SoccerRuleAspect::UpdateGameOver()
{
    // wait for 10 seconds to finish
    if (mGameState->GetModeTime() < 9)
    {
        return;
    }
    mGameState->Finish();

    if (mGameState->GetModeTime() >= 10)
    {
        boost::shared_ptr<GameControlServer> gameControlServer =
            shared_dynamic_cast<GameControlServer>(GetCore()->Get("/sys/server/gamecontrol"));
        gameControlServer->Quit();
    }
}

void
SoccerRuleAspect::CheckTime()
{
    TTime now = mGameState->GetTime();
    TGameHalf half = mGameState->GetGameHalf();

    if ((half == GH_FIRST) && (now >= mHalfTime))
    {
        if (mSingleHalfTime)
        {
            // we want to play only one half of the match
            mGameState->SetPlayMode(PM_GameOver);
        } else {
            // the first game half is over
            mGameState->SetPlayMode(PM_BeforeKickOff);
            mGameState->SetGameHalf(GH_SECOND);
        }
    }
    else if ((half == GH_SECOND) && (now >= 2 * mHalfTime))
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

    static bool updated = false;

    mLastModeWasPlayOn = false;

    switch (playMode)
    {
    case PM_BeforeKickOff:
        // At the beginning of the match, we update the member variables
        // with the values from the ruby script (once). At this point in time,
        // the ruby script has definitely been processed.
        if (! updated)
        {
            UpdateCachedInternal();
            updated = true;
        }
        // Below is the check we do during before kick off mode.
        UpdateBeforeKickOff();
        break;

    case PM_PlayOn:
        UpdatePlayOn();
        mLastModeWasPlayOn = true;
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

    case PM_OFFSIDE_LEFT:
        UpdateOffside(TI_LEFT);
        break;
    case PM_OFFSIDE_RIGHT:
        UpdateOffside(TI_RIGHT);
        break;

    case PM_GameOver:
        UpdateGameOver();
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

    if (mGameState.get() == NULL)
    {
        GetLog()->Error() << "(SoccerRuleAspect) ERROR: could not get GameStateAspect\n";
    }

    mBallState = shared_dynamic_cast<BallStateAspect>
        (GetControlAspect("BallStateAspect"));

    if (mBallState.get() == NULL)
    {
        GetLog()->Error() << "(SoccerRuleAspect) ERROR: could not get BallStateAspect\n";
    }

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
SoccerRuleAspect::UpdateCachedInternal()
{
    SoccerBase::GetSoccerVar(*this,"BallRadius",mBallRadius);
    SoccerBase::GetSoccerVar(*this,"RuleGoalPauseTime",mGoalPauseTime);
    SoccerBase::GetSoccerVar(*this,"RuleKickInPauseTime",mKickInPauseTime);
    SoccerBase::GetSoccerVar(*this,"RuleHalfTime",mHalfTime);
    SoccerBase::GetSoccerVar(*this,"RuleDropBallTime",mDropBallTime);
    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);
    SoccerBase::GetSoccerVar(*this,"FieldWidth",mFieldWidth);
    SoccerBase::GetSoccerVar(*this,"GoalWidth",mGoalWidth);
    SoccerBase::GetSoccerVar(*this,"FreeKickDistance",mFreeKickDist);
    SoccerBase::GetSoccerVar(*this,"AutomaticKickOff",mAutomaticKickOff);
    SoccerBase::GetSoccerVar(*this,"WaitBeforeKickOff",mWaitBeforeKickOff);
    SoccerBase::GetSoccerVar(*this,"SingleHalfTime",mSingleHalfTime);
    SoccerBase::GetSoccerVar(*this,"UseOffside",mUseOffside);

    // set up bounding boxes for halfs and goal areas

    // the right and the left half are intentionally oversized towards the sides and
    // the end of the field so that no opponents sneak up from behind.
    mRightHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                             Vector2f(mFieldLength/2.0 + 10, mFieldWidth/2.0 + 10.0));
    mLeftHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                            Vector2f(-mFieldLength/2.0 - 10, mFieldWidth/2.0 + 10.0));

    // the penalty areas (exact sizes)
    mRightPenaltyArea = salt::AABB2(Vector2f(mFieldLength/2.0 - 16.5, -16.5 - mGoalWidth/2.0),
                                    Vector2f(mFieldLength/2.0 , 16.5 + mGoalWidth/2.0));
    mLeftPenaltyArea = salt::AABB2(Vector2f(-mFieldLength/2.0 + 16.5, -16.5 - mGoalWidth/2.0),
                                   Vector2f(-mFieldLength/2.0, 16.5 + mGoalWidth/2.0));
}

void
SoccerRuleAspect::Broadcast(const string& message, const Vector3f& pos,
                            int number, TTeamIndex idx)
{
#if 0
    TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
    {
        return;
    }

    TAgentStateList opponent_agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, opponent_agent_states,
                                     SoccerBase::OpponentTeam(idx)))
    {
        return;
    }

    if (message.size() > mSayMsgSize)
    {
        return;
    }

    salt::BoundingSphere sphere(pos, mAudioCutDist);

    shared_ptr<Transform> transform_parent;
    shared_ptr<Body> agent_body;

    for (
        TAgentStateList::const_iterator it = agent_states.begin();
        it != agent_states.end();
        it++
        )
    {
        if ( (*it)->GetUniformNumber() == number)
        {
            (*it)->AddSelfMessage(message);
            continue;
        }
        SoccerBase::GetTransformParent(*(*it), transform_parent);

        // call GetAgentBody with matching AgentAspect
        SoccerBase::GetAgentBody(transform_parent, agent_body);

        // if the player is in the range, send the message
        Vector3f new_pos = agent_body->GetPosition();
        if (sphere.Contains(new_pos))
        {
            Vector3f relPos = pos - new_pos;
            relPos = SoccerBase::FlipView(relPos, idx);
            float direction = salt::gRadToDeg(salt::gArcTan2(relPos[1], relPos[0]));
            (*it)->AddMessage(message, direction, true);
        }
    }

    for (
        SoccerBase::TAgentStateList::const_iterator it = opponent_agent_states.begin();
        it != opponent_agent_states.end();
        it++
        )
    {
        SoccerBase::GetTransformParent(*(*it), transform_parent);

        // call GetAgentBody with matching AgentAspect
        SoccerBase::GetAgentBody(transform_parent, agent_body);

        // if the player is in the range, send the message
        Vector3f new_pos = agent_body->GetPosition();
        if (sphere.Contains(new_pos))
        {
            Vector3f relPos = pos - new_pos;
            relPos = SoccerBase::FlipView(relPos, SoccerBase::OpponentTeam(idx));
            float direction = salt::gRadToDeg(salt::gArcTan2(relPos[1], relPos[0]));
            (*it)->AddMessage(message, direction, false);
        }
    }
#endif
}

bool
SoccerRuleAspect::CheckOffside()
{
#if 0
    shared_ptr<AgentAspect> collidingAgent;
    shared_ptr<AgentAspect> kickingAgent;
    shared_ptr<AgentAspect> agent;
    shared_ptr<AgentState> agentState;
    TTime collidingTime;
    TTime kickingTime;
    TTime time;
    static TTime lastCollisionTime = 0.0;

    if (! mLastModeWasPlayOn)
    {
        mInOffsideLeftPlayers.clear();
        mInOffsideRightPlayers.clear();
    }

    if (! mBallState->GetLastCollidingAgent(collidingAgent,collidingTime) )
    {
        return false;
    }

    if (! mBallState->GetLastKickingAgent(kickingAgent,kickingTime) )
    {
        return false;
    }

    if (collidingTime > kickingTime)
    {
        time = collidingTime;
        agent = collidingAgent;
    }
    else
    {
        time = kickingTime;
        agent = kickingAgent;
    }

    // if the last colliding agent is the first agent that touches the ball
    // after "a goal kick" or "a kick-in(FIFA: throw-in)" or "a corner kick"
    TTime lastModeChange = mGameState->GetLastModeChange();

    if (time == lastModeChange)
    {
        mFirstCollidingAgent = true;
    }

    // only when an agent collides with the ball it maybe affect
    // offside, if we sometime want to consider
    // "interfering with an opponent" we should remove this condition
    if (lastCollisionTime == time)
    {
        return false;
    }
    lastCollisionTime = time;

    // if the second last collinding agent is the last colliding agent,
    // there is no offside
    if (mPreLastCollidingAgent == agent)
    {
        mNotOffside = true;
    }
    else
    {
        mPreLastCollidingAgent = agent;
        mNotOffside = false;
    }

    // find the positions of the last opponent defender and the second last one
    if (! SoccerBase::GetAgentState(agent,agentState))
    {
        return false;
    }

    TTeamIndex idx = agentState->GetTeamIndex();

    list<shared_ptr<AgentState> > opp_agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, opp_agent_states,
          SoccerBase::OpponentTeam(idx)))
    {
        return false;
    }

    float opp_goalkeeper_pos;
    float opp_defender_pos;

    opp_goalkeeper_pos = 0.0;
    opp_defender_pos   = 0.0;

    shared_ptr<Transform> transform_parent;
    shared_ptr<Body> agent_body;

    list<shared_ptr<AgentState> >::const_iterator it;
    for (it = opp_agent_states.begin(); it != opp_agent_states.end(); it++)
    {
        SoccerBase::GetTransformParent(*(*it), transform_parent);
        SoccerBase::GetAgentBody(transform_parent, agent_body);

        Vector3f opp_agent_pos = agent_body->GetPosition();

        if (SoccerBase::OpponentTeam(idx) == TI_LEFT)
        {
            if (opp_agent_pos[0] <= opp_goalkeeper_pos)
            {
                opp_defender_pos   = opp_goalkeeper_pos;
                opp_goalkeeper_pos = opp_agent_pos[0];
            }
            else if (opp_agent_pos[0] <= opp_defender_pos)
            {
                opp_defender_pos = opp_agent_pos[0];
            }
        }
        else
        {
            if (opp_agent_pos[0] >= opp_goalkeeper_pos)
            {
                opp_defender_pos   = opp_goalkeeper_pos;
                opp_goalkeeper_pos = opp_agent_pos[0];
            }
            else if (opp_agent_pos[0] >= opp_defender_pos)
            {
                opp_defender_pos = opp_agent_pos[0];
            }
        }
    }

    Vector3f ball_pos = mBallBody->GetPosition();

    SoccerBase::GetTransformParent(*agentState, transform_parent);
    SoccerBase::GetAgentBody(transform_parent, agent_body);
    Vector3f agent_pos = agent_body->GetPosition();

    if ( mFirstCollidingAgent )
    {
        if (((idx == TI_LEFT) && (agent_pos[0] > opp_defender_pos)
                && (agent_pos[0] > ball_pos[0])) ||
            ((idx == TI_RIGHT) && (agent_pos[0] < opp_defender_pos)
                && (agent_pos[0] < ball_pos[0])))
        {
            mFirstCollidingAgent = true;
        }
        else
        {
            mFirstCollidingAgent = false;
        }
    }

    // if the agent,that touches the ball was in
    // offside position before the last shoot, change the mode to offside
    bool offside = false;

    if ((idx == TI_LEFT) && !mFirstCollidingAgent && !mNotOffside)
    {
        vector<int>::const_iterator it_offside;
        for (
            it_offside = mInOffsideLeftPlayers.begin();
            it_offside != mInOffsideLeftPlayers.end();
            it_offside++)
        {
            if (agentState->GetUniformNumber() == *it_offside)
            {
                mGameState->SetPlayMode(PM_OFFSIDE_RIGHT);
                offside = true;
            }
        }
        if (!offside && (agent_pos[0] > opp_defender_pos))
            mFirstCollidingAgent = true;
    }
    else if (!mFirstCollidingAgent && !mNotOffside)
    {
        vector<int>::const_iterator it_offside;
        for (
            it_offside = mInOffsideRightPlayers.begin();
            it_offside != mInOffsideRightPlayers.end();
            it_offside++
            )
        {
            if (agentState->GetUniformNumber() == *it_offside)
            {
                mGameState->SetPlayMode(PM_OFFSIDE_LEFT);
                offside = true;
            }
        }
        if (!offside && (agent_pos[0] < opp_defender_pos))
            mFirstCollidingAgent = true;
    }

    // update the list of players that are in offside poistion
    mInOffsideLeftPlayers.clear();
    mInOffsideRightPlayers.clear();

    mFreeKickPos = mBallState->GetLastValidBallPosition();
    mFreeKickPos[2] = mBallRadius;

    list<shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
    {
        return false;
    }

    for (it = agent_states.begin(); it != agent_states.end(); it++)
    {
        SoccerBase::GetTransformParent(*(*it), transform_parent);
        SoccerBase::GetAgentBody(transform_parent, agent_body);

        Vector3f agent_pos = agent_body->GetPosition();

        if (idx == TI_LEFT)
        {
            if ((agent_pos[0] > opp_defender_pos)
                 && (agent_pos[0] > ball_pos[0]))
            {
                mInOffsideLeftPlayers.push_back((*it)->GetUniformNumber());
            }
        }
        else
        {
            if ((agent_pos[0] < opp_defender_pos)
                 && (agent_pos[0] < ball_pos[0]))
            {
                mInOffsideRightPlayers.push_back((*it)->GetUniformNumber());
            }
        }
    }

    return true;
#endif
}

void
SoccerRuleAspect::UpdateOffside(TTeamIndex idx)
{
#if 0
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }

    // move away opponent team
    Vector3f ball_pos = mBallBody->GetPosition();
    ClearPlayers(mFreeKickPos, mFreeKickDist, mFreeKickMoveDist,
                 SoccerBase::OpponentTeam(idx));

    // if no player touched the ball for mDropBallTime, we move away
    // all players and set the play mode to play on
    if (mDropBallTime > 0 &&
        mGameState->GetModeTime() > mDropBallTime)
    {
        DropBall(mFreeKickPos);
        return;
    }

    // after the first agent touches the ball move to PM_PLAY_ON. the
    // time when the agent last touches the ball must be after the
    // change to the OffsideKick mode
    shared_ptr<AgentAspect> agent;
    shared_ptr<AgentState> agentState;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        GetLog()->Error()
            << "ERROR: (SoccerRuleAspect) no agent collided yet\n";
        return;
    }

    if (! SoccerBase::GetAgentState(agent,agentState))
    {
        return;
    }

    TTeamIndex collidingAgentIdx = agentState->GetTeamIndex();

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange && collidingAgentIdx==idx)
    {
        mGameState->SetPlayMode(PM_PlayOn);
    }
    else
    {
        // move the ball back on the free kick position
        MoveBall(mFreeKickPos);
    }
#endif
}

void
SoccerRuleAspect::ClearPlayersWithException(const salt::Vector3f& pos,
                               float radius, float min_dist, TTeamIndex idx,
                               shared_ptr<AgentState> agentState)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    std::list<boost::shared_ptr<AgentState> > agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, agent_states, idx))
        return;

    salt::BoundingSphere sphere(pos, radius);
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    std::list<boost::shared_ptr<AgentState> >::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        if (agentState == (*i))
            continue;

        SoccerBase::GetTransformParent(**i, agent_aspect);
        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();

        Vector3f test_pos = new_pos;
        test_pos[2] = pos[2];

        if (sphere.Contains(test_pos))
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
            MoveAgent(agent_aspect, new_pos);
        }
    }
#if 0
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
        if (agentState == (*i))
            continue;

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
#endif
}
