/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: beameffector.cpp,v 1.2 2004/02/12 14:07:25 fruit Exp $

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
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

BeamEffector::BeamEffector() : oxygen::Effector()
{
}

BeamEffector::~BeamEffector()
{
}

bool
BeamEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (
        (mBody.get() == 0) ||
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return false;
    }

    shared_ptr<BeamAction> beamAction =
        shared_dynamic_cast<BeamAction>(action);

    if (beamAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (BeamEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    // the beam effector only has an effect in PM_BeforeKickOff
    if (mGameState->GetPlayMode() == PM_BeforeKickOff)
        {
            Vector3f pos = SoccerBase::FlipView
                (
                 beamAction->GetPosition(),
                 mAgentState->GetTeamIndex()
                );

            // an agent can only beam within it's own field half
            float minX = -mFieldLength/2 + mAgentRadius;
            pos[0] = std::max<float>(pos[0],minX);

            float minY = -mFieldWidth/2 + mAgentRadius;
            pos[2] = std::max<float>(minY,pos[1]);

            pos[1] = mAgentRadius;

            mBody->SetPosition(pos);
            mBody->SetVelocity(Vector3f(0,0,0));
            mBody->SetAngularVelocity(Vector3f(0,0,0));
        }

    return true;
}

shared_ptr<ActionObject>
BeamEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (BeamEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  Predicate::Iterator iter = predicate.begin();
  Vector3f pos;
  if (! predicate.GetValue(iter, pos))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) Vector3f parameter expected\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return shared_ptr<ActionObject>(new BeamAction(GetPredicate(),pos));
}

void
BeamEffector::OnLink()
{
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetGameState(*this, mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);

    mFieldWidth = 64.0;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",mFieldWidth);

    mFieldLength = 100.0;
    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);

    mAgentRadius = 0.22;
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);
}

void
BeamEffector::OnUnlink()
{
    mBody.reset();
    mGameState.reset();
    mAgentState.reset();
}

