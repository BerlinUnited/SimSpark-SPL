/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of the trainer for rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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

   Parser that gets a list of predicates and interprets the trainer
   commands contained in them

*/

#include <oxygen/physicsserver/body.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <soccertypes.h>
#include <gamestateaspect/gamestateaspect.h>
#include "trainercommandparser.h"

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

TrainerCommandParser::TrainerCommandParser() : Leaf()
{
    // setup command map
    mCommandMap["agent"]    = CT_PLAYER;
    mCommandMap["ball"]     = CT_BALL;
    mCommandMap["playMode"] = CT_PLAYMODE;

    // setup team index map
    mTeamIndexMap["L"]       = TI_LEFT;
    mTeamIndexMap["R"]       = TI_RIGHT;
    mTeamIndexMap["N"]       = TI_NONE;

    // setup play mode map
    mPlayModeMap[STR_PM_BeforeKickOff] = PM_BeforeKickOff;
    mPlayModeMap[STR_PM_KickOff_Left]  = PM_KickOff_Left;
    mPlayModeMap[STR_PM_KickOff_Right] = PM_KickOff_Right;
    mPlayModeMap[STR_PM_PlayOn]        = PM_PlayOn;
    mPlayModeMap[STR_PM_KickIn_Left]   = PM_KickIn_Left;
    mPlayModeMap[STR_PM_KickIn_Right]  = PM_KickIn_Right;
    mPlayModeMap[STR_PM_Goal_Left]     = PM_Goal_Left;
    mPlayModeMap[STR_PM_Goal_Right]    = PM_Goal_Right;
    mPlayModeMap[STR_PM_GameOver]      = PM_GameOver;
}

TrainerCommandParser::~TrainerCommandParser()
{

}

void TrainerCommandParser::ParsePredicates(oxygen::PredicateList & predList)
{
    for (
         PredicateList::TList::const_iterator iter = predList.begin();
         iter != predList.end();
         ++iter
         )
    {
        const Predicate & predicate = (*iter);

        if (! ParsePredicate(predicate))
        {
            continue;
        }
    }
}

bool TrainerCommandParser::ParsePredicate(const oxygen::Predicate & predicate)
{
    // lookup the command type corresponding to the predicate name
    TCommandMap::iterator iter = mCommandMap.find(predicate.name);

    if (iter == mCommandMap.end())
    {
        return false;
    }

    switch ((*iter).second)
    {
    case CT_PLAYER:
        ParsePlayerCommand(predicate);
        break;
    case CT_BALL:
        ParseBallCommand(predicate);
        break;
    case CT_PLAYMODE:
        ParsePlayModeCommand(predicate);
        break;
    default:
        return false;
    }

    return true;
}

void TrainerCommandParser::ParsePlayerCommand(const oxygen::Predicate & predicate)
{

    Predicate::Iterator unumParam(predicate);
    int                 unum;

    // extract unum
    if (predicate.FindParameter(unumParam, "unum"))
    {
        if (! predicate.GetValue(unumParam, unum))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get unum\n";
            return;
        }
    }

    string              team;
    TTeamIndex          idx;
    Predicate::Iterator teamParam(predicate);

    // extract side
    if (predicate.FindParameter(teamParam, "team"))
    {
        if (! predicate.GetValue(teamParam, team))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get team name\n";
            return;
        }

        idx = mTeamIndexMap[team];
    }

    Predicate::Iterator posParam(predicate);

    if (predicate.FindParameter(posParam, "pos"))
    {
        salt::Vector3f pos;

        // extract position vector
        if (! predicate.GetValue(posParam, pos))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent pos\n";
            return;
        }

        shared_ptr<Body> body;

        if (SoccerBase::GetAgentBody(*this, idx, unum, body))
        {
            // set new velocity in the body
            body->SetPosition(pos);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent body\n";
            return;
        }
    }

    Predicate::Iterator velParam(predicate);

    if (predicate.FindParameter(velParam, "vel"))
    {
        salt::Vector3f vel;

        // extract velocity vector
        if (! predicate.GetValue(velParam, vel))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent vel\n";
            return;
        }

        shared_ptr<Body> body;

        if (SoccerBase::GetAgentBody(*this, idx, unum, body))
        {
            // set new velocity in the body
            body->SetVelocity(vel);
            body->SetAngularVelocity(salt::Vector3f(0.0f,0.0f,0.0f));
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent body\n";
            return;
        }
    }

    Predicate::Iterator batParam(predicate);

    if (predicate.FindParameter(batParam, "battery"))
    {
        double battery;

        // extract battery value
        if (! predicate.GetValue(batParam, battery))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get battery value\n";
            return;
        }

        shared_ptr<AgentState> agentState;

        // get agent state
        if (SoccerBase::GetAgentState(*this, idx, unum, agentState))
        {
            // set new battery
            agentState->SetBattery(battery);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent state\n";
            return;
        }
    }

    Predicate::Iterator tempParam(predicate);

    if (predicate.FindParameter(tempParam, "temperature"))
    {
        float temperature;

        // extract temperature value
        if (! predicate.GetValue(tempParam, temperature))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get temperatur value\n";
            return;
        }

        shared_ptr<AgentState> agentState;

        // get agent state
        if (SoccerBase::GetAgentState(*this, idx, unum, agentState))
        {
            // set new temperature
            agentState->SetBattery(temperature);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent state\n";
            return;
        }
    }
}

void TrainerCommandParser::ParseBallCommand(const oxygen::Predicate & predicate)
{
    Predicate::Iterator posParam(predicate);

    if (predicate.FindParameter(posParam, "pos"))
    {
        salt::Vector3f pos;

        // extract position vector
        if (! predicate.GetValue(posParam, pos))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get ball pos\n";
            return;
        }

        shared_ptr<Body> body;

        if (SoccerBase::GetBallBody(*this, body))
        {
            // set new velocity in the body
            body->SetPosition(pos);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get ball body\n";

            return;
        }
    }

    Predicate::Iterator velParam(predicate);

    if (predicate.FindParameter(velParam, "vel"))
    {
        salt::Vector3f vel;

        // extract velocity vector
        if (! predicate.GetValue(velParam, vel))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get ball vel\n";
            return;
        }

        shared_ptr<Body> body;

        if (SoccerBase::GetBallBody(*this, body))
        {
            // set new velocity in the body
            body->SetVelocity(vel);
            body->SetAngularVelocity(salt::Vector3f(0.0f,0.0f,0.0f));
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get ball body\n";
            return;
        }
    }
}

void TrainerCommandParser::ParsePlayModeCommand(const oxygen::Predicate & predicate)
{
    Predicate::Iterator pmParam(predicate);
    string              mode;

    if (predicate.GetValue(pmParam,mode))
    {
        if (mPlayModeMap.find(mode) != mPlayModeMap.end())
        {
            // set new playmode
            shared_ptr<GameStateAspect> gameState;

            SoccerBase::GetGameState(*this,gameState);

            gameState->SetPlayMode(mPlayModeMap[mode]);
        }
        else
        {
            GetLog()->Debug()
                << "(TrainerCommandParser) ERROR: unknown playmode"
                << mode << "was passed\n";
        }
    }
    else
    {
        GetLog()->Debug()
            << "(TrainerCommandParser) ERROR: could not parse playmode "
            << mode << "\n";
    }
}
