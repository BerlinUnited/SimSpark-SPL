/* -*-c++-*- ***************************************************************
                           conselectnode.cc
    console command to select a node in the entity graph
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
#include "conselectnode.h"
#include "signature/signatures.h"

#include <entitytree.h>
#include <forwarder.h>

#include <string>

using namespace rcss::EntityTree;
using namespace Utility;
using namespace std;


ConSelectnode::ConSelectnode()
{
    M_usage = "  Selectnode <STRING path>";

    M_help = getUsage() + "\n" "Select a node in the scenegraph.";

    // the signatures
    M_signatures.add((new CommandSignature())->add(new ComSigString()));
}

ConCommand::ConExecResult 
ConSelectnode::executeSignature(int signature, 
                                ConVar::ConVars& parameter) const
{
    // get the string to execute
    string path;
    parameter[0]->getString(path);


    if (!EntityTree::instance().changeNode(path))
    {
        smux.error() << "Invalid path to scene graph node: '"
                     << path << "'.\n";
    }

    return ConCommand::S_CER_OK;
}

