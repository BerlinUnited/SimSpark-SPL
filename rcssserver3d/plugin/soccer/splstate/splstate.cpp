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
#include "splstate.h"
#include <zeitgeist/logserver/logserver.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <salt/random.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;
using namespace spl;

SPLState::SPLState() : GameStateAspect()
{
    mSPLState = Unknown;
}

SPLState::~SPLState()
{
}

void SPLState::Update(float deltaTime)
{
    UpdateTime(deltaTime);
}

void SPLState::KickOff(TTeamIndex ti)
{
  if (mSPLState == Initial)
  {
    SetState(Ready);
  }
}

void SPLState::SetState(TSPLState state)
{
  if (mSPLState == state)
  {
    return;
  }
  mSPLState = state;
  mLastModeChange = mTime;
}

string SPLState::GetPlayModeStr() const
{
  return SoccerBase::SPLState2Str(mSPLState);
}

void SPLState::UpdateTime(float deltaTime)
{
    switch (mSPLState)
    {
    case Initial:
        mLeadTime += deltaTime;
        break;
    case Finished:
        mFupTime += deltaTime;
        break;
    default:
        mTime += deltaTime;
    }
}

TTime SPLState::GetStateTime() const
{
    switch (mSPLState)
    {
    case Initial:
        return mLeadTime;
    case Finished:
        return mFupTime;
    default:
        return mTime - mLastModeChange;
    }
}
