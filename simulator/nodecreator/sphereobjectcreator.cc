/* -*-c++-*- ***************************************************************
                           sphereobjectcreator.cc
    class for objects that create sphere object nodes
                           ------------------------
    begin                : Oct 10 2002  Oliver Obst
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
#include "sphereobjectcreator.h"

#include <basenode.h>
#include <consoleparser.h>
#include <dirnode.h>
#include <entitytree.h>
#include <forwarder.h>
#include <objects/bodynode.h>
#include <objects/spherenode.h>
#include <objects/worldnode.h>

#include <ode.h>

using namespace rcss::entity;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

BaseNode* 
SphereobjectCreator::create(const ConVar::ConVars& parameter)
{
        // check parameter list
        if (parameter.size() != 2)
        {
            smux.error() << "(ObjectCreator::create) syntax error.\n";
            return 0;
        }

        dReal size;
        parameter[0]->getFloat(size);
        string name;
        parameter[1]->getString(name);

        DirNode* current = 
            dynamic_cast<DirNode*>(EntityTree::instance().getCurrentNode());

        WorldNode* world = 0;
        if (current != 0) 
            world = dynamic_cast<WorldNode*>(current->getAncestor(BaseNode::S_GEOM_BASE));
        DirNode* dir = 0;
        if (world != 0)
        {
            dir = new DirNode(name,current);
            SphereNode* sphere;
            BodyNode* body;
            sphere = new SphereNode(*world, size, name+"Geom", dir);
            body = new BodyNode(*world, name+"Body", dir);
            sphere->setBody(body->getID());
            dMass sphere_mass;
            dMassSetSphere(&sphere_mass, 5.0, size);
            dMassAdjust(&sphere_mass, 75);
            body->setMass(sphere_mass);
            dGeomSetPosition(sphere->getID(), 0, 0, 10);
            
        }
        return dir;
}
