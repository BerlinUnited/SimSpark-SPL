/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.h,v 1.1.2.2 2003/12/02 16:57:31 rollmark Exp $

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

#include "baseparser.h"
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>

namespace oxygen
{

class AgentAspect;

class GameControlServer : public zeitgeist::Node
{
public:
    GameControlServer();
    ~GameControlServer();

    bool Init(const std::string& parserName);

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

private:
    typedef std::map<int, boost::shared_ptr<AgentAspect> > TAgentMap;

    TAgentMap mAgentMap;
    boost::shared_ptr<BaseParser> mParser;

};

DECLARE_CLASS(GameControlServer);

} // namespace oxygen

#endif // OXYGEN_GAMECONTROLSERVER_H
