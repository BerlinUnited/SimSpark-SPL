/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gametimeperceptor.cpp 21 2009-01-14 14:38:57Z yxu $

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
#include "gametimeperceptor.h"
#include <zeitgeist/logserver/logserver.h>
//#include <soccer/soccerbase/soccerbase.h>
#include <gamestateaspect/gamestateaspect.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

GameTimePerceptor::GameTimePerceptor() : oxygen::Perceptor()
{
}

GameTimePerceptor::~GameTimePerceptor()
{
}


bool
GameTimePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if (mGameState.get() == 0)
    {
        return false;
    }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "time";
    predicate.parameter.Clear();

    // with the first GameState percept after the player is assigned
    // to a team it receives info about it's team and unum assignment
    // along with outher soccer parameters

    // time
    ParameterList& timeElement = predicate.parameter.AddList();
    timeElement.AddValue(string("now"));
    timeElement.AddValue(mGameState->GetTime());

    return true;
}

void
GameTimePerceptor::OnLink()
{
    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));
    
    if (mGameState.get() == 0)
    {
        GetLog()->Error()
            << "Error: (SoccerBase: " << GetName()
            << ") found no GameStateAspect\n";
    }

    return;
}

void
GameTimePerceptor::OnUnlink()
{
    mGameState.reset();
}
