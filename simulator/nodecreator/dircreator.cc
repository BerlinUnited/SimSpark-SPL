/* -*-c++-*- ***************************************************************
                           dircreator.cc
    class for objects that create directory nodes
                           ------------------------
    begin                : Oct 03 2002  Oliver Obst
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
#include "dircreator.h"

#include <basenode.h>
#include <consoleparser.h>
#include <dirnode.h>
#include <entitytree.h>
#include <forwarder.h>

using namespace rcss::entity;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

BaseNode* 
DirCreator::create(const ConVar::ConVars& parameter)
{
    // check parameter list
    if (parameter.size() != 1)
    {
        smux.error() << "(DirCreator::create) syntax error.\n";
        return 0;
    }
    
    string name;
    parameter[0]->getString(name);
    
    DirNode* current = 
        dynamic_cast<DirNode*>(EntityTree::instance().getCurrentNode());
    DirNode* dir = 0;
    
    if (current != 0) // && (current->getType() & BaseNode::S_DIRECTORY != 0))
    {
        dir = new DirNode(name, current);
    }
    return dir;
}
