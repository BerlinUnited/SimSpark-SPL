/* -*-c++-*- ***************************************************************
                           spherenode.h
    node for a sphere within the entity graph
                           ------------------------
    begin                : Oct 02 2002  Oliver Obst
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
#include "spherenode.h"
#include "worldnode.h"

using namespace rcss::EntityTree;
using namespace std;

SphereNode::SphereNode(WorldNode& world_node, dReal radius,
                       const std::string& name, BaseNode* parent)
    : GeometryNode(world_node, name, parent)
{
    M_geometry_id = dCreateSphere(world_node.getSpace(),radius);
    dGeomSetPosition(M_geometry_id, 0, 0, 0);
}
