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
#include "soccerruleaspect.h"
#include <salt/random.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <soccerbase/soccerbase.h>
#include <gamestateaspect/gamestateaspect.h>
#include <ballstateaspect/ballstateaspect.h>
#include <agentstate/agentstate.h>
#include <algorithm>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

SoccerRuleAspect::SoccerRuleAspect() :
    SoccerControlAspect(),
    mBallRadius(0.111),
    mGoalPauseTime(3),
    mKickInPauseTime(1),
    mHalfTime(2.25 * 60),
    mFreeKickDist(9.15),
    mFreeKickMoveDist(15.15),
    mGoalKickDist(1.0),
    mAutomaticKickOff(false),
    mWaitBeforeKickOff(1.0),
    mSingleHalfTime(false),
    mSayMsgSize(20),
    mAudioCutDist(50.0),
    mFirstCollidingAgent(true),
    mNotOffside(false),
    mLastModeWasPlayOn(false),
    mUseOffside(true),
    mDropBallTime(15),
    mNotStandingMaxTime(1000),       // max time player may be sitted or laying down before being repositioned
    mGoalieNotStandingMaxTime(1000), // max time goalie may be sitted or laying down before being repositioned
    mGroundMaxTime(1000),            // max time player may be on the ground before being repositioned
    mGoalieGroundMaxTime(1000),      // max time goalie (pl number 1) may be on the ground before being repositioned
    mMaxPlayersInsideOwnArea(1000),  // maximum number of players of the defending team that may be inside own penalty area
    mMinOppDistance(0),          // min dist for closest Opponent to ball in order to use repositions for 2nd, 3rd player
    mMin2PlDistance(0),            // min dist for second closest of team before being repositioned
    mMin3PlDistance(0),            // min dist for third closest of team before being repositioned
    mMaxTouchGroupSize(1000),
    mMaxFaultTime(0.0)               // maximum time allowed for a player to commit a positional fault before being repositioned
{
    mFreeKickPos = Vector3f(0.0,0.0,mBallRadius);
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
    mBallBody->Enable();
}

/* Uses only Ball and Players positions and detects overcrowind near ball and areas and
players inappropriate behavior (laying on the ground or not walking for too much time) */
void
SoccerRuleAspect::AutomaticSimpleReferee(TPlayMode playMode)
{
    // Reset counters before kickoff
    if (playMode == PM_BeforeKickOff)
    {
        ResetFaultCounter(TI_LEFT);
        ResetFaultCounter(TI_RIGHT);
    }
    else
    {
        CalculateDistanceArrays(TI_LEFT);    	// Calculates distance arrays for left team
        CalculateDistanceArrays(TI_RIGHT);   	// Calculates distance arrays for right team
        AnalyseFaults(TI_LEFT);   		// Analyzes simple faults for the left team
        AnalyseFaults(TI_RIGHT);   		// Analyzes simple faults for the right team
        AnalyseTouchGroups(TI_LEFT);
        AnalyseTouchGroups(TI_RIGHT);

        // Only apply rules during play-on, leaves some time for agents to
        // solve it themselves
        if (playMode == PM_PlayOn)
        {
          ClearPlayersAutomatic(TI_LEFT);   	// enforce standing and not overcrowding rules for left team
          ClearPlayersAutomatic(TI_RIGHT);  	// enforce standing and not overcrowding rules for right team
        }

        // Reset touch groups
        ResetTouchGroups(TI_LEFT);
        ResetTouchGroups(TI_RIGHT);
    }
}


void
SoccerRuleAspect::ResetFaultCounterPlayer(int unum, TTeamIndex idx)
{
    playerGround[unum][idx] = 0;
    playerNotStanding[unum][idx] = 0;
    playerStanding[unum][idx] = 5/0.02;   // Considers player has been standing for some time in playoff
    prevPlayerInsideOwnArea[unum][idx] = 0;
    playerInsideOwnArea[unum][idx] = 0;
    playerFaultTime[unum][idx] = 0;
}

void
SoccerRuleAspect::ResetFaultCounter(TTeamIndex idx)
{
    for(int t=1; t<=11; t++) {
	ResetFaultCounterPlayer(t,idx);
    }
}

// Process agent state: standing, sitted, laying down, ...
void
SoccerRuleAspect::ProcessAgentState(salt::Vector3f pos, int unum, TTeamIndex idx)
{
    float groundZVal = 0.15;  //bellow this player is on the ground
    float middleZVal = 0.25;  //abovce this player is standing (or trying...)

    //increase player not standing if it is not in upward position and inside of field
    if (pos.z() < middleZVal && fabs(pos.y())< mFieldWidth / 2 + 0.1)
    {
        playerNotStanding[unum][idx]++;
        playerStanding[unum][idx] = 0;  	//player not standing
    }

    //increase player near ground if it is very low and inside of field
    if (pos.z() < groundZVal && fabs(pos.y())< mFieldWidth / 2 + 0.1)
    {
        playerGround[unum][idx]++;
    }

    //increase player standing or at least trying... Reset ground
    if (pos.z() >= middleZVal)
    {
        playerStanding[unum][idx]++;
        playerGround[unum][idx] = 0;
    }

    //Player standing for some cycles (0.5 seconds) reset not standing count
    if (playerStanding[unum][idx] > 0.5 / 0.02) {
        playerNotStanding[unum][idx] = 0;
    }
}

// Calculates ordering on a distance vector
void SoccerRuleAspect::SimpleOrder(float dArr[][3], int oArr[][3], TTeamIndex idx)
{
    for(int t1 = 1; t1 <= 10; t1++)
        for(int t2 = t1 + 1; t2 <= 11; t2++)
            if (dArr[t1][idx] >= dArr[t2][idx])
                oArr[t1][idx]++;
            else
                oArr[t2][idx]++;

//    DEBUG
//    if (dArr[1][idx]<1000.0) {
//      cout << "Team: " << idx << "  --> ";
//      for(int t1=1; t1<=6; t1++)
//	if (dArr[t1][idx]<5.0) cout << t1 << " o:" << oArr[t1][idx] << " d: " << dArr[t1][idx] << "  | ";
//      cout << endl;
//    }
}

// Calculate Distance arrays and ordering to the ball and own goal
void SoccerRuleAspect::CalculateDistanceArrays(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0)
        return;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    salt::Vector3f ballPos = mBallBody->GetPosition();
    salt::Vector3f ownGoalPos;
    if (idx == TI_LEFT)
        ownGoalPos = Vector3f(-mFieldLength/2.0, 0.0, 0.0);
    else
        ownGoalPos = Vector3f(mFieldLength/2.0, 0.0, 0.0);

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;

    numPlInsideOwnArea[idx] = 0;
    closestPlayer[idx] = 1;
    closestPlayerDist[idx] = 1000.0;
    for(int t = 1; t <= 11; t++)
    {
        distArr[t][idx]=1000.0;
        ordArr[t][idx]=1;
        distGArr[t][idx]=1000.0;
        ordGArr[t][idx]=1;
    }

    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);
        Vector3f agentPos = agent_aspect->GetWorldTransform().Pos();
        int unum = (*i)->GetUniformNumber();
        distArr[unum][idx] = sqrt((agentPos.x()-ballPos.x())*(agentPos.x()-ballPos.x()) +
                                  (agentPos.y()-ballPos.y())*(agentPos.y()-ballPos.y()));
        distGArr[unum][idx] = sqrt((agentPos.x()-ownGoalPos.x())*(agentPos.x()-ownGoalPos.x()) +
                                   (agentPos.y()-ownGoalPos.y())*(agentPos.y()-ownGoalPos.y()));

        // determine closest player
        if (distArr[unum][idx] < closestPlayerDist[idx])
        {
            closestPlayerDist[idx] = distArr[unum][idx];
            closestPlayer[idx] = unum;
        }

        // save player inside area state in previous cycle
        prevPlayerInsideOwnArea[unum][idx] = playerInsideOwnArea[unum][idx];

        // determine number of players inside area and set inside area state of player
        if (idx == TI_LEFT && mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y())) ||
            idx == TI_RIGHT && mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y())))
        {
            numPlInsideOwnArea[idx]++;
            playerInsideOwnArea[unum][idx] = 1;

            //goalie is not repositioned when inside own area...
            if (unum == 1)
            {
                distGArr[unum][idx] = 0.0;
            }
        }
        else
          playerInsideOwnArea[unum][idx] = 0;

        // Process agent state: standing, sitted, laying down, ...
        ProcessAgentState(agentPos, unum, idx);
   }

   // compute rank of distance to ball
   SimpleOrder(distArr, ordArr, idx);
   // compute rank of distance to own goal
   SimpleOrder(distGArr, ordGArr, idx);
}

void SoccerRuleAspect::AnalyseTouchGroups(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0)
        return;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    random_shuffle(agent_states.begin(), agent_states.end());
    SoccerBase::TAgentStateList::iterator i = agent_states.begin();
    for (; i != agent_states.end(); ++i)
    {
        boost::shared_ptr<TouchGroup> touchGroup = (*i)->GetOldTouchGroup();

        // Wasn't touching before, joined group making group too large
        if (touchGroup->size() == 1 && touchGroup->size() > mMaxTouchGroupSize)
        {
            // determine the team that has more players in the touch group
            int pl[3] = { 0 };
            TTeamIndex oppIdx;
            TouchGroup::iterator oppIt; // stores the last opponent in touch group
            for (TouchGroup::iterator agentIt = touchGroup->begin();
                    agentIt != touchGroup->end(); ++agentIt)
            {
                pl[(*agentIt)->GetTeamIndex()]++;
                if ((*agentIt)->GetTeamIndex() != idx)
                {
                    oppIdx = (*agentIt)->GetTeamIndex();
                    oppIt = agentIt;
                }
            }

            if (pl[idx] >= touchGroup->size() - pl[idx])
            {
                playerFaultTime[(*i)->GetUniformNumber()][idx]++;
                // Remove player from touch group so no more agents are replaced
                touchGroup->erase(*i);
            }
            else
            {
                // I am the last one to enter the group, but the number of
                // opponents in the group are more than us
                playerFaultTime[(*oppIt)->GetUniformNumber()][oppIdx]++;
                touchGroup->erase(*oppIt);
            }
        }
    }
}

void SoccerRuleAspect::ResetTouchGroups(TTeamIndex idx)
{
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    SoccerBase::TAgentStateList::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); i++)
    {
        (*i)->NewTouchGroup();
        (*i)->GetTouchGroup()->insert(*i);
    }
}

// Analyse Faults and Creates Fault Time Array
void SoccerRuleAspect::AnalyseFaults(TTeamIndex idx)
{
    TTeamIndex idx2;
    if (idx == TI_LEFT)
        idx2 = TI_RIGHT;
    else
        idx2 = TI_LEFT;   //Other team

    for(int unum=1; unum<=11; unum++)
    {
        // I am the third closest player but i am too near the ball (and not the goalie)
        if (unum != 1 && closestPlayerDist[idx2] < mMinOppDistance &&
            (distArr[unum][idx] <= mMin3PlDistance + 0.01 && ordArr[unum][idx] == 3))
        {
            playerFaultTime[unum][idx]++;
        }
        // I am the second closest player but i am too near the ball (and not the goalie)
        else if(unum != 1 && closestPlayerDist[idx2] < mMinOppDistance &&
                distArr[unum][idx] <= mMin2PlDistance + 0.01 && ordArr[unum][idx] == 2 )
        {
            playerFaultTime[unum][idx]++;
        }
        // Too many players inside my own penalty area and Im am the last one to enter or
        // the last one to enter was the goalie and I am the one further away from own goal
        else if((numPlInsideOwnArea[idx] > mMaxPlayersInsideOwnArea && unum != 1 && playerInsideOwnArea[unum][idx] == 1 &&
                (prevPlayerInsideOwnArea[unum][idx] == 0 ||
                 (prevPlayerInsideOwnArea[1][idx] == 0 && playerInsideOwnArea[1][idx] == 1 && mMaxPlayersInsideOwnArea + 1 == ordGArr[unum][idx]))))
        {
            playerFaultTime[unum][idx]++;
        }
        // I am a field player and on the ground for too much time
        else if (unum != 1 && playerGround[unum][idx] > mGroundMaxTime / 0.02)
        {
            playerFaultTime[unum][idx]++;
        }
        // I am a field player and I am not standing for too much time
        else if(unum!=1 && playerNotStanding[unum][idx] > mNotStandingMaxTime / 0.02)
        {
            playerFaultTime[unum][idx]++;
        }
        // I am the goalie and I am on the ground for too much time
        else if (unum == 1 && playerGround[unum][idx] > mGoalieGroundMaxTime / 0.02)
        {
            playerFaultTime[unum][idx]++;
        }
        // I am the goalie and I and not standing for too much time
        else if (unum == 1 && playerNotStanding[unum][idx] > mGoalieNotStandingMaxTime / 0.02)
        {
            playerFaultTime[unum][idx]++;
        }
        else
        {
            playerFaultTime[unum][idx]=0;
        }
    }
}


salt::Vector3f SoccerRuleAspect::RepositionOutsidePos(salt::Vector3f posIni, int unum, TTeamIndex idx)
{
    salt::Vector3f pos;
    // Choose x side based on team
    float xFac = idx == TI_LEFT ? -0.6 : 0.6;
    // Choose side on oppisite side of field
    float yFac = posIni.y() < 0 ? 1.0 : -1.0;

    pos = Vector3f(xFac * (7 - unum), (mFieldWidth / 2 + 0.5) * yFac, 1.0);

    return pos;
}


// Clear Players that are violating the rules
void
SoccerRuleAspect::ClearPlayersAutomatic(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0)
        return;

    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    salt::Vector3f ballPos = mBallBody->GetPosition();

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;

    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);
        Vector3f agentPos = agent_aspect->GetWorldTransform().Pos();
        int unum = (*i)->GetUniformNumber();
        if (playerFaultTime[unum][idx] > mMaxFaultTime / 0.02)
        {
            // I am not a very good soccer player... I am violating the rules...
            salt::Vector3f new_pos = RepositionOutsidePos(ballPos, unum, idx);
            //Calculate my Reposition pos outside of the field
            SoccerBase::MoveAgent(agent_aspect, new_pos);
            //Oh my God!! I am flying!! I am going outside of the field
            ResetFaultCounterPlayer(unum, idx);
            //cout << "*********Player Repos Num: " << unum << "  Team: " << team << "  Pos: " << new_pos << endl;
        }
    }
}

void
SoccerRuleAspect::ClearPlayers(const salt::Vector3f& pos, float radius,
                               float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;

    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    salt::BoundingSphere sphere(pos, radius);
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);

        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();
        AABB3 agentAABB = SoccerBase::GetAgentBoundingBox(*agent_aspect);

        if (sphere.Intersects(agentAABB))
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
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
}

void
SoccerRuleAspect::ClearPlayers(const salt::AABB2& box,
                               float min_dist, TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);

        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();
        AABB2 agentAABB2 = SoccerBase::GetAgentBoundingRect(*agent_aspect);

        if (box.Intersects(agentAABB2))
        {
            if (idx == TI_LEFT)
            {
                new_pos[0] = box.minVec[0] -
                    salt::UniformRNG<>(min_dist, min_dist * 2.0)();
            } else {
                new_pos[0] = box.maxVec[0] +
                    salt::UniformRNG<>(min_dist, min_dist * 2.0)();
            }
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
}

void SoccerRuleAspect::ClearPlayersBeforeKickOff(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;

    // move the non-kick off team to own half field except the center
    // circle
    TTeamIndex opp = SoccerBase::OpponentTeam(idx);
    if ( TI_RIGHT == opp ){
        ClearPlayers(mLeftHalf, mFreeKickMoveDist, opp);
    }
    else {
        ClearPlayers(mRightHalf, mFreeKickMoveDist, opp);
    }
    ClearPlayers(Vector3f(0,0,0), mFreeKickDist, mFreeKickMoveDist,opp);

    // move the kick off team to own half field and the center circle
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    salt::AABB2 box;
    if ( TI_RIGHT == idx ){
        box = mLeftHalf;
    }
    else{
        box = mRightHalf;
    }

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;
    float freeKickDist2 = mFreeKickDist*mFreeKickDist;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);
        // if agent is on opponent half field, move it away
        AABB2 agentAABB2 = SoccerBase::GetAgentBoundingRect(*agent_aspect);
        if (box.Intersects(agentAABB2))
        {
            Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();
            // if agent is in the center circle, do not move it
            if ( agentAABB2.minVec.SquareLength() < freeKickDist2
                 && agentAABB2.maxVec.SquareLength() < freeKickDist2
                 && Vector2f(agentAABB2.minVec.x(),agentAABB2.maxVec.y()).SquareLength() < freeKickDist2
                 && Vector2f(agentAABB2.maxVec.x(),agentAABB2.minVec.y()).SquareLength() < freeKickDist2)
                continue;

            if (idx == TI_LEFT)
            {
                new_pos[0] = box.minVec[0] -
                    salt::UniformRNG<>(mFreeKickMoveDist, mFreeKickMoveDist * 2.0)();
            } else {
                new_pos[0] = box.maxVec[0] +
                    salt::UniformRNG<>(mFreeKickMoveDist, mFreeKickMoveDist * 2.0)();
            }
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
}

void
SoccerRuleAspect::ClearSelectedPlayers()
{
    float min_dist = mFreeKickMoveDist;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, TI_NONE))
        return;

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agent_aspect);

        // if agent is selected, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();

        if ((*i)->IsSelected())
        {
            float dist = salt::UniformRNG<>(min_dist, min_dist + 2.0)();

            if ((*i)->GetTeamIndex() == TI_LEFT)
            {
                if (new_pos[0] - dist < -mFieldLength/2.0)
                {
                    new_pos[1] = new_pos[1] < 0 ? new_pos[1] + dist : new_pos[1] - dist;
                } else {
                    new_pos[0] = new_pos[0] - dist;
                }
            } else {
                if (new_pos[0] + dist > mFieldLength/2.0)
                {
                    new_pos[1] = new_pos[1] < 0 ? new_pos[1] + dist : new_pos[1] - dist;
                } else {
                    new_pos[0] = new_pos[0] + dist;
                }
            }
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
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

    // do not drop the ball outside the field
    if (pos.y() <= -mFieldWidth / 2)
    {
        pos.y() = -mFieldWidth / 2 + mBallRadius;
    }
    else if (pos.y() >= mFieldWidth / 2)
    {
        pos.y() = mFieldWidth / 2 - mBallRadius;
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
    static boost::shared_ptr<GameControlServer> game_control;

    if  (game_control.get() == 0)
    {
        game_control = shared_dynamic_cast<GameControlServer>
            (GetCore()->Get("/sys/server/gamecontrol"));

        if (game_control.get() == 0)
        {
            GetLog()->Error() << "(SoccerRuleAspect) Error: can't get GameControlServer.\n";
            return;
        }
    }

    // if no players are connected, just return
    if (! game_control->GetAgentCount()) return;


    // before the game starts the ball should stay in the middle of
    // the playing field
    Vector3f pos(0,0,mBallRadius);
    MoveBall(pos);

    ClearPlayers(mRightHalf, mFreeKickMoveDist, TI_LEFT);
    ClearPlayers(mLeftHalf, mFreeKickMoveDist, TI_RIGHT);

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
    ClearPlayersBeforeKickOff(idx);

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
    boost::shared_ptr<AgentAspect> agent;
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
    // change to the KickIn mode *plus* pause time
    boost::shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "no agent collided yet\n";
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange + mKickInPauseTime)
    {
        mGameState->SetPlayMode(PM_PlayOn);
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "Set Playmode to playon\n";
    } else
    {
        // move the ball back on the ground where it left the playing
        // field
        MoveBall(mFreeKickPos);
    }
}


//-----------------------------------


void
SoccerRuleAspect::UpdateFreeKick(TTeamIndex idx)
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
//---------------

    salt::Vector2f ball_pos(mFreeKickPos.x(), mFreeKickPos.y());
    // we do not drop the ball within the penalty area
    if (mLeftPenaltyArea.Contains(ball_pos))
    {
        mFreeKickPos[0] = mLeftPenaltyArea.maxVec[0];
        mFreeKickPos[1] = mFreeKickPos.y() < 0 ?
            mLeftPenaltyArea.minVec[1] : mLeftPenaltyArea.maxVec[1];
    }
    else if (mRightPenaltyArea.Contains(ball_pos))
    {
        mFreeKickPos[0] = mRightPenaltyArea.minVec[0];
        mFreeKickPos[1] = mFreeKickPos.y() < 0 ?
            mRightPenaltyArea.minVec[1] : mRightPenaltyArea.maxVec[1];
    }

    MoveBall(mFreeKickPos);
//--------------------------

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
    boost::shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "no agent collided yet\n";
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange + mKickInPauseTime)
    {
        mGameState->SetPlayMode(PM_PlayOn);
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "Set Playmode to playon\n";
    } else
    {
        // move the ball back on the ground where it left the playing
        // field
        MoveBall(mFreeKickPos);
    }
}

//-----------------------------------



void
SoccerRuleAspect::UpdateGoalKick(TTeamIndex idx)
{
    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        return;
    }
    // move away opponent team
    ClearPlayers(idx == TI_LEFT ? mLeftPenaltyArea : mRightPenaltyArea,
                 mFreeKickMoveDist, SoccerBase::OpponentTeam(idx));

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
    boost::shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    // if the team with the goal kick touched the ball and the ball is
    // outside the penalty area, we switch to play on.
    if (time > lastChange + mKickInPauseTime)
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
    // change to the KickIn mode *plus* pause time
    boost::shared_ptr<AgentAspect> agent;
    TTime time;
    if (! mBallState->GetLastCollidingAgent(agent,time))
    {
        return;
    }

    TTime lastChange = mGameState->GetLastModeChange();
    if (time > lastChange + mKickInPauseTime)
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
//     cerr << "CheckBallLeftField\n";
    if (mBallState->GetBallOnField())
    {
        // update freekickpos
        mFreeKickPos = mBallState->GetLastValidBallPosition();

        return false;
    }

    // get the team of the last agent touching the ball and set the
    // correct kick in playmode
    boost::shared_ptr<AgentAspect> agent;
    boost::shared_ptr<AgentState> agentState;
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
                mFreeKickPos[0] = mFieldLength / 2 - mGoalKickDist;
                mFreeKickPos[1] = 0.0;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_GOAL_KICK_RIGHT);
            }
            // check goal kick left team
            else if (!last_touch_left && ball_left)
            {
                mFreeKickPos[0] = -mFieldLength / 2 + mGoalKickDist;
                mFreeKickPos[1] = 0.0;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_GOAL_KICK_LEFT);
            }
            // check corner kick right team
            else if (last_touch_left && ball_left)
            {
                mFreeKickPos[0] = -mFieldLength / 2 + mBallRadius;
                mFreeKickPos[1] = ball_pos[1] > 0 ?
                    mFieldWidth / 2 - 0.05 : -mFieldWidth / 2 + 0.05;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_CORNER_KICK_RIGHT);
            }
            // check corner kick left team
            else
            {
                mFreeKickPos[0] = mFieldLength / 2 - mBallRadius;
                mFreeKickPos[1] = ball_pos[1] > 0 ?
                    mFieldWidth / 2 - mBallRadius : -mFieldWidth / 2 + mBallRadius;
                mFreeKickPos[2] = mBallRadius;
                mGameState->SetPlayMode(PM_CORNER_KICK_LEFT);
            }
        }
        // handle kick in
        else if (salt::gAbs(ball_pos.y()) >= mFieldWidth / 2)
        {
            mFreeKickPos = mBallState->GetLastValidBallPosition();
            mFreeKickPos[1] = mFreeKickPos[1] > 0 ?
                mFieldWidth / 2 - mBallRadius : -mFieldWidth / 2 + mBallRadius;
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
        const salt::Vector3f ballPos = mBallBody->GetPosition();
        const float xDist2Goal = fabs(ballPos.x()) - mGoalBallLineX;

        // check if ball is completely out of the field
        if (xDist2Goal < 0)
            return false;

        salt::Vector3f normBVel = mBallBody->GetVelocity();
        // ball should be inside the field recently (assumes that the simulation
        // step size is smaller than 1 second)
        if (fabs(ballPos.x() - normBVel.x()) > mGoalBallLineX)
            return false;

        normBVel.Normalize();
        float velCos = normBVel.x();
        float dist = xDist2Goal / velCos;
        salt::Vector3f crossPoint = ballPos - normBVel * dist;

        if (fabs(crossPoint.y()) < mGoalWidth / 2.0 &&
                crossPoint.z() < mGoalHeight)
        {
            if (ballPos.x() < 0)
                idx = TI_LEFT;
            else
                idx = TI_RIGHT;
        }
        else
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

    case PM_FREE_KICK_LEFT:
        UpdateFreeKick(TI_LEFT);
        break;
    case PM_FREE_KICK_RIGHT:
        UpdateFreeKick(TI_RIGHT);
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

    // Simple Referee
    AutomaticSimpleReferee(playMode);
}

void
SoccerRuleAspect::OnLink()
{
    SoccerControlAspect::OnLink();

    GetControlAspect(mGameState, "GameStateAspect");

    if (mGameState.expired())
        {
            GetLog()->Error()
                    << "(SoccerRuleAspect) ERROR: could not get GameStateAspect\n";
        }

    GetControlAspect(mBallState, "BallStateAspect");

    if (mBallState.expired())
        {
            GetLog()->Error()
                    << "(SoccerRuleAspect) ERROR: could not get BallStateAspect\n";
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
    SoccerBase::GetSoccerVar(*this,"GoalHeight",mGoalHeight);
    SoccerBase::GetSoccerVar(*this,"FreeKickDistance",mFreeKickDist);
    SoccerBase::GetSoccerVar(*this,"FreeKickMoveDist",mFreeKickMoveDist);
    SoccerBase::GetSoccerVar(*this,"GoalKickDist",mGoalKickDist);
    SoccerBase::GetSoccerVar(*this,"AutomaticKickOff",mAutomaticKickOff);
    SoccerBase::GetSoccerVar(*this,"WaitBeforeKickOff",mWaitBeforeKickOff);
    SoccerBase::GetSoccerVar(*this,"SingleHalfTime",mSingleHalfTime);
    SoccerBase::GetSoccerVar(*this,"UseOffside",mUseOffside);
    float penaltyLength, penaltyWidth;
    SoccerBase::GetSoccerVar(*this,"PenaltyLength",penaltyLength);
    SoccerBase::GetSoccerVar(*this,"PenaltyWidth",penaltyWidth);

    // auto ref parameters
    SoccerBase::GetSoccerVar(*this,"NotStandingMaxTime",mNotStandingMaxTime);
    SoccerBase::GetSoccerVar(*this,"GoalieNotStandingMaxTime",mGoalieNotStandingMaxTime);
    SoccerBase::GetSoccerVar(*this,"GroundMaxTime",mGroundMaxTime);
    SoccerBase::GetSoccerVar(*this,"GoalieGroundMaxTime",mGoalieGroundMaxTime);
    SoccerBase::GetSoccerVar(*this,"MaxPlayersInsideOwnArea",mMaxPlayersInsideOwnArea);
    SoccerBase::GetSoccerVar(*this,"MinOppDistance",mMinOppDistance);
    SoccerBase::GetSoccerVar(*this,"Min2PlDistance",mMin2PlDistance);
    SoccerBase::GetSoccerVar(*this,"Min3PlDistance",mMin3PlDistance);
    SoccerBase::GetSoccerVar(*this,"MaxTouchGroupSize",mMaxTouchGroupSize);
    //SoccerBase::GetSoccerVar(*this,"MaxFaultTime",mMaxFaultTime);


    // cout << "MaxInside " << mMaxPlayersInsideOwnArea << endl << endl;
    // set up bounding boxes for halfs and goal areas

    // the right and the left half are intentionally oversized towards the sides and
    // the end of the field so that no opponents sneak up from behind.
    mRightHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                             Vector2f(mFieldLength/2.0 + 10, mFieldWidth/2.0 + 10.0));
    mLeftHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0 - 10.0),
                            Vector2f(-mFieldLength/2.0 - 10, mFieldWidth/2.0 + 10.0));

    // the penalty areas (exact sizes)
    mRightPenaltyArea = salt::AABB2(Vector2f(mFieldLength/2.0 - penaltyLength,
                                             -(penaltyWidth + mGoalWidth)/2.0),
                                    Vector2f(mFieldLength/2.0,
                                             (penaltyWidth + mGoalWidth)/2.0));
    mLeftPenaltyArea = salt::AABB2(Vector2f(-mFieldLength/2.0 + penaltyLength,
                                            -(penaltyWidth + mGoalWidth)/2.0),
                                   Vector2f(-mFieldLength/2.0,
                                            (penaltyWidth + mGoalWidth)/2.0));

    mGoalBallLineX = mFieldLength / 2.0 + mBallRadius;
}

void
SoccerRuleAspect::Broadcast(const string& message, const Vector3f& pos,
                            int number, TTeamIndex idx)
{
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
    {
        return;
    }

    SoccerBase::TAgentStateList opponent_agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), opponent_agent_states,
                                     SoccerBase::OpponentTeam(idx)))
    {
        return;
    }

    if (static_cast<int>(message.size()) > mSayMsgSize)
    {
        return;
    }

    salt::BoundingSphere sphere(pos, mAudioCutDist);

    boost::shared_ptr<Transform> transform_parent;
    boost::shared_ptr<RigidBody> agent_body;

    for (
        SoccerBase::TAgentStateList::const_iterator it = agent_states.begin();
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
}

bool
SoccerRuleAspect::CheckOffside()
{
    return true;

#if 0
    boost::shared_ptr<AgentAspect> collidingAgent;
    boost::shared_ptr<AgentAspect> kickingAgent;
    boost::shared_ptr<AgentAspect> agent;
    boost::shared_ptr<AgentState> agentState;
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

    list<boost::shared_ptr<AgentState> > opp_agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState, opp_agent_states,
          SoccerBase::OpponentTeam(idx)))
    {
        return false;
    }

    float opp_goalkeeper_pos;
    float opp_defender_pos;

    opp_goalkeeper_pos = 0.0;
    opp_defender_pos   = 0.0;

    boost::shared_ptr<Transform> transform_parent;
    boost::shared_ptr<RigidBody> agent_body;

    list<boost::shared_ptr<AgentState> >::const_iterator it;
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

    list<boost::shared_ptr<AgentState> > agent_states;
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
    boost::shared_ptr<AgentAspect> agent;
    boost::shared_ptr<AgentState> agentState;
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
                               boost::shared_ptr<AgentState> agentState)
{
    if (idx == TI_NONE || mBallState.get() == 0) return;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    salt::BoundingSphere sphere(pos, radius);
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::TAgentStateList::const_iterator i;
    for (i = agent_states.begin(); i != agent_states.end(); ++i)
    {
        if (agentState == (*i))
            continue;

        SoccerBase::GetTransformParent(**i, agent_aspect);
        // if agent is too close, move it away
        Vector3f new_pos = agent_aspect->GetWorldTransform().Pos();
        AABB3 agentAABB = SoccerBase::GetAgentBoundingBox(*agent_aspect);

        if (sphere.Intersects(agentAABB))
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
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
}

Vector2f
SoccerRuleAspect::GetFieldSize() const
{
    return Vector2f(mFieldLength,mFieldWidth);
}

void
SoccerRuleAspect::ResetAgentSelection()
{
    SoccerBase::TAgentStateList agent_states;
    if (SoccerBase::GetAgentStates(*mBallState.get(), agent_states, TI_NONE))
    {

        SoccerBase::TAgentStateList::const_iterator i;
        for (i = agent_states.begin(); i != agent_states.end(); ++i)
          (*i)->UnSelect();
    }
}

void
SoccerRuleAspect::SelectNextAgent()
{
    SoccerBase::TAgentStateList agent_states;
    bool selectNext = false;
    if (SoccerBase::GetAgentStates(*mBallState.get(), agent_states, TI_NONE) && agent_states.size() > 0)
    {
        boost::shared_ptr<AgentState> first = agent_states.front();

        SoccerBase::TAgentStateList::const_iterator i;
        for (i = agent_states.begin(); i != agent_states.end(); ++i)
        {
            if ((*i)->IsSelected())
            {
                (*i)->UnSelect();
                selectNext = true;
                continue;
            }
            else if (selectNext)
            {
              (*i)->Select();
              return;
            }
        }

        // No agent selected, select first
        first->Select();
    }
}
