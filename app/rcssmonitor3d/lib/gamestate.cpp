/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestate.cpp,v 1.1 2004/03/10 20:44:24 rollmark Exp $

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

   Main of the rcssmonitor3D application

   This is a very simple 3D monitor example which will hopefully be
   useful for the first steps of 3D SoccerServer
*/
#include "gamestate.h"

GameState::GameState()
{
  mTeamL    = "teamL";
  mTeamR    = "teamR";
  mTime     = 0.0f;
  mHalf     = GH_FIRST;
  mScoreL   = 0;
  mScoreR   = 0;
  mPlayMode = PM_BeforeKickOff;
}

GameState::~GameState()
{
}

void GameState::SetTeamL(std::string name)
{
    mTeamL = name;
}

std::string GameState::GetTeamL()
{
    return mTeamL;
}

void GameState::SetTeamR(std::string name)
{
    mTeamR = name;
}

std::string GameState::GetTeamR()
{
    return mTeamR;
}

void GameState::SetHalf(TGameHalf half)
{
    mHalf = half;
}

TGameHalf GameState::GetHalf()
{
    return mHalf;
}

void GameState::SetTime(float time)
{
    mTime = time;
}

float GameState::GetTime()
{
    return mTime;
}

void GameState::SetScoreL(int score)
{
    mScoreL = score;
}

int GameState::GetScoreL()
{
    return mScoreL;
}

void GameState::SetScoreR(int score)
{
    mScoreR = score;
}

int GameState::GetScoreR()
{
    return mScoreR;
}

void GameState::SetPlayMode(TPlayMode playMode)
{
    mPlayMode = playMode;
}

TPlayMode GameState::GetPlayMode()
{
    return mPlayMode;
}



