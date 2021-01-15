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
#include "passaction.h"
#include "passeffector.h"
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <soccerruleaspect/soccerruleaspect.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

PassEffector::PassEffector() : oxygen::Effector()
{
}

PassEffector::~PassEffector()
{
}

void
PassEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (
        (mAction.get() == 0) ||
        (mSoccerRule.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return;
    }

    boost::shared_ptr<PassAction> passAction =
        dynamic_pointer_cast<PassAction>(mAction);

    mAction.reset();

    if (passAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (PassEffector) cannot realize an unknown ActionObject\n";
        return;
    }
                
    TTeamIndex team = mAgentState->GetTeamIndex();
    int unum = mAgentState->GetUniformNumber();
    if (mSoccerRule->CanActivatePassMode(unum, team)) {
        mSoccerRule->StartPassMode(team);
    }
}

boost::shared_ptr<ActionObject>
PassEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (PassEffector) invalid predicate"
                        << predicate.name << "\n";
      return boost::shared_ptr<ActionObject>();
    }

  return boost::shared_ptr<ActionObject>(new PassAction(GetPredicate()));
}

void
PassEffector::OnLink()
{
    SoccerBase::GetGameState(*this, mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);
    SoccerBase::GetSoccerRuleAspect(*this,mSoccerRule);
}

void
PassEffector::OnUnlink()
{
    mGameState.reset();
    mAgentState.reset();
    mSoccerRule.reset();
}

