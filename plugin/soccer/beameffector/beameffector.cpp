/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: beameffector.cpp,v 1.11.2.1 2007/05/31 14:17:04 jboedeck Exp $

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
#include "beamaction.h"
#include "beameffector.h"
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/agentstate/agentstate.h>
#include <cmath>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

BeamEffector::BeamEffector() : oxygen::Effector()
{
}

BeamEffector::~BeamEffector()
{
}

#ifdef __APPLE_CC__
bool
isfinite( float f )
{
    return f == f && f != f * 0.5f;
}
#endif

void
BeamEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (
        (mAction.get() == 0) ||
        (mBody.get() == 0) ||
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return;
    }

    shared_ptr<BeamAction> beamAction =
        shared_dynamic_cast<BeamAction>(mAction);
   mAction.reset();
    if (beamAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (BeamEffector) cannot realize an unknown ActionObject\n";
        return;
    }

    // the beam effector only has an effect in PM_BeforeKickOff
    if (mGameState->GetPlayMode() == PM_BeforeKickOff)
    {
        Vector3f pos = beamAction->GetPosition();

        // reject nan or infinite numbers in the beam position
        if (
            (! isfinite(pos[0])) ||
            (! isfinite(pos[1])) ||
            (! isfinite(pos[2]))
            )
            {
                return;
            }

        // an agent can only beam within it's own field half
        float minX = -mFieldLength/2;
        pos[0] = std::max<float>(pos[0],minX);
        pos[0] = std::min<float>(pos[0],0.0f);

        float minY = -mFieldWidth/2;
        float maxY = mFieldWidth/2;
        pos[1] = std::max<float>(minY,pos[1]);
        pos[1] = std::min<float>(maxY,pos[1]);

        pos[2] = mAgentRadius;

        // swap x and y coordinates accordingly for the current
        // team; after the flip pos is global and not independent
        // on the team
        pos = SoccerBase::FlipView
            (
                pos,
                mAgentState->GetTeamIndex()
                );

	Vector3f bodyPos = mBody->GetPosition();
        //mBody->SetPosition(pos);
        //mBody->SetVelocity(Vector3f(0,0,0));
        //mBody->SetAngularVelocity(Vector3f(0,0,0));

	shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
	    (GetParent().lock());
	parent = shared_dynamic_cast<Transform>
	    (parent->GetParent().lock());

	if (parent.get()==0)
	{
    	    return;
	}

        Leaf::TLeafList leafList;

        // get a list of all child body nodes
        parent->ListChildrenSupportingClass<Body>(leafList, true);

        if (leafList.size() == 0)
        {
            GetLog()->Error()
                << "ERROR: (BeamEffector) agent aspect doesn't have "
                << "children of type Body\n";

            return;
        }

        Leaf::TLeafList::iterator iter = leafList.begin();
       
        // move all child bodies 
        for (iter;
             iter != leafList.end();
             ++iter
            )
        {            
	    shared_ptr<Body> childBody = 
                shared_dynamic_cast<Body>(*iter);
	    
    	    Vector3f childPos = childBody->GetPosition();

    	    childBody->SetPosition(pos + (childPos-bodyPos));
    	    childBody->SetVelocity(Vector3f(0,0,0));
    	    childBody->SetAngularVelocity(Vector3f(0,0,0));
    	}
    }
}

shared_ptr<ActionObject>
BeamEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (BeamEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  Vector3f pos;
  if (! predicate.GetValue(predicate.begin(), pos))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) Vector3f parameter expected\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return shared_ptr<ActionObject>(new BeamAction(GetPredicate(),pos));
}

void
BeamEffector::OnLink()
{
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetGameState(*this, mGameState);
    SoccerBase::GetAgentState(*this,mAgentState);

    mFieldWidth = 64.0;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",mFieldWidth);

    mFieldLength = 100.0;
    SoccerBase::GetSoccerVar(*this,"FieldLength",mFieldLength);

    mAgentRadius = 0.22;
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);
}

void
BeamEffector::OnUnlink()
{
    mBody.reset();
    mGameState.reset();
    mAgentState.reset();
}

