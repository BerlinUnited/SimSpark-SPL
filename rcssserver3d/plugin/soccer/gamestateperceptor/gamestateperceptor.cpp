/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateperceptor.cpp 21 2009-01-14 14:38:57Z yxu $

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
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <gamestateaspect/gamestateaspect.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

GameStatePerceptor::GameStatePerceptor() : oxygen::Perceptor()
{
    mFirstPercept = true;
    mReportScore = true;
}

GameStatePerceptor::~GameStatePerceptor()
{
}

void
GameStatePerceptor::InsertSoccerParam(Predicate& predicate, const std::string& name)
{
    float value;
    if (! SoccerBase::GetSoccerVar(*this,name,value))
    {
        return;
    }

    ParameterList& element = predicate.parameter.AddList();
    element.AddValue(name);
    element.AddValue(value);
}

void
GameStatePerceptor::InsertInitialPercept(Predicate& predicate)
{
    // uniform number
    ParameterList& unumElement = predicate.parameter.AddList();
    unumElement.AddValue(string("unum"));
    unumElement.AddValue(mAgentState->GetUniformNumber());

    // team index
    std::string team;
    switch (mAgentState->GetTeamIndex())
    {
    case TI_NONE :
        team = "none";
        break;
    case TI_LEFT :
        team = "left";
        break;
    case TI_RIGHT :
        team = "right";
        break;
    }

    ParameterList& teamElement = predicate.parameter.AddList();
    teamElement.AddValue(string("team"));
    teamElement.AddValue(team);

    // soccer variables
    // field geometry parameter
//     InsertSoccerParam(predicate,"FieldLength");
//     InsertSoccerParam(predicate,"FieldWidth");
//     InsertSoccerParam(predicate,"FieldHeight");
//     InsertSoccerParam(predicate,"GoalWidth");
//     InsertSoccerParam(predicate,"GoalDepth");
//     InsertSoccerParam(predicate,"GoalHeight");
//     InsertSoccerParam(predicate,"BorderSize");

//     // agent parameter
//     InsertSoccerParam(predicate,"AgentMass");
//     InsertSoccerParam(predicate,"AgentRadius");
//     InsertSoccerParam(predicate,"AgentMaxSpeed");

//     // ball parameter
//     InsertSoccerParam(predicate,"BallRadius");
//     InsertSoccerParam(predicate,"BallMass");
}

bool
GameStatePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if (
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return false;
    }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "GS";
    predicate.parameter.Clear();

    // with the first GameState percept after the player is assigned
    // to a team it receives info about it's team and unum assignment
    // along with outher soccer parameters
    if (
        (mFirstPercept) &&
        (mAgentState->GetTeamIndex() != TI_NONE)
        )
    {
        mFirstPercept = false;
        InsertInitialPercept(predicate);
    }

    if (mReportScore) {
        // score left
        ParameterList& slElement = predicate.parameter.AddList();
        slElement.AddValue(string("sl"));
        slElement.AddValue(mGameState->GetScore(TI_LEFT));
        
        // score right
        ParameterList& srElement = predicate.parameter.AddList();
        srElement.AddValue(string("sr"));
        srElement.AddValue(mGameState->GetScore(TI_RIGHT));
    }

    // time
    ParameterList& timeElement = predicate.parameter.AddList();
    timeElement.AddValue(string("t"));
    timeElement.AddValue(mGameState->GetTime());

    // playmode
    ParameterList& pmElement = predicate.parameter.AddList();
    pmElement.AddValue(string("pm"));
    pmElement.AddValue(SoccerBase::PlayMode2Str(mGameState->GetPlayMode()));

    return true;
}

void
GameStatePerceptor::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);
    SoccerBase::GetSoccerVar(*this,"ReportScore",mReportScore);
}

void
GameStatePerceptor::OnUnlink()
{
    mGameState.reset();
    mAgentState.reset();
}
