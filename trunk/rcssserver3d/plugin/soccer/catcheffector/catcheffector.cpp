/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#include "catchaction.h"
#include "catcheffector.h"
#include <salt/random.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <soccer/ballstateaspect/ballstateaspect.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/soccercontrolaspect/soccercontrolaspect.h>
#include <soccer/soccerruleaspect/soccerruleaspect.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

CatchEffector::CatchEffector()
    : oxygen::Effector(),
      mCatchMargin(1.00),mPlayerRadius(0.0),mBallRadius(0.0)
{
}

CatchEffector::~CatchEffector()
{
}

void
CatchEffector::MoveBall(const Vector3f& pos)
{
    mBallBody->SetPosition(pos);
    mBallBody->SetVelocity(Vector3f(0,0,0));
    mBallBody->SetAngularVelocity(Vector3f(0,0,0));
}

void
CatchEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // this should also include the case when there is no ball
    // (because then there will be no body, neither).
    if (mAction.get() ==0 || mBallBody.get() == 0)
    {
        return;
    }

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) parent node is not derived "
            << "from BaseNode\n";
        return;
    }

    if (mAgentState.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) parent node is not derived "
            << "from BaseNode\n";
        return;
    }

    shared_ptr<CatchAction> catchAction =
        shared_dynamic_cast<CatchAction>(mAction);
    mAction.reset();
    if (catchAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) cannot realize an unknown "
            << "ActionObject\n";
        return;
    }

    if (mAgentState->GetUniformNumber() != 1)
    {
        return;
    }

    Vector3f pos = mBallBody->GetWorldTransform().Pos();
    if ( mAgentState->GetTeamIndex() == TI_LEFT )
    {
        if (! mLeftPenaltyArea.Contains(Vector2f(pos[0], pos[1])))
        {
            return;
        }
    }
    else
    {
        if (! mRightPenaltyArea.Contains(Vector2f(pos[0], pos[1])))
        {
            return;
        }
    }

    Vector3f ballVec =
        mBallBody->GetWorldTransform().Pos() -
        mAgent->GetWorldTransform().Pos();

    // the ball can be catched if the distance is
    // less then Ball-Radius + Player-Radius + CatchMargin AND
    // the player is close to the ground
    if (mAgent->GetWorldTransform().Pos().z() > mPlayerRadius + 0.01 ||
        ballVec.Length() > mPlayerRadius + mBallRadius + mCatchMargin)
    {
        // ball is out of reach, or player is in the air:
        // catch has no effect
        return;
    }

    Vector3f ballPos = mAgent->GetWorldTransform().Pos();
    ballPos[2]= mBallRadius;
    if (mAgentState->GetTeamIndex() == TI_LEFT)
    {
        ballPos[0] += mBallRadius + mPlayerRadius + 0.07;
    }
    else
    {
        ballPos[0] -= mBallRadius + mPlayerRadius + 0.07;
    }

    mSoccerRule->ClearPlayersWithException(ballPos,
        2.0, 5.0, TI_LEFT, mAgentState);
    mSoccerRule->ClearPlayersWithException(ballPos,
        2.0, 5.0, TI_RIGHT, mAgentState);

    MoveBall(ballPos);
}

shared_ptr<ActionObject>
CatchEffector::GetActionObject(const Predicate& predicate)
{
  do
  {
      if (predicate.name != GetPredicate())
          {
              GetLog()->Error() << "ERROR: (CatchEffector) invalid predicate"
                                << predicate.name << "\n";
              break;
          }

      // construct the CatchAction object
      return shared_ptr<CatchAction>(new CatchAction(GetPredicate()));

  } while (0);

  // some error happened
  return shared_ptr<ActionObject>();
}

void
CatchEffector::OnLink()
{
    SoccerBase::GetBallBody(*this,mBallBody);
    SoccerBase::GetAgentState(*this,mAgentState);

    SoccerBase::GetSoccerRuleAspect(*this,mSoccerRule);

    mAgent = shared_dynamic_cast<AgentAspect>(GetParent().lock());

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) parent node is not derived "
            << "from AgentAspect\n";
        return;
    }

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(mAgent->GetChild("geometry"));
    if (geom.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) parent node has no SphereCollider "
            << "child\n";
    } 
    else
    {
        mPlayerRadius = geom->GetRadius();
    }

    if (! SoccerBase::GetBallCollider(*this,geom))
    {
        GetLog()->Error()
            << "ERROR: (CatchEffector) ball node has no SphereCollider "
            << "child\n";
    } 
    else
    {
        mBallRadius = geom->GetRadius();
    }

    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);
    SoccerBase::GetSoccerVar(*this,"GoalWidth",mGoalWidth);

    // the penalty areas (exact sizes)
    mRightPenaltyArea = salt::AABB2(
                     Vector2f(mFieldLength/2.0 - 16.5, -16.5 - mGoalWidth/2.0),
                     Vector2f(mFieldLength/2.0 , 16.5 + mGoalWidth/2.0));
    mLeftPenaltyArea = salt::AABB2(
                     Vector2f(-mFieldLength/2.0 + 16.5, -16.5 - mGoalWidth/2.0),
                     Vector2f(-mFieldLength/2.0, 16.5 + mGoalWidth/2.0));
}

void
CatchEffector::OnUnlink()
{
    mSoccerRule.reset();
    mBallBody.reset();
    mAgent.reset();
    mAgentState.reset();
}

void
CatchEffector::SetCatchMargin(float margin)
{
    mCatchMargin = margin;
}
