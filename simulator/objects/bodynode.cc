/* -*-c++-*- ***************************************************************
                             bodynode.cc
    node for a rigid body within the entity graph
                           ------------------------
    begin                : Sep 27 2002  Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/
#include "bodynode.h"
#include "worldnode.h"

using namespace rcss::entity;
using namespace std;

BodyNode::BodyNode(WorldNode& world_node, const string& name,
                   DirNode* parent)
    : BaseNode(S_CONTAINER, name, parent)
{
    M_body_id = dBodyCreate(world_node.getWorld());
}

BodyNode::~BodyNode()
{
    dBodyDestroy(M_body_id);
}

dBodyID
BodyNode::getID()
{
    return M_body_id;
}

void
BodyNode::setMass(const dMass& mass)
{
    dBodySetMass(M_body_id, &mass);
}
