/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorparser.cpp,v 1.4.2.1 2004/03/28 15:44:18 rollmark Exp $

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
#include "monitorparser.h"

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

MonitorParser::MonitorParser() : Leaf()
{
    mParsedInfoHeader = false;
    SetupExprMap();
}

MonitorParser::~MonitorParser()
{
}

void MonitorParser::SetupExprMap()
{
    mExprMap.clear();

    mExprMap["agent"]     = ET_AGENT;
    mExprMap["flag"]      = ET_FLAG;
    mExprMap["ball"]      = ET_BALL;
    mExprMap["ballAgent"] = ET_BALLAGENT;
    mExprMap["teamL"]     = ET_TEAML;
    mExprMap["teamR"]     = ET_TEAMR;
    mExprMap["half"]      = ET_HALF;
    mExprMap["time"]      = ET_TIME;
    mExprMap["scoreL"]    = ET_SCOREL;
    mExprMap["scoreR"]    = ET_SCORER;
    mExprMap["playMode"]  = ET_PLAYMODE;
}

bool MonitorParser::ParseInfoHeader(PredicateList& predicates, GameParam& param)
{
    // true if we received an init
    bool recvInit = false;

    // first look for "(init (...))"
    // then read the inner breakets
    for (
         PredicateList::TList::const_iterator iter = predicates.begin();
         iter != predicates.end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            // check if it's the init information
            // if so, remember that we received an init
            if (predicate.name != "init")
                {
                    continue;
                } else
                    {
                        recvInit = true;
                    }

            //parse object params
            float value;

            if (GetObjectParam(predicate, "FieldLength", value))
                {
                    param.SetFieldLength(value);
                }

            if (GetObjectParam(predicate, "FieldWidth",  value))
                {
                    param.SetFieldWidth(value);
                }

            if (GetObjectParam(predicate, "FieldHeigth", value))
                {
                    param.SetFieldHeight(value);
                }

            if (GetObjectParam(predicate, "GoalWidth",   value))
                {
                    param.SetGoalWidth(value);
                }

            if (GetObjectParam(predicate, "GoalDepth",   value))
                {
                    param.SetGoalDepth(value);
                }

            if (GetObjectParam(predicate, "GoalHeight",  value))
                {
                    param.SetGoalHeight(value);
                }

            if (GetObjectParam(predicate, "BorderSize",  value))
                {
                    param.SetBorderSize(value);
                }

            if (GetObjectParam(predicate, "AgentMass",  value))
                {
                    param.SetAgentMass(value);
                }

            if (GetObjectParam(predicate, "AgentRadius",  value))
                {
                    param.SetAgentRadius(value);
                }

            if (GetObjectParam(predicate, "AgentMaxSpeed",  value))
                {
                    param.SetAgentMaxSpeed(value);
                }

            if (GetObjectParam(predicate, "BallRadius",  value))
                {
                    param.SetBallRadius(value);
                }

            if (GetObjectParam(predicate, "BallMass",  value))
                {
                    param.SetBallMass(value);
                }
        }

    return recvInit;
}

bool MonitorParser::GetObjectParam(const Predicate& predicate,
                                   const string& name, float& value)
{
    // find the PerfectVision data about the object
    Predicate::Iterator objIter(predicate);

    // advance to the section about object 'name'
    if (! predicate.FindParameter(objIter,name))
        {
            return false;
        }

    // read the position vector
    if (! predicate.GetValue(objIter,value))
        {
            return false;
        }

    return true;
}

void MonitorParser::ParseObject(const Predicate& predicate, Expression& expr)
{
    Predicate::Iterator posParam(predicate);
    if (predicate.FindParameter(posParam, "pos"))
        {
            predicate.GetValue(posParam,expr.pos);
        } else
            {
                // pos not given
                expr.pos = salt::Vector3f(0,0,0);
            }

    Predicate::Iterator unumParam(predicate);
    if (predicate.FindParameter(unumParam, "unum"))
        {
            predicate.GetValue(unumParam,expr.unum);
        } else
            {
                // unum not given
                expr.unum = 0;
            }

    Predicate::Iterator teamParam(predicate);
    if (predicate.FindParameter(teamParam, "team"))
        {
            string team;
            predicate.GetValue(teamParam,team);

            switch (team[0])
                {
                case 'L' :
                    expr.team = TI_LEFT;
                    break;

                case 'R' :
                    expr.team = TI_RIGHT;
                    break;

                default:
                case 'N' :
                    expr.team = TI_NONE;
                    break;
                }
        } else
            {
                // team not given
                expr.team = TI_NONE;
            }
}

void MonitorParser::ParseGameState(const Predicate& predicate, EExprType& type, GameState& state)
{
    switch (type)
        {
        case ET_TEAML :
            {
                std::string teaml;
                if (predicate.GetValue(predicate.begin(),teaml))
                    {
                        state.SetTeamL(teaml);
                    }
                break;
            }
        case ET_TEAMR :
            {
                std::string teamr;
                if (predicate.GetValue(predicate.begin(),teamr))
                    {
                        state.SetTeamR(teamr);
                    }
                break;
            }
        case ET_HALF :
            {
                int half;
                if (predicate.GetValue(predicate.begin(),half))
                    {
                        switch (half)
                            {
                            case 2:
                                state.SetHalf(GH_SECOND);
                                break;

                            default:
                            case 1:
                                state.SetHalf(GH_FIRST);
                                break;
                            }
                    }
                break;
            }
        case ET_TIME :
            {
                float time;
                if (predicate.GetValue(predicate.begin(),time))
                    {
                        state.SetTime(time);
                    }
                break;
            }
        case ET_SCOREL :
            {
                int score;
                if (predicate.GetValue(predicate.begin(),score))
                    {
                        state.SetScoreL(score);
                    }
                break;
            }
        case ET_SCORER :
            {
                int score;
                if (predicate.GetValue(predicate.begin(),score))
                    {
                        state.SetScoreR(score);
                    }
                break;
            }
        case ET_PLAYMODE :
            {
                int mode;
                if (predicate.GetValue(predicate.begin(),mode))
                    {
                        state.SetPlayMode((TPlayMode)mode);
                    }
                break;
            }

        default:
            break;
        }
}

bool MonitorParser::ParsePredicate(const oxygen::Predicate& predicate, GameState& state,
                                Expression& expr)
{
    // lookup the expression type corresponding to the predicate name
    TExprMap::iterator iter = mExprMap.find(predicate.name);

    if (iter == mExprMap.end())
        {
            return false;
        }

    expr.etype = (*iter).second;

    switch (expr.etype)
        {
        case ET_AGENT:
        case ET_BALLAGENT:
        case ET_BALL:
        case ET_FLAG:
            ParseObject(predicate, expr);
            break;

        case ET_TEAML:
        case ET_TEAMR:
        case ET_HALF:
        case ET_TIME:
        case ET_SCOREL:
        case ET_SCORER:
        case ET_PLAYMODE:
            ParseGameState(predicate, expr.etype, state);
            break;

        default:
            return false;
        }

    return true;
}

void MonitorParser::ParsePredicates(oxygen::PredicateList& predList,
                                    GameState& state, GameParam& param,
                                    TExprList& exprList)
{
    exprList.clear();

    if (! mParsedInfoHeader)
        {
            if (ParseInfoHeader(predList, param))
                {
                    mParsedInfoHeader = true;
                }

            return;
        }

    for (
         PredicateList::TList::const_iterator iter = predList.begin();
         iter != predList.end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            Expression expr;
            if (! ParsePredicate(predicate, state, expr))
            {
                continue;
            }

            exprList.push_back(expr);
        }
}
