/* -*-c++-*- ***************************************************************
                           concreate.cc
    console command to create new nodes in the entity graph
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
#include "concreate.h"
#include "signature/signatures.h"

#include <basenode.h>
#include <classserver.h>
#include <entitytree.h>
#include <forwarder.h>
#include <nodecreator/nodecommands.h>
#include <nodecreator/nodecreator.h>

using namespace rcss::entity;
using namespace rcss::NodeCreator;
using namespace Utility;
using namespace std;

ConCreate::ConCreate()
{
    // Register all node creators to the ClassServer
    NodeCommands::registerNodeCreators();

    M_usage = "  Create <STRING type> <creator parameter list>";

    M_help = getUsage() + "\nAdds a new child node to the entity graph"
        " below the currently selected node.\n";

    // the signatures
    M_signatures.add((new CommandSignature())->add(new ComSigString())->
                     add(new ComSigElliptic()));
}

ConCommand::ConExecResult 
ConCreate::executeSignature(int signature,
                            ConVar::ConVars& parameter) const
{
    BaseNode* current = EntityTree::instance().getCurrentNode();

    if (current == 0)
    {
        smux.error() << "ConCreate::executeSignature:"
            " there is no selected node !\n";
        return ConCommand::S_CER_ERROR;
    }

    string type;
    parameter[0]->getString(type);

    const string classname = NodeCommands::formatCreator(type);

    NodeCreator* creator = static_cast<NodeCreator*>(
        ClassServer::instance().create("NodeCreator", classname.c_str()));

    if (creator == 0)
    {
        smux.error() << "unknown node creator type '" << type << "'\n";
        return ConCommand::S_CER_ERROR;
    }

    // copy the command list without the first parameter
    ConVar::ConVars creator_parameter;
    ConVar::ConVars::iterator iter = parameter.begin();
    ++iter;
    copy(iter, parameter.end(), back_inserter(creator_parameter));

    // create the node
    BaseNode* node = creator->create(creator_parameter);
    if (node == 0)
    {
        smux.error() << "ConCreate::executeSignature:"
            " error creating node.\n";
        return ConCommand::S_CER_ERROR;
    }
#if 0
    // create parameter string
    string sparam;
    for (iter = creator_parameter.begin(); 
         iter != creator_parameter.end();
         ++iter)
    {
        sparam += (*iter)->dumpValue();
        sparam += " ";
    }

    node->SetCreatorType(type.c_str());
    node->SetCreatorParameter(sparam.c_str());
#endif
    // insert the node
    EntityTree::instance().addNode(*node);

    return ConCommand::S_CER_OK;
}

