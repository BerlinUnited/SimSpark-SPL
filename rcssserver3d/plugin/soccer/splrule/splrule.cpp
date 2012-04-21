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
  mReadyDuration(45),
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
                  << "(SoccerRuleAspect) ERROR: could not get SPLState\n";
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
      float factor = 0.1;
      mReadyDuration *= factor;
      mSetDuration *= factor;
      mHalfTime *= factor;
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
  //all robots must be in the initial state and must be placed on the
  //sidelines in their own half of the field

  // TODO
}

void SPLRule::UpdateReady()
{
  if (mState->GetStateTime() > mReadyDuration)
  {
    mState->SetState(Set);
  }
}

void SPLRule::UpdateSet()
{
  if (mState->GetStateTime() > mSetDuration)
  {
    mState->SetState(Playing);
  }
}

void SPLRule::UpdatePlaying()
{

}

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

