/* -*-c++-*- ***************************************************************
                           conquit.cc
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
#include "conquit.h"

#include <console.h>
#include <forwarder.h>

using namespace std;
using namespace Utility;

ConQuit::ConQuit()
{
    M_usage = "  Quit\n";
    M_help = getUsage() + "\n" + "Quits the game.\n";

    // the signatures
    M_signatures.add(new CommandSignature());
}

ConCommand::ConExecResult 
ConQuit::executeSignature(int signature, ConVar::ConVars& parameter) const
{
    Console::instance().setQuit();
    return ConCommand::S_CER_OK;
}
