/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.cpp,v 1.6 2004/04/23 15:22:14 fruit Exp $

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
#include "gamestateaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>
#include <salt/random.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

GameStateAspect::GameStateAspect() : SoccerControlAspect()
{
    mPlayMode = PM_BeforeKickOff;
    mTime = 0;
    mLeadTime = 0;
    mLastModeChange = 0;
    mGameHalf = GH_FIRST;
    mScore[0] = 0;
    mScore[1] = 0;
    mMaxUnum[0] = 0;
    mMaxUnum[1] = 0;
    mLastKickOff = TI_NONE;
    mLeftInit = Vector3f(0,0,0);
    mRightInit = Vector3f(0,0,0);
}

GameStateAspect::~GameStateAspect()
{
}

void
GameStateAspect::UpdateTime(float deltaTime)
{
  if (
      (mPlayMode != PM_BeforeKickOff) &&
      (mPlayMode != PM_GameOver)
      )
  {
      mTime += deltaTime;
  }
  else if (mPlayMode == PM_BeforeKickOff)
  {
      mLeadTime += deltaTime;
  }
}

void
GameStateAspect::Update(float deltaTime)
{
    UpdateTime(deltaTime);
}

TPlayMode
GameStateAspect::GetPlayMode() const
{
    return mPlayMode;
}

void
GameStateAspect::SetPlayMode(TPlayMode mode)
{
    if (mode == mPlayMode)
    {
        return;
    }

    GetLog()->Normal() << "(GameStateAspect) playmode changed to "
                       << SoccerBase::PlayMode2Str(mode) << " at t="
                       << mTime << "\n";
    mPlayMode = mode;
    mLastModeChange = mTime;
    mLeadTime = 0.0;
}

void
GameStateAspect::KickOff(TTeamIndex ti)
{
    if (mGameHalf == GH_FIRST)
    {
        // throw a coin to determine which team kicks off
        if (ti == TI_NONE)
        {
            ti = (salt::UniformRNG<>(0,1)() <= 0.5) ? TI_LEFT : TI_RIGHT;
        }

        SetPlayMode((ti == TI_LEFT) ? PM_KickOff_Left : PM_KickOff_Right);

        mLastKickOff = ti;
    }
    else
    {
        // in the second half, let the opposite team kick off
        SetPlayMode((mLastKickOff == TI_LEFT) ? PM_KickOff_Right : PM_KickOff_Left);
    }
}

TTime
GameStateAspect::GetTime() const
{
    return mTime;
}

TTime
GameStateAspect::GetModeTime() const
{
    if (mPlayMode == PM_BeforeKickOff) return mLeadTime;
    return mTime - mLastModeChange;
}

TTime
GameStateAspect::GetLastModeChange() const
{
    return mLastModeChange;
}

void
GameStateAspect::SetTeamName(TTeamIndex idx, std::string name)
{
    if (
        (idx != TI_LEFT) &&
        (idx != TI_RIGHT)
        )
    {
        return;
    }

    mTeamName[idx] = name;
}

std::string
GameStateAspect::GetTeamName(TTeamIndex idx) const
{
    if (
        (idx != TI_LEFT) &&
        (idx != TI_RIGHT)
        )
    {
        return "";
    }

    return mTeamName[idx];
}

TTeamIndex
GameStateAspect::GetTeamIndex(const std::string& teamName)
{
    for (int i=TI_LEFT;i<=TI_RIGHT;++i)
    {
        if (mTeamName[i] == "")
        {
            mTeamName[i] = teamName;
            return (TTeamIndex)i;
        }

        if (mTeamName[i] == teamName)
        {
            return (TTeamIndex)i;
        }
    }

    return TI_NONE;
}

bool
GameStateAspect::InsertUnum(TTeamIndex idx, int unum)
{
    if (
        (idx != TI_LEFT) &&
        (idx != TI_RIGHT)
        )
    {
        return false;
    }

    TUnumSet& set = mUnumSet[idx];

    if (
        (set.size() >= 11) ||
        (set.find(unum) != set.end())
        )
    {
        return false;
    }

    set.insert(unum);
    mMaxUnum[idx] = std::max<int>(unum, mMaxUnum[idx]);

    return true;
}

bool
GameStateAspect::RequestUniform(shared_ptr<AgentState> agentState,
                                std::string teamName, unsigned int unum)
{
    if (agentState.get() == 0)
    {
        return false;
    }

    TTeamIndex idx = GetTeamIndex(teamName);

    if (idx == TI_NONE)
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::RequestUniform) invalid teamname "
            << teamName << "\n";
        return false;
    }

    if (unum == 0)
    {
        unum = RequestUniformNumber(idx);
    }

    if (! InsertUnum(idx,unum))
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::RequestUniform) cannot insert uniform"
            " number " << unum << " to team " << teamName << "\n";
        return false;
    }

    agentState->SetUniformNumber(unum);
    agentState->SetTeamIndex(idx);
    //agentState->SetPerceptName(teamName, ObjectState::PT_Default);
    agentState->SetPerceptName(teamName, ObjectState::PT_Default, ObjectState::PT_Player );

    agentState->SetPerceptName("player", ObjectState::PT_TooFar);

    GetLog()->Normal() << "(GameStateAspect) handed out uniform number "
                       << unum << " for team " << teamName << "\n";

    return true;
}

void
GameStateAspect::SetGameHalf(TGameHalf half)
{
    if (
        (half != GH_FIRST) &&
        (half != GH_SECOND)
        )
    {
        return;
    }

    mGameHalf = half;
}

TGameHalf
GameStateAspect::GetGameHalf() const
{
    return mGameHalf;
}

void
GameStateAspect::ScoreTeam(TTeamIndex idx)
{
    if (
        (idx != TI_LEFT) &&
        (idx != TI_RIGHT)
        )
    {
        return;
    }

    ++mScore[idx];
}

int
GameStateAspect::GetScore(TTeamIndex idx) const
{
    if (
        (idx != TI_LEFT) &&
        (idx != TI_RIGHT)
        )
    {
        return 0;
    }

    return mScore[idx];
}

Vector3f
GameStateAspect::RequestInitPosition(const TTeamIndex ti)
{
    if (ti == TI_NONE)
    {
        GetLog()->Debug()
            << "(GameStateAspect) RequestInitPosition called with "
            << "ti=TI_NONE\n";
        return Vector3f(0,20,0);
    }

    salt::Vector3f& init = (ti ==TI_LEFT) ? mLeftInit : mRightInit;

    Vector3f pos = init;
    init[1] -= mAgentRadius * 3;

    return pos;
}

void
GameStateAspect::OnLink()
{
    // setup the initial starting positions for the agents
    float fieldWidth = 64.0;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",fieldWidth);

    float fieldLength = 100.0;
    SoccerBase::GetSoccerVar(*this,"FieldLength",fieldLength);

    mAgentRadius = 0.22;
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);

    mLeftInit = Vector3f
        (
            -fieldLength/2.0 + mAgentRadius*2,
            fieldWidth/2 - mAgentRadius*2,
            mAgentRadius
            );

    mRightInit = Vector3f
        (
            +fieldLength/2.0 - mAgentRadius*2,
            fieldWidth/2  - mAgentRadius*2,
            mAgentRadius
            );
}

int
GameStateAspect::RequestUniformNumber(TTeamIndex ti) const
{
    return mMaxUnum[ti] + 1;
}






