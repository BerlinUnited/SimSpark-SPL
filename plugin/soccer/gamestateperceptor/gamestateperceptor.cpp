/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateperceptor.cpp,v 1.1.2.1 2004/02/03 09:35:32 rollmark Exp $

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
#include "gamestateperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <soccer/gamestateaspect/gamestateaspect.h>

using namespace oxygen;
using namespace boost;
using namespace std;

GameStatePerceptor::GameStatePerceptor() : oxygen::Perceptor()
{
}

GameStatePerceptor::~GameStatePerceptor()
{
}

string GameStatePerceptor::PlayMode2Str(const TPlayMode mode) const
{
    switch (mode)
        {
        case    PM_BeforeKickOff:
            return "BeforeKickOff";
        case    PM_KickOff:
            return "KickOff";
        case    PM_PlayOn:
            return "KickOff";
        case    PM_KickOff_Left:
            return "KickOff_Left";
        case    PM_KickOff_Right:
            return "KickOff_Right";
        case    PM_FirstHalfOver:
            return "FirstHalfOver";
        default:
            return "unknown";
        };
}

bool
GameStatePerceptor::Percept(Predicate& predicate)
{
    if (mGameState.get() == 0)
        {
            return false;
        }

    predicate.name = "GameState";
    predicate.parameter.clear();

    // time
    Predicate::TParameterList element;
    element.push_back(string("time"));
    element.push_back(mGameState->GetTime());
    predicate.parameter.push_back(element);

    // playmode
    element.clear();
    element.push_back(string("playmode"));
    element.push_back(PlayMode2Str(mGameState->GetPlayMode()));
    predicate.parameter.push_back(element);

    return true;
}

void GameStatePerceptor::OnLink()
{
    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));

    if (mGameState.get() == 0)
        {
            GetLog()->Error()
                << "(GameStatePerceptor) found no GameStateAspect\n";
        }
}

void GameStatePerceptor::OnUnlink()
{
    mGameState.reset();
}
