/* -*-c++-*- ***************************************************************
                             worldnode.h
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
#ifndef RCSS_ENTITY_WORLDNODE_H
#define RCSS_ENTITY_WORLDNODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <dirnode.h>

#include <ode.h>

namespace rcss
{ 
    namespace entity
    { 
#if 0
    }}
#endif

class WorldNode : public DirNode
{
public:
    ~WorldNode();

    //! get the singleton instance of the World node
    static WorldNode& instance(const std::string& name = "",
                               DirNode* parent = 0);

    void process();

    dWorldID getWorld();
    dSpaceID getSpace();
    
private:
    WorldNode(const std::string& name = "", DirNode* parent = 0);

    static void nearCallback(void *data, dGeomID o1, dGeomID o2);

    dWorldID M_world;
    dSpaceID M_space;    
    dJointGroupID M_contact_group;
};

    } // namespace 
} // namespace

#endif
