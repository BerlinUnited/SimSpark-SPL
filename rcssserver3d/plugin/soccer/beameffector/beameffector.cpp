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
#include "beamaction.h"
#include "beameffector.h"
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <cmath>
#include <oxygen/agentaspect/agentaspect.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

BeamEffector::BeamEffector() : oxygen::Effector()
{
}

BeamEffector::~BeamEffector()
{
    mNoiseRng.reset();
}

#ifdef __APPLE_CC__
bool
isfinite( float f )
{
    return f == f && f != f * 0.5f;
}
#endif

void
BeamEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (
        (mAction.get() == 0) ||
        (mBody.get() == 0) ||
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0) ||
        (mNoiseRng.get() == 0)
        )
    {
        return;
    }

    boost::shared_ptr<BeamAction> beamAction =
        dynamic_pointer_cast<BeamAction>(mAction);

   mAction.reset();

    if (beamAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (BeamEffector) cannot realize an unknown ActionObject\n";
        return;
    }

    // the beam effector only has an effect in PM_BeforeKickOff
    if (mGameState->GetPlayMode() == PM_BeforeKickOff
        || mGameState->GetPlayMode() == PM_Goal_Left
        || mGameState->GetPlayMode() == PM_Goal_Right)
    {
        Vector3f pos;
        pos[0] = beamAction->GetPosX() + mBeamNoiseXY*(*(mNoiseRng.get()))();
        pos[1] = beamAction->GetPosY() + mBeamNoiseXY*(*(mNoiseRng.get()))();

        float angle = beamAction->GetXYAngle() + mBeamNoiseAngle*(*(mNoiseRng.get()))();

        // reject nan or infinite numbers in the beam position
        if (
            (! gIsFinite(pos[0])) ||
            (! gIsFinite(pos[1]))
            )
            {
                return;
            }

        if (!mStartAnyFieldPosition) 
        {
            // an agent can only beam within it's own field half
            float minX = -mFieldLength/2;
            pos[0] = std::max<float>(pos[0],minX);
            pos[0] = std::min<float>(pos[0],0.0f);
            
            float minY = -mFieldWidth/2;
            float maxY = mFieldWidth/2;
            pos[1] = std::max<float>(minY,pos[1]);
            pos[1] = std::min<float>(maxY,pos[1]);
        }

        // fix z coordinate
        pos[2] = mAgentRadius;

        // swap x and y coordinates accordingly for the current
        // team; after the flip pos is global and not independent
        // on the team
        pos = SoccerBase::FlipView(pos, mAgentState->GetTeamIndex());     

        boost::shared_ptr<Transform> agentAspect;
        SoccerBase::GetTransformParent(*this, agentAspect);
        if (agentAspect.get() == 0)
        {
            GetLog()->Error()
            << "ERROR: (BeamEffector) cannot get AgentAspect\n";
            return;
        }
        
        TTeamIndex team = mAgentState->GetTeamIndex();
        angle += mGameState->RequestInitOrientation(team);

        if (!SoccerBase::MoveAndRotateAgent(agentAspect, pos, angle))
            return;
    }
}

boost::shared_ptr<ActionObject>
BeamEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (BeamEffector) invalid predicate"
                        << predicate.name << "\n";
      return boost::shared_ptr<ActionObject>();
    }

  Predicate::Iterator iter = predicate.begin();

  float posX;
  if (! predicate.AdvanceValue(iter, posX))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter1\n";
      return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  float posY;
  if (! predicate.AdvanceValue(iter, posY))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter2\n";
      return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  float angle;
  if (! predicate.AdvanceValue(iter, angle))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter3\n";
      return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return boost::shared_ptr<ActionObject>(new BeamAction(GetPredicate(), posX, posY, angle));
}

void
BeamEffector::OnLink()
{
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetGameState(*this, mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);

    mFieldWidth = 64.0f;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",mFieldWidth);

    mFieldLength = 100.0f;
    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);

    mAgentRadius = 0.22f;
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);

    mBeamNoiseXY = 0.05f;
    SoccerBase::GetSoccerVar(*this, "BeamNoiseXY",mBeamNoiseXY);

    mBeamNoiseAngle = 10.0f;
    SoccerBase::GetSoccerVar(*this, "BeamNoiseAngle",mBeamNoiseAngle);

    mStartAnyFieldPosition = false;
    SoccerBase::GetSoccerVar(*this,"StartAnyFieldPosition",mStartAnyFieldPosition);

    UniformRngPtr rng1(new salt::UniformRNG<>(-1,1));
    mNoiseRng = rng1;
}

void
BeamEffector::OnUnlink()
{
    mBody.reset();
    mGameState.reset();
    mAgentState.reset();
    mNoiseRng.reset();
}

