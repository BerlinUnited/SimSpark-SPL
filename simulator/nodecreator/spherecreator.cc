/* -*-c++-*- ***************************************************************
                            spherecreator.cc
    class for objects that create sphere nodes
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
#include "spherecreator.h"

#include <basenode.h>
#include <consoleparser.h>
#include <entitytree.h>
#include <forwarder.h>
#include <objects/spherenode.h>
#include <objects/worldnode.h>

#include <ode.h>

using namespace rcss::EntityTree;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

BaseNode* 
SphereCreator::create(const ConVar::ConVars& parameter)
{
        // check parameter list
        if (parameter.size() != 2)
        {
            smux.error() << "(SphereCreator::create) syntax error.\n";
            return 0;
        }

        dReal size;
        parameter[0]->getFloat(size);
        string name;
        parameter[1]->getString(name);

        BaseNode* current = EntityTree::EntityTree::instance().getCurrentNode();
        WorldNode* world = 0;
        if (current != 0) 
            world = dynamic_cast<WorldNode*>(current->getAncestor(BaseNode::S_GEOM_BASE));
        SphereNode* sphere = 0;
        if (world != 0)
        {
            sphere = new SphereNode(*world, size, name, current);
        }
        return sphere;
}
