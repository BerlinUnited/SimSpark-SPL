/* -*-c++-*- ***************************************************************
                           geometrynode.cc
    base node for a geometry within the entity graph
                           ------------------------
    begin                : Oct 01 2002  Oliver Obst
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
#include "geometrynode.h"
#include "worldnode.h"

using namespace rcss::EntityTree;
using namespace std;

GeometryNode::GeometryNode(WorldNode& world_node,
                           const string& name, BaseNode* parent)
    : BaseNode(S_CONTAINER, name, parent)
{
}

GeometryNode::GeometryNode(WorldNode& world_node, NodeType node_type,
                           const std::string& name, BaseNode* parent)
    : BaseNode(node_type, name, parent)
{
}

GeometryNode::~GeometryNode()
{
    dGeomDestroy(M_geometry_id);
}

dGeomID
GeometryNode::getID()
{
    return M_geometry_id;
}
