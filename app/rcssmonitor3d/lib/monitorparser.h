/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorparser.h,v 1.2.2.1 2004/03/28 15:44:18 rollmark Exp $

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
#ifndef MONITORPARSER_H
#define MONITORPARSER_H

#include <string>
#include <zeitgeist/class.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <salt/vector.h>
#include <soccertypes.h>
#include "gameparam.h"
#include "gamestate.h"

class MonitorParser : public zeitgeist::Leaf
{
public:
    // possible s-expression types
    enum EExprType
        {
            ET_AGENT,     // an agent
            ET_BALLAGENT, // the agent with the most recent ball
                          // contact
            ET_BALL,      // the ball
            ET_FLAG,      // a flag marking the playing field
            ET_TEAML,     // team name left
            ET_TEAMR,     // team name right
            ET_HALF,      // half time (1 or 2)
            ET_TIME,      // game time in seconds
            ET_SCOREL,    // score of left team
            ET_SCORER,    // score of right team
            ET_PLAYMODE,  // playmode
        };

    struct Expression
    {
        EExprType etype;

        //! object position
        salt::Vector3f pos;

        //! team index of a player
        TTeamIndex team;

        //! uniform number of a player
        int unum;
    };

    typedef std::map<std::string, EExprType> TExprMap;
    typedef std::list<Expression> TExprList;

public:
    MonitorParser();
    virtual ~MonitorParser();

    /** parses the list of predicates into the given expression list
        and GameState; returns true on success
     */
    void ParsePredicates(oxygen::PredicateList& predList,
                         GameState& state, GameParam& param,
                         TExprList& exprList);

protected:
    //! sets up the expression map
    void SetupExprMap();


    /** parses the game parameters sent by the server during the init
        sequence
     */
    bool ParseInfoHeader(oxygen::PredicateList& predicates,
                         GameParam& param);

    /** reads the parameters for an object with given name from given
        predicate
     */
    bool GetObjectParam(const oxygen::Predicate& predicate,
                        const std::string& name, float& value);

    /** parses the given predicate and filles the given Expression
        struct; returns true if successful
     */
    bool ParsePredicate(const oxygen::Predicate& predicate, GameState& state,
                     Expression& expr);

    /** parses the gamestate information contained in the given
        predicate into the GameState object
    */
    void ParseGameState(const oxygen::Predicate& predicate,
                        EExprType& type, GameState& state);

    /** parses object information contained in the given predicate
        into the Expression struct
    */
    void ParseObject(const oxygen::Predicate& predicate, Expression& expr);


protected:
    //! true, if an infor header was succesfully parsed
    bool mParsedInfoHeader;
    //! mapping from string to expression type
    TExprMap mExprMap;
};

DECLARE_CLASS(MonitorParser);

#endif // MONITORPARSER_H
