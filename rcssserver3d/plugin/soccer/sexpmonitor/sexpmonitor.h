/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

   FileSystemSTD
*/
#ifndef SEXPMONITOR_H__
#define SEXPMONITOR_H__

#include <string>
#include <oxygen/monitorserver/monitorsystem.h>
#include <trainercommandparser/trainercommandparser.h>

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
    virtual void ParseMonitorMessage(const std::string& data);

    /** This function will be called periodically to get information
     * about the current state of the world.
     * \param items holds a list of additional name value pairs. These
     * predicates are collected from MonitorItem objects registered to
     * the MonitorServer. The monitor should transfer them to the
     * client if possible.
     */
    virtual std::string GetMonitorInformation(const oxygen::PredicateList& pList);

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should return any header/setup
     *  information that is needed.
     *  \param items holds a list of additional name value
     *  pairs. These predicates are collected from MonitorItem objects
     *  registered to the MonitorServer. The monitor should transfer
     *  them to the client if possible.
     */
    virtual std::string GetMonitorHeaderInfo(const oxygen::PredicateList& pList);

    /** This method can be used to set the flag for sending the rotation matrix
        of every object to the monitor.
        \param send flag wether to send the matrix or not
    */
    void SendRotationMatrix(bool send);

protected:
    /** called after the object has been created and attached to a
        core to allow for object dependent internal
        construction */
    virtual bool ConstructInternal();

    /** collects data from agentaspects and constructs a S-Expr description */
    void AddAgents(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss) const;

    /** collects flag data and constructs a S-Expression description */
    void AddFlags(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss);

    /** adds ball data to output string stream */
    void AddBall(boost::shared_ptr<oxygen::Scene> activeScene, std::ostringstream& ss) const;

    /** sets the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

    virtual void UpdateCached();

    void ResetSentFlags();

    /** constructs a S-Expression from the predicate list received
        from the MonitorServer
    */
    void AddPredicates(std::ostringstream& ss, const oxygen::PredicateList& pList);

protected:
    boost::shared_ptr<GameStateAspect> mGameState;

    boost::shared_ptr<TrainerCommandParser> mCommandParser;

    boost::shared_ptr<BallStateAspect> mBallState;

    // flag if we want to send the rotation matrix
    bool mSendRotMatrix;

    // flags for sent information
    //! flag if the monitors received field flags information
    bool mSentFlags;
};

DECLARE_CLASS(SexpMonitor);

#endif // SEXPMONITOR_H__
