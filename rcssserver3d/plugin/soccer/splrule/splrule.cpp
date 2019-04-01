/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
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
#include "splrule.h"
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
using namespace spl;

SPLRule::SPLRule() : SoccerRuleAspect(),
  mReadyDuration(45),
  mSetDuration(10)
{
	//mSayMsgSize = 20; // default is the same as in S3D
}

SPLRule::~SPLRule()
{
}

void SPLRule::OnLink()
{
  SoccerRuleAspect::OnLink();

  SoccerBase::GetSoccerVar(*this,"SayMsgSize",mSayMsgSize);
  GetLog()->Normal() << "SayMsgSize = " << mSayMsgSize <<"\n";

  GetControlAspect(mState, "GameStateAspect");

  if (mState.expired())
  {
    GetLog()->Error()
            << "(SPLRule) ERROR: could not get SPLState\n";
  }
}

void SPLRule::OnUnlink()
{
  SoccerRuleAspect::OnUnlink();
  mState.reset();
}

void SPLRule::UpdateCachedInternal()
{
    SoccerRuleAspect::UpdateCachedInternal();

    mFieldRightHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0f),
                                  Vector2f(mFieldLength/2.0f, mFieldWidth/2.0f));
    mFieldLeftHalf = salt::AABB2(Vector2f(0, -mFieldWidth/2.0f),
                                 Vector2f(-mFieldLength/2.0f, mFieldWidth/2.0f));
    mFieldRightHalfDefense = salt::AABB2(Vector2f(1.2f, -mFieldWidth/2.0f),
                                         Vector2f(mFieldLength/2.0f, mFieldWidth/2.0f));
    mFieldLeftHalfDefense = salt::AABB2(Vector2f(-1.2f, -mFieldWidth/2.0f),
                                        Vector2f(-mFieldLength/2.0f, mFieldWidth/2.0f));
    mCenterCircle = salt::AABB2(Vector2f(-mFreeKickDist, -mFreeKickDist),
                                        Vector2f(mFreeKickDist, mFreeKickDist)); // approx. of the center circle
    mWholeField = salt::AABB2(Vector2f(mFieldLength/2.0f+0.7f, -mFieldWidth/2.0f-0.7f),
                              Vector2f(-mFieldLength/2.0f-0.7f, mFieldWidth/2.0f+0.7f));
}

void SPLRule::Update(float /*deltaTime*/)
{
    if (
        (mState.get() == 0) ||
        (mBallState.get() == 0) ||
        (mBallBody.get() == 0)
        )
    {
        return;
    }

    TSPLState state = mState->GetState();
    switch (state)
    {
    case Unknown:
    {
      UpdateCachedInternal();
      mState->SetState(Initial);

      SoccerBase::GetSoccerVar(*this,"ReadyDuration",mReadyDuration);
      SoccerBase::GetSoccerVar(*this,"SetDuration",mSetDuration);
	  
      // debug ----
      /*float factor = 0.1;
      mReadyDuration *= factor;
      mSetDuration *= factor;
      mHalfTime *= factor;*/
      // ----------
    }
      break;
    case Initial:
      UpdateInitialKickOff();
      break;
    case Ready:
      UpdateReady();
      break;
    case Set:
      UpdateSet();
      break;
    case Playing:
      UpdatePlaying();
      break;
    case Finished:
      UpdateFinish();
      break;
    }

	SoccerBase::GetSoccerVar(*this,"ReadyDuration",mReadyDuration);

    CheckTime();
    mState->UpdateKickOff();
}

void SPLRule::UpdateInitialKickOff()
{
  if(!mdropBallForced) {
    HideBall();
  }
  //all robots must be in the initial state and must be placed on the
  //sidelines in their own half of the field

  // auto kick off
  if (mAutomaticKickOff && mState->GetStateTime() > mWaitBeforeKickOff)
  {
      mGameState->KickOff();
  }
}

// for testing
void SPLRule::Test(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0) {
      return;
    }

    SoccerBase::TAgentStateList robots;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), robots, idx)) {
      return;
    }

    for (SoccerBase::TAgentStateList::const_iterator i = robots.begin();
         i != robots.end(); ++i)
    {
      RemoveRobot(*i);
    }
}

void SPLRule::RemoveRobot(boost::shared_ptr<AgentState> robot)
{
  // move the robot to outside of the field
  // for penalized robots

  // Choose x side based on team
  float xFac = (robot->GetTeamIndex() == TI_LEFT) ? -0.5f : 0.5f;
  salt::Vector3f pos(xFac * robot->GetUniformNumber(), -1.f*(mFieldWidth / 2.f + 1.f), 0.4f);

  boost::shared_ptr<oxygen::Transform> agent_aspect;
  SoccerBase::GetTransformParent(*robot, agent_aspect);
  //float height = agent_aspect->GetWorldTransform().Pos().z();

  SoccerBase::MoveAndRotateAgent(agent_aspect, pos, -180);

  robot->Penalize(mGameState->GetTime());

}


void SPLRule::UnpenalizeRobot(boost::shared_ptr<AgentState> robot)
{
  // move the robot back to the field

  // Choose x side based on team
  float xFac = (robot->GetTeamIndex() == TI_LEFT) ? -0.5f : 0.5f;
  salt::Vector3f pos(xFac * robot->GetUniformNumber(), -1.f*(mFieldWidth / 2.f - 0.4f), 0.4f);

  boost::shared_ptr<oxygen::Transform> agent_aspect;
  SoccerBase::GetTransformParent(*robot, agent_aspect);
  //float height = agent_aspect->GetWorldTransform().Pos().z();

  SoccerBase::MoveAndRotateAgent(agent_aspect, pos, -180);

  robot->UnPenalize();

}


void SPLRule::UpdateReady()
{
  if(!mdropBallForced) {
    HideBall();
  }

  if (mState->GetStateTime() > mReadyDuration)
  {
    mState->SetState(Set);
  }
}

float findFreePosition(float minPos, float maxPos, const vector<float>& others)
{
    set<float> sorted;
    sorted.insert(minPos);
    sorted.insert(maxPos);
    for(vector<float>::const_iterator iter=others.begin(); iter!=others.end(); ++iter)
    {
        sorted.insert(*iter);
    }

    float maxDist = 0;
    float freePos = (minPos + maxPos) * 0.5;
    set<float>::const_iterator iterA = sorted.begin();
    set<float>::const_iterator iterB = sorted.begin();
    ++iterB;
    for(; iterB!=sorted.end(); ++iterA, ++iterB)
    {
        float d = *iterB - *iterA;
        if (d > maxDist)
        {
            freePos = (*iterB + *iterA) * 0.5;
            maxDist = d;
        }
    }
    return freePos;
}

Vector3f SPLRule::GetRobotBodyPos(boost::shared_ptr<AgentState> robot)
{
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::GetTransformParent(*robot, agent_aspect);
    boost::shared_ptr<RigidBody> body;
    SoccerBase::GetAgentBody(agent_aspect, body);
    return body->GetPosition();
}

void SPLRule::ManualPlaceRobot(boost::shared_ptr<AgentState> robot)
{
    TTeamIndex idx = robot->GetTeamIndex();
    float angle = -90 * (idx==TI_RIGHT?-1:1);
    float z = GetRobotBodyPos(robot).z();

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::GetTransformParent(*robot, agent_aspect);

    if (robot->GetUniformNumber() == 1) // goalie
    {
        float xLine = (-mFieldLength/2) * (idx==TI_RIGHT?-1:1);
        Vector3f pos(xLine, 0, z);
        SoccerBase::MoveAndRotateAgent(agent_aspect, pos, angle);
        return;
    }

    float xMin = -mFieldLength/2;
    float xLine = mLeftPenaltyArea.maxVec.x() + 0.3;
    float xMax = xLine + (xLine - xMin);
    if (TI_RIGHT==idx)
    {
        xMin = -xMax;
        xLine *= -1;
        xMax = mFieldLength/2;
    }

    // find y position of other robots inside placement area
    Vector2f mn(xMin, -mFieldWidth/2);
    Vector2f mx(xMax, mFieldWidth/2);
    salt::AABB2 placementArea(mn, mx);
    SoccerBase::TAgentStateList robots = FindRobotsIn(placementArea);
    vector<float> yRobots;
    for(SoccerBase::TAgentStateList::const_iterator iter=robots.begin(); iter!=robots.end(); ++iter)
    {
        if (robot.get() != iter->get())
        {
            float y = GetRobotBodyPos(*iter).y();
            yRobots.push_back(y);
        }
    }

    float y = findFreePosition(-mFieldWidth/2, mFieldWidth/2, yRobots);

    Vector3f pos(xLine, y, z);
    SoccerBase::MoveAndRotateAgent(agent_aspect, pos, angle);
}

SoccerBase::TAgentStateList SPLRule::FindRobotsIn(const salt::AABB2& box, TTeamIndex idx)
{
    SoccerBase::TAgentStateList robots; // TODO: cache it?
    SoccerBase::TAgentStateList robotsIn;
    if ( mBallState.get() == 0 )
        return robotsIn;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), robots, idx))
        return robotsIn;

    for (SoccerBase::TAgentStateList::const_iterator i = robots.begin(); i != robots.end(); ++i)
    {
        Vector3f pos = GetRobotBodyPos(*i); // using bounding box?
        if (box.Contains(Vector2f(pos.x(),pos.y())))
        {
            robotsIn.push_back(*i);
        }
    }
    return robotsIn;
}

void SPLRule::UpdateSet()
{
    // TODO: it may not correct for the second half kickoff

    //static TTime lastTimeBeforePlacement = mGameState->GetTime();
    //static float range = (float)salt::NormalRNG<>(4.0, 1.5)();


    //TTime timeNow = mGameState->GetTime();
    //float crange = clamp(range, 1.8, mSetDuration-1);

    bool wasManual = 0;

    //if (timeNow - lastTimeBeforePlacement > crange) {

    // the ball doesn't get hidden after this state
    resetDropBallForced();

    SoccerBase::TAgentStateList agentStates;
    if (mBallState.get() == 0)
        return;

    if (! SoccerBase::GetAgentStates(*mBallState.get(), agentStates, TI_NONE))
        return;

    for (SoccerBase::TAgentStateList::const_iterator i = agentStates.begin(); i != agentStates.end(); ++i)
    {
        if ( IsIllegalPosition(*i) )
        {
            ManualPlaceRobot(*i);
        }
    }

    MoveBall(Vector3f(0.0,0.0,0.0));

    if (mState->GetStateTime() > mSetDuration)
    {
        mState->SetState(Playing);
    }
}

bool SPLRule::IsLeavingTheField(boost::shared_ptr<AgentState> robot)
{
    Vector3f pos = GetRobotBodyPos(robot);
    return !mWholeField.Contains(Vector2f(pos.x(), pos.y()));
}

bool SPLRule::IsIllegalDefender(boost::shared_ptr<AgentState> robot)
{
    if ( robot->GetUniformNumber()==1 )
    {
        return false;
    }

    Vector3f pos = GetRobotBodyPos(robot);
    Vector2f pos2D = Vector2f(pos.x(), pos.y());
    TTeamIndex idx = robot->GetTeamIndex();

    if (idx == TI_LEFT) {
        //check if agent inside penalty area and move
        if (mLeftPenaltyArea.Contains(pos2D)) {
            return true;
        }
    }
    else if (idx == TI_RIGHT) {
        //check if agent inside penalty area and move
        if (mRightPenaltyArea.Contains(pos2D)) {
            return true;
        }
    }

    return false;
}

bool SPLRule::IsIllegalPosition(boost::shared_ptr<AgentState> robot)
{
    Vector3f pos = GetRobotBodyPos(robot);
    Vector2f pos2D = Vector2f(pos.x(), pos.y());
    TTeamIndex idx = robot->GetTeamIndex();

    if (robot->GetUniformNumber() == 1)
    {
        if (idx == TI_LEFT ) {
            //check if agent inside penalty area and move
            if (!mLeftPenaltyArea.Contains(pos2D)) {
                return true;
            }
        }
        else if (idx == TI_RIGHT ) {
            //check if agent inside penalty area and move
            if (!mRightPenaltyArea.Contains(pos2D)) {
                return true;
            }
        }
    }
    else
    {
		if (mGameState->GetPlayMode() == PM_KickOff_Left || mGameState->GetPlayMode() == PM_Goal_Right)
        {
            if (idx == TI_LEFT) {
                if (!mFieldLeftHalf.Contains(pos2D)) {
                    return true;
                }
            }
            else if (idx == TI_RIGHT) {
                if (!mFieldRightHalf.Contains(pos2D) || mCenterCircle.Contains(pos2D)) {
                    return true;
                }
            }
        }
        else if (mGameState->GetPlayMode() == PM_KickOff_Right || mGameState->GetPlayMode() == PM_Goal_Left)
        {
            if (idx == TI_LEFT) {
                if (!mFieldLeftHalf.Contains(pos2D) ||  mCenterCircle.Contains(pos2D)) {
                    return true;
                }
            }
            else if (idx == TI_RIGHT) {
                if (!mFieldRightHalf.Contains(pos2D)) {
                    return true;
                }
            }
        }
		else
		{
			GetLog()->Error() << "(SPLRule) ERROR: IsIllegalPosition cannot be only be used in state " << SoccerBase::PlayMode2Str(mGameState->GetPlayMode()) << "\n";
			assert(false);
		}
    }

    return IsIllegalDefender(robot);
}

void SPLRule::ManualPlacement(TTeamIndex /*idx*/) {

}

//void SPLRule:.checkRobotsIfUnpenalized(boost::shared_ptr<AgentState>, boost::shared_ptr<oxygen::Transform> agentAspectTrans, TTeamIndex idx) {


//}

void SPLRule::UpdatePlaying()
{

    // the ball doesn't get hidden after this state
    resetDropBallForced();

    if(CheckGoal()) {
        mState->SetState(Ready);
        return;
    }

    //
    //ball outside the field
    //
    static TTime lastTimeBallOnField = mGameState->GetTime();
    static salt::NormalRNG<> rangeRNG = salt::NormalRNG<>(2.0, 1.8);

    if (!mBallState->GetBallOnField()) {
      TTime timeNow = mGameState->GetTime();
      float crange = (float)rangeRNG();
      crange = salt::gClamp(crange, 0.5f, 5.0f);

      if (timeNow - lastTimeBallOnField > crange) {
          Vector3f ballPos = GetBallDropInPosition();
          std::cout << "(SPLRule) Put back after " << crange << " seconds" << std::endl;
          MoveBall(ballPos);
      }
    }
    else
    {
        lastTimeBallOnField = mGameState->GetTime();
    }

  //
  //check the rules for each robot
  //
    SoccerBase::TAgentStateList agentStates;

    if (mBallState.get() == 0)
        return;

    if (! (SoccerBase::GetAgentStates(*mBallState.get(), agentStates, TI_NONE)))
        return;

        
    for (SoccerBase::TAgentStateList::const_iterator i = agentStates.begin(); i != agentStates.end(); ++i)
    {
        if ( !(*i)->IsPenalized() )
        {
            if (IsLeavingTheField(*i))   // check player leave the field
            {
                RemoveRobot(*i);
                std::cout << "(SPLRule) team "<<(*i)->GetTeamIndex()<<" player No.  " << (*i)->GetUniformNumber() << " leaving the field" << std::endl;
            } 
			else if (IsIllegalDefender(*i)) // check illegal defender 
			{
				RemoveRobot(*i);
				std::cout << "(SPLRule) team "<<(*i)->GetTeamIndex()<<" player No.  " << (*i)->GetUniformNumber() << " illegal defender" << std::endl;
			}
        } 
		else if(mGameState->GetTime() > (*i)->GetWhenPenalized() + 30)
		{
			(*i)->UnPenalize();
		}
    }//end for

}

Vector3f SPLRule::GetBallDropInPosition()
{
	//TODO: those should be parameters
	const float dropInLineOffsetX = 1.0;
	const float dropInLineOffsetY = 0.4;
	const float dropInPenaltyShiftX = 1.0;

    boost::shared_ptr<AgentAspect> agentAspect;
    boost::shared_ptr<AgentState> agentState;
    TTime time;

	float newBallPosX = 0.0f, newBallPosY = 0.0f;

	// get the player who kicked out the ball
    if (mBallState->GetLastCollidingAgent(agentAspect,time) &&
        SoccerBase::GetAgentState(agentAspect,agentState))
    {
        //Player position
		boost::shared_ptr<oxygen::Transform> agentAspectTrans;
        SoccerBase::GetTransformParent(*agentState, agentAspectTrans);
        const Vector3f& agentPos = agentAspectTrans->GetWorldTransform().Pos();
		
		const salt::Vector3f ballPos = mBallBody->GetPosition();
		bool outByLeftLeft = (agentState->GetTeamIndex() == TI_LEFT);
		

		if (ballPos.y() < 0) { // out at the bottom of the field
			newBallPosY = -mFieldWidth/2.0f + dropInLineOffsetY;
		} else {
			newBallPosY = mFieldWidth/2.0f - dropInLineOffsetY;
		}

		
		// out at the side line
		if(fabs(ballPos.y()) >  mFieldWidth/2.0f) 
		{
			newBallPosX = agentPos.x() + (outByLeftLeft?-1.0f:+1.0f)*dropInPenaltyShiftX;
			newBallPosX = salt::gClamp(newBallPosX, -mFieldLength/2.0f + dropInLineOffsetX, -mFieldLength/2.0f + dropInLineOffsetX);
		}
		// else {}
		// NOTE: there is no special treatment for the case where the ball is out at the goal line
		//       so, it will be put at the position newBallPosX = 0

    //if outside without any collision by a robot??
    } else {
        //TODO: what to do here?
    }

	return Vector3f(newBallPosX, newBallPosY, mBallRadius);
} //getBallPositionAfterOutsideField()


void SPLRule::CheckTime()
{
    TTime now = mState->GetTime();
    TGameHalf half = mState->GetGameHalf();

    if ((half == GH_FIRST) && (now >= mHalfTime))
    {
        if (mSingleHalfTime)
        {
            // we want to play only one half of the match
            mState->SetState(Finished);
        } else {
            // the first game half is over
            mState->SetState(Initial);
            mState->SetGameHalf(GH_SECOND);
        }
    }
    else if ((half == GH_SECOND) && (now >= 2 * mHalfTime))
    {
        // the game is over
        mState->SetState(Finished);
    }
}

void SPLRule::UpdateFinish()
{
    // the ball doesn't get hidden after this state
    resetDropBallForced();

    if (mAutomaticQuit)
    {
        boost::shared_ptr<GameControlServer> gameControlServer =
            dynamic_pointer_cast<GameControlServer>(GetCore()->Get("/sys/server/gamecontrol"));
        gameControlServer->Quit();
    }
}


//TOOLS

void SPLRule::HideBall() {

    //HACK: making the Ball invisible
    //must outside the field, "check if outside and wait till put back" using it
    MoveBall(Vector3f(0,-500,500));
}

