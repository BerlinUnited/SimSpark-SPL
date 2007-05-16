/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group

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
#include "singlematiniteffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
//#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/physicsserver/body.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>
//#include <soccer/gamestateaspect/gamestateaspect.h>
#include <kerosin/sceneserver/singlematnode.h>
#include <sstream>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace kerosin;

SingleMatInitEffector::SingleMatInitEffector() : InitEffector::InitEffector()
{
}

SingleMatInitEffector::~SingleMatInitEffector()
{
}

bool
SingleMatInitEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    InitEffector::Realize(action);

    // search for the AgentState
    shared_ptr<AgentState> state = shared_static_cast<AgentState>
        (mAgentAspect->GetChildOfClass("AgentState",true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SingleMatInitEffector) cannot find AgentState\n";

        return false;
    }

    TTeamIndex team = state->GetTeamIndex();

    if (team == TI_LEFT)
        {
            shared_ptr<SingleMatNode> matNode = shared_dynamic_cast<SingleMatNode>
                (mAgentAspect->GetChildSupportingClass("SingleMatNode", true));

            if (matNode.get() == 0)
                {
                    GetLog()->Error()
                        << "ERROR: (SingleMatInitEffector) cannot find SingleMatNode\n";

                    return false;   
                }

            matNode->SetMaterial("matLightBlue");
        }
       
    return true;
}

