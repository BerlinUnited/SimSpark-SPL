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
#ifndef TRAINERCOMMANDPARSER_H
#define TRAINERCOMMANDPARSER_H

#include <string>
#include <zeitgeist/class.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <salt/vector.h>
#include <soccer/soccertypes.h>
#include <soccer/soccerruleaspect/soccerruleaspect.h>

class TrainerCommandParser : public zeitgeist::Leaf
{
public:

    enum ECommandType
    {
        CT_PLAYER,
        CT_BALL,
        CT_PLAYMODE,
        CT_DROP_BALL,
        CT_KICK_OFF,
        CT_ACK
    };

    typedef std::map<std::string, ECommandType>  TCommandMap;
    typedef std::map<std::string, TTeamIndex> TTeamIndexMap;

    // mapping from string to TPlayMode
    typedef std::map<std::string, TPlayMode> TPlayModeMap;

public:
    TrainerCommandParser();

    virtual ~TrainerCommandParser();

    /** parses the list of predicates; returns true on success
     */
    void ParsePredicates(oxygen::PredicateList& predList);

    bool SendAck(std::string &reply);

protected:

    /** parses the given predicate and calls one of the specialized
        parse methods given below depending the predicate name;
        returns true if successful
     */
    bool ParsePredicate(const oxygen::Predicate & predicate);

    /** parses and executes the player command contained in the given
        predicate
    */
    void ParsePlayerCommand(const oxygen::Predicate & predicate);

    /** parses and executes the ball command contained in the given
        predicate
    */
    void ParseBallCommand(const oxygen::Predicate & predicate);

    /** parses and executes the play mode command contained in the given
        predicate
    */
    void ParsePlayModeCommand(const oxygen::Predicate & predicate);

protected:
    TCommandMap    mCommandMap;

    TTeamIndexMap  mTeamIndexMap;

    TPlayModeMap   mPlayModeMap;

    //! cached reference for the gamestate aspect
    boost::shared_ptr<GameStateAspect> mGameState;
    //! cached reference for the soccer rule aspect
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule;

    bool mGetAck;
    std::string mAckString;
};

DECLARE_CLASS(TrainerCommandParser);

#endif // TRAINERCOMMANDPARSER_H
