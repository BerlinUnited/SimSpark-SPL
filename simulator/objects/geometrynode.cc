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

#include <iostream>

using namespace rcss::entity;
using namespace std;

GeometryNode::GeometryNode(WorldNode& world_node,
                           const string& name, DirNode* parent)
    : BaseNode(S_CONTAINER, name, parent)
{
}

GeometryNode::GeometryNode(WorldNode& world_node, NodeType node_type,
                           const std::string& name, DirNode* parent)
    : BaseNode(node_type, name, parent)
{
}

GeometryNode::~GeometryNode()
{
    cerr << "~GeometryNode: " << M_name << endl;
    // dGeomDestroy(M_geometry_id);
    cerr << "~GeometryNode done" << endl;
}

dGeomID
GeometryNode::getID()
{
    return M_geometry_id;
}

void
GeometryNode::setBody(dBodyID body_id)
{
    dGeomSetBody(M_geometry_id, body_id);
}

dBodyID
GeometryNode::getBody() const
{
    return dGeomGetBody(M_geometry_id);
}
