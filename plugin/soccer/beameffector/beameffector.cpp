/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: beameffector.cpp,v 1.11.4.1 2007/06/08 00:07:36 hedayat Exp $

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

bool
BeamEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (
        (mBody.get() == 0) ||
        (mGameState.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return false;
    }

    shared_ptr<BeamAction> beamAction =
        shared_dynamic_cast<BeamAction>(action);

    if (beamAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (BeamEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    // the beam effector only has an effect in PM_BeforeKickOff
    if (mGameState->GetPlayMode() == PM_BeforeKickOff)
    {
        Vector3f pos;
        pos[0] = beamAction->GetPosX();
        pos[1] = beamAction->GetPosY();

        float angle = beamAction->GetXYAngle();

        // reject nan or infinite numbers in the beam position
        if (
            (! isfinite(pos[0])) ||
            (! isfinite(pos[1]))
            )
            {
                return false;
            }

        // an agent can only beam within it's own field half
        float minX = -mFieldLength/2;
        pos[0] = std::max<float>(pos[0],minX);
        pos[0] = std::min<float>(pos[0],0.0f);

        float minY = -mFieldWidth/2;
        float maxY = mFieldWidth/2;
        pos[1] = std::max<float>(minY,pos[1]);
        pos[1] = std::min<float>(maxY,pos[1]);

        // fix z coordinate
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

	shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
	    (((GetParent().lock())->GetParent().lock())->GetParent().lock());
	        
	if (parent.get()==0)
            {
                GetLog()->Error() << "(BeamEffector) ERROR: can't get "
                                  << "parent node.\n";
                return false;
            }

        Leaf::TLeafList leafList;

        // get a list of all child body nodes
        parent->ListChildrenSupportingClass<Body>(leafList, true);

        if (leafList.size() == 0)
        {
            GetLog()->Error()
                << "ERROR: (BeamEffector) agent aspect doesn't have "
                << "children of type Body\n";

            return false;
        }

        Matrix mat;
        mat.RotationZ(gDegToRad(angle));        

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

            // EXPERIMENTAL
            // We are setting an absolute rotation here! This will reset
            // any orientation to the current rotation matrix which only has
            // entries for a rotation around Z, which means that any other
            // rotation will be lost. I assume this is okay, since we probably
            // don't care about any current pose of the robot if we use beam...
            childBody->SetRotation(mat);
            
    	}
    }
    
    return true;
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

  Predicate::Iterator iter = predicate.begin();

  float posX;
  if (! predicate.AdvanceValue(iter, posX))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter1\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  float posY;
  if (! predicate.AdvanceValue(iter, posY))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter2\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  float angle;
  if (! predicate.AdvanceValue(iter, angle))
  {
      GetLog()->Error()
          << "ERROR: (BeamEffector) float expected for parameter3\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return shared_ptr<ActionObject>(new BeamAction(GetPredicate(), posX, posY, angle));
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

