/* -*-c++-*- ***************************************************************
                           planecreator.cc
    class for objects that create plane nodes
                           ------------------------
    begin                : Oct 05 2002  Oliver Obst
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
#include "planecreator.h"

#include <basenode.h>
#include <consoleparser.h>
#include <entitytree.h>
#include <forwarder.h>
#include <objects/planenode.h>
#include <objects/worldnode.h>

#include <ode.h>

using namespace rcss::EntityTree;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

BaseNode* 
PlaneCreator::create(const ConVar::ConVars& parameter)
{
        // check parameter list
        if (parameter.size() != 5)
        {
            smux.error() << "(PlaneCreator::create) syntax error.\nParameters: "
                         << "<REAL a> <REAL b> <REAL c> <REAL d> <STRING name>\n";
            smux << "The plane equation is a*x+b*y+c*z = d. The plane's normal "
                 << "vector is (a,b,c), and it must have length 1.\n\n";
            return 0;
        }

        dReal v[4];
        for (int i=0; i<4; ++i)
        {
            parameter[i]->getFloat(v[i]);
        }
        string name;
        parameter[4]->getString(name);

        BaseNode* current = EntityTree::EntityTree::instance().getCurrentNode();
        WorldNode* world = 0;
        if (current != 0) 
            world = dynamic_cast<WorldNode*>(current->getAncestor(BaseNode::S_GEOM_BASE));
        PlaneNode* plane = 0;
        if (world != 0)
        {
            plane = new PlaneNode(*world, v, name, current);
        }
        return plane;
}
