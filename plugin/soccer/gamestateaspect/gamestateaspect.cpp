/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.cpp,v 1.1.2.11 2004/02/10 20:40:17 rollmark Exp $

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

GameStateAspect::GameStateAspect() : SoccerControlAspect()
{
    mPlayMode = PM_BeforeKickOff;
    mTime = 0;
    mLastModeChange = 0;
    mGameHalf = GH_FIRST;
    mScore[0] = 0;
    mScore[1] = 0;
    mLastKickOff = TI_NONE;
}

GameStateAspect::~GameStateAspect()
{
}

void GameStateAspect::UpdateTime(float deltaTime)
{
  if (
      (mPlayMode != PM_BeforeKickOff) &&
      (mPlayMode != PM_GameOver)
      )
      {
          mTime += deltaTime;
      }
}

void GameStateAspect::Update(float deltaTime)
{
    UpdateTime(deltaTime);
}

TPlayMode GameStateAspect::GetPlayMode() const
{
    return mPlayMode;
}

void GameStateAspect::SetPlayMode(TPlayMode mode)
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
}

void GameStateAspect::KickOff()
{
    if (mGameHalf == GH_FIRST)
        {
            // throw a coin to determine which team kicks off
            TTeamIndex ti = (salt::UniformRNG<>(0,1)() <= 0.5) ?
                TI_LEFT : TI_RIGHT;

            SetPlayMode((ti == TI_LEFT)
                        ? PM_KickOff_Left : PM_KickOff_Right);

            mLastKickOff = ti;
        } else
            {
                // in the second half, let the opposite team kick off
                SetPlayMode((mLastKickOff == TI_LEFT)
                            ? PM_KickOff_Right : PM_KickOff_Left);
            }
}

TTime GameStateAspect::GetTime() const
{
    return mTime;
}

TTime GameStateAspect::GetModeTime()
{
    return mTime - mLastModeChange;
}

TTime GameStateAspect::GetLastModeChange()
{
    return mLastModeChange;
}

void GameStateAspect::SetTeamName(TTeamIndex idx, std::string name)
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

std::string GameStateAspect::GetTeamName(TTeamIndex idx)
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

TTeamIndex GameStateAspect::GetTeamIndex(const std::string& teamName)
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

bool GameStateAspect::InsertUnum(TTeamIndex idx, int unum)
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

    return true;
}

bool GameStateAspect::RequestUniform(shared_ptr<AgentState> agentState,
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

    if (! InsertUnum(idx,unum))
        {
            GetLog()->Error()
                << "ERROR: (GameStateAspect::RequestUniform) cannot insert uniform"
                " number " << unum << " to team " << teamName << "\n";
            return false;
        }

    agentState->SetUniformNumber(unum);
    agentState->SetTeamIndex(idx);
    agentState->SetPerceptName(teamName, ObjectState::PT_Default);
    agentState->SetPerceptName("player", ObjectState::PT_TooFar);

    GetLog()->Normal() << "(GameStateAspect) handed out uniform number "
                       << unum << " for team " << teamName << "\n";

    return true;
}

void GameStateAspect::SetGameHalf(TGameHalf half)
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

TGameHalf GameStateAspect::GetGameHalf()
{
    return mGameHalf;
}

void GameStateAspect::ScoreTeam(TTeamIndex idx)
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

int GameStateAspect::GetScore(TTeamIndex idx)
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




