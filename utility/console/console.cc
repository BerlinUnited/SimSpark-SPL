/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <algorithm>

#include <forwarder.h>

#include "console.h"
#include "consolebaseview.h"

using namespace std;
using namespace Utility;

// the console is per default inactive
Console::Console():M_active(false), M_stream(&cout), M_settings(M_stream)
{
    // add the forwarder to the message multiplexer
    smux.addStream(M_stream);
}

Console& 
Console::instance()
{
    static Console sInstance;
    return sInstance;
}

void
Console::setActive(bool active)
{
    M_active = active;
}

bool 
Console::isActive() const
{
    return M_active;
}
#if 0
void
Console::processInput(const BaseInputDevice::Input & input)
{
    if (isActive())
    {
        // pipe the input through our inputline
        M_history.processInput(input);
    }
    else
    {
        // execute the binding
        invokeBinding(input);
    }
}
#endif
#if 0
bool 
Console::invokeBinding(const BaseInputDevice::Input& input)
{
    // if a strings is bound to the input, execute it
    if (mBindings.exists(input.id))
    {
        execute(mBindings.get(input.id));
        return true;
    }
    else
    {
        return false;
    }
}
#endif
bool 
Console::execute(const string& input)
{
    pair < bool, ConsoleParser::StatementList > scan_result;

    // try to scan the input
    scan_result = M_parser.scan(input);
    if (!scan_result.first)
    {
        smux.error() << "Couldn't execute input: Scan error." << endl;
        return false;
    }

    // try to parse the input
    if (!ConsoleParser::parse(scan_result.second))
    {
        ConsoleParser::deleteStatements(scan_result.second);
        smux.error() << "Couldn't execute input: Syntax error." << endl;
        return false;
    }

    // execute every parsed statement
    bool success = true;
    for (ConsoleParser::StatementList::iterator iter =
         scan_result.second.begin(); iter != scan_result.second.end(); ++iter)
    {
        success = success && perform(*iter);
    }

    // garbage collection
    M_parser.deleteStatements(scan_result.second);

    return success;
}

bool 
Console::perform(ConVar::ConVars& conVars)
{
    ConVar* commandVar;
    const ConCommand* command;
    ConVar::ConVars parameters;

    // separate command and parameters
    ConVar::ConVars::iterator iter = conVars.begin();

    commandVar = (*iter);
    commandVar->getCommand(&command);
    ++iter;

    copy(iter, conVars.end(), back_inserter(parameters));

    // execute the command
    ConCommand::ConExecResult exec_result = command->execute(parameters);

    if (exec_result == ConCommand::S_CER_INV_SIGNATURE)
    {
        smux.normal() << "the command " 
                      << commandVar->getAttributes().getName() 
                      << " could not be executed: wrong type of arguments.\n";
        smux << command->getUsage() << endl;
        return false;
    }

    return exec_result == ConCommand::S_CER_OK;
}

std::ostream* 
Console::getStream()
{
    return M_stream;
}

ConsoleSettings& 
Console::getSettings()
{
    return M_settings;
}
#if 0
ConsoleHistory& 
Console::getHistory()
{
    return M_history;
}

ConsoleBindings& 
Console::getBindings()
{
    return M_bindings;
}
#endif
ConsoleAliases& 
Console::getAliases()
{
    return M_aliases;
}

ConsoleParser& 
Console::getParser()
{
    return M_parser;
}

void
Console::draw() const
{
    // only draw if active
    if (isActive())
    {
        // tell every view to draw itself
        for (TViews::const_iterator iter = M_views.begin();
             iter != M_views.end(); ++iter)
        {
            (*iter)->draw();
        }
    }
}

void
Console::registerView(ConsoleBaseView* view)
{
    M_views.push_back(view);
}

void
Console::unregisterView(ConsoleBaseView* view)
{
    for (TViews::iterator iter = M_views.begin(); iter != M_views.end();)
    {
        if (*iter == view)
        {
            iter = M_views.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}
