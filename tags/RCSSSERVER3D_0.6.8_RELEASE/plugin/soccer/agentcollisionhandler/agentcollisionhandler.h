/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef AGENTCOLLISIONHANDLER_H
#define AGENTCOLLISIONHANDLER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/collider.h>
#include <agentstate/agentstate.h>
#include <boost/enable_shared_from_this.hpp>

/** \class AgentCollisionHandler is a CollisionHandler that passes
     collision information of the Collider it belongs to to the owning
     AgentState.
*/
class AgentCollisionHandler : public oxygen::CollisionHandler
{
public:
    AgentCollisionHandler() : oxygen::CollisionHandler() {};
    virtual ~AgentCollisionHandler() {};

    virtual void OnLink();
    
    /** Adds to collision list if \collidee is part of another agent

    \param collidee is the geom ID of the colliders collision
    partner

    \param holds the contact points between the two affected geoms
    as returned from the ODE dCollide function
    */
    virtual void HandleCollision
    (boost::shared_ptr<oxygen::Collider> collidee, oxygen::GenericContact& contact);
    
private:
    boost::shared_ptr<AgentState> FindAgentState(BaseNode* node);
    
    boost::shared_ptr<AgentState> mAgentState;
};

    DECLARE_CLASS(AgentCollisionHandler);

#endif // AGENTCOLLISIONHANDLER_H
