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

SPLRule::SPLRule() : SoccerRuleAspect()
{
}

SPLRule::~SPLRule()
{
}

void SPLRule::OnLink()
{
  SoccerRuleAspect::OnLink();

  GetControlAspect(mSPLState, "GameStateAspect");

  if (mSPLState.expired())
      {
          GetLog()->Error()
                  << "(SoccerRuleAspect) ERROR: could not get SPLState\n";
      }
}

void SPLRule::OnUnlink()
{
  SoccerRuleAspect::OnUnlink();
  mSPLState.reset();
}

void SPLRule::Update(float /*deltaTime*/)
{
    if (
        (mSPLState.get() == 0) ||
        (mBallState.get() == 0) ||
        (mBallBody.get() == 0)
        )
    {
        return;
    }

    TSPLState state = mSPLState->GetSPLState();
    switch (state)
    {
    case Unknown:
      UpdateCachedInternal();
      mSPLState->SetSPLState(Initial);
      break;
    case Initial:
      // do nothing, until game starts
      break;
    case Ready:
      break;
    case Set:
      break;
    case Playing:
      break;
    case Finished:
      break;
    }

    //CheckTime();
}
