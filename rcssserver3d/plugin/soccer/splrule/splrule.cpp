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

SPLRule::SPLRule() : SoccerRuleAspect(),
  mReadyDuration(45), //old 45
  mSetDuration(10) //old 10
{
}

SPLRule::~SPLRule()
{
}

void SPLRule::OnLink()
{
  SoccerRuleAspect::OnLink();

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
      break;
    }

    CheckTime();
}

void SPLRule::UpdateInitialKickOff()
{

  HideBall();
  //all robots must be in the initial state and must be placed on the
  //sidelines in their own half of the field

}

// for testing
void SPLRule::Test(TTeamIndex idx)
{
    if (idx == TI_NONE || mBallState.get() == 0)
        return;

    SoccerBase::TAgentStateList robots;
    if (! SoccerBase::GetAgentStates(*mBallState.get(), robots, idx))
        return;

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
  SoccerBase::MoveAndRotateAgent(agent_aspect, pos, -180);

  robot->Penalize(mGameState->GetTime());

}

void SPLRule::UpdateReady()
{
  HideBall();

  if (mState->GetStateTime() > mReadyDuration)
  {
    mState->SetState(Set);
  }
}

void SPLRule::UpdateSet()
{
    //static TTime lastTimeBeforePlacement = mGameState->GetTime();
    //static float range = (float)salt::NormalRNG<>(4.0, 1.5)();


    //TTime timeNow = mGameState->GetTime();
    //float crange = clamp(range, 1.8, mSetDuration-1);

    bool wasManual = 0;

    //if (timeNow - lastTimeBeforePlacement > crange) {

        //check position - otherwise manual
        if(wasManual = CheckIllegalPosition(TI_LEFT)) {
            ManualPlacement(TI_LEFT);
        //    wasManual = true;
        }
        if(wasManual = CheckIllegalPosition(TI_RIGHT)) {
            ManualPlacement(TI_RIGHT);
        //    wasManual = true;
        }
    //}

  MoveBall(Vector3f(0.0,0.0,0.0));

  if (mState->GetStateTime() > mSetDuration)
  {
    mState->SetState(Playing);
  }
}



bool SPLRule::CheckIllegalPosition(TTeamIndex idx) {

    boost::shared_ptr<oxygen::Transform> agentAspectTrans;
    SoccerBase::TAgentStateList agentStates;

    Vector3f agentPos;

    if (idx == TI_NONE || mBallState.get() == 0)
        return 0;

    if (! SoccerBase::GetAgentStates(*mBallState.get(), agentStates, idx))
        return 0;


    for (SoccerBase::TAgentStateList::const_iterator i = agentStates.begin(); i != agentStates.end(); ++i)
    {

        SoccerBase::GetTransformParent(**i, agentAspectTrans);
        agentPos = agentAspectTrans->GetWorldTransform().Pos();

        //boost::shared_ptr<AgentState> agentState;

        if (idx == TI_LEFT) {
            //check if agent inside penalty area and move
            if (!mLeftHalf.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                return true;
            }
        }        
        if (idx == TI_RIGHT) {
            //check if agent inside penalty area and move
            if (!mRightHalf.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                return true;
            }
        }

        //check goalie in box
        if (idx == TI_LEFT && (*i)->GetUniformNumber() == 1) {
            //check if agent inside penalty area and move
            if (!mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                return true;
            }
        }
        if (idx == TI_RIGHT && (*i)->GetUniformNumber() == 1) {
            //check if agent inside penalty area and move
            if (!mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                return true;
            }
        }
    }

    return false;

}

void SPLRule::ManualPlaceRobot(boost::shared_ptr<oxygen::Transform> agentAspectTrans, int number, bool left, bool haveKickOff) {

    number--;
    assert(number >= 0);

    // todo: is it correct?
    float site = left?-1.f:1.f;
    if (haveKickOff) {
        float xPos [4] = {2.9f, 2.4f, 0.7f, 1.2f};
        float yPos [4] = {0.0f, 1.1f, 0.0f, -1.1f};
        SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(xPos[number]*site,  yPos[number], 0.3f), 90.f*site);

    } else {
        float xPos [4] = {2.9f, 2.4f, 2.4f, 2.4f};
        float yPos [4] = {0.0f, 1.6f, 0.3f, -1.6f};
        SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(xPos[number]*site,  yPos[number], 0.3f), 90.f*site);
    }
}

void SPLRule::ManualPlacement(TTeamIndex idx) {

    boost::shared_ptr<oxygen::Transform> agentAspectTrans;
    SoccerBase::TAgentStateList agentStates;

    Vector3f agentPos;

    if (idx == TI_NONE || mBallState.get() == 0)
        return;

    if (! SoccerBase::GetAgentStates(*mBallState.get(), agentStates, idx))
        return;

    for (SoccerBase::TAgentStateList::const_iterator i = agentStates.begin(); i != agentStates.end(); ++i)
    {
        //Player infos
        SoccerBase::GetTransformParent(**i, agentAspectTrans);
        agentPos = agentAspectTrans->GetWorldTransform().Pos();

        if (idx == TI_LEFT) {
            ManualPlaceRobot(agentAspectTrans, (*i)->GetUniformNumber(), true, mGameState->GetPlayMode() == PM_KickOff_Left);
        } else {
            ManualPlaceRobot(agentAspectTrans, (*i)->GetUniformNumber(), false, mGameState->GetPlayMode() == PM_KickOff_Right);
        }

     }
}

//void SPLRule:.checkRobotsIfUnpenalized(boost::shared_ptr<AgentState>, boost::shared_ptr<oxygen::Transform> agentAspectTrans, TTeamIndex idx) {


//}

void SPLRule::UpdatePlaying()
{
  //
  //check the robots
  //
    //boost::shared_ptr<AgentAspect> agentAspect;
    boost::shared_ptr<oxygen::Transform> agentAspectTrans;
    SoccerBase::TAgentStateList agentStatesLeft;
    SoccerBase::TAgentStateList agentStatesRight;

    if (mBallState.get() == 0)
        return;

    if (! (SoccerBase::GetAgentStates(*mBallState.get(), agentStatesLeft, TI_LEFT) ||
          SoccerBase::GetAgentStates(*mBallState.get(), agentStatesRight, TI_RIGHT)))
        return;

    for (SoccerBase::TAgentStateList::const_iterator i = agentStatesLeft.begin(); i != agentStatesLeft.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agentAspectTrans);

        if((*i)->IsPenalized()) {
          CheckPenalized(agentAspectTrans, (*i), TI_LEFT);
        } else {
          CheckIllegalDefender(agentAspectTrans, (*i), TI_LEFT);
          CheckOutsideField(agentAspectTrans, (*i), TI_LEFT);
        }
    }

    for (SoccerBase::TAgentStateList::const_iterator i = agentStatesRight.begin(); i != agentStatesRight.end(); ++i)
    {
        SoccerBase::GetTransformParent(**i, agentAspectTrans);
        if((*i)->IsPenalized()) {
          CheckPenalized(agentAspectTrans, (*i), TI_RIGHT);
        } else {
          CheckIllegalDefender(agentAspectTrans,(*i), TI_RIGHT);
          CheckOutsideField(agentAspectTrans, (*i), TI_RIGHT);
        }
    }


  //
  //ball outside the field
  //

  static TTime lastTimeBallOnField = mGameState->GetTime();
  static float range = (float)salt::NormalRNG<>(2.0, 1.8)();

  if (!mBallState->GetBallOnField()) {

    //ball infos
    Vector3f ballPos = mBallState->GetLastValidBallPosition();
    std::cout << "ball: " << ballPos << std::endl;
    if(checkIfGoal(ballPos)) {
        std::cout << "ValidPose:" << mBallState->GetLastValidBallPosition() << std::endl;
        mState->SetState(Ready);
        return;
    }

    TTime timeNow = mGameState->GetTime();
    float crange = clamp(range, 0.5, 5.0);

    if (timeNow - lastTimeBallOnField > crange) {

        Vector3f ballPos = getBallPositionAfterOutsideField(ballPos);
        std::cout << "(SPLRule) Put back after " << crange << " seconds" << std::endl;
        MoveBall(ballPos);

    } else {
        HideBall();
    }


  } else {
    lastTimeBallOnField = mGameState->GetTime();
        //N(mean, sigma) - randomly put the ball back
    range = (float)salt::NormalRNG<>(2.0f, 1.8f)();
  }

}


void SPLRule::CheckPenalized(boost::shared_ptr<oxygen::Transform> agentAspectTrans, boost::shared_ptr<AgentState> agentState, TTeamIndex idx) 
{
  TTime now = mGameState->GetTime();

  if(agentState->IsPenalized() && now - agentState->getWhenPenalized() > 30) {
    Vector3f ballPos = mBallState->GetLastValidBallPosition();
    float side = ballPos.y() > 0?-1.f:1.f;
    salt::Vector3f pos(0, side*(mFieldWidth / 2.f), 0.4f);

    boost::shared_ptr<oxygen::Transform> agent_aspect;
    SoccerBase::GetTransformParent(*agentState, agent_aspect);
    SoccerBase::MoveAndRotateAgent(agent_aspect, pos, 180);

    agentState->UnPenalize();
  }
}//end CheckPenalized


void SPLRule::CheckIllegalDefender(boost::shared_ptr<oxygen::Transform> agentAspectTrans, boost::shared_ptr<AgentState> agentState, TTeamIndex idx) {
        Vector3f agentPos;

        agentPos = agentAspectTrans->GetWorldTransform().Pos();

        //boost::shared_ptr<AgentState> agentState;

        if (idx == TI_LEFT && agentState->GetUniformNumber() != 1) { //not goali
                    //check if agent inside penalty area and move
                    if (mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        RemoveRobot(agentState);
                        std::cout << "(SPLRule) Blue player No.  " << agentState->GetUniformNumber() << " leaving the Field" << std::endl;
                    }
                }
        if (idx == TI_RIGHT && agentState->GetUniformNumber() != 1) {
                    //check if agent inside penalty area and move
                    if (mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        RemoveRobot(agentState);
                        std::cout << "(SPLRule) Red player No.  " << agentState->GetUniformNumber() << " leaving the Field" << std::endl;
                    }
        }


}

void SPLRule::CheckOutsideField(boost::shared_ptr<oxygen::Transform> agentAspectTrans, boost::shared_ptr<AgentState> agentState, TTeamIndex idx) {

    Vector3f agentPos;

    agentPos = agentAspectTrans->GetWorldTransform().Pos();

    if (agentPos.x() < 0) agentPos.x() = -1 * agentPos.x();
    if (agentPos.y() < 0) agentPos.y() = -1 * agentPos.y();

    if (agentPos.y() > mFieldWidth/2+0.5 || agentPos.x() > mFieldLength/2+0.3) {
        RemoveRobot(agentState);
        if(idx == TI_LEFT) {
            std::cout << "(SPLRule) Blue player No. " << agentState->GetUniformNumber() << " leaving the Field" << std::endl;
        } else {
            std::cout << "(SPLRule) Red player No.  " << agentState->GetUniformNumber() << " leaving the Field" << std::endl;
        }
    }

}

bool SPLRule::checkIfGoal(Vector3f ballPos) 
{
/*
    if (fabs(ballPos.y()) < mGoalWidth / 2.0 ) {
        if (ballPos.x() > mFieldLength/2) {
            mGameState->ScoreTeam(TI_LEFT);
            std::cout << "(SPLRule) Goal by blue" << std::endl;
            return true;
        } else if (ballPos.x() < -mFieldLength/2) {
            mGameState->ScoreTeam(TI_RIGHT);
            std::cout << "(SPLRule) Goal by red " << std::endl;
            return true;
        }
    }

    return false;
    */

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

Vector3f SPLRule::getBallPositionAfterOutsideField(Vector3f ballPos) {

    boost::shared_ptr<AgentAspect> agentAspect;
    boost::shared_ptr<oxygen::Transform> agentAspectTrans;
    boost::shared_ptr<AgentState> agentState;
    Vector3f agentPos;
    TTime time;

    if (mBallState->GetLastCollidingAgent(agentAspect,time) &&
        SoccerBase::GetAgentState(agentAspect,agentState))
    {

        //Player infos
        SoccerBase::GetTransformParent(*agentState, agentAspectTrans);
        agentPos = agentAspectTrans->GetWorldTransform().Pos();

        //calculate constraints
        bool lastTouchRed = (agentState->GetTeamIndex() == TI_LEFT);
        bool ballOutBottom = (ballPos.y() < 0);
        //bool ballLeft = (ballPos.x() < 0);

        //calculate x, y
        float newX, newY;

        //out right by red
        if (ballPos.x() > 3.0 && lastTouchRed) {

            newX = (0 < agentPos.x()-1.0f) ? 0 : agentPos.x()-1.0f;
        //out left by blue
        } else if (ballPos.x() < 3.0 && !lastTouchRed) {

            newX = (0 > agentPos.x()+1.0f) ? 0 : agentPos.x()+1.0f;

        //out top/bottom
        } else {


            if(lastTouchRed) {
                std::cout << "(SPLRule) Out by blue team" << std::endl;
                    //calculate new position
                newX = (ballPos.x()-1.0f < agentPos.x()-1.0f) ? ballPos.x()-1.0f : agentPos.x()-1.0f;
                if (newX < -2.0f) newX = -2.0f;
            } else {
                std::cout << "(SPLRule) Out by red team" << std::endl;
                    //calculate new position
                newX = (ballPos.x()+1.0f > agentPos.x()+1.0f) ? ballPos.x()+1.0f : agentPos.x()+1.0f;
                if (newY > 2.0f) newY = 2.0f;
            } //lastTouchRed

        }// out top/bottom

        if (ballOutBottom) {
            newY = -1.80f;
        } else {
            newY = 1.80f;
        }

        return Vector3f(newX, newY, 0);

    //if outside without any collision by a robot??
    } else {
        //TODO: does it make sense?
        return Vector3f(0, 0, 0);
    }

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

//TOOLS

void SPLRule::MoveBall(Vector3f toPosition) {

    mBallBody->SetPosition(toPosition);
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
    mBallBody->Enable();

}

void SPLRule::HideBall() {

    //HACK: making the Ball invisible
    //must outside the field, "check if outside and wait till put back" using it
    mBallBody->SetPosition(Vector3f(0,-500,500));
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
    mBallBody->Disable();
}


float SPLRule::clamp(float val, const float min, const float max) {

    if (min <= max)
    {
        if (val<min) val=min;
        if (val>max) val=max;
    } else {
        if (val>=min || val<=max) return val;
        if (val>=(min+max)/2.0) val = min; else val = max;
    }
    return val;

}
