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
#ifndef RCSS_ENTITY_SPHERENODE_H
#define RCSS_ENTITY_SPHERENODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "geometrynode.h"

#include <ode.h>

namespace rcss
{ 
    namespace entity
    { 
#if 0
}}
#endif

class WorldNode;

class SphereNode : public GeometryNode
{
public:
    SphereNode(WorldNode& world_node,
               dReal radius,
               const std::string& name = "<unknown>",
               DirNode* parent = 0);

};

    } // namespace 
} // namespace

#endif


