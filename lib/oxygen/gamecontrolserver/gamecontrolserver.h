/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.h,v 1.3 2003/12/27 17:53:41 fruit Exp $

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
#ifndef OXYGEN_GAMECONTROLSERVER_H
#define OXYGEN_GAMECONTROLSERVER_H

#include "actionobject.h"
#include "baseparser.h"
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>

namespace oxygen
{
class AgentAspect;
class Effector;
class Scene;

class GameControlServer : public zeitgeist::Node
{
public:
    GameControlServer();
    ~GameControlServer();

    /** creates a parser \param parserName instance and registers it
        to the GameControlServer
     */
    bool InitParser(const std::string& parserName);

    /** sets \param effectorName as the effector class that is
        initially created with every new AgentAspect instance
    */
    void InitEffector(const std::string& effectorName);

    /** returns the parser currently registered to the
        GameControlServer
    */
    boost::shared_ptr<BaseParser> GetParser();

    /** parses a command string using the registerd parser and uses
        the registered effectors of the agent identified by param \id
        to construct an ActionObject. This method must be const as it
        is called from SpadesServer::ParseAct, which is const to
        prevent it from modifying the world model.
    */
    boost::shared_ptr<ActionObject::TList> Parse(int id, std::string str) const;

    /** notifies the GameControlServer that an agent has connected to
        the simulation. \param id should be a unique identifier for
        the new agent. It is not assigned by the GameControlServer.
    */
    bool AgentConnect(int id);

    /** notifies the GameControlServer that an agent has disappeared
        from the simulation. \param id was the unique identifier of
        the disappeared agent. The GameControlServer uses the
        AgentAspect corresponding to \param id to clean up after the
        agent left
    */
    bool AgentDisappear(int id);

    /** returns the time between to queries of the agents
        perceptors in seconds
    */
    float GetSenseInterval(int id);

    /** GetSenseLatency returns latency it takes to query the sensors
        of an agent in seconds
     */
    float GetSenseLatency(int id);

    /** GetActionLatency returns the latency it takes to realize an
        agent action in seconds
    */
    float GetActionLatency(int id);

    /** returns the AgentAspect for the given \param id */
    boost::shared_ptr<AgentAspect> GetAgentAspect(int id);

protected:
    /** helper method that queries the SceneServer for the currently
        active Scene
     */
    boost::shared_ptr<Scene> GetActiveScene();

protected:
    typedef std::map<int, boost::shared_ptr<AgentAspect> > TAgentMap;

    /** a map from agent IDs to agent instances */
    TAgentMap mAgentMap;

    /** the parser instance used */
    boost::shared_ptr<BaseParser> mParser;

    /** the name of the initial effector class of each new agent has
        to construct all remaining parts */
    std::string mCreateEffector;
};

DECLARE_CLASS(GameControlServer);

} // namespace oxygen

#endif // OXYGEN_GAMECONTROLSERVER_H
