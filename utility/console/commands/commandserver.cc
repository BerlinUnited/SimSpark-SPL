/* -*-c++-*- ***************************************************************
                             commandserver.cc
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

#include "commandserver.h"

#include "concmdlist.h"
#include "conhelp.h"
#include "conquit.h"
#include "conset.h"
#include "conusage.h"

#if 0
#include "conunset.h"
#include "conalias.h"
#include "conunalias.h"
#include "conexec.h"
#include "conbind.h"
#include "conunbind.h"
#include "conecho.h"
#include "coninvoke.h"
#include "conunaliasall.h"
#include "conunbindall.h"
#include "conunsetall.h"
#include "conunsetall.h"
#include "conimport.h"
#include "conexport.h"
#include "conmount.h"
#include "conumount.h"
#include "conaddforwarder.h"
#include "conaddchild.h"
#include "conaddcontroller.h"
#include "conremovenode.h"
#include "conlistnodes.h"
#include "conselectnode.h"
#include "conmovenode.h"
#include "coninfonode.h"
#include "conexportnode.h"
#include "conimportshader.h"
#include "conturnnode.h"
#include "connamenode.h"
#endif

#include <classserver.h>

#include <algorithm>

using namespace std;

// create the ClassServer factories

CS_CreateFactory(ConCmdlist)
CS_CreateFactory(ConHelp)
CS_CreateFactory(ConQuit)
CS_CreateFactory(ConSet)
CS_CreateFactory(ConUsage)

#if 0
CS_CreateFactory(ConUnset)
CS_CreateFactory(ConAlias)
CS_CreateFactory(ConUnalias)
CS_CreateFactory(ConExec)
CS_CreateFactory(ConBind)
CS_CreateFactory(ConUnbind)
CS_CreateFactory(ConEcho)
CS_CreateFactory(ConInvoke)
CS_CreateFactory(ConUnaliasall)
CS_CreateFactory(ConUnbindall)
CS_CreateFactory(ConUnsetall)
CS_CreateFactory(ConImport)
CS_CreateFactory(ConExport)
CS_CreateFactory(ConMount)
CS_CreateFactory(ConUmount)
CS_CreateFactory(ConAddforwarder)
CS_CreateFactory(ConImportshader)
CS_CreateFactory(ConAddchild)
CS_CreateFactory(ConAddcontroller)
CS_CreateFactory(ConRemovenode)
CS_CreateFactory(ConListnodes)
CS_CreateFactory(ConSelectnode)
CS_CreateFactory(ConMovenode)
CS_CreateFactory(ConExportnode)
CS_CreateFactory(ConInfonode)
CS_CreateFactory(ConTurnnode)
CS_CreateFactory(ConNamenode) 
#endif

CommandServer::CommandServer()
{
    // Register all commands to the ClassServer
    CS_FunctionBegin()
        CS_FunctionRegisterToGroup(ConCommand, ConCmdlist)
        CS_FunctionRegisterToGroup(ConCommand, ConHelp)
        CS_FunctionRegisterToGroup(ConCommand, ConQuit)
        CS_FunctionRegisterToGroup(ConCommand, ConSet)
        CS_FunctionRegisterToGroup(ConCommand, ConUsage)
#if 0
        CS_FunctionRegisterToGroup(ConCommand, ConUnset)
        CS_FunctionRegisterToGroup(ConCommand, ConAlias)
        CS_FunctionRegisterToGroup(ConCommand, ConUnalias)
        CS_FunctionRegisterToGroup(ConCommand, ConExec)
        CS_FunctionRegisterToGroup(ConCommand, ConBind)
        CS_FunctionRegisterToGroup(ConCommand, ConUnbind)
        CS_FunctionRegisterToGroup(ConCommand, ConEcho)
        CS_FunctionRegisterToGroup(ConCommand, ConInvoke)
        CS_FunctionRegisterToGroup(ConCommand, ConUnaliasall)
        CS_FunctionRegisterToGroup(ConCommand, ConUnbindall)
        CS_FunctionRegisterToGroup(ConCommand, ConUnsetall)
        CS_FunctionRegisterToGroup(ConCommand, ConImport)
        CS_FunctionRegisterToGroup(ConCommand, ConExport)
        CS_FunctionRegisterToGroup(ConCommand, ConMount)
        CS_FunctionRegisterToGroup(ConCommand, ConUmount)
        CS_FunctionRegisterToGroup(ConCommand, ConAddforwarder)
        CS_FunctionRegisterToGroup(ConCommand, ConImportshader)
        CS_FunctionRegisterToGroup(ConCommand, ConAddchild)
        CS_FunctionRegisterToGroup(ConCommand, ConAddcontroller)
        CS_FunctionRegisterToGroup(ConCommand, ConRemovenode)
        CS_FunctionRegisterToGroup(ConCommand, ConListnodes)
        CS_FunctionRegisterToGroup(ConCommand, ConSelectnode)
        CS_FunctionRegisterToGroup(ConCommand, ConMovenode)
        CS_FunctionRegisterToGroup(ConCommand, ConInfonode)
        CS_FunctionRegisterToGroup(ConCommand, ConExportnode)
        CS_FunctionRegisterToGroup(ConCommand, ConTurnnode)
        CS_FunctionRegisterToGroup(ConCommand, ConNamenode)
#endif
    CS_FunctionEnd()
}

CommandServer::~CommandServer()
{
    // delete the command pool
    for (Commands::iterator iter = M_commands.begin(); 
         iter != M_commands.end();
         ++iter)
    {
        delete(*iter).second;
    }
}

CommandServer& 
CommandServer::instance()
{
    static CommandServer sInstance;

    return sInstance;
}

const ConCommand*
CommandServer::get(const string& name)
{
    // use the lowercase version of the command's name to store and access it
    string lower;
    transform(name.begin(), name.end(), back_inserter(lower), tolower);

    // does the command already exist?
    Commands::const_iterator iter = M_commands.find(lower);
    if (iter != M_commands.end())
    {
        // yes, return the old instance
        return (*iter).second;
    }
    else
    {
        // no, create a new instance
        return createCommand(lower);
    }
}

CommandServer::ComNames 
CommandServer::listNames() const
{
    ClassServer::CommandNames classes;
    ClassServer::instance().listNames(classes, "ConCommand");

    ComNames commands;
    for (ClassServer::CommandNames::const_iterator iter = classes.begin();
         iter != classes.end(); 
         ++iter)
    {
        commands.push_back(unformatCommand(*iter));
    }

    return commands;
}

const ConCommand*
CommandServer::createCommand(const std::string& name)
{
    // create the instance
    const ConCommand* con_command =
        (ConCommand*) ClassServer::instance().create("ConCommand",
                                                     formatCommand(name).c_str());

    // if the creation was successful add the command to the pool
    if (con_command != 0)
    {
        M_commands[name] = con_command;
    }

    return con_command;
}

string
CommandServer::formatCommand(const string& command) const
{
    string formated;

    transform(command.begin(), command.end(), back_inserter(formated), tolower);

    // make the first letter uppercase
    if (command.size() > 0)
    {
        formated[0] = toupper(formated[0]);
    }

    // add the prefix
    return "Con" + formated;
}

string
CommandServer::unformatCommand(const string& command) const
{
    return command.substr(3, command.size() - 3);
}
