/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.cpp,v 1.1.2.3 2004/01/25 12:51:47 rollmark Exp $

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

using namespace oxygen;
using namespace boost;
using namespace std;

GameStateAspect::GameStateAspect() : ControlAspect()
{
    mPlayMode = PM_BeforeKickOff;
    mTime = 0;
}

GameStateAspect::~GameStateAspect()
{
}

void GameStateAspect::Update(float deltaTime)
{
  if (mPlayMode == PM_PlayOn)
      {
          mTime += deltaTime;
      }
}

GameStateAspect::TPlayMode GameStateAspect::GetPlayMode() const
{
    return mPlayMode;
}

void GameStateAspect::SetPlayMode(TPlayMode mode)
{
    mPlayMode = mode;
}

GameStateAspect::TTime GameStateAspect::GetTime() const
{
    return mTime;
}

