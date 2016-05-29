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

#ifdef RVDRAW
#include <rvdraw/rvdraw.h>
#endif // RVDRAW

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

SoccerRuleAspect::SoccerRuleAspect() :
    SoccerControlAspect(),
    mBallRadius(0.111),
    mAgentRadius(0.4),
    mGoalPauseTime(3),
    mKickInPauseTime(1),
    mHalfTime(2.25 * 60),
    mDropBallTime(15),
    mFreeKickDist(9.15),
    mFreeKickMoveDist(15.15),
    mRepelPlayersForKick(false),
    mKickRepelDist(0.5),
    mGoalKickDist(1.0),
    mAutomaticKickOff(false),
    mWaitBeforeKickOff(1.0),
    mSingleHalfTime(false),
    mAutomaticQuit(true),
    mChangeSidesInSecondHalf(true),
    mAutoKickOffTimeOrigin(1000000.0),
    mSayMsgSize(20),
    mAudioCutDist(50.0),
    mNotStandingMaxTime(1000),       // max time player may be sitted or laying down before being repositioned
    mGroundMaxTime(1000),            // max time player may be on the ground before being repositioned
    mGoalieNotStandingMaxTime(1000), // max time goalie may be sitted or laying down before being repositioned
    mGoalieGroundMaxTime(1000),      // max time goalie (pl number 1) may be on the ground before being repositioned
    mMinOppDistance(0),          // min dist for closest Opponent to ball in order to use repositions for 2nd, 3rd player
    mMin2PlDistance(0),            // min dist for second closest of team before being repositioned
    mMin3PlDistance(0),            // min dist for third closest of team before being repositioned
    mMaxPlayersInsideOwnArea(1000),  // maximum number of players of the defending team that may be inside own penalty area
    mMaxTouchGroupSize(1000),
    mMaxFoulTime(0.0),              // maximum time allowed for a player to commit a positional foul before being repositioned
    mFirstCollidingAgent(true),
    mNotOffside(false),
    mLastModeWasPlayOn(false),
    mUseOffside(true),
    mUseCharging(false),
    mChargingMinSpeed(0.2),
    mChargingMinBallSpeedAngle(15),
    mChargingMinDeltaDist(0.2),
    mChargingMinDeltaAng(30),
    mChargingImmunityTime(1),
    mChargingCollisionMinTime(0.2),
    mChargingMaxBallRulesDist(100),
    mChargingMinCollBallDist(0.1),
    mMinCollisionSpeed(0.15),
    mFoulHoldTime(0.5),
    mLastFreeKickKickTime(0),
    mCheckFreeKickKickerFoul(false),
    mAllowKickOffTeamToScore(true),
    mIndirectKick(false),
    mPenaltyShootout(false),
    mKeepaway(false),
    mKeepawayCenterX(0.0),
    mKeepawayCenterY(0.0),
    mKeepawayLength(20.0),
    mKeepawayWidth(20.0),
    mKeepawayLengthReductionRate(4.0),
    mKeepawayWidthReductionRate(4.0)
{
    mFreeKickPos = Vector3f(0.0,0.0,mBallRadius);
    ResetFoulCounter(TI_LEFT);
    ResetFoulCounter(TI_RIGHT);
      
#ifdef RVDRAW
    // Create connection for sending draw commands to roboviz
    mRVSender = boost::shared_ptr<RVSender>(new RVSender());
    if (mRVSender->getSockFD() == -1)
    {
        mRVSender.reset();
    }
#endif // RVDRAW

    // Initialize values
    for (int i = 0; i <= 11; i++) 
    {
        for (int t = 0; t <= 2; t++) 
        {
            playerTimeSinceLastWasMoved[i][t] = 1/.02;
            for(int j = 0; j < AVERAGE_VELOCITY_MEASUREMENTS; j++)
            {
                playerVelocities[i][t][j] = salt::Vector3f(0,0,0);
            }
        }
    }
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
    // Set the free kick position to where we have moved the ball so as to not
    // accidentally use an old ball position for the next free kick.
    mFreeKickPos = Vector3f(pos.x(), pos.y(), mBallRadius);
}

/* Uses only Ball and Players positions and detects overcrowind near ball and areas and
players inappropriate behavior (laying on the ground or not walking for too much time) */
void
SoccerRuleAspect::AutomaticSimpleReferee()
{
    if (mKeepaway)
    {
        TTime game_time = mGameState->GetTime();

        float areaMinX = mKeepawayCenterX - mKeepawayLength/2.0 + (mKeepawayLengthReductionRate/2.0*game_time/60.0);
        float areaMaxX = mKeepawayCenterX + mKeepawayLength/2.0 - (mKeepawayLengthReductionRate/2.0*game_time/60.0);
        float areaMinY = mKeepawayCenterY - mKeepawayWidth/2.0 + (mKeepawayWidthReductionRate/2.0*game_time/60.0);
        float areaMaxY = mKeepawayCenterY + mKeepawayWidth/2.0 - (mKeepawayWidthReductionRate/2.0*game_time/60.0);

#ifdef RVDRAW
        if (mRVSender) 
        {
            mRVSender->clear();
            mRVSender->drawLine("KeepawayArea", areaMinX, areaMinY, areaMinX, areaMaxY, RVSender::RED);
            mRVSender->drawLine("KeepawayArea", areaMinX, areaMaxY, areaMaxX, areaMaxY, RVSender::RED);
            mRVSender->drawLine("KeepawayArea", areaMaxX, areaMaxY, areaMaxX, areaMinY, RVSender::RED);
            mRVSender->drawLine("KeepawayArea", areaMaxX, areaMinY, areaMinX, areaMinY, RVSender::RED);
            mRVSender->refresh();
        }
#endif // RVDRAW

        if (game_time > 0)
        {
            bool fBallOutsideKeepawayBox = false;  
            Vector3f ball_pos = mBallBody->GetPosition();
            if (ball_pos.x() < areaMinX
                || ball_pos.x() > areaMaxX
                || ball_pos.y() < areaMinY
                || ball_pos.y() > areaMaxY)
            {
                fBallOutsideKeepawayBox = true;
            }   
        
            bool fBallCollidedWithTaker = mBallState->GetBallCollidingWithAgentTeam(TI_RIGHT);

            if (fBallOutsideKeepawayBox || fBallCollidedWithTaker)
            {
                mGameState->SetPlayMode(PM_GameOver);
            }
        }
	}

    if (mPenaltyShootout && mGameState->GetPlayMode() == PM_Goal_Left) {
        // Cancel penalty shootout mode now that a goal has been scored
        mPenaltyShootout = false;
    }

    // Reset counters and do not consider players' fouls when game is not
    // running (when players can beam)
    if (mGameState->IsPaused())
    {
        ResetFoulCounter(TI_LEFT);
        ResetFoulCounter(TI_RIGHT);
    }
    else
    {
        CalculateDistanceArrays(TI_LEFT);    	// Calculates distance arrays for left team
        CalculateDistanceArrays(TI_RIGHT);   	// Calculates distance arrays for right team
        UpdateTimesSinceLastBallTouch();        // Resets time since last ball touch for agents currently colliding with ball
        AnalyseChargingFouls();                 // Analyzes if there are any charging fouls
        AnalyseFouls(TI_LEFT);   		// Analyzes simple fouls for the left team
        AnalyseFouls(TI_RIGHT);   		// Analyzes simple fouls for the right team
        AnalyseTouchGroups(TI_LEFT);            // Analyzes whether too many players are touching for the left team
        AnalyseTouchGroups(TI_RIGHT);           // Analyzes whether too many players are touching for the right team

        ClearPlayersAutomatic(TI_LEFT);   	// enforce standing and not overcrowding rules for left team
        ClearPlayersAutomatic(TI_RIGHT);  	// enforce standing and not overcrowding rules for right team

        // Reset touch groups
        ResetTouchGroups(TI_LEFT);
        ResetTouchGroups(TI_RIGHT);

        // If in penalty shootout mode check that the goalie remains in the penalty area
        if (mPenaltyShootout) 
        {
            SoccerBase::TAgentStateList agent_states;
            if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, TI_RIGHT))
                return;
            boost::shared_ptr<oxygen::Transform> agent_aspect;
            SoccerBase::TAgentStateList::const_iterator i;
            for (i = agent_states.begin(); i != agent_states.end(); ++i)
            {
                SoccerBase::GetTransformParent(**i, agent_aspect);
                Vector3f agentPos = agent_aspect->GetWorldTransform().Pos();
                if (agentPos.x() < mRightPenaltyArea.minVec[0] || agentPos.y() < mRightPenaltyArea.minVec[1] || agentPos.y() > mRightPenaltyArea.maxVec[1]) 
                {
                    // Penalty shootout goalie has left penalty area so award goal
                    mPenaltyShootout = false;
                    mGameState->ScoreTeam(TI_LEFT);
                    mGameState->SetPlayMode(PM_Goal_Left);
                }
            }
        }
    }
}


void
SoccerRuleAspect::ResetFoulCounterPlayer(int unum, TTeamIndex idx)
{
    playerGround[unum][idx] = 0;
    playerNotStanding[unum][idx] = 0;
    playerStanding[unum][idx] = 5/0.02;   // Considers player has been standing for some time in playoff
    prevPlayerInsideOwnArea[unum][idx] = 0;
    playerInsideOwnArea[unum][idx] = 0;
    playerFoulTime[unum][idx] = 0;
    playerTimeSinceLastBallTouch[unum][idx] = mChargingImmunityTime/0.02;
    playerChargingTime[unum][idx] = mChargingCollisionMinTime/0.02;
    playerLastFoul[unum][idx] = FT_None;
}

void
SoccerRuleAspect::ResetFoulCounter(TTeamIndex idx)
{
    for (int t = 1; t <= 11; t++)
    {
        ResetFoulCounterPlayer(t, idx);
    }
}

// Resets time since last ball touch for agents currently colliding with ball
void 
SoccerRuleAspect::UpdateTimesSinceLastBallTouch()
{
    if (mBallState.get() == 0)
        return;

    list<boost::shared_ptr<AgentAspect> > agents;
    if (mBallState->GetCollidingAgents(agents))
    {
        for (list<boost::shared_ptr<AgentAspect> > ::const_iterator agentIt = agents.begin();
             agentIt != agents.end();
             agentIt++) 
        {
            boost::shared_ptr<AgentState> agentState;
            if (!SoccerBase::GetAgentState(*agentIt, agentState))
            {
                GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot get "
                    "AgentState from an AgentAspect\n";
            }
            else
            {
                int unum = agentState->GetUniformNumber();
                int idx = agentState->GetTeamIndex();
                playerTimeSinceLastBallTouch[unum][idx] = 0;
            }
        }
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

    playerTimeSinceLastBallTouch[unum][idx]++;
    playerChargingTime[unum][idx]++;
    playerTimeSinceLastWasMoved[unum][idx]++;
}

// Calculates ordering on a distance vector
void SoccerRuleAspect::SimpleOrder(float dArr[][3], int oArr[][3], TTeamIndex idx)
{
    for(int t1 = 1; t1 <= 11; t1++) {
        if (HaveEnforceableFoul(t1,idx)) {
            // Don't count players who have committed a foul
            oArr[t1][idx] = -1;
            continue;
        }
        for(int t2 = t1 + 1; t2 <= 11; t2++) {
            if (HaveEnforceableFoul(t2,idx)) {
                // Don't count players who have committed a foul
                continue;
            }
            if (dArr[t1][idx] >= dArr[t2][idx])
                oArr[t1][idx]++;
            else
                oArr[t2][idx]++;
        }
    }
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
        if (((idx == TI_LEFT && mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) ||
             (idx == TI_RIGHT && mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))))
            && !HaveEnforceableFoul(unum,idx))
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

void SoccerRuleAspect::AnalyseChargingFouls()
{
    if (!mUseCharging)
    {
        return;
    }

    if (mBallState.get() == 0)
        return;
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states, TI_LEFT))
        return;

    SoccerBase::TAgentStateList::iterator asIt = agent_states.begin();
    for (; asIt != agent_states.end(); ++asIt)
    {
        boost::shared_ptr<TouchGroup> touchGroup = (*asIt)->GetTouchGroup();
        if (touchGroup->size() != 2)
        {
            continue;
        }
        
        salt::Vector3f agentAvgVel[2];
        salt::Vector3f agentPos[2];
        float s[2]; //agentSpeed
        float d[2]; //agentDistToBall
        float aVB[2]; //agentAngleOfSpeedVectorToBall
        float aVO[2]; //agentAngleOfSpeedVectorToOpponent
        int agentUNum[2];
        TTeamIndex agentTeamIndex[2];
        bool isCharging[2]; // Boolean if an agent had committed a foul
        salt::Vector3f ballPos = mBallBody->GetPosition();
        
        int i = 0;
        for (TouchGroup::iterator agentIt = touchGroup->begin(); agentIt != touchGroup->end(); ++agentIt)
        {
            boost::shared_ptr<Transform> transform_parent;
            boost::shared_ptr<RigidBody> agent_body;
            SoccerBase::GetTransformParent(*(*agentIt), transform_parent);
            SoccerBase::GetAgentBody(transform_parent, agent_body);
            
            // Get agent uniform number and team index
            agentUNum[i] = (*agentIt)->GetUniformNumber();
            agentTeamIndex[i] = (*agentIt)->GetTeamIndex();

            // Get agent position
            agentPos[i] = agent_body->GetPosition();
            
            // Compute agent distance to ball
            d[i] = sqrt(pow(agentPos[i].x() - ballPos.x(), 2)
                        + pow(agentPos[i].y() - ballPos.y(), 2));
            
            // Get average agent velocity
            agentAvgVel[i] = salt::Vector3f(0,0,0);

            for(int j = 0; j < AVERAGE_VELOCITY_MEASUREMENTS; j++)
            {
            	agentAvgVel[i].x() = agentAvgVel[i].x() + playerVelocities[agentUNum[i]][agentTeamIndex[i]][j].x();
            	agentAvgVel[i].y() = agentAvgVel[i].y() + playerVelocities[agentUNum[i]][agentTeamIndex[i]][j].y();
            	agentAvgVel[i].z() = agentAvgVel[i].z() + playerVelocities[agentUNum[i]][agentTeamIndex[i]][j].z();
            }

            agentAvgVel[i].x() = agentAvgVel[i].x()/AVERAGE_VELOCITY_MEASUREMENTS;
            agentAvgVel[i].y() = agentAvgVel[i].y()/AVERAGE_VELOCITY_MEASUREMENTS;
            agentAvgVel[i].z() = agentAvgVel[i].z()/AVERAGE_VELOCITY_MEASUREMENTS;

            // Compute agent speed
            s[i] = sqrt(pow(agentAvgVel[i].x(), 2) + pow(agentAvgVel[i].y(), 2));
            
            // Compute speed vector angle to ball
            aVB[i] = abs(salt::gRadToDeg(salt::gNormalizeRad(
                                                             salt::gArcTan2(agentAvgVel[i].y(), agentAvgVel[i].x()) -
                                                             salt::gArcTan2(ballPos.y()-agentPos[i].y(), ballPos.x() - agentPos[i].x())
                                                             )));
            
            

            i++;
        }
        
        // Check if these are opponent agents meaning they have different team indexes
        bool haveOpponent = agentTeamIndex[0] != agentTeamIndex[1];
        
        // Check if one of the agents is a goalie in their own penalty area
        bool goalieInOwnPenaltyArea[2];
        for (int i = 0; i <= 1; i++)
        {
            goalieInOwnPenaltyArea[i] = false;
            if (agentUNum[i] == 1) 
            {
                // Could use playerInsideOwnArea[agentUNum[i]][agentTeamIndex[i]], however prefer to stretch the area for a goalie behind the goal line
                if (agentTeamIndex[i] == TI_LEFT) {
                    if (agentPos[i].x() <= mLeftPenaltyArea.maxVec[0] && agentPos[i].y() >= mLeftPenaltyArea.minVec[1] && agentPos[i].y() <= mLeftPenaltyArea.maxVec[1]) {
                        goalieInOwnPenaltyArea[i] = true;
                        break;
                    }
                } else if (agentTeamIndex[i] == TI_RIGHT) {
                    if (agentPos[i].x() >= mRightPenaltyArea.minVec[0] && agentPos[i].y() >= mRightPenaltyArea.minVec[1] && agentPos[i].y() <= mRightPenaltyArea.maxVec[1]) {
                        goalieInOwnPenaltyArea[i] = true;
                        break;
                    }
                }
            }
        }
                        
            
        if(!haveOpponent
           || playerNotStanding[agentUNum[0]][agentTeamIndex[0]] != 0
           || playerNotStanding[agentUNum[1]][agentTeamIndex[1]] != 0
           || playerTimeSinceLastWasMoved[agentUNum[0]][agentTeamIndex[0]] < 1/.02
           || playerTimeSinceLastWasMoved[agentUNum[1]][agentTeamIndex[1]] < 1/.02
           || HaveEnforceableFoul(agentUNum[0],agentTeamIndex[0])
           || HaveEnforceableFoul(agentUNum[1],agentTeamIndex[1])
           )
        {
            continue;
        }
        
        // Compute speed vector angle to opponent
        for (i = 0; i <= 1; i++)
        {
            aVO[i] = abs(salt::gRadToDeg(salt::gNormalizeRad(
                                                             salt::gArcTan2(agentAvgVel[i].y(), agentAvgVel[i].x()) -
                                                             salt::gArcTan2(agentPos[1-i].y()-agentPos[i].y(),
                                                                            agentPos[1-i].x()-agentPos[i].x()))
                                         ));
        } 

        isCharging[0] = true;
        isCharging[1] = true;
        if(d[0] < mChargingMaxBallRulesDist || d[1] < mChargingMaxBallRulesDist)
        {
        	for (int i = 0; i <= 1; i++)
                {
                    isCharging[i] = (aVB[i] >= aVO[i] && aVB[i] >= mChargingMinBallSpeedAngle && s[i] >= mChargingMinSpeed);
                }

                float deltaD;
                float deltaaVO;
                if (!isCharging[0] && !isCharging[1])
                {
                    // Handle case where both agents are apparently heading for the ball
                    deltaD = abs(d[0] - d[1]);
                    deltaaVO = abs(aVO[0] - aVO[1]);
                    
                    if(deltaD > mChargingMinDeltaDist)
                    {
                        isCharging[0] = d[0] > d[1] && (deltaaVO > mChargingMinDeltaAng || s[1] < mChargingMinSpeed);
                        isCharging[1] = d[1] > d[0] && (deltaaVO > mChargingMinDeltaAng || s[0] < mChargingMinSpeed);
                    }
                }
        }

        salt::Vector3f agentToCollisionVec[2];
        float agentCollisionSpeed[2];

        for (int i = 0; i <= 1; i++)
        {
            agentToCollisionVec[i] = salt::Vector3f(((*asIt)->mCollisionPos.x()+agentPos[1-i].x())/2.0-agentPos[i].x(), ((*asIt)->mCollisionPos.y()+agentPos[1-i].y())/2.0-agentPos[i].y(), 0);
            agentToCollisionVec[i].Normalize();
        }
        
        for (int i = 0; i <= 1; i++)
        {
            // Take into account the opponent's velocity vector when determining
            // collision speed and subtract half the opponent's speed moving
            // away from the the collision point as that speed reduces the
            // collision force.
            agentCollisionSpeed[i] = (agentAvgVel[i].x()*agentToCollisionVec[i].x() + agentAvgVel[i].y()*agentToCollisionVec[i].y()) +
                0.5*min(0.0f, agentAvgVel[1-i].x()*agentToCollisionVec[1-i].x() + agentAvgVel[1-i].y()*agentToCollisionVec[1-i].y());

            isCharging[i] = (isCharging[i]
                             && s[i] >= mChargingMinSpeed
                             && agentCollisionSpeed[i] >= mMinCollisionSpeed);
                
        }

#ifdef RVDRAW
        if (mRVSender) {
            mRVSender->clearStaticDrawings();
            mRVSender->drawPoint((*asIt)->mCollisionPos.x(), (*asIt)->mCollisionPos.y(), 10, RVSender::PINK);
            if (mChargingMinCollBallDist > 0) {
                mRVSender->drawCircle(ballPos.x(), ballPos.y(), mChargingMinCollBallDist, RVSender::ORANGE);
            }
            bool shade = goalieInOwnPenaltyArea[0] || playerTimeSinceLastBallTouch[agentUNum[0]][agentTeamIndex[0]] < mChargingImmunityTime/0.02 || playerChargingTime[agentUNum[0]][agentTeamIndex[0]] < mChargingCollisionMinTime/0.02;
            mRVSender->drawPoint(agentPos[0].x(), agentPos[0].y(), 10, agentTeamIndex[0] == TI_LEFT ? RVSender::BLUE : RVSender::RED, shade);
            mRVSender->drawLine(agentPos[0].x(), agentPos[0].y(), agentPos[0].x()+agentAvgVel[0].x(), agentPos[0].y()+agentAvgVel[0].y(), agentTeamIndex[0] == TI_LEFT ? RVSender::BLUE : RVSender::RED, shade);
            shade = goalieInOwnPenaltyArea[1] || playerTimeSinceLastBallTouch[agentUNum[1]][agentTeamIndex[1]] < mChargingImmunityTime/0.02;
            mRVSender->drawPoint(agentPos[1].x(), agentPos[1].y(), 10, agentTeamIndex[1] == TI_LEFT ? RVSender::BLUE : RVSender::RED, shade);
            mRVSender->drawLine(agentPos[1].x(), agentPos[1].y(), agentPos[1].x()+agentAvgVel[1].x(), agentPos[1].y()+agentAvgVel[1].y(), agentTeamIndex[1] == TI_LEFT ? RVSender::BLUE : RVSender::RED, shade);
            if (agentCollisionSpeed[0] > 0) {
                mRVSender->drawLine(agentPos[0].x(), agentPos[0].y(), agentPos[0].x()+(agentCollisionSpeed[0]*agentToCollisionVec[0].x()), agentPos[0].y()+(agentCollisionSpeed[0]*agentToCollisionVec[0].y()), RVSender::YELLOW, !isCharging[0]);
            }
            if (agentCollisionSpeed[1] > 0) {
                mRVSender->drawLine(agentPos[1].x(), agentPos[1].y(), agentPos[1].x()+(agentCollisionSpeed[1]*agentToCollisionVec[1].x()), agentPos[1].y()+(agentCollisionSpeed[1]*agentToCollisionVec[1].y()), RVSender::YELLOW, !isCharging[1]);
            }
        }
#endif // RVDRAW
        
        for (int i = 0; i <= 1; i++)
        {
            if (isCharging[i]) {
                if (goalieInOwnPenaltyArea[i])
                {
                    playerTimeSinceLastBallTouch[agentUNum[1-i]][agentTeamIndex[1-i]] = 0;
                }
                else if (playerTimeSinceLastBallTouch[agentUNum[i]][agentTeamIndex[i]] < mChargingImmunityTime/0.02)
                {
                    playerTimeSinceLastBallTouch[agentUNum[1-i]][agentTeamIndex[1-i]] = min(playerTimeSinceLastBallTouch[agentUNum[1-i]][agentTeamIndex[1-i]],
                    																		playerTimeSinceLastBallTouch[agentUNum[i]][agentTeamIndex[i]]);
                }
                playerChargingTime[agentUNum[1-i]][agentTeamIndex[1-i]] = min(playerChargingTime[agentUNum[1-i]][agentTeamIndex[1-i]],
                                                                              playerChargingTime[agentUNum[i]][agentTeamIndex[i]]);
            }
        }
        
        float ballToCollDist = sqrt(pow((*asIt)->mCollisionPos.x()- ballPos.x(), 2)
                                    + pow((*asIt)->mCollisionPos.y() - ballPos.y(), 2));
     
        if((isCharging[0] && isCharging[1]) || ballToCollDist < mChargingMinCollBallDist) //Type 3
        {
            isCharging[0] = false;
            isCharging[1] = false;
        }

        for (int i = 0; i <= 1; i++)
        {
            if (mKeepaway && agentTeamIndex[i] == TI_RIGHT) 
            {
                // Don't call charging fouls on agents trying to take ball
                // during keepaway
                continue;
            }
            
            if (isCharging[i] && !goalieInOwnPenaltyArea[i] && playerChargingTime[agentUNum[i]][agentTeamIndex[i]] >= mChargingCollisionMinTime/0.02
                && playerTimeSinceLastBallTouch[agentUNum[i]][agentTeamIndex[i]] >= mChargingImmunityTime/0.02)
            {
                playerFoulTime[agentUNum[i]][agentTeamIndex[i]]++;
                playerLastFoul[agentUNum[i]][agentTeamIndex[i]] = FT_Charging;
            }
        }

        if(!isCharging[0] && !isCharging[1])
        {
            playerChargingTime[agentUNum[0]][agentTeamIndex[0]] = 0;
            playerChargingTime[agentUNum[1]][agentTeamIndex[1]] = 0;
        }
    }
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
        boost::shared_ptr<TouchGroup> touchGroup = (*i)->GetTouchGroup();

        if (HaveEnforceableFoul((*i)->GetUniformNumber(),idx)) {
            // Once a player has been called for a foul don't check for
            // additional fouls
            
            // Remove player from touch group so no more agents are replaced
            touchGroup->erase(*i);

            continue;
        }

        // Wasn't touching before, joined group making group too large
        if ((*i)->GetOldTouchGroup()->size() == 1 &&
            (int)touchGroup->size() > mMaxTouchGroupSize)
        {
            // determine the team that has more players in the touch group
            int pl[3] = { 0 };
            TTeamIndex oppIdx = TI_NONE; // Initialize value to prevent compiler warning
            TouchGroup::iterator oppIt; // stores the last opponent in touch group
            TouchGroup::iterator teammateIt; // stores the last teammate in touch group

            int numPlayersCommittedFoul = 0;
            for (TouchGroup::iterator agentIt = touchGroup->begin();
                    agentIt != touchGroup->end(); ++agentIt)
            {
                if (HaveEnforceableFoul((*agentIt)->GetUniformNumber(),(*agentIt)->GetTeamIndex())) 
                {
                    // Don't count players who have committed fouls
                    numPlayersCommittedFoul++;
                    continue;
                }   

                pl[(*agentIt)->GetTeamIndex()]++;
                
                // Only call touching foul on non-goalies in group
                if ((*agentIt)->GetUniformNumber() != 1 || mMaxTouchGroupSize < 2) 
                {
                    if ((*agentIt)->GetTeamIndex() != idx)
                    {
                        oppIdx = (*agentIt)->GetTeamIndex();
                        oppIt = agentIt;
                    } 
                    else 
                    {
                        teammateIt = agentIt;
                    }
                }
            }

            if ((int)touchGroup->size() <= mMaxTouchGroupSize-numPlayersCommittedFoul)
            {
                // Not enough players who haven't already committed fouls in 
                // group to call a touching foul
                continue;
            }   

            if (pl[idx] >= (int)touchGroup->size() - pl[idx])
            {
                if ((*i)->GetUniformNumber() == 1 && mMaxTouchGroupSize > 1) 
                {
                    // I am the last one to enter the group, but remove teammate as
                    // I'm the goalie
                    playerFoulTime[(*teammateIt)->GetUniformNumber()][idx]++;
                    playerLastFoul[(*teammateIt)->GetUniformNumber()][idx] = FT_Touching;
                    
                    // Remove player from touch group so no more agents are replaced
                    touchGroup->erase(*teammateIt);
                }
                else 
                {
                    playerFoulTime[(*i)->GetUniformNumber()][idx]++;
                    playerLastFoul[(*i)->GetUniformNumber()][idx] = FT_Touching;
                    
                    // Remove player from touch group so no more agents are replaced
                    touchGroup->erase(*i);
                }
            }
            else
            {
                // I am the last one to enter the group, but the number of
                // opponents in the group are more than us
                playerFoulTime[(*oppIt)->GetUniformNumber()][oppIdx]++;
                playerLastFoul[(*oppIt)->GetUniformNumber()][oppIdx] = FT_Touching;

                // Remove player from touch group so no more agents are replaced
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

// Analyse Fouls and Creates Foul Time Array
void SoccerRuleAspect::AnalyseFouls(TTeamIndex idx)
{
    TTeamIndex idx2 = SoccerBase::OpponentTeam(idx); // //Other team   

    for(unsigned int unum=1; unum<=11; unum++)
    {
        if (HaveEnforceableFoul(unum,idx)) 
        {
            // Once a player has been called for a foul don't check for
            // additional fouls
            continue;
        }

        TPlayMode playMode = mGameState->GetPlayMode();
        bool checkCrowding = (playMode == PM_KickOff_Left && idx != TI_LEFT)
            || (playMode == PM_KickOff_Right && idx != TI_RIGHT)
            || (playMode != PM_KickOff_Left && playMode != PM_KickOff_Right);
 
        // I am the third closest player but i am too near the ball (and not the goalie)
        if (checkCrowding && unum != 1 && closestPlayerDist[idx2] < mMinOppDistance
            && (distArr[unum][idx] <= mMin3PlDistance + 0.01
                && ordArr[unum][idx] == 3))
        {
                playerFoulTime[unum][idx]++;
                playerLastFoul[unum][idx] = FT_Crowding;
        }
        // I am the second closest player but i am too near the ball (and not the goalie)
        else if (checkCrowding && unum != 1 && closestPlayerDist[idx2] < mMinOppDistance
                 && distArr[unum][idx] <= mMin2PlDistance + 0.01
                 && ordArr[unum][idx] == 2)
        {
                playerFoulTime[unum][idx]++;
                playerLastFoul[unum][idx] = FT_Crowding;
        } 
        // Too many players inside my own penalty area and Im am the last one to enter or
        // the last one to enter was the goalie and I am the one further away from own goal
        else if((numPlInsideOwnArea[idx] > mMaxPlayersInsideOwnArea && unum != 1 && playerInsideOwnArea[unum][idx] == 1 &&
                (prevPlayerInsideOwnArea[unum][idx] == 0 ||
                 (prevPlayerInsideOwnArea[1][idx] == 0 && playerInsideOwnArea[1][idx] == 1 && mMaxPlayersInsideOwnArea + 1 == ordGArr[unum][idx]))))
        {
            playerFoulTime[unum][idx]++;
            playerLastFoul[unum][idx] = FT_IllegalDefence;
        }
        // I am a field player and on the ground for too much time
        else if (unum != 1 && playerGround[unum][idx] > mGroundMaxTime / 0.02)
        {
            playerFoulTime[unum][idx]++;
            playerLastFoul[unum][idx] = FT_Incapable;
        }
        // I am a field player and I am not standing for too much time
        else if(unum!=1 && playerNotStanding[unum][idx] > mNotStandingMaxTime / 0.02)
        {
            playerFoulTime[unum][idx]++;
            playerLastFoul[unum][idx] = FT_Incapable;
        }
        // I am the goalie and I am on the ground for too much time
        else if (unum == 1 && playerGround[unum][idx] > mGoalieGroundMaxTime / 0.02)
        {
            playerFoulTime[unum][idx]++;
            playerLastFoul[unum][idx] = FT_Incapable;
        }
        // I am the goalie and I and not standing for too much time
        else if (unum == 1 && playerNotStanding[unum][idx] > mGoalieNotStandingMaxTime / 0.02)
        {
            playerFoulTime[unum][idx]++;
            playerLastFoul[unum][idx] = FT_Incapable;
        }
        else
        {
            playerFoulTime[unum][idx]=0;
            playerLastFoul[unum][idx] = FT_None;
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

        if (HaveEnforceableFoul(unum,idx))
        {
            // Record foul
            mFouls.push_back(Foul(mFouls.size() + 1, playerLastFoul[unum][idx], *i));
            
            if (playerFoulTime[unum][idx] <= mFoulHoldTime/0.02) {
                playerFoulTime[unum][idx]++;
                agentPos[2] = 1.0 + playerFoulTime[unum][idx]*0.01;
                MoveAgent(agent_aspect, agentPos);
            } else {
                // I am not a very good soccer player... I am violating the rules...
                salt::Vector3f new_pos = RepositionOutsidePos(ballPos, unum, idx);
                //Calculate my Reposition pos outside of the field
                if (mFoulHoldTime > 0) {
                    new_pos[2] = mAgentRadius;
                }
                //Oh my God!! I am flying!! I am going outside of the field
                MoveAgent(agent_aspect, new_pos);
                
                ResetFoulCounterPlayer(unum, idx);
                //cout << "*********Player Repos Num: " << unum << "  Team: " << team << "  Pos: " << new_pos << endl;
            }
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
            MoveAgent(agent_aspect, new_pos);
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
            MoveAgent(agent_aspect, new_pos);
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
            MoveAgent(agent_aspect, new_pos);
        }
    }
}

void
SoccerRuleAspect::RepelPlayers(const salt::Vector3f& pos, float radius,
                               TTeamIndex idx)
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
        float dist = sqrt((new_pos.x()-pos.x())*(new_pos.x()-pos.x()) +
                          (new_pos.y()-pos.y())*(new_pos.y()-pos.y()));
        if (dist < radius)
        {
            Vector2f agent_pos = Vector2f(new_pos.x(),new_pos.y());
            Vector2f avoid_pos = Vector2f(pos.x(),pos.y());
            Vector2f pos2Agent = agent_pos-avoid_pos;
            Vector2f updated_pos = avoid_pos+pos2Agent.Normalize()*radius;
            new_pos[0] = updated_pos[0];
            new_pos[1] = updated_pos[1];
            SoccerBase::MoveAgent(agent_aspect, new_pos);
        }
    }
}

void
SoccerRuleAspect::RepelPlayersForKick(TTeamIndex idx)
{
    if (mRepelPlayersForKick) {
        // Move players taking the kick out of the way of the ball 
        // when it is first placed
        RepelPlayers(mFreeKickPos, mKickRepelDist, idx);
        mRepelPlayersForKick = false;
    }
}

void
SoccerRuleAspect::SwapTeamSides()
{
    SoccerBase::TAgentStateList agent_states;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), agent_states))
        return;

    mGameState->SwapTeamIndexes();

    SoccerBase::TAgentStateList::iterator it;
    for (it = agent_states.begin(); it != agent_states.end(); ++it)
    {
        (*it)->SetTeamIndex(SoccerBase::OpponentTeam((*it)->GetTeamIndex()));
    }

    // make sure that team names (and probably other things) are updated on
    // monitors
    GetActiveScene()->SetModified(true);
}

void
SoccerRuleAspect::AwardGoalKick(TTeamIndex idx)
{
    if (idx == TI_LEFT)
        {
            mFreeKickPos[0] = -mFieldLength / 2 + mGoalKickDist;
            mFreeKickPos[1] = 0.0;
            mFreeKickPos[2] = mBallRadius;
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode(PM_GOAL_KICK_LEFT);
        }
    else if (idx == TI_RIGHT)
        {
            mFreeKickPos[0] = mFieldLength / 2 - mGoalKickDist;
            mFreeKickPos[1] = 0.0;
            mFreeKickPos[2] = mBallRadius;
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode(PM_GOAL_KICK_RIGHT);
        }
    else {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "invalid team index for awarding goal kick\n";
    }
}

void
SoccerRuleAspect::AwardCornerKick(TTeamIndex idx)
{
    Vector3f ball_pos = mBallBody->GetPosition();
    if (idx == TI_LEFT)
        {
            // temp value for RoboCup 2012
            // correct value: mFieldWidth / 2 - mBallRadius
            const float cornerKickY = (mFieldWidth + mGoalWidth) / 4.0
                - mBallRadius;
            mFreeKickPos[0] = mFieldLength / 2 - mBallRadius;
            mFreeKickPos[1] = ball_pos[1] > 0 ?
                cornerKickY : -cornerKickY;
            mFreeKickPos[2] = mBallRadius;
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode(PM_CORNER_KICK_LEFT);
            
        }
    else if (idx == TI_RIGHT)
        {
            // temp value for RoboCup 2012
            // correct value: mFieldWidth / 2 - mBallRadius
            const float cornerKickY = (mFieldWidth + mGoalWidth) / 4.0
                - mBallRadius;
            mFreeKickPos[0] = -mFieldLength / 2 + mBallRadius;
            mFreeKickPos[1] = ball_pos[1] > 0 ?
                cornerKickY : -cornerKickY;
            mFreeKickPos[2] = mBallRadius;
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode(PM_CORNER_KICK_RIGHT);
        }
    else {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "invalid team index for awarding corner kick\n";
    }
}

void
SoccerRuleAspect::AwardKickIn(TTeamIndex idx)
{
    if (idx == TI_LEFT || idx == TI_RIGHT)
        {
            mFreeKickPos = mBallState->GetLastValidBallPosition();
            mFreeKickPos[1] = mFreeKickPos[1] > 0 ?
                mFieldWidth / 2 - mBallRadius : -mFieldWidth / 2 + mBallRadius;
            mFreeKickPos[2] = mBallRadius;
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode((idx == TI_LEFT) ?
                                    PM_KickIn_Left : PM_KickIn_Right);
        }
    else {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "invalid team index for awarding kick in\n";
    }
}

void
SoccerRuleAspect::AwardFreeKick(TTeamIndex idx, bool indirect)
{
    if (idx == TI_LEFT || idx == TI_RIGHT)
        {
            mRepelPlayersForKick = true;
            mGameState->SetPlayMode((idx == TI_LEFT) ?
                                    (indirect ? PM_FREE_KICK_LEFT : 
                                     PM_DIRECT_FREE_KICK_LEFT) :
                                    (indirect ? PM_FREE_KICK_RIGHT : 
                                     PM_DIRECT_FREE_KICK_RIGHT));
        }
    else {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "invalid team index for awarding free kick\n";
    }
}

void
SoccerRuleAspect::PunishIndirectKickGoal(
    boost::shared_ptr<oxygen::AgentAspect> agent, 
    TTeamIndex scoredOnTeamIdx)
{
    boost::shared_ptr<AgentState> agentState;
    if (!SoccerBase::GetAgentState(agent, agentState))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot get "
                "AgentState from an AgentAspect\n";
    }
    else
    {
        mIndirectKick = false;
        if (agentState->GetTeamIndex() == scoredOnTeamIdx) 
        {
            // We have an own goal on an indirect kick so award opponent a corner kick
            TTeamIndex opp = SoccerBase::OpponentTeam(agentState->GetTeamIndex());
            AwardCornerKick(opp);
        } else {
            // Award team who was scored on during indirect kick a goal kick
            AwardGoalKick(scoredOnTeamIdx);  
        }
    }
}

void
SoccerRuleAspect::PunishFreeKickFoul(
    boost::shared_ptr<oxygen::AgentAspect> agent)
{
    boost::shared_ptr<AgentState> agentState;
    if (!SoccerBase::GetAgentState(agent, agentState))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot get "
                "AgentState from an AgentAspect\n";
    }
    else
    {
        // Award the opponent team a free kick
        TTeamIndex opp = SoccerBase::OpponentTeam(agentState->GetTeamIndex());
        AwardFreeKick(opp, true);
    }
}

inline bool SoccerRuleAspect::WasLastKickFromFreeKick(
    boost::shared_ptr<oxygen::AgentAspect> &lastKicker)
{
    TTime kickTime;
    // notice that a kick is not necessarily an immediate action, it can
    // take some time...
    return mBallState->GetLastCollidingAgent(lastKicker, kickTime)
            && kickTime - mLastFreeKickKickTime < 0.1 // kick duration = 0.1
            && lastKicker == mLastFreeKickTaker;
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
            MoveAgent(agent_aspect, new_pos);
        }
    }
}

vector<SoccerRuleAspect::Foul>
SoccerRuleAspect::GetFouls() const
{
    return mFouls;
}

vector<SoccerRuleAspect::Foul>
SoccerRuleAspect::GetFoulsSince(unsigned index) const
{
    Foul f(index+1, (EFoulType)0, boost::shared_ptr<AgentState>());
    vector<SoccerRuleAspect::Foul>::const_iterator low = lower_bound(mFouls.begin(), mFouls.end(), f);
    return vector<Foul>(low, mFouls.end());
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

    // After a drop ball allow anyone to touch it
    ResetKickChecks();
 
    mGameState->SetPlayMode(PM_PlayOn);
}

void
SoccerRuleAspect::UpdateBeforeKickOff()
{
    // get game control server to check agent count
    boost::shared_ptr<GameControlServer> game_control;

    if (!SoccerBase::GetGameControlServer(*this, game_control))
    {
        return;
    }

    // if no players are connected, just return
    if (! game_control->GetAgentCount()) return;


    // before the game starts the ball should stay in the middle of
    // the playing field
    Vector3f pos(0,0,mBallRadius);
    MoveBall(pos);

    mGameState->SetPaused(true);
    ClearPlayers(mRightHalf, mFreeKickMoveDist, TI_LEFT);
    ClearPlayers(mLeftHalf, mFreeKickMoveDist, TI_RIGHT);

#if 0
    //
    // TODO: this has to be tested (compiles and no crashes at least)
    mInOffsideLeftPlayers.clear();
    mInOffsideRightPlayers.clear();
#endif

    float kickOffWaitTime = 0;
    if (mAutoKickOffTimeOrigin > mGameState->GetModeTime())
        mAutoKickOffTimeOrigin = mGameState->GetModeTime();
    else
        kickOffWaitTime = mGameState->GetModeTime() - mAutoKickOffTimeOrigin;

    if (mAutomaticKickOff && kickOffWaitTime > mWaitBeforeKickOff)
    {
        mGameState->KickOff();
    }
}

void
SoccerRuleAspect::UpdateKickOff(TTeamIndex idx)
{
    mGameState->SetPaused(false);

    ResetKickChecks();
    mAllowKickOffTeamToScore = true;

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
        boost::shared_ptr<GameControlServer> game_control;
        if ((SoccerBase::GetGameControlServer(*this, game_control)
             && game_control->GetAgentCount() > 2) // todo: remove this when there is a "penalty" playmode
            && !mPenaltyShootout)
        {
            SetKickTakenValues(time, agent, true);
            mLastKickOffTaker = agent;
            mAllowKickOffTeamToScore = false;
        }
        mGameState->SetPlayMode(PM_PlayOn);
    }
}

void
SoccerRuleAspect::UpdateKickIn(TTeamIndex idx)
{
    mGameState->SetPaused(false);

    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        //mGameState->SetPaused(true);
        mRepelPlayersForKick = true;
        return;
    }

    ResetKickChecks();

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
    if (time > lastChange + mKickInPauseTime && !mRepelPlayersForKick)
    {
        SetKickTakenValues(time, agent, true);
        mGameState->SetPlayMode(PM_PlayOn);
    } else
    {
        RepelPlayersForKick(idx);

        // move the ball back on the ground where it left the playing
        // field
        MoveBall(mFreeKickPos);
    }
}


//-----------------------------------


void
SoccerRuleAspect::UpdateFreeKick(TTeamIndex idx, bool indirect)
{
    mGameState->SetPaused(false);

    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        //mGameState->SetPaused(true);
        mRepelPlayersForKick = true;
        return;
    }
//---------------

    ResetKickChecks();
    
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
    if (time > lastChange + mKickInPauseTime && !mRepelPlayersForKick)
    {
        SetKickTakenValues(time, agent, indirect);
        mGameState->SetPlayMode(PM_PlayOn);
        //GetLog()->Error() << "ERROR: (SoccerRuleAspect) " << "Set Playmode to playon\n";
    } else
    {
        RepelPlayersForKick(idx);
 
        // move the ball back on the ground where it left the playing
        // field
        MoveBall(mFreeKickPos);
    }
}

//-----------------------------------



void
SoccerRuleAspect::UpdateGoalKick(TTeamIndex idx)
{
    mGameState->SetPaused(false);

    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        //mGameState->SetPaused(true);
        mRepelPlayersForKick = true;
        return;
    }

    ResetKickChecks();

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
    if (time > lastChange + mKickInPauseTime && !mRepelPlayersForKick)
    {
        Vector2f pos(mBallBody->GetPosition().x(),
                     mBallBody->GetPosition().y());
        if ((idx == TI_RIGHT && !mRightPenaltyArea.Contains(pos)) ||
            (idx == TI_LEFT && !mLeftPenaltyArea.Contains(pos)) ||
            (idx == TI_NONE))
        {
            // Additional check that we don't have ball sitting on endline
            // of goal box and accidentally think that the ball has left the
            // goal box but is still in bounds
            if (!mBallState->GetBallOnField() ||
                (idx == TI_RIGHT && (pos.x() < mRightPenaltyArea.minVec[0] || pos.y() < mRightPenaltyArea.minVec[1] || pos.y() > mRightPenaltyArea.maxVec[1])) ||
                (idx == TI_LEFT && (pos.x() > mLeftPenaltyArea.maxVec[0] || pos.y() < mLeftPenaltyArea.minVec[1] || pos.y() > mLeftPenaltyArea.maxVec[1])) ||
                (idx == TI_NONE))
            {
                SetKickTakenValues(time, agent, false);
                mGameState->SetPlayMode(PM_PlayOn);
            }
        }
        return;
    }
    // the ball was not touched yet
    else
    {
        RepelPlayersForKick(idx);
        
        // move the ball back on the free kick position
        MoveBall(mFreeKickPos);
    }
}

void
SoccerRuleAspect::UpdateCornerKick(TTeamIndex idx)
{
    mGameState->SetPaused(false);

    // do nothing for the duration of mKickInPauseTime
    if (mGameState->GetModeTime() < mKickInPauseTime)
    {
        //mGameState->SetPaused(true);
        mRepelPlayersForKick = true;
        return;
    }

    ResetKickChecks();

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
    if (time > lastChange + mKickInPauseTime && !mRepelPlayersForKick)
    {
        SetKickTakenValues(time, agent, false);
        mGameState->SetPlayMode(PM_PlayOn);
    } else
    {
        RepelPlayersForKick(idx);
        
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
        mFreeKickPos[2] = mBallRadius;

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
        //bool ball_up = (ball_pos[1] < 0);

        // handle corner / goal kick
        if (salt::gAbs(ball_pos.x()) >= mFieldLength / 2)
        {
            // check goal kick right team
            if (last_touch_left && !ball_left)
            {
                AwardGoalKick(TI_RIGHT);
            }
            // check goal kick left team
            else if (!last_touch_left && ball_left)
            {
                AwardGoalKick(TI_LEFT);
            }
            // check corner kick right team
            else if (last_touch_left && ball_left)
            {
                AwardCornerKick(TI_RIGHT);
            }
            // check corner kick left team
            else
            {
                AwardCornerKick(TI_LEFT);
            }
        }
        // handle kick in
        else if (salt::gAbs(ball_pos.y()) >= mFieldWidth / 2)
        {
            // Award the opponent team a kick in
            TTeamIndex opp = SoccerBase::OpponentTeam(agentState->GetTeamIndex());
            AwardKickIn(opp);
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
        // sometimes, ball can't record goals due to approximation errors,
        // so we check for goals analytically
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
    
    /* Don't allow goals kicked from inside center circle directly after 
       kickoff
     */
    if (!mAllowKickOffTeamToScore) {
         boost::shared_ptr<AgentState> agentState;
         if (!SoccerBase::GetAgentState(mLastKickOffTaker, agentState))
             {
                 GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot"
                     " get AgentState from an AgentAspect\n";
             }
         else {
             TTeamIndex team = agentState->GetTeamIndex();
             if (idx != team) {
                 // Team that scored is same team that took kickoff
                 PunishIndirectKickGoal(mLastKickOffTaker, idx);
                 // Return true so that we know the ball is in the goal and 
                 // don't check for other conditions such as the ball being out
                 // of bounds
                 return true;
             }
         }
    }

    /* don't allow goals directly from kickoff (penalty shot kickoffs are 
     * direct)
     *
     * todo it is allowed in FIFA rules, so we should get rid of it e.g. by
     * adding noise to the beam effector so that kickoff kicks cannot be
     * precisely planned
     */
    boost::shared_ptr<AgentAspect> agent;
    boost::shared_ptr<GameControlServer> game_control;
    // Don't allow goals off indirect kicks or own goals off direct kicks
    if (WasLastKickFromFreeKick(agent)) {
        boost::shared_ptr<AgentState> agentState;
        if (!SoccerBase::GetAgentState(agent, agentState))
             {
                 GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot"
                     " get AgentState from an AgentAspect\n";
             }
        else if ((mIndirectKick || agentState->GetTeamIndex() == idx)
            && (SoccerBase::GetGameControlServer(*this, game_control)
            && game_control->GetAgentCount() > 2) // todo: remove this when there is a "penalty" playmode
            && !mPenaltyShootout) 
            {
                PunishIndirectKickGoal(agent, idx);
                // Return true so that we know the ball is in the goal and don't
                // check for other conditions such as the ball being out of 
                // bounds
                return true;
            }
    }

    // score the lucky team
    mGameState->ScoreTeam((idx == TI_LEFT) ? TI_RIGHT : TI_LEFT);
    mGameState->SetPlayMode((idx == TI_LEFT) ? PM_Goal_Right : PM_Goal_Left);

    return true;
}

void
SoccerRuleAspect::ResetKickChecks()
{
    mCheckFreeKickKickerFoul = false;
    mIndirectKick = false;
}

void
SoccerRuleAspect::SetKickTakenValues(TTime time,  
                                     boost::shared_ptr<oxygen::AgentAspect> agent, 
                                     bool indirect)
{
    mCheckFreeKickKickerFoul = true;
    mLastFreeKickKickTime = time;
    mLastFreeKickTaker = agent;
    mIndirectKick = indirect;
}

bool
SoccerRuleAspect::CheckFreeKickTakerFoul()
{
    if (!mCheckFreeKickKickerFoul)
        return false;

    boost::shared_ptr<AgentAspect> agent;
    if (!WasLastKickFromFreeKick(agent)) // second kick
    {
        mCheckFreeKickKickerFoul = false;
        mIndirectKick = false;
        if (agent == mLastFreeKickTaker)
        {
            PunishFreeKickFoul(mLastFreeKickTaker);
            return true;
        }
    }
    return false;
}

void
SoccerRuleAspect::UpdatePlayOn()
{
    mGameState->SetPaused(false);

    // check that player who took free kick doesn't touch the ball a second
    // time before another agent touches the ball
    if (CheckFreeKickTakerFoul())
    {
        return;
    }

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
    mGameState->SetPaused(true);

    // check if the pause time after the goal has elapsed
    if (mGameState->GetModeTime() < mGoalPauseTime)
    {
        return;
    }

    // put the ball back in the middle of the playing field
    Vector3f pos(0, 0, mBallRadius);
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
    mGameState->SetPaused(false);

    // wait for 10 seconds to finish
    if (mGameState->GetModeTime() < 9 || !mAutomaticQuit)
    {
        return;
    }
    mGameState->Finish();

    if (mGameState->GetModeTime() >= 10)
    {
        boost::shared_ptr<GameControlServer> gameControlServer =
            dynamic_pointer_cast<GameControlServer>(GetCore()->Get("/sys/server/gamecontrol"));
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
            if (mChangeSidesInSecondHalf)
                SwapTeamSides();
        }
    }
    else if ((half == GH_SECOND) && (now >= 2 * mHalfTime))
    {
        // the game is over
        mGameState->SetPlayMode(PM_GameOver);
    }
}

#ifdef RVDRAW
void 
SoccerRuleAspect::DrawVelocities(TTeamIndex idx)
{
    if (mBallState.get() == 0 || !mRVSender)
        return;
       
    SoccerBase::TAgentStateList agent_states;
    if (!SoccerBase::GetAgentStates(*mBallState.get(), agent_states, idx))
        return;

    SoccerBase::TAgentStateList::iterator asIt = agent_states.begin();
    for (; asIt != agent_states.end(); ++asIt)
    {
        boost::shared_ptr<Transform> transform_parent;
        boost::shared_ptr<RigidBody> agent_body;
        SoccerBase::GetTransformParent(*(*asIt), transform_parent);
        SoccerBase::GetAgentBody(transform_parent, agent_body);
        
        // Get agent values
        int uNum = (*asIt)->GetUniformNumber();
        salt::Vector3f agentPos = agent_body->GetPosition();

        // Get average agent velocity
        salt::Vector3f agentAvgVel = salt::Vector3f(0,0,0);
        for(int j = 0; j < AVERAGE_VELOCITY_MEASUREMENTS; j++)
        {
            agentAvgVel.x() = agentAvgVel.x() + playerVelocities[uNum][idx][j].x();
            agentAvgVel.y() = agentAvgVel.y() + playerVelocities[uNum][idx][j].y();
            agentAvgVel.z() = agentAvgVel.z() + playerVelocities[uNum][idx][j].z();
        }

        agentAvgVel.x() = agentAvgVel.x()/AVERAGE_VELOCITY_MEASUREMENTS;
        agentAvgVel.y() = agentAvgVel.y()/AVERAGE_VELOCITY_MEASUREMENTS;
        agentAvgVel.z() = agentAvgVel.z()/AVERAGE_VELOCITY_MEASUREMENTS;

        float speed = sqrt(pow(agentAvgVel.x(), 2) + pow(agentAvgVel.y(), 2));
        
        RVSender::Color c = RVSender::MAGENTA;
        if (speed < mChargingMinSpeed) {
            c = RVSender::GREEN;
        }
        
        if (playerNotStanding[uNum][idx] || playerTimeSinceLastWasMoved[uNum][idx] < 1/.02) {
            c = RVSender::VIOLET;
        }
        
        bool goalieInOwnPenaltyArea = false;
        if (uNum == 1) 
        {
            // Could use playerInsideOwnArea[uNum][idx], however prefer to stretch the area for a goalie behind the goal line
            if (idx == TI_LEFT) {
                if (agentPos.x() <= mLeftPenaltyArea.maxVec[0] && agentPos.y() >= mLeftPenaltyArea.minVec[1] && agentPos.y() <= mLeftPenaltyArea.maxVec[1]) {
                    goalieInOwnPenaltyArea = true;
                }
            } else if (idx == TI_RIGHT) {
                if (agentPos.x() >= mRightPenaltyArea.minVec[0] && agentPos.y() >= mRightPenaltyArea.minVec[1] && agentPos.y() <= mRightPenaltyArea.maxVec[1]) {
                    goalieInOwnPenaltyArea = true;
                }
            }
        }

        bool shade = goalieInOwnPenaltyArea || playerTimeSinceLastBallTouch[uNum][idx] < mChargingImmunityTime/0.02 || playerChargingTime[uNum][idx] < mChargingCollisionMinTime/0.02;
        char buf[512];
        sprintf(buf, "Vel_%s", idx == TI_LEFT ? "Left" : "Right");
        string label = string(buf);
        mRVSender->drawLine(label, agentPos.x(), agentPos.y(), agentPos.x()+agentAvgVel.x(), agentPos.y()+agentAvgVel.y(), c, shade);
    }
}
#endif // RVDRAW

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

    if (!mAllowKickOffTeamToScore) {
        // Check if requirements/rules have been met for team taking kickoff 
        // to score
        boost::shared_ptr<AgentState> agentState;
        if (!SoccerBase::GetAgentState(mLastKickOffTaker, agentState))
            {
                GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot"
                    " get AgentState from an AgentAspect\n";
            }
        else {
            TTeamIndex team = agentState->GetTeamIndex();
            bool ballTouchedByKickOffTeam = 
                mBallState->GetBallCollidingWithAgentTeam(team);
            bool ballTouchedByNonKickOffTeam = 
                mBallState->GetBallCollidingWithAgentTeam(SoccerBase::OpponentTeam(team));
            salt::Vector2f ball_pos(mBallBody->GetPosition().x(), mBallBody->GetPosition().y());
            bool ballOutsideCenterCircle = ball_pos.Length() > mFreeKickDist;
            mAllowKickOffTeamToScore = ballTouchedByNonKickOffTeam ||
                (ballTouchedByKickOffTeam && ballOutsideCenterCircle);
        }
    }

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
        UpdateFreeKick(TI_LEFT, true);
        break;
    case PM_FREE_KICK_RIGHT:
        UpdateFreeKick(TI_RIGHT, true);
        break;

    case PM_DIRECT_FREE_KICK_LEFT:
        UpdateFreeKick(TI_LEFT, false);
        break;
    case PM_DIRECT_FREE_KICK_RIGHT:
        UpdateFreeKick(TI_RIGHT, false);
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
        ClearPlayersBeforeKickOff(TI_RIGHT);
        UpdateGoal();
        break;
    case PM_Goal_Right:
        ClearPlayersBeforeKickOff(TI_LEFT);
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

    //Get velocity to calculate averages
    SoccerBase::TAgentStateList agent_states;
    if (!SoccerBase::GetAgentStates(*mBallState.get(), agent_states))
        return;
    
    SoccerBase::TAgentStateList::iterator asIt = agent_states.begin();
    for (; asIt != agent_states.end(); ++asIt)
    {
        boost::shared_ptr<Transform> transform_parent;
        boost::shared_ptr<RigidBody> agent_body;
        SoccerBase::GetTransformParent(*(*asIt), transform_parent);
        SoccerBase::GetAgentBody(transform_parent, agent_body);
        int unum;
        int idx;
        unum = (*asIt)->GetUniformNumber();
        idx = (*asIt)->GetTeamIndex();
        
        for(int i = AVERAGE_VELOCITY_MEASUREMENTS-1; i > 0; i--)
        {
            playerVelocities[unum][idx][i] = playerVelocities[unum][idx][i-1];
        }

        playerVelocities[unum][idx][0] = agent_body->GetVelocity();
            
    }
     
#ifdef RVDRAW
    if (mRVSender) {
        mRVSender->clear();
        DrawVelocities(TI_LEFT);
        DrawVelocities(TI_RIGHT);
        mRVSender->refresh();
    }
#endif // RVDRAW

    // Simple Referee
    AutomaticSimpleReferee();
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
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);
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
    SoccerBase::GetSoccerVar(*this,"KickRepelDist",mKickRepelDist);
    SoccerBase::GetSoccerVar(*this,"GoalKickDist",mGoalKickDist);
    SoccerBase::GetSoccerVar(*this,"AutomaticKickOff",mAutomaticKickOff);
    SoccerBase::GetSoccerVar(*this,"WaitBeforeKickOff",mWaitBeforeKickOff);
    SoccerBase::GetSoccerVar(*this,"PenaltyShootout",mPenaltyShootout);
    SoccerBase::GetSoccerVar(*this,"SingleHalfTime",mSingleHalfTime);
    SoccerBase::GetSoccerVar(*this,"AutomaticQuit",mAutomaticQuit);
    SoccerBase::GetSoccerVar(*this,"ChangeSidesInSecondHalf",mChangeSidesInSecondHalf);
    SoccerBase::GetSoccerVar(*this,"UseOffside",mUseOffside);
    float penaltyLength, penaltyWidth;
    SoccerBase::GetSoccerVar(*this,"PenaltyLength",penaltyLength);
    SoccerBase::GetSoccerVar(*this,"PenaltyWidth",penaltyWidth);

    // keepaway mode parameters
    SoccerBase::GetSoccerVar(*this,"Keepaway",mKeepaway);
    SoccerBase::GetSoccerVar(*this,"KeepawayCenterX",mKeepawayCenterX);
    SoccerBase::GetSoccerVar(*this,"KeepawayCenterY",mKeepawayCenterY);
    SoccerBase::GetSoccerVar(*this,"KeepawayLength",mKeepawayLength);
    SoccerBase::GetSoccerVar(*this,"KeepawayWidth",mKeepawayWidth);
    SoccerBase::GetSoccerVar(*this,"KeepawayLengthReductionRate",mKeepawayLengthReductionRate);
    SoccerBase::GetSoccerVar(*this,"KeepawayWidthReductionRate",mKeepawayWidthReductionRate);

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
    //SoccerBase::GetSoccerVar(*this,"MaxFoulTime",mMaxFoulTime);
    SoccerBase::GetSoccerVar(*this,"UseCharging",mUseCharging);
    SoccerBase::GetSoccerVar(*this,"ChargingMinSpeed",mChargingMinSpeed);
    SoccerBase::GetSoccerVar(*this,"ChargingMinBallSpeedAngle",mChargingMinBallSpeedAngle);
    SoccerBase::GetSoccerVar(*this,"ChargingMinDeltaDist",mChargingMinDeltaDist);
    SoccerBase::GetSoccerVar(*this,"ChargingMinDeltaAng",mChargingMinDeltaAng);
    SoccerBase::GetSoccerVar(*this,"ChargingImmunityTime",mChargingImmunityTime);
    SoccerBase::GetSoccerVar(*this,"ChargingCollisionMinTime",mChargingCollisionMinTime);
    SoccerBase::GetSoccerVar(*this,"ChargingMaxBallRulesDist",mChargingMaxBallRulesDist);
    SoccerBase::GetSoccerVar(*this,"ChargingMinCollBallDist",mChargingMinCollBallDist);
    SoccerBase::GetSoccerVar(*this,"ChargingMinCollisionSpeed",mMinCollisionSpeed);
    SoccerBase::GetSoccerVar(*this,"FoulHoldTime",mFoulHoldTime);


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

    std::string team = "";

    for (
        SoccerBase::TAgentStateList::const_iterator it = agent_states.begin();
        it != agent_states.end();
        it++
        )
    {
        // Get name of team to label all messages with
        team = (*it)->GetPerceptName(ObjectState::PT_Player);
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
            (*it)->AddMessage(message, team, direction, true);
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
            (*it)->AddMessage(message, team, direction, false);
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
        mRepelPlayersForKick = true;
        return;
    }

    ResetKickChecks();

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
    if (time > lastChange && collidingAgentIdx==idx && !mRepelPlayersForKick)
    {
        mGameState->SetPlayMode(PM_PlayOn);
    }
    else
    {
        RepelPlayersForKick(idx);
        
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
            MoveAgent(agent_aspect, new_pos);
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


bool
SoccerRuleAspect::MoveAgent(boost::shared_ptr<Transform> agent_aspect, const Vector3f& pos)
{
    boost::shared_ptr<AgentState> agentState;
    if (!SoccerBase::GetAgentState(agent_aspect, agentState))
    {
        GetLog()->Error() << "ERROR: (SoccerRuleAspect) Cannot get "
            "AgentState from an AgentAspect\n";
    }
    else
    {
        int unum = agentState->GetUniformNumber();
        int idx = agentState->GetTeamIndex();
        playerTimeSinceLastWasMoved[unum][idx] = 0; 
    }

    return SoccerBase::MoveAgent(agent_aspect, pos);
}

bool
SoccerRuleAspect::HaveEnforceableFoul(int unum, TTeamIndex ti)
{
    return playerLastFoul[unum][ti] != FT_None
        && (playerFoulTime[unum][ti] > mMaxFoulTime / 0.02
            || playerLastFoul[unum][ti] == FT_Charging
            || playerLastFoul[unum][ti] == FT_Touching);
}
