/* -*-c++-*- ***************************************************************
                           conhelp.cc
                           ------------------------
    begin                : Aug 20 2002  Oliver Obst
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

#include "conhelp.h"
#include "commandserver.h"
#include "signature/signatures.h"

#include <console.h>
#include <variables/convarserver.h>

#include <classserver.h>
#include <forwarder.h>

using namespace std;
using namespace Utility;

ConHelp::ConHelp()
{
    M_usage = "  Help\n" "  Help <COMMAND>\n" "  Help <VARIABLE>\n";
    M_help = getUsage() + "\nProvides a short help on the console, " +
        "a console command or a variable.\n";

    // the signatures
    M_signatures.add((new CommandSignature()));
    M_signatures.add((new CommandSignature())->add(new ComSigCommand()));
    M_signatures.add((new CommandSignature())->add(new ComSigVariable()));
}

ConCommand::ConExecResult 
ConHelp::displayGeneralHelp() const
{
#ifndef CONSOLE_VERSION
#define CONSOLE_VERSION 0.1
#endif
    smux.normal() << "Console " << CONSOLE_VERSION << "\n\n";
    smux << "Type 'help <command>' or 'usage <command>' to get a help "
         << "on a command.\n";
    smux << "Use the command 'cmdlist' to get a list of all available "
         << "commands." << endl;

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConHelp::displayCommandHelp(const ConVar::ConVars& parameter) const
{
    const ConCommand* command;
    parameter[0]->getCommand(&command);

    smux.normal() << command->getHelp();

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConHelp::displayVariableHelp(const ConVar::ConVars& parameter) const
{
    ConVar* variable = labelToVariable(parameter[1]);

    string help = variable->getAttributes().getHelp();
    if (help == "")
    {
        help = "no help defined for variable " + 
            variable->getAttributes().getName();
    }

    smux.normal() << help << endl;

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConHelp::executeSignature(int signature, ConVar::ConVars& parameter) const
{
    switch (signature)
    {
        case 0:
        {
            return displayGeneralHelp();
        }
        case 1:
        {
            return displayCommandHelp(parameter);
        }
        case 2:
        {
            return displayVariableHelp(parameter);
        }
    }

    return ConCommand::S_CER_OK;
}

bool
ConHelp::complete(const ConCommand::StringList& parameters,
                  ConCommand::StringList& arguments) const
{
    if (parameters.size() == 1)
    {
        // the commands
        CommandServer::ComNames commands = 
            CommandServer::instance().listNames();
        copy(commands.begin(), commands.end(), back_inserter(arguments));

        // the variables
        ConVarServer::VarNames variables = ConVarServer::instance().listNames();
        copy(variables.begin(), variables.end(), back_inserter(arguments));
    }

    return true;
}
