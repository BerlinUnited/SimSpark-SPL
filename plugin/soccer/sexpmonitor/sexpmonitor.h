/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sexpmonitor.h,v 1.6 2004/05/10 14:10:46 fruit Exp $

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

   FileSystemSTD
*/
#ifndef SEXPMONITOR_H__
#define SEXPMONITOR_H__

#include <string>
#include <oxygen/monitorserver/monitorsystem.h>
#include <oxygen/gamecontrolserver/baseparser.h>
#include <plugin/soccer/trainercommandparser/trainercommandparser.h>

class GameStateAspect;
class BallStateAspect;

namespace oxygen
{
    class AgentAspect;
    class Scene;
}

/** \class SexpMonitor is a monitor plugin that generates S-Expressions for the rcssmonitor3D
 *
 *  Protocoll:
 *  (Init ...) Sends information about the simulator setup
 */
class SexpMonitor : public oxygen::MonitorSystem
{
public:
    SexpMonitor();
    virtual ~SexpMonitor();

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     * @param data data sent from monitor to monitorsystem via SPADES.
     */
    void ParseMonitorMessage(const std::string& data);

    /** This function will be called periodically to get information
     * about the current state of the world.
     */
    virtual std::string GetMonitorInfo();

    /** This function is called once for every MonitorSystem. It
     *  should return any header/setup information that is needed.
     */
    virtual std::string GetMonitorHeaderInfo();

protected:
    /** called after the object has been created and attached to a
        core to allow for object dependent internal
        construction */
    virtual bool ConstructInternal();

    /** collects data from the GameStateAspect and describing
        constructs S-Expressions  */
    void AddGameState(std::ostringstream& ss);

    /** collects data from agentaspects and constructs a S-Expr description */
    void AddAgents(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss) const;

    /** collects flag data and constructs a S-Expression description */
    void AddFlags(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss);

    /** adds ball data to output string stream */
    void AddBall(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss) const;

    /** helper method that queries the ScriptServer for the float
        variable name and generates a S-Expression describing it
    */
    std::string PutFloatParam(const std::string& name);

    /** sets the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

    virtual void UpdateCached();

    void ResetSentFlags();

protected:

    // cache for parsed predicates
    boost::shared_ptr<oxygen::PredicateList> mPredicates;

    boost::shared_ptr<GameStateAspect> mGameState;

    boost::shared_ptr<oxygen::BaseParser> mSexpParser;

    boost::shared_ptr<TrainerCommandParser> mCommandParser;

    boost::shared_ptr<BallStateAspect> mBallState;

    // flags for sent information
    //! flag if we already sent the left teamname
    bool mSentLeftTeamname;
    //! flag if we already sent the left teamname
    bool mSentRightTeamname;
    //! the last half sent out to monitors
    TGameHalf mLastHalf;
    //! the last left score sent out to monitors
    int mLastLeftScore;
    //! the last right score sent out to monitors
    int mLastRightScore;
    //! the last playmode sent out to monitors
    TPlayMode mLastPlayMode;
    //! flag if the monitors received field flags information
    bool mSentFlags;

};

DECLARE_CLASS(SexpMonitor);

#endif // SEXPMONITOR_H__
