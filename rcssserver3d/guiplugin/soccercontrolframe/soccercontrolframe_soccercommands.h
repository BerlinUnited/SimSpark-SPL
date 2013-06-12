/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Author: Patrick Geib
   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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

#ifndef SOCCERCONTROLFRAME_SOCCERCOMMANDS_H
#define SOCCERCONTROLFRAME_SOCCERCOMMANDS_H
/**
 * \file soccercontrolframe_soccercommands.h
 * \brief Contains SoccerControlFrameUtil::SoccerCommand and derived command classes.
 */

#include <boost/shared_ptr.hpp>

#include <isimcontrol.h>
#include <cutelogger/logger.h>
#include <sparkcommands.h>

#include <soccer/soccertypes.h>

class SparkController;
class GameStateAspect;
class AgentAspect;
class BallStateAspect;
class SoccerRuleAspect;

namespace SoccerControlFrameUtil
{

class CommandQueue;

/*!
 \brief Abstract parent class for soccer commands that can be queued in a SparkCommand::CommandQueue.

 Queued commands are executed by the simulation server thread at the end of a simulation cycle.
*/
class SoccerCommand : public SparkCommand::Command
{
    friend class CommandQueue;

public:
    //public types

    /*!
     \brief Types of soccer commands.
    */
    enum ESoccerCommandType
    {
        SCT_CHANGE_PLAY_MODE,     /*!< Changes the play mode. */
        SCT_SET_TIME,             /*!< Changes the game time. */
        SCT_KICKOFF,              /*!< Changes the play mode to KickOff. */
        SCT_SET_TEAM_GOALS,       /*!< Sets the goals of a team. */
        SCT_SET_TEAM_NAME,        /*!< Sets the name of a team. */
        SCT_SET_AGENT_BATTERY,    /*!< Sets the battery state of an agent. */
        SCT_SET_AGENT_TEMPERATURE /*!< Sets the temperature state of an agent. */
    };

public:
    // public functions

    /*!
     \brief Destructor.
    */
    virtual ~SoccerCommand();

    /*!
     \brief Abstract execution function.
    */
    virtual bool execute() = 0;
    /*!
     \brief Abstract string conversion function.
    */
    virtual QString toString() = 0;

protected:
    //protected functions

    /*!
     \brief Protected constructor.

     \param type soccer command type id(ESoccerCommandType)
    */
    SoccerCommand(int type);
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes the soccer play mode.
*/
class ChangePlayMode : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor

     \param gameState game state aspect to change
     \param mode new play mode to change to
    */
    ChangePlayMode(boost::shared_ptr<GameStateAspect> gameState, TPlayMode mode) :
      SoccerCommand(SCT_CHANGE_PLAY_MODE), mMode(mode), mGameState(gameState) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "ChangePlayMode";}

private:
    //private members

    boost::shared_ptr<GameStateAspect> mGameState; /*!< Game state aspect to change. */
    TPlayMode mMode; /*!< New play mode to change to. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes the current game time.
*/
class SetTime : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param gameState game state aspect to change
     \param time value to change game time to
    */
    SetTime(boost::shared_ptr<GameStateAspect> gameState, float time) :
      SoccerCommand(SCT_SET_TIME), mTime(time), mGameState(gameState) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "SetTime";}

private:
    //private members

    boost::shared_ptr<GameStateAspect> mGameState; /*!< Game state aspect to change. */
    float mTime; /*!< Value to change game time to. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes the soccer play mode to "KickOff".
*/
class KickOff : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param gameState game state aspect to change
    */
    KickOff(boost::shared_ptr<GameStateAspect> gameState) :
      SoccerCommand(SCT_KICKOFF), mGameState(gameState) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "KickOff";}

private:
    //private members

    boost::shared_ptr<GameStateAspect> mGameState; /*!< Game state aspect to change. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes a teams goal value.
*/
class SetTeamGoals : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param gameState game state aspect to change
     \param team team to change goals for
     \param goals new goal value
    */
    SetTeamGoals(boost::shared_ptr<GameStateAspect> gameState, TTeamIndex team, int goals) :
      SoccerCommand(SCT_SET_TEAM_GOALS), mTeam(team), mGoals(goals), mGameState(gameState) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "SetTeamGoals";}

private:
    boost::shared_ptr<GameStateAspect> mGameState; /*!< Game state aspect to change. */
    TTeamIndex mTeam; /*!< Team to change goals for. */
    int mGoals; /*!< New goal value. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes a teams name.
*/
class SetTeamName : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param gameState game state aspect to change
     \param team id of team to change name for
     \param name new name for the team
    */
    SetTeamName(boost::shared_ptr<GameStateAspect> gameState, TTeamIndex team, const QString& name) :
      SoccerCommand(SCT_SET_TEAM_NAME), mTeam(team), mName(name), mGameState(gameState) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "SetTeamName";}

private:
    //private members

    boost::shared_ptr<GameStateAspect> mGameState; /*!< Game state aspect to change. */
    TTeamIndex mTeam; /*!< Id of team to change name for. */
    QString mName; /*!< New name for the team. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes an agents battery value.
*/
class SetAgentBattery : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param agent id of the agent
     \param value new battery value
    */
    SetAgentBattery(int agent, float value) :
      SoccerCommand(SCT_SET_AGENT_BATTERY), mAgent(agent), mValue(value) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "SetAgentBattery";}

private:
    //private members

    int mAgent; /*!< Id of the agent. */
    float mValue; /*!< New battery value. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Changes an agents temperature value.
*/
class SetAgentTemperature : public SoccerCommand
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param agent id of the agent
     \param value new temperature value
    */
    SetAgentTemperature(int agent, float value) :
      SoccerCommand(SCT_SET_AGENT_TEMPERATURE), mAgent(agent), mValue(value) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "SetAgentTemperature";}

private:
    //private members

    int mAgent; /*!< Id of the agent. */
    float mValue; /*!< New temperature value. */
};

} //SoccerControlFrameUtil

#endif //SOCCERCONTROLFRAME_SOCCERCOMMANDS_H
