/* -*-c++-*- ***************************************************************
                           concommand.cc
                           ------------------------
    begin                : Aug 16 2002  Oliver Obst
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

#include "concommand.h"

#include <console.h>
#include <variables/convarserver.h>

using namespace std;

ConCommand::ConCommand()
{
}


ConCommand::~ConCommand()
{
}

string
ConCommand::getHelp() const
{
    return M_help;
}

string
ConCommand::getUsage() const
{
    return M_usage;
}

const CommandSignatures&
ConCommand::getSignature() const
{
    return M_signatures;
}

ConCommand::ConExecResult 
ConCommand::execute(ConVar::ConVars& parameter) const
{
    // validate the parameters against the command signature
    int signature = getSignature().validate(parameter);
    if (signature < 0)
    {
        return S_CER_INV_SIGNATURE;
    }

    return executeSignature(signature, parameter);
}

bool
ConCommand::complete(const ConCommand::StringList& parameters,
                     ConCommand::StringList& arguments) const
{
    return false;
}

bool
ConCommand::shouldStringify(const StringList& parameters) const
{
    return false;
}

ConCommand::StringList 
ConCommand::ignorePrefixes() const
{
    StringList prefixes;

    return prefixes;
}

ConVar*
ConCommand::labelToVariable(const ConVar* label) const
{
    string name;
    label->getLabel(name);

    if (ConVarServer::instance().exists(name))
    {
        return ConVarServer::instance().get(name);
    }
    else
    {
        return 0;
    }
}

bool
ConCommand::labelToBool(const ConVar* label) const
{
    string value;
    label->getLabel(value);

    if (value == "true")
    {
        return true;
    }
    else if (value == "false")
    {
        return false;
    }

    return false;
}

bool
ConCommand::addBlank() const
{
    return true;
}
