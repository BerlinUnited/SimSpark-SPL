/* -*-c++-*- ***************************************************************
                           conset.cc
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

#include "conset.h"
#include "signature/signatures.h"
#include <console.h>
#include <consolesettings.h>
#include <variables/convarserver.h>
#include <forwarder.h>

using namespace std;
using namespace Utility;

ConSet::ConSet()
{
    M_usage = "  Set\n"
        "  Set <VARIABLE>\n"
        "  Set <VARIABLE> <bool | int | float | string | vector | VARIABLE>\n";
    M_help = getUsage() + "\n"
        + "Displays the values of all variables (-> cs_show_var_signature),\n"
        +
        "displays the value of one variable or sets the value of a variable.\n"
        + "\n" + "A variable may be set to an int, float, string or vector,\n" +
        "or to another variable's value.\n" +
        "If the value is not convertable to the type of the variable,\n" +
        "the variables value remains unchanged\n" + "\n" +
        "If the variable does not already exist,\n" +
        "it is implicitly defined with the type of the value.\n";

    // the signatures
    // display all variables
    // 0
    M_signatures.add((new CommandSignature()));

    // display a variable
    // 1
    M_signatures.add((new CommandSignature())->add(new ComSigVariable()));

    // these set a variable
    // 2
    M_signatures.add((new CommandSignature())->add(
                         new ComSigVariable())->add(new ComSigBool()));

    // 3
    M_signatures.add((new CommandSignature())->add(
                         new ComSigVariable())->add(new ComSigInt()));

    // 4
    M_signatures.add((new CommandSignature())->add(
                         new ComSigVariable())->add(new ComSigFloat()));

    // 5
    M_signatures.add((new CommandSignature())->add(
                         new ComSigVariable())->add(new ComSigString()));

    // 6
    M_signatures.add((new CommandSignature())->add(
                         new ComSigVariable())->add(new ComSigVector()));

    // these create a variable
    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigBool()));

    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigInt()));

    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigFloat()));

    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigString()));

    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigVector()));

    M_signatures.add((new CommandSignature())->add(
                         new ComSigLabel())->add(new ComSigVariable()));
}

ConCommand::ConExecResult 
ConSet::executeSignature(int signature, ConVar::ConVars& parameter) const
{
    switch (signature)
    {
        case 0:
        {
            return displayVariables();
        }
        case 1:
        {
            return displayVariable(parameter);
        }
        case 2:
        {
            return setBool(parameter);
        }
        case 3:
        case 4:
        case 5:
        case 6:
        {
            return setVariable(parameter);
        }
        case 7:
        {
            return createBool(parameter);
        }
        case 8:
        {
            return createInt(parameter);
        }
        case 9:
        {
            return createFloat(parameter);
        }
        case 10:
        {
            return createString(parameter);
        }
        case 11:
        {
            return createVector(parameter);
        }
        case 12:
        {
            return createVariable(parameter);
        }
    }

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::displayVariables() const
{
    if (Console::instance().getSettings().getShowVarSignature())
    {
        smux.normal() << ConVarServer::instance().dumpWithSignature();
    }
    else
    {
        smux.normal() << ConVarServer::instance().dump();
    }

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::displayVariable(ConVar::ConVars& parameter) const
{
    ConVar* variable = labelToVariable(parameter[0]);

    if (Console::instance().getSettings().getShowVarSignature())
    {
        smux.normal() << variable->dumpWithSignature();
    }
    else
    {
        smux.normal() << variable->dump();
    }

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::setBool(ConVar::ConVars& parameter) const
{
    ConVar* variable = labelToVariable(parameter[0]);
    bool value = labelToBool(parameter[1]);

    // try to set the variable
    if (variable->setBool(value))
    {
        return ConCommand::S_CER_OK;
    }

    // what went wrong?
    examineAFailedSet(variable);
    return ConCommand::S_CER_ERROR;
}

ConCommand::ConExecResult 
ConSet::setVariable(ConVar::ConVars& parameter) const
{
    ConVar* variable = labelToVariable(parameter[0]);
    ConVar* value;

    if (parameter[1]->isLabel())
    {
        value = labelToVariable(parameter[1]);
    }
    else
    {
        value = parameter[1];
    }

    // try to set the variable
    if (variable->setVariable(value))
    {
        return ConCommand::S_CER_OK;
    }

    // what went wrong?
    examineAFailedSet(variable);
    return ConCommand::S_CER_ERROR;
}

ConCommand::ConExecResult 
ConSet::createBool(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);
    bool value = labelToBool(parameter[1]);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createBool(attr, value);

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::createInt(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);

    int value;
    parameter[1]->getInt(value);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createInt(attr, value);

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::createFloat(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);

    TFloat value;
    parameter[1]->getFloat(value);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createFloat(attr, value);

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::createString(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);

    string value;
    parameter[1]->getString(value);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createString(attr, value);

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::createVector(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);

    Vector3 value;
    parameter[1]->getVector(value);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createVector(attr, value);

    return ConCommand::S_CER_OK;
}

ConCommand::ConExecResult 
ConSet::createVariable(ConVar::ConVars& parameter) const
{
    string label;
    parameter[0]->getLabel(label);

    ConVar* value = labelToVariable(parameter[0]);

    ConVarAttributes attr;
    attr.setName(label);
    ConVarServer::instance().createVariable(attr, value);

    return ConCommand::S_CER_OK;
}

void
ConSet::examineAFailedSet(ConVar* variable) const
{
    if (variable->getAttributes().isConstant())
    {
        // ah, the variable is constant
        smux.normal() << "couldn't set variable " 
                      << variable->getAttributes().getName()
                      << ": it's a constant\n";                          
    }
    else
    {
        // then it must be a type clash
        smux.normal() << "couldn't set variable "
                      << variable->getAttributes().getName()
                      << ": type clash\n";
        smux << variable->dump();
    }
}

bool
ConSet::complete(const ConCommand::StringList&, ConCommand::StringList&) const
{       
    return false;
}
