/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.cpp,v 1.1.2.3 2003/12/02 16:57:31 rollmark Exp $

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
#include "gamecontrolserver.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;

GameControlServer::GameControlServer() : zeitgeist::Node()
{
}

GameControlServer::~GameControlServer()
{
}

bool
GameControlServer::Init(const std::string& parserName)
{
    GetLog()->Debug() << "GameControlServer::Init: creating parser: "
                      << parserName << "\n";
    mParser = boost::shared_static_cast<BaseParser>(GetCore()->New(parserName));

    if (!mParser)
    {
        // could not create parser
        GetLog()->Error() << "ERROR: Unable to create " << parserName << "\n";
        return false;
    }
    // should we also initialize the parser? --> add Init() to BaseParser in that case
    return true;
}

bool
GameControlServer::AgentConnect(int id)
{
    // for map::insert(Elem), the test here is not required. map::insert does not
    // overwrite existing elements. The test is required to report if the agent
    // was already connected or not.
    if (mAgentMap.find(id) == mAgentMap.end())
    {
        return false;
    }

    // insert an agent ID with an empty AgentAspect reference
    mAgentMap[id] = boost::shared_ptr<AgentAspect>();

    GetLog()->Debug() << "(GameControlServer) A new agent connected (id: " << id << ")\n";
    return true;
}

bool GameControlServer::AgentDisappear(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
        {
            return false;
        }

    // remove the AgentAspect from the set of children and from our
    // private map. The AgentAspect does all the necessary cleanup
    RemoveChildReference((*iter).second);
    mAgentMap.erase(id);

    return true;
}

float GameControlServer::GetSenseInterval(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 0.1;
}
