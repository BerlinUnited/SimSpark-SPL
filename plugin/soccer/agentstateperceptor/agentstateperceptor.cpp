/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentstateperceptor.cpp,v 1.3.2.2 2004/03/28 15:36:58 rollmark Exp $

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
#include "agentstateperceptor.h"
#include <soccer/agentstate/agentstate.h>
#include <soccer/soccerbase/soccerbase.h>

using namespace oxygen;
using namespace zeitgeist;

AgentStatePerceptor::AgentStatePerceptor() : oxygen::Perceptor()
{
    mPerceptRate = 10;
    mSenses = 0;
}

AgentStatePerceptor::~AgentStatePerceptor()
{
}

bool
AgentStatePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if (mAgentState.get() == 0)
    {
        return false;
    }

    --mSenses;

    if (mSenses > 0)
    {
        return false;
    }

    mSenses = mPerceptRate;

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "AgentState";
    predicate.parameter.Clear();

    // battery
    ParameterList& batteryElement = predicate.parameter.AddList();
    batteryElement.AddValue(std::string("battery"));
    batteryElement.AddValue(mAgentState->GetBattery());

    // temperature
    ParameterList& tempElement = predicate.parameter.AddList();
    tempElement.AddValue(std::string("temp"));
    tempElement.AddValue(mAgentState->GetTemperature());

    return true;
}

void
AgentStatePerceptor::OnLink()
{
    SoccerBase::GetAgentState(*this,mAgentState);
}

void
AgentStatePerceptor::OnUnlink()
{
    mAgentState.reset();
}
