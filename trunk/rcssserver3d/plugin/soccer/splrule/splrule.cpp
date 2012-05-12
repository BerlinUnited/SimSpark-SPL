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
  mReadyDuration(15),
  mSetDuration(10)
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

  // TODO
  //Test(TI_LEFT);
  std::cout << "Width: " << mRightPenaltyArea.GetWidth() << std::endl;
  std::cout << "Height: " << mRightPenaltyArea.GetHeight() << std::endl;
  std::cout << "MiddleX: " << mRightPenaltyArea.GetMiddle().x() << std::endl;
  std::cout << "MiddleY: " << mRightPenaltyArea.GetMiddle().y() << std::endl;
  std::cout << "FieldLength: " << mFieldLength << std::endl;
  std::cout << "FieldWidth: " << mFieldWidth << std::endl;
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

void SPLRule::RemoveRobot(boost::shared_ptr<AgentState> robot) const
{
  // move the robot to outside of the field
  // for penalized robots

  // Choose x side based on team
  float xFac = (robot->GetTeamIndex() == TI_LEFT ? -1 : 1) * 0.5;
  salt::Vector3f pos = Vector3f(xFac * robot->GetUniformNumber(), (mFieldWidth / 2 + 1) *-1, 0.4);

  boost::shared_ptr<oxygen::Transform> agent_aspect;
  SoccerBase::GetTransformParent(*robot, agent_aspect);
  SoccerBase::MoveAndRotateAgent(agent_aspect, pos, -180);

  robot->isPenelized = true;

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

    //TODO

    CheckIllegalPosition(TI_LEFT);
    CheckIllegalPosition(TI_RIGHT);
    //otherwise manual


  MoveBall(Vector3f(-3.0,0,0));

  if (mState->GetStateTime() > mSetDuration)
  {
    mState->SetState(Playing);
  }
}

void SPLRule::CheckIllegalPosition(TTeamIndex idx) {
    //boost::shared_ptr<AgentAspect> agentAspect;
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

        //boost::shared_ptr<AgentState> agentState;

        if (idx == TI_LEFT) {
                    //check if agent inside penalty area and move
                    if (mRightHalf.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        manualPlacement(idx);
                    }
                }
        if (idx == TI_RIGHT) {
                    //check if agent inside penalty area and move
                    if (mLeftHalf.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        manualPlacement(idx);
                    }
        }

        //check goalie in box
        if (idx == TI_LEFT && (*i)->GetUniformNumber() == 1) {
                    //check if agent inside penalty area and move
                    if (!mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        manualPlacement(idx);
                    }
                }
        if (idx == TI_RIGHT && (*i)->GetUniformNumber() == 1) {
                    //check if agent inside penalty area and move
                    if (!mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        manualPlacement(idx);
                    }
        }

     }
}

void SPLRule::manualPlacement(TTeamIndex idx) {

    //boost::shared_ptr<AgentAspect> agentAspect;
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
            if (mGameState->GetPlayMode() == PM_KickOff_Right) {

                if((*i)->GetUniformNumber() == 1)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.9, 0.0, 0.3), -90);
                if((*i)->GetUniformNumber() == 2)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.4, 1.6, 0.3), -90);
                if((*i)->GetUniformNumber() == 3)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.4, 0.3, 0.3), -90);
                if((*i)->GetUniformNumber() == 4)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.4, -1.6, 0.3), -90);
            } else {
                if((*i)->GetUniformNumber() == 1)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.9, 0.0, 0.3), -90);
                if((*i)->GetUniformNumber() == 2)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-2.4, 1.1, 0.3), -90);
                if((*i)->GetUniformNumber() == 3)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-0.7, 0.0, 0.3), -90);
                if((*i)->GetUniformNumber() == 4)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(-1.2, -1.1, 0.3), -90);
            }
        }

        if (idx == TI_RIGHT) {
            if (mGameState->GetPlayMode() == PM_KickOff_Left) {
                if((*i)->GetUniformNumber() == 1)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.9, 0.0, 0.3), 90);
                if((*i)->GetUniformNumber() == 2)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.4, 1.6, 0.3), 90);
                if((*i)->GetUniformNumber() == 3)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.4, 0.3, 0.3), 90);
                if((*i)->GetUniformNumber() == 4)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.4, -1.6, 0.3), 90);
            } else {
                if((*i)->GetUniformNumber() == 1)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.9, 0.0, 0.3), 90);
                if((*i)->GetUniformNumber() == 2)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(2.4, 1.1, 0.3), 90);
                if((*i)->GetUniformNumber() == 3)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(0.7, 0.0, 0.3), 90);
                if((*i)->GetUniformNumber() == 4)
                    SoccerBase::MoveAndRotateAgent(agentAspectTrans, Vector3f(1.2, -1.1, 0.3), 90);
            }
        }

     }
}

void SPLRule::UpdatePlaying()
{

  //check agents (illegal defender)
  CheckIllegalDefender(TI_LEFT);
  CheckIllegalDefender(TI_RIGHT);


  //ball outside the field
  static TTime lastTimeBallOnField = mGameState->GetTime();
  static float range = salt::NormalRNG<>(2.0, 1.8)();

  if (!mBallState->GetBallOnField()) {

    //ball infos
    Vector3f ballPos = mBallState->GetLastValidBallPosition();

    if(checkIfGoal(ballPos)) {
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
    range = salt::NormalRNG<>(2.0, 1.8)();
  }

}

void SPLRule::CheckIllegalDefender(TTeamIndex idx) {

    //boost::shared_ptr<AgentAspect> agentAspect;
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

        //boost::shared_ptr<AgentState> agentState;

        if (idx == TI_LEFT && (*i)->GetUniformNumber() != 1) { //not goali
                    //check if agent inside penalty area and move
                    if (mLeftPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        RemoveRobot(*i);
                    }
                }
        if (idx == TI_RIGHT && (*i)->GetUniformNumber() != 1) {
                    //check if agent inside penalty area and move
                    if (mRightPenaltyArea.Contains(Vector2f(agentPos.x(), agentPos.y()))) {
                        RemoveRobot(*i);
                    }
        }


    }


}

bool SPLRule::checkIfGoal(Vector3f ballPos) {

    if (ballPos.y() > -650 &&  ballPos.y() < 650 ) {
        if (ballPos.x() > 0) {
            mGameState->ScoreTeam(TI_LEFT);
            std::cout << "(SPLRule) Goal by blue" << std::endl;
        } else {
            mGameState->ScoreTeam(TI_RIGHT);
            std::cout << "(SPLRule) Goal by red " << std::endl;
        }

        return true;
    }

    return false;

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

            newX = (0 < agentPos.x()-1.0) ? 0 : agentPos.x()-1.0;
        //out left by blue
        } else if (ballPos.x() < 3.0 && !lastTouchRed) {

            newX = (0 > agentPos.x()+1.0) ? 0 : agentPos.x()+1.0;

        //out top/bottom
        } else {


            if(lastTouchRed) {
                std::cout << "(SPLRule) Out by blue team" << std::endl;
                    //calculate new position
                newX = (ballPos.x()-1.0 < agentPos.x()-1.0) ? ballPos.x()-1.0 : agentPos.x()-1.0;
                if (newX < -2.0) newX = -2.0;
            } else {
                std::cout << "(SPLRule) Out by red team" << std::endl;
                    //calculate new position
                newX = (ballPos.x()+1.0 > agentPos.x()+1.0) ? ballPos.x()+1.0 : agentPos.x()+1.0;
                if (newY > 2.0) newY = 2.0;
            } //lastTouchRed

        }// out top/bottom

        if (ballOutBottom)
            newY = -1.80;
        else
            newY = 1.80;

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
