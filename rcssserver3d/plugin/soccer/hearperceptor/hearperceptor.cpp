/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group

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
#include "hearperceptor.h"
#include <agentstate/agentstate.h>
#include <soccerbase/soccerbase.h>
#include <gamestateaspect/gamestateaspect.h>
//#include <soccer/soccertypes.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;

HearPerceptor::HearPerceptor() : oxygen::Perceptor()
{
}

HearPerceptor::~HearPerceptor()
{
}

bool
HearPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if ( 
        (mAgentState.get() == 0) ||
        (mGameState.get() == 0)
        )		 
    {
        return false;
    }

    bool result = false;

    string message;
    float direction;
		
    if (mAgentState->GetSelfMessage(message))
    {
        string self = "self";
        Predicate& predicate = predList->AddPredicate();
        predicate.name = "hear";
        predicate.parameter.Clear();
        predicate.parameter.AddValue(mGameState->GetTime());
        predicate.parameter.AddValue(self);
        predicate.parameter.AddValue(message);
        result = true;
    }

    if (mAgentState->GetMessage(message, direction, true))
    {
        Predicate& predicate = predList->AddPredicate();
        predicate.name = "hear";
        predicate.parameter.Clear();
        predicate.parameter.AddValue(mGameState->GetTime());
        predicate.parameter.AddValue(direction);
        predicate.parameter.AddValue(message);
        result = true;
    }

    if (mAgentState->GetMessage(message, direction, false))
    {
        Predicate& predicate = predList->AddPredicate();
        predicate.name = "hear";
        predicate.parameter.Clear();
        predicate.parameter.AddValue(mGameState->GetTime());
        predicate.parameter.AddValue(direction);
        predicate.parameter.AddValue(message);
        result = true;
    }

    return result;
}

void
HearPerceptor::OnLink()
{
    SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetGameState(*this, mGameState);
}

void
HearPerceptor::OnUnlink()
{
    mAgentState.reset();
    mGameState.reset();
}
