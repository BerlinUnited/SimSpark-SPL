/* -*-c++-*- ***************************************************************
                           geometrynode.h
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
#ifndef RCSS_ENTITY_GEOMETRYNODE_H
#define RCSS_ENTITY_GEOMETRYNODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <basenode.h>

#include <ode.h>

namespace rcss
{ 
    namespace entity
    { 
#if 0
}}
#endif

class DirNode;
class WorldNode;

class GeometryNode : public BaseNode
{
public:
    GeometryNode(WorldNode& world_node,
                 const std::string& name = "<unknown>",
                 DirNode* parent = 0);

    ~GeometryNode();

    dGeomID getID();
    
    void setBody(dBodyID body_id);
    dBodyID getBody() const;

protected:
    GeometryNode(WorldNode& world_node,
                 NodeType node_type,
                 const std::string& name = "<unknown>",
                 DirNode* parent = 0);

    dGeomID M_geometry_id;
};

    } // namespace 
} // namespace

#endif


