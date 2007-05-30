/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: initeffector.cpp,v 1.7 2007/05/30 09:09:22 jboedeck Exp $

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
#include <oxygen/physicsserver/body.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <sstream>

using namespace boost;
using namespace oxygen;
using namespace salt;

InitEffector::InitEffector() : oxygen::Effector()
{
}

InitEffector::~InitEffector()
{
}

bool
InitEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (
        (mGameState.get() == 0) ||
        (mAgentAspect.get() == 0)
        )
        {
            return false;
        }

    shared_ptr<InitAction> initAction =
        shared_dynamic_cast<InitAction>(action);

    if (initAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    // search for the AgentState
    shared_ptr<AgentState> state = shared_static_cast<AgentState>
        (mAgentAspect->GetChildOfClass("AgentState",true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot find AgentState\n";
        return false;
    }

    // register the uniform number and team index to the GameStateAspect
    mGameState->RequestUniform
        (state, initAction->GetName(), initAction->GetNumber());

    TTeamIndex team = state->GetTeamIndex();

    // request an initial position for the agent and move it there
    Vector3f pos = mGameState->RequestInitPosition(team);

    // agents are now encapsulated in their own collision spaces, so we need
    // to get the parent of the parent of the agent aspect
    shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
        ((mAgentAspect->GetParent().lock())->GetParent().lock());

    if (parent.get() == 0)
	{
            GetLog()->Error()
                << "ERROR: (InitEffector) can not get parent of current agent aspect\n";

    	    return false;
	}

    Leaf::TLeafList leafList;  

    // get absolute position of the agent
    Vector3f bodyPos = mAgentAspect->GetWorldTransform().Pos();

    // get all the bodies below the parent of the agent aspect
    parent->GetChildrenSupportingClass("Body", leafList, true);

    if (leafList.size() == 0)
        {
            GetLog()->Error()
                << "ERROR: (InitEffector) agent aspect doesn't have "
                << "children of type Body\n";

            return false;
        }

    Leaf::TLeafList::iterator iter = leafList.begin();
    
    // move all the child bodies to the new relative position
    for (iter;
         iter != leafList.end();
         ++iter
         )
        {    
            shared_ptr<Body> childBody =
                shared_dynamic_cast<Body>(*iter);
                
            Vector3f childPos = childBody->GetPosition();
            Vector3f newPos   = pos + (childPos-bodyPos);            

            childBody->SetPosition(newPos);
            childBody->SetVelocity(Vector3f(0,0,0));
            childBody->SetAngularVelocity(Vector3f(0,0,0));
        }
       
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
    predicate.GetValue(predicate.begin(),"teamname",name);

    int unum = 0;
    predicate.GetValue(predicate.begin(),"unum",unum);

    return shared_ptr<ActionObject>(new InitAction(GetPredicate(),name,unum));
}

void InitEffector::OnLink()
{
    mGameState = shared_dynamic_cast<GameStateAspect>
        (SoccerBase::GetControlAspect(*this,"GameStateAspect"));
    mAgentAspect = GetAgentAspect();
    if (mAgentAspect.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot get AgentAspect\n";
    }
}

void InitEffector::OnUnlink()
{
    mGameState.reset();
    mAgentAspect.reset();
}


