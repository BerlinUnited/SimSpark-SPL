/* -*-c++-*- ***************************************************************
                           concmdlist.cc
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

#include "concmdlist.h"
#include "commandserver.h"
// #include <consolehistory.h>
#include <console.h>
#include <forwarder.h>

using namespace std;
using namespace Utility;

ConCmdlist::ConCmdlist()
{
    M_usage = "  Cmdlist\n";
    M_help = getUsage() + "\n" + "Lists all console commands\n";

    // the signatures
    M_signatures.add((new CommandSignature()));
}

ConCommand::ConExecResult 
ConCmdlist::executeSignature(int signature, ConVar::ConVars& parameter) const
{
    CommandServer::ComNames commands = CommandServer::instance().listNames();
    commands.sort();

    for (CommandServer::ComNames::const_iterator iter = commands.begin();
         iter != commands.end(); ++iter)
    {
        // Console::instance().getHistory().printOutput(*iter);
        smux.normal() << *iter << "\n";
    }

    return ConCommand::S_CER_OK;
}
