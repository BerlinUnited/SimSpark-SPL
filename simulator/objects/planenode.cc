/* -*-c++-*- ***************************************************************
                           planenode.cc
    node for a plane within the entity graph
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
#include "planenode.h"
#include "worldnode.h"

using namespace rcss::EntityTree;
using namespace std;

PlaneNode::PlaneNode(WorldNode& world_node,
                     dReal a, dReal b, dReal c, dReal d,
                     const std::string& name, BaseNode* parent)
    : GeometryNode(world_node,name,parent)
{
    M_geometry_id = dCreatePlane(world_node.getSpace(),a,b,c,d);
}

PlaneNode::PlaneNode(WorldNode& world_node,
                     dReal v[4],
                     const std::string& name, BaseNode* parent)
    : GeometryNode(world_node,name,parent)
{
    M_geometry_id = dCreatePlane(world_node.getSpace(),v[0],v[1],v[2],v[3]);
}
