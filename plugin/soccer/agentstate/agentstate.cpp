/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentstate.cpp,v 1.1.2.4 2004/02/07 18:48:38 fruit Exp $

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
#include "agentstate.h"
#include <sstream>

using namespace oxygen;

AgentState::AgentState() : ObjectState(), mTeamIndex(TI_NONE)
{
    // set mID and mUniformNumber into a joint state
    SetUniformNumber(0);
}

AgentState::~AgentState()
{
}

void
AgentState::SetTeamIndex(TTeamIndex idx)
{
    mTeamIndex = idx;
}

TTeamIndex
AgentState::GetTeamIndex() const
{
    return mTeamIndex;
}


void
AgentState::SetUniformNumber(int number)
{
    mUniformNumber = number;
    std::ostringstream ss;
    ss << number;
    mID = ss.str();
}

int
AgentState::GetUniformNumber() const
{
    return mUniformNumber;
}

void
AgentState::SetID(const std::string& id)
{
    std::istringstream iss(id);
    iss >> mUniformNumber;
    if (!iss)
    {
        // conversion failed. mUniformNumber is not changed.
        return;
    }
    mID = id;
}
