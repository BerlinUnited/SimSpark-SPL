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

#include <iostream>
#include <string>

using namespace rcss::entity;
using namespace std;

WorldNode::WorldNode(const string& name, DirNode* parent)
    : DirNode(static_cast<NodeType>(S_DIRECTORY | S_CONTAINER | S_GEOM_BASE),
              name, parent)
{
    M_world = dWorldCreate(); 
    M_space = dHashSpaceCreate();
    M_contact_group = dJointGroupCreate(0);

    dWorldSetGravity(M_world,0,0,-9.5);
}

WorldNode::~WorldNode()
{
    dJointGroupDestroy(M_contact_group);
    dSpaceDestroy(M_space);
    dWorldDestroy(M_world);
}

WorldNode&
WorldNode::instance(const string& name, DirNode* parent)
{
    static WorldNode S_instance;

    if (!name.empty())
    {
        S_instance.M_name = name;
    }
    if (parent != 0)
    {
        parent->registerChild(&S_instance);
        S_instance.M_parent = parent;
    }
    return S_instance;
}

void
WorldNode::process()
{
    dSpaceCollide(M_space, 0, &WorldNode::nearCallback);
    dWorldStep(M_world, 0.05);
    // remove all contact joints
    dJointGroupEmpty(M_contact_group);
    
    list<BaseNode*>::iterator i;
    for (i = M_children.begin(); i != M_children.end(); ++i)
    {
        (*i)->process();
    }
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

void 
WorldNode::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    int i;
    // if (o1->body && o2->body) return;

    // exit without doing anything if the two bodies are connected by a joint
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnected (b1,b2)) return;

    dContact contact[3];			// up to 3 contacts per box
    for (i=0; i<3; i++) {
        contact[i].surface.mode = dContactBounce; //dContactMu2;
        contact[i].surface.mu = dInfinity;
        contact[i].surface.mu2 = 0;
        contact[i].surface.bounce = 0.5;
        contact[i].surface.bounce_vel = 0.1;
    }
    if (int numc = dCollide (o1,o2,3,&contact[0].geom,sizeof(dContact))) {
        // dMatrix3 RI;
        // dRSetIdentity (RI);
        // const dReal ss[3] = {0.02,0.02,0.02};
        for (i=0; i<numc; i++) {
            dJointID c = dJointCreateContact(instance().M_world,
                                             instance().M_contact_group, 
                                             contact+i);
            dJointAttach(c,b1,b2);
            // dsDrawBox (contact[i].geom.pos,RI,ss);
        }
    }
}
