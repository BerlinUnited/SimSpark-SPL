/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: initeffector.cpp,v 1.2.2.3 2004/01/25 12:54:41 rollmark Exp $

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
#include "initaction.h"
#include "initeffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <soccer/agentstate/agentstate.h>
#include <sstream>

using namespace boost;
using namespace oxygen;

InitEffector::InitEffector() : oxygen::Effector()
{
}

InitEffector::~InitEffector()
{
}

bool
InitEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    shared_ptr<InitAction> initAction =
        shared_dynamic_cast<InitAction>(action);

    if (initAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    // search for the AgentState
    shared_ptr<AgentAspect> aspect = GetAgentAspect();

    if (aspect.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot get AgentAspect\n";
        return false;
    }

    shared_ptr<AgentState> state =
        shared_static_cast<AgentState>(aspect->GetChildOfClass("AgentState",true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot find AgentAspect\n";
        return false;
    }

    // set the team name an uniform number in the AgentState
    state->SetTeamName(initAction->GetName());
    state->SetUniformNumber(initAction->GetNumber());

    return true;
}

shared_ptr<ActionObject>
InitEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (InitEffector) invalid predicate"
                          << predicate.name << "\n";
        return shared_ptr<ActionObject>();
    }

    std::string name;
    predicate.GetValue("teamname",name);

    int unum = 0;
    predicate.GetValue("unum",unum);

    return shared_ptr<ActionObject>(new InitAction(GetPredicate(),name,unum));
}

