/* -*-c++-*- ***************************************************************
                           worldcreator.cc
    class for objects that create world nodes
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
#include "worldcreator.h"

#include <basenode.h>
#include <consoleparser.h>
#include <entitytree.h>
#include <forwarder.h>
#include <objects/worldnode.h>

using namespace rcss::entity;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

BaseNode* 
WorldCreator::create(const ConVar::ConVars& parameter)
{
    // check parameter list
    if (parameter.size() != 1)
    {
        smux.error() << "(WorldCreator::create) syntax error.\n";
        return 0;
    }
    
    string name;
    parameter[0]->getString(name);
    
    DirNode* current = 
        dynamic_cast<DirNode*>(EntityTree::instance().getCurrentNode());
    WorldNode* world = 0;
    
    if (current != 0 && (current->getType() & BaseNode::S_DIRECTORY != 0))
    {
        world = &WorldNode::instance(name, current);
    }
    return world;
}
