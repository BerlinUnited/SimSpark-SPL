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

#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <soccertypes.h>
#include <gamestateaspect/gamestateaspect.h>
#include <oxygen/agentaspect/agentaspect.h>
#include "trainercommandparser.h"

using namespace std;
using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;

TrainerCommandParser::TrainerCommandParser() : MonitorCmdParser()
{
    // setup command map
    mCommandMap["agent"] = CT_PLAYER;
    mCommandMap["ball"] = CT_BALL;
    mCommandMap["playMode"] = CT_PLAYMODE;
    mCommandMap["dropBall"] = CT_DROP_BALL;
    mCommandMap["kickOff"] = CT_KICK_OFF;
    mCommandMap["getAck"] = CT_ACK;
    mCommandMap["select"] = CT_SELECT;
    mCommandMap["kill"] = CT_KILL;
    mCommandMap["repos"] = CT_REPOS;
    
    // setup team index map
    // Originally  team sides were "L","R" and "N"
    // But this seems to be unused
    // ParseKickOffCommand depends on the long names
    mTeamIndexMap["Left"]  = TI_LEFT;
    mTeamIndexMap["Right"] = TI_RIGHT;
    mTeamIndexMap["None"]  = TI_NONE;

    // setup play mode map
    mPlayModeMap[STR_PM_BeforeKickOff] = PM_BeforeKickOff;
    mPlayModeMap[STR_PM_KickOff_Left]  = PM_KickOff_Left;
    mPlayModeMap[STR_PM_KickOff_Right] = PM_KickOff_Right;
    mPlayModeMap[STR_PM_PlayOn]        = PM_PlayOn;
    mPlayModeMap[STR_PM_KickIn_Left]   = PM_KickIn_Left;
    mPlayModeMap[STR_PM_KickIn_Right]  = PM_KickIn_Right;
    mPlayModeMap[STR_PM_CORNER_KICK_LEFT] = PM_CORNER_KICK_LEFT;
    mPlayModeMap[STR_PM_CORNER_KICK_RIGHT] = PM_CORNER_KICK_RIGHT;
    mPlayModeMap[STR_PM_GOAL_KICK_LEFT] = PM_GOAL_KICK_LEFT;
    mPlayModeMap[STR_PM_GOAL_KICK_RIGHT] = PM_GOAL_KICK_RIGHT;
    mPlayModeMap[STR_PM_OFFSIDE_LEFT] = PM_OFFSIDE_LEFT;
    mPlayModeMap[STR_PM_OFFSIDE_RIGHT] = PM_OFFSIDE_RIGHT;
    mPlayModeMap[STR_PM_FREE_KICK_LEFT] = PM_FREE_KICK_LEFT;
    mPlayModeMap[STR_PM_FREE_KICK_RIGHT] = PM_FREE_KICK_RIGHT;
    mPlayModeMap[STR_PM_Goal_Left]     = PM_Goal_Left;
    mPlayModeMap[STR_PM_Goal_Right]    = PM_Goal_Right;
    mPlayModeMap[STR_PM_GameOver]      = PM_GameOver;

    mGetAck = false;
}

TrainerCommandParser::~TrainerCommandParser()
{

}

bool
TrainerCommandParser::SendAck(std::string &reply)
{
    if (!mGetAck)
    {
        return false;
    }

    reply = "best";
    mGetAck= false;
    return true;
}

void
TrainerCommandParser::OnLink()
{
    // we need the SexpParser to generate the predicates
    // from S-Expressions
    mSexpParser = shared_dynamic_cast<oxygen::BaseParser>(GetCore()->New("SexpParser"));

    if (mSexpParser.get() == 0)
        {
            GetLog()->Error() << "ERROR: (TrainerCommnadParser) failed to create SexpParser\n";
            return;
        }

    mGameControl = shared_dynamic_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));

    if (mGameControl.get() == 0)
        {
            GetLog()->Error() << "ERROR: (TrainerCommandParser) Unable to get GameControlServer\n";
        }

}

void TrainerCommandParser::OnUnlink()
{
    mSexpParser.reset();
}

void TrainerCommandParser::ParseMonitorMessage(const std::string& data)
{
    if (mSexpParser.get() == 0)
    {
        GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get SexpParser\n";
        return;
    }

    boost::shared_ptr<PredicateList> predList = mSexpParser->Parse(data);
    ParsePredicates(*predList);
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

bool
TrainerCommandParser::ParsePredicate(const oxygen::Predicate & predicate)
{
    SoccerBase::GetGameState(*this,mGameState);
    SoccerBase::GetSoccerRuleAspect(*this,mSoccerRule);

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
    case CT_DROP_BALL:
        // DEBUG
        GetLog()->Error() << "(TrainerCommandParser) Dropping ball.\n";
        mSoccerRule->DropBall();
        break;
    case CT_KICK_OFF:
        ParseKickOffCommand(predicate);
        break;
    case CT_ACK:
    {
        mGetAck=true;
        // Predicate::Iterator ackParam(predicate);
        // ++ackParam;

        /* if (! predicate.GetValue(ackParam, mAckString))
         {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get mAckString\n";
            return false;
            }*/

        break;
    }
    case CT_SELECT:
        ParseSelectCommand(predicate);
        break;
    case CT_KILL:
        ParseKillCommand(predicate);
        break;
    case CT_REPOS:
        ParseReposCommand(predicate);
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
    bool specified = true;

    // extract unum
    if (predicate.FindParameter(unumParam, "unum"))
    {
        if (! predicate.GetValue(unumParam, unum))
          specified = false;
    }
    else
      specified = false;
    
    string team;
    TTeamIndex idx;
    Predicate::Iterator teamParam(predicate);

    // extract side
    if (predicate.FindParameter(teamParam, "team"))
    {
        if (! predicate.GetValue(teamParam, team))
          specified = false;
        else
            idx = mTeamIndexMap[team];
    }    
    else
      specified = false;

    if (!specified)
    {
      mSoccerRule->ClearSelectedPlayers();
      return;
    }
    
    SoccerBase::TAgentStateList agentStates;
    SoccerBase::GetAgentStates(*this, agentStates, (specified ? idx : TI_NONE)); 
    SoccerBase::TAgentStateList::iterator iter = agentStates.begin();
    bool found = false;

    while (iter != agentStates.end() && !found)
        {   
            if ((specified && (*iter)->GetUniformNumber() == unum && (*iter)->GetTeamIndex() == idx) ||
                (!specified && (*iter)->IsSelected()))
                found = true;                     
            else
                ++iter;
        }

    if (!found)
        {
           GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get correct AgentState\n";
           return; 
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

        boost::shared_ptr<Transform> agent_aspect;

        if (SoccerBase::GetTransformParent(*(*iter), agent_aspect))
        {
            // move all the bodies belonging to this agent
            SoccerBase::MoveAgent(agent_aspect, pos);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent body\n";
            return;
        }
    }

    Predicate::Iterator moveParam(predicate);
    if (predicate.FindParameter(moveParam, "move"))
    {
        salt::Vector3f pos;
        float ang;

        // extract position vector
        if (! predicate.GetValue(moveParam, pos))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent rot\n";
            return;
        }
        // extract direction
        if (! predicate.GetValue(moveParam, ang))
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent ang\n";
            return;
        }

        boost::shared_ptr<Transform> agent_aspect;

        if (SoccerBase::GetTransformParent(*(*iter), agent_aspect))
        {
            // move all the bodies belonging to this agent
            SoccerBase::MoveAndRotateAgent(agent_aspect, pos, ang);
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get agent body\n";
            return;
        }
    }

    // Joschka: I removed the part to set a velocity because it doesn't really  
    // seem to have a meaning for agents that have more than just a single body

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

        // set new battery
        (*iter)->SetBattery(battery);
        
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
       
         // set new temperature
        (*iter)->SetBattery(temperature);
        
    }
}

void
TrainerCommandParser::ParseBallCommand(const oxygen::Predicate& predicate)
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

        boost::shared_ptr<RigidBody> body;

        if (SoccerBase::GetBallBody(*this, body))
        {
            // set new position in the body
            body->SetPosition(pos);
            body->Enable();
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

        boost::shared_ptr<RigidBody> body;

        if (SoccerBase::GetBallBody(*this, body))
        {
            // set new velocity in the body
            body->SetVelocity(vel);
            body->SetAngularVelocity(salt::Vector3f(0.0f,0.0f,0.0f));
            body->Enable();
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get ball body\n";
            return;
        }
    }
}

void
TrainerCommandParser::ParsePlayModeCommand(const oxygen::Predicate& predicate)
{
    Predicate::Iterator pmParam(predicate);
    string mode;

    if (predicate.GetValue(pmParam,mode))
    {
        TPlayModeMap::const_iterator playmode = mPlayModeMap.find(mode);
        if (playmode != mPlayModeMap.end())
        {
            mGameState->SetPlayMode(playmode->second);
        }
        else
        {
            GetLog()->Debug()
                << "(TrainerCommandParser) ERROR: an unknown playmode"
                << mode << " was passed\n";
        }
    }
    else
    {
        GetLog()->Debug()
            << "(TrainerCommandParser) ERROR: could not parse playmode "
            << mode << "\n";
    }
}


void
TrainerCommandParser::ParseKickOffCommand(const oxygen::Predicate& predicate)
{
    Predicate::Iterator koParam(predicate);
    string team;


    if (predicate.GetValue(koParam,team))
    {
        TTeamIndexMap::const_iterator kickoffteam = mTeamIndexMap.find(team);
        if (kickoffteam != mTeamIndexMap.end())
        {
            if (mGameState.get() == 0)
            {
                GetLog()->Error() << "(TrainerCommandParser) ERROR "
                                  << "no GameStateAspect found, cannot kick off\n";
            }
            else
            {
                mGameState->KickOff(kickoffteam->second);
            }
        }
        else
        {
            GetLog()->Error() << "(TrainerCommandParser) ERROR: unknown team"
                              << team << "\n";
            
        }
    }
    else
    {
        GetLog()->Debug()
            << "(TrainerCommandParser) ERROR: could not parse team "
            << team << "\n";
    }
}

void TrainerCommandParser::ParseSelectCommand(const oxygen::Predicate & predicate)
{    
    Predicate::Iterator unumParam(predicate);
    int                 unum;
    bool specified = true;
    
    boost::shared_ptr<SoccerRuleAspect> soccerRuleAspect;
    if (!SoccerBase::GetSoccerRuleAspect(*this, soccerRuleAspect))
    {
        GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get soccer rule aspect\n";
        return;
    }

    // extract unum
    if (predicate.FindParameter(unumParam, "unum"))
    {
        if (! predicate.GetValue(unumParam, unum))
          specified = false;
    }
    else
      specified = false;
    
    if (specified && unum == -1)
    {
        soccerRuleAspect->ResetAgentSelection();
        return;
    }
      
    string team;
    TTeamIndex idx;
    Predicate::Iterator teamParam(predicate);

    // extract side
    if (predicate.FindParameter(teamParam, "team"))
    {
        if (! predicate.GetValue(teamParam, team))
          specified = false;
        else
            idx = mTeamIndexMap[team];
    }    
    else
      specified = false;
      
    if (!specified)
    {
        soccerRuleAspect->SelectNextAgent();
        return;
    }
    
    SoccerBase::TAgentStateList agentStates;
    SoccerBase::GetAgentStates(*this, agentStates, idx); 
    SoccerBase::TAgentStateList::iterator iter = agentStates.begin();
    bool found = false;

    while (iter != agentStates.end() && !found)
        {   
            if ((*iter)->GetUniformNumber() == unum)                
                {
                    found = true;                     
                }
            else
                ++iter;
        }

    if (!found)
        {
           GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get correct AgentState\n";
           return; 
        }

    soccerRuleAspect->ResetAgentSelection();
    (*iter)->Select();
}

void TrainerCommandParser::ParseKillCommand(const oxygen::Predicate & predicate)
{    
    Predicate::Iterator unumParam(predicate);
    int                 unum;
    bool specified = true;
    
    boost::shared_ptr<SoccerRuleAspect> soccerRuleAspect;
    if (!SoccerBase::GetSoccerRuleAspect(*this, soccerRuleAspect))
    {
        GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get soccer rule aspect\n";
        return;
    }

    // extract unum
    if (predicate.FindParameter(unumParam, "unum"))
    {
        if (! predicate.GetValue(unumParam, unum))
          specified = false;
    }
    else
      specified = false;
    
    string team;
    TTeamIndex idx;
    Predicate::Iterator teamParam(predicate);

    // extract side
    if (predicate.FindParameter(teamParam, "team"))
    {
        if (! predicate.GetValue(teamParam, team))
          specified = false;
        else
            idx = mTeamIndexMap[team];
    }    
    else
      specified = false;
    
    GameControlServer::TAgentAspectList agentAspects;
    mGameControl->GetAgentAspectList(agentAspects);
    GameControlServer::TAgentAspectList::iterator aaiter;
    for (
          aaiter = agentAspects.begin();
          aaiter != agentAspects.end();
          ++aaiter
        )
    {
        // search for the first agent of the left/right side
        boost::shared_ptr<AgentState> agentState =
            shared_dynamic_cast<AgentState>((*aaiter)->GetChild("AgentState", true));

        if ((specified && agentState->GetUniformNumber() == unum && agentState->GetTeamIndex() == idx) ||
            (!specified && agentState->IsSelected()))
        {
            mGameControl->pushDisappearedAgent((*aaiter)->ID());
            break;
        }
    }
}

void TrainerCommandParser::ParseReposCommand(const oxygen::Predicate & predicate)
{    
        cerr << "repos 2" << endl;

    Predicate::Iterator unumParam(predicate);
    int                 unum;
    bool specified = true;
    
    boost::shared_ptr<SoccerRuleAspect> soccerRuleAspect;
    if (!SoccerBase::GetSoccerRuleAspect(*this, soccerRuleAspect))
    {
        GetLog()->Error() << "(TrainerCommandParser) ERROR: can't get soccer rule aspect\n";
        return;
    }

    // extract unum
    if (predicate.FindParameter(unumParam, "unum"))
    {
        if (! predicate.GetValue(unumParam, unum))
          specified = false;
    }
    else
      specified = false;
    
    string team;
    TTeamIndex idx;
    Predicate::Iterator teamParam(predicate);

    // extract side
    if (predicate.FindParameter(teamParam, "team"))
    {
        if (! predicate.GetValue(teamParam, team))
          specified = false;
        else
            idx = mTeamIndexMap[team];
    }    
    else
      specified = false;

    SoccerBase::TAgentStateList agentStates;
    SoccerBase::GetAgentStates(*this, agentStates, TI_NONE); 
    SoccerBase::TAgentStateList::iterator iter = agentStates.begin();
    boost::shared_ptr<oxygen::Transform> agent_aspect;
    for (;iter != agentStates.end(); ++iter)
    {
        if ((specified && (*iter)->GetUniformNumber() == unum && (*iter)->GetTeamIndex() == idx) ||
            (!specified && (*iter)->IsSelected()))
        {
            Vector3f ballPos(0,0,0);
            boost::shared_ptr<RigidBody> ballBody;

            if (SoccerBase::GetBallBody(*this, ballBody))
                ballPos = ballBody->GetPosition();

            SoccerBase::GetTransformParent(**iter, agent_aspect);
        cerr << "repos 3" << endl;
            Vector3f new_pos = mSoccerRule->RepositionOutsidePos(ballPos, (*iter)->GetUniformNumber(), (*iter)->GetTeamIndex()); 
            SoccerBase::MoveAgent(agent_aspect, new_pos);     

            break;
        }
    }
}
    
