/* -*-c++-*- ***************************************************************
                           conlistnodes.cc
    console command to list nodes in the entity graph
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
#include "conlistnodes.h"
#include "signature/signatures.h"

#include <basenode.h>
#include <entitytree.h>
#include <forwarder.h>

#include <set>
#include <string>

// #include "connodecommands.h"
// #include "conselectnode.h"

using namespace rcss::entity;
using namespace Utility;
using namespace std;

ConListnodes::ConListnodes()
{
    // Register all node creators to the ClassServer
    // ConNodeCommands::RegisterNodeCreators();

    M_usage = "  Listnodes";

    M_help = getUsage() + "\n"
        "List nodes below the currently selected node.";

    // the signatures
    M_signatures.add(new CommandSignature());
}

void
ConListnodes::listNodes(BaseNode* current, int depth) const
{
    set<string> names;
    current->leafNames(names);
    set<string>::const_iterator i;
    for (i = names.begin(); i != names.end(); ++i)
    {
        smux.normal() << " " << *i << "\n";
    }
    
}

ConCommand::ConExecResult 
ConListnodes::executeSignature(int signature,
                               ConVar::ConVars& parameter) const
{
    BaseNode* current = EntityTree::instance().getCurrentNode();

    if (current == 0)
    {
        smux.error() << "ConListnodes::executeSignature: "
            "there is no selected node !\n";
        return ConCommand::S_CER_ERROR;
    }

    smux.normal() << "[" << current->getName() << "/]\n";
    listNodes(current, 0);

    return ConCommand::S_CER_OK;
}
