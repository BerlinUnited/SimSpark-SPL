/* -*-c++-*- ***************************************************************
                             bodynode.h
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
#ifndef RCSS_ENTITYGRAPH_BODYNODE_H
#define RCSS_ENTITYGRAPH_BODYNODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <basenode.h>

#include <ode.h>

namespace rcss
{ 
    namespace EntityTree
    { 
#if 0
}}
#endif

class WorldNode;

class BodyNode : public BaseNode
{
public:
    BodyNode(WorldNode& world_node, 
             const std::string& name = "<unknown>",
             BaseNode* parent = 0);
    ~BodyNode();

    dBodyID getID();

protected:
    dBodyID M_body_id;
};

    } // namespace 
} // namespace

#endif
