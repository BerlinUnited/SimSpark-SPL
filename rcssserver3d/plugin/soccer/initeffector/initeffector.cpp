/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
*/
#include "initaction.h"
#include "initeffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/space.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <gamestateaspect/gamestateaspect.h>
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

void
InitEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if ( ( mAction.get() == 0 ) ||
        (mGameState.get() == 0) ||
        (mAgentAspect.get() == 0)
        )
        {
            return;
        }

    boost::shared_ptr<InitAction> initAction =
        shared_dynamic_cast<InitAction>(mAction);
    mAction.reset();

    if (initAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot realize an unknown ActionObject\n";
        return;
    }

    // search for the AgentState
    boost::shared_ptr<AgentState> state = shared_static_cast<AgentState>
        (mAgentAspect->GetChildOfClass("AgentState", true));

    if (state.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InitEffector) cannot find AgentState\n";
        return;
    }

    // register the uniform number and team index to the GameStateAspect
    mGameState->RequestUniform
        (state, initAction->GetName(), initAction->GetNumber());

    TTeamIndex team = state->GetTeamIndex();

    // request an initial position for the agent and move it there
    Vector3f pos = mGameState->RequestInitPosition(team);
     // request the initial orientation for the agent and turn
    float angle = mGameState->RequestInitOrientation(team);

    SoccerBase::MoveAndRotateAgent(mAgentAspect, pos, angle);

#if 0
    Matrix mat;
    mat.RotationZ(gDegToRad(angle));

    // agents may be encapsulated in their own collision spaces, so we need
    // to get the parent of the parent of the agent aspect in this case
    boost::shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
        (mAgentAspect->GetParentSupportingClass("Transform").lock());
    
    
    if (parent.get() == 0)
	{
            GetLog()->Error()
                << "ERROR: (InitEffector) can not get parent of current agent aspect\n";

    	    return;
	}

    Leaf::TLeafList leafList;  

    // get absolute position of the agent
    Vector3f bodyPos = mAgentAspect->GetWorldTransform().Pos();

    // get all the bodies below the parent of the agent aspect
    parent->ListChildrenSupportingClass<Body>(leafList, true);

    if (leafList.size() == 0)
        {
            GetLog()->Error()
                << "ERROR: (InitEffector) agent aspect doesn't have "
                << "children of type Body \n";

            return;
        }

    Leaf::TLeafList::iterator iter = leafList.begin();
    
    // move all the child bodies to the new relative position
    for (iter;
         iter != leafList.end();
         ++iter
         )
        {    
            boost::shared_ptr<Body> childBody =
                shared_dynamic_cast<Body>(*iter);
                
            Vector3f childPos = childBody->GetPosition();
            Matrix childR = childBody->GetRotation();
            childR = mat*childR;
            Vector3f newPos   = pos + (mat.Rotate(childPos-bodyPos));

            childBody->SetPosition(newPos);
            childBody->SetRotation(childR);
            childBody->SetVelocity(Vector3f(0,0,0));
            childBody->SetAngularVelocity(Vector3f(0,0,0));
        }
#endif
}

boost::shared_ptr<ActionObject>
InitEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (InitEffector) invalid predicate"
                          << predicate.name << "\n";
        return boost::shared_ptr<ActionObject>();
    }

    std::string name;
    predicate.GetValue(predicate.begin(),"teamname",name);

    int unum = 0;
    predicate.GetValue(predicate.begin(),"unum",unum);

    return boost::shared_ptr<ActionObject>(new InitAction(GetPredicate(),name,unum));
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


