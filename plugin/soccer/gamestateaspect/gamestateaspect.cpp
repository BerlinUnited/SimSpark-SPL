/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.cpp,v 1.1.2.8 2004/02/08 15:21:33 rollmark Exp $

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
#include <soccer/agentstate/agentstate.h>

using namespace oxygen;
using namespace boost;
using namespace std;

GameStateAspect::GameStateAspect() : SoccerControlAspect()
{
    mPlayMode = PM_BeforeKickOff;
    mTime = 0;
    mGameHalf = GH_FIRST;
    mScore[0] = 0;
    mScore[1] = 0;
}

GameStateAspect::~GameStateAspect()
{
}

void GameStateAspect::UpdateTime(float deltaTime)
{
  if (mPlayMode == PM_PlayOn)
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
    mPlayMode = mode;
}

TTime GameStateAspect::GetTime() const
{
    return mTime;
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

/** returns the name of a team */
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
    agentState->SetName(teamName);

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




