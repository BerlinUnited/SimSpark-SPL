/* -*-c++-*- ***************************************************************
                             worldnode.cc
    node for a world/space within the entity graph
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
#include "worldnode.h"

#include <string>

using namespace rcss::EntityTree;
using namespace std;

WorldNode::WorldNode(const string& name, BaseNode* parent)
    : DirNode(static_cast<NodeType>(S_DIRECTORY | S_CONTAINER | S_GEOM_BASE),
              name, parent)
{
    M_world = dWorldCreate(); 
    M_space = dHashSpaceCreate();
}

WorldNode::~WorldNode()
{
    dSpaceDestroy(M_space);
    dWorldDestroy(M_world);
}

dWorldID
WorldNode::getWorld()
{
    return M_world;
}

dSpaceID
WorldNode::getSpace()
{
    return M_space;
}
