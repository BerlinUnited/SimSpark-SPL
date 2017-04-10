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
#include <oxygen/monitorserver/monitorcmdparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/gamecontrolserver/baseparser.h>
#include <salt/vector.h>
#include <soccertypes.h>
#include <soccerruleaspect/soccerruleaspect.h>
#include <oxygen/simulationserver/simulationserver.h>
#include <oxygen/simulationserver/monitorcontrol.h>

namespace oxygen
{
  class GameControlServer;
}

class TrainerCommandParser : public oxygen::MonitorCmdParser
{
public:

    enum ECommandType
    {
        CT_PLAYER,
        CT_BALL,
        CT_PLAYMODE,
        CT_DROP_BALL,
        CT_KICK_OFF,
        CT_ACK,
        CT_SELECT,
        CT_KILL,
        CT_REPOS,
        CT_KILLSIM,
        CT_REQFULLSTATE,
        CT_TIME,
        CT_SCORE
    };

    typedef std::map<std::string, ECommandType>  TCommandMap;
    typedef std::map<std::string, TTeamIndex> TTeamIndexMap;

    // mapping from string to TPlayMode
    typedef std::map<std::string, TPlayMode> TPlayModeMap;

public:
    TrainerCommandParser();

    virtual ~TrainerCommandParser();

    bool SendAck(std::string &reply);

    /** This function will be called be called from the monitor server
        implementation to parse any command strings received from the
        monitor client process
     */
    virtual void ParseMonitorMessage(const std::string& data);

    virtual void OnLink();

    virtual void OnUnlink();

protected:
    /** parses the list of predicates; returns true on success
     */
    void ParsePredicates(oxygen::PredicateList& predList);

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

    /** parses and executes the kick off command contained in the given
        predicate
    */
    void ParseKickOffCommand(const oxygen::Predicate & predicate);
    
    /** parses and executes the select command contained in the given
        predicate
    */
    void ParseSelectCommand(const oxygen::Predicate & predicate);

    /** parses and executes the kill command contained in the given
        predicate
    */
    void ParseKillCommand(const oxygen::Predicate & predicate);

    /** parses and executes the reposition command contained in the given
        predicate
    */
    void ParseReposCommand(const oxygen::Predicate & predicate);
    
    /** parses and executes the killsim command  contained in the given
        predicate
    */
    void ParseKillSimCommand(const oxygen::Predicate & predicate);

    /** parses and executes the time command contained in the given
        predicate
    */
    void ParseTimeCommand(const oxygen::Predicate & predicate);

    /** parses and executes the score command contained in the given
        predicate
    */
    void ParseScoreCommand(const oxygen::Predicate & predicate);
    
    
protected:
    TCommandMap    mCommandMap;

    TTeamIndexMap  mTeamIndexMap;

    TPlayModeMap   mPlayModeMap;

    //! cached reference for the gamestate aspect
    boost::shared_ptr<GameStateAspect> mGameState;
    //! cached reference for the soccer rule aspect
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule;
    //! the parser used to create the PredicateList
    boost::shared_ptr<oxygen::BaseParser> mSexpParser;
    //! cached reference to the game control server
    boost::shared_ptr<oxygen::GameControlServer> mGameControl;
    //! cached reference to the simulation server
    boost::shared_ptr<oxygen::SimulationServer> mSimServer;
    //! cahced reference to the monitor control node
    boost::shared_ptr<oxygen::MonitorControl> mMonitorControl;
    
    bool mGetAck;
    std::string mAckString;
};

DECLARE_CLASS(TrainerCommandParser);

#endif // TRAINERCOMMANDPARSER_H
