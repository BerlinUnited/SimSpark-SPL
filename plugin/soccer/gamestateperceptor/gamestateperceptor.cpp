/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateperceptor.cpp,v 1.1.2.3 2004/02/06 13:46:13 rollmark Exp $

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
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/gamestateaspect/gamestateaspect.h>

using namespace oxygen;
using namespace boost;
using namespace std;

GameStatePerceptor::GameStatePerceptor() : oxygen::Perceptor()
{
    mFirstPercept = true;
}

GameStatePerceptor::~GameStatePerceptor()
{
}

string
GameStatePerceptor::PlayMode2Str(const TPlayMode mode) const
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

void
GameStatePerceptor::InsertSoccerParam(Predicate& predicate, const std::string& name)
{
    float value;
    if (! SoccerBase::GetSoccerVar(*this,name,value))
        {
            return;
        }

    Predicate::TParameterList element;
    element.push_back(name);
    element.push_back(value);
    predicate.parameter.push_back(element);
}

void
GameStatePerceptor::InsertInitialPercept(Predicate& predicate)
{
    // uniform number
    Predicate::TParameterList element;
    element.push_back(string("unum"));
    element.push_back(mAgentState->GetUniformNumber());
    predicate.parameter.push_back(element);

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

    element.clear();
    element.push_back(string("team"));
    element.push_back(team);
    predicate.parameter.push_back(element);

    // soccer variables
    // field geometry parameter
    InsertSoccerParam(predicate,"FieldLength");
    InsertSoccerParam(predicate,"FieldWidth");
    InsertSoccerParam(predicate,"FieldHeight");
    InsertSoccerParam(predicate,"GoalWidth");
    InsertSoccerParam(predicate,"GoalDepth");
    InsertSoccerParam(predicate,"GoalHeight");
    InsertSoccerParam(predicate,"BorderSize");

    // agent parameter
    InsertSoccerParam(predicate,"AgentMass");
    InsertSoccerParam(predicate,"AgentRadius");
    InsertSoccerParam(predicate,"AgentMaxSpeed");

    // ball parameter
    InsertSoccerParam(predicate,"BallRadius");
    InsertSoccerParam(predicate,"BallMass");
}

bool
GameStatePerceptor::Percept(Predicate& predicate)
{
    if (
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0)
        )
        {
            return false;
        }

    predicate.name = "GameState";
    predicate.parameter.clear();

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

void
GameStatePerceptor::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);
}

void
GameStatePerceptor::OnUnlink()
{
    mGameState.reset();
    mAgentState.reset();
}
