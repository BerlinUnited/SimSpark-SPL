/* -*-c++-*- ***************************************************************
                           conusage.cc
                           ------------------------
    begin                : Aug 23 2002  Oliver Obst
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

#include "conusage.h"
#include "signature/signatures.h"
#include "commandserver.h"

#include <console.h>
#include <classserver.h>
#include <forwarder.h>

using namespace std;
using namespace Utility;

ConUsage::ConUsage()
{
    M_usage = "  Usage <COMMAND>\n";
    M_help = getUsage() + "\n"
        + "Displays the call syntax of a console command.\n";

    // the signatures
    M_signatures.add((new CommandSignature())->add(new ComSigCommand()));
}

ConCommand::ConExecResult 
ConUsage::executeSignature(int signature, ConVar::ConVars& parameter) const
{
    const ConCommand* command;
    parameter[0]->getCommand(&command);

    smux.normal() << command->getUsage();

    return ConCommand::S_CER_OK;
}

bool
ConUsage::complete(const ConCommand::StringList& parameters,
                   ConCommand::StringList& arguments) const
{
    if (parameters.size() == 1)
    {
        // the commands
        CommandServer::ComNames commands = 
            CommandServer::instance().listNames();
        copy(commands.begin(), commands.end(), back_inserter(arguments));
    }

    return true;
}
