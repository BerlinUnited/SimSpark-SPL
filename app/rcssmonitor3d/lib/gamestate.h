/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestate.h,v 1.1 2004/03/10 20:44:24 rollmark Exp $

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
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <soccertypes.h>

class GameState
{
 public:
    GameState();
    virtual ~GameState();

    void SetTeamL(std::string name);
    std::string GetTeamL();

    void SetTeamR(std::string name);
    std::string GetTeamR();

    void SetHalf(TGameHalf half);
    TGameHalf GetHalf();

    void SetTime(float time);
    float GetTime();

    void SetScoreL(int score);
    int GetScoreL();

    void SetScoreR(int score);
    int GetScoreR();

    void SetPlayMode(TPlayMode playMode);
    TPlayMode GetPlayMode();

 protected:
    std::string mTeamL;
    std::string mTeamR;
    float mTime;
    TGameHalf mHalf;
    int mScoreL;
    int mScoreR;
    TPlayMode mPlayMode;
};

#endif // GAMESTATE_H
