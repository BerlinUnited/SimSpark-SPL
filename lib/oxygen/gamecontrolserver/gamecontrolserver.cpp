/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.cpp,v 1.1.2.1 2003/12/01 16:14:07 fruit Exp $

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
    if (mAgentMap.find(id) != mAgentMap.end())
    {
        // insert an agent ID and clear the reference to the AgentAspect
        AgentAspect* tmp = 0;
        mAgentMap.insert(std::make_pair(id, boost::shared_ptr<AgentAspect>(tmp)));
        GetLog()->Debug() << "A new agent connected (id: " << id << ")\n";
        return true;
    }
    return false;
}
