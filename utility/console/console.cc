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

#include "console.h"
#include "consolebaseview.h"

using namespace std;

// the console is per default inactive
Console::Console()
    : mActive(false), mForwarder(), mSettings(&mForwarder)    
{
    // add the forwarder to the message multiplexer
    gMux.AddTarget(&mForwarder);
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
    mActive = active;
}

bool 
Console::isActive() const
{
    return mActive;
}

void 
Console::processInput(const BaseInputDevice::Input &input)
{
    if (isActive())
    {
        // pipe the input through our inputline
        mHistory.processInput(input);
    } else {
        // execute the binding
        invokeBinding(input);
    }
}

bool 
Console::invokeBinding(const BaseInputDevice::Input &input)
{
    // if a strings is bound to the input, execute it
    if (mBindings.exists(input.id))
    {
        execute(mBindings.get(input.id));
        return true;
    } else {
        return false;
    }
}

bool 
Console::execute(const string &input)
{
    pair <bool, ConsoleParser::TStatementList> scanResult;
    
    // try to scan the input
    scanResult = mParser.scan(input);
    if (!scanResult.first)
    {
        gDispatchError("Couldn't execute input: Scan error.\n");
        return false;
    }

    // try to parse the input
    if (!ConsoleParser::parse(scanResult.second))
    {
        ConsoleParser::deleteStatements(scanResult.second);
        gDispatchError("Couldn't execute input: Syntax error.\n");
        return false;
    }
    
    // execute every parsed statement
    bool success = true;
    for (ConsoleParser::TStatementList::iterator iter = scanResult.second.begin(); 
    iter != scanResult.second.end(); 
    ++iter)
    {
        success = success && perform(*iter);
    }
    
    // garbage collection
    mParser.deleteStatements(scanResult.second);
        
    return success;
}

bool 
Console::perform(ConVar::tConVars &conVars)
{
    ConVar*             commandVar;
    const ConCommand    *command;
    ConVar::tConVars    parameters;

    // separate command and parameters
    ConVar::tConVars::iterator iter = conVars.begin();
    
    commandVar = (*iter);
    commandVar->getCommand(&command);
    ++iter;
    
    copy(iter, conVars.end(), back_inserter(parameters));

    // execute the command
    ConCommand::eConExecResult execResult = command->execute(parameters);
        
    if (execResult == ConCommand::CER_INV_SIGNATURE)
    {
        gDispatchNormal("the command %s could not be executed: "
         "wrong type of arguments.\n", 
         commandVar->getAttributes().getName().c_str());
        gDispatchNormal("%s\n", command->getUsage().c_str());

        return false;
    } 

    return execResult == ConCommand::CER_OK;
}

ConsoleForwarder& 
Console::getForwarder()
{
    return mForwarder;
}

ConsoleSettings& 
Console::getSettings()
{
    return mSettings;
}

ConsoleHistory& 
Console::getHistory()
{
    return mHistory;
}

ConsoleBindings& 
Console::getBindings()
{
    return mBindings;
}

ConsoleAliases& 
Console::getAliases()
{
    return mAliases;
}

ConsoleParser& 
Console::getParser()
{
    return mParser;
}

void 
Console::draw() const
{
    // only draw if active
    if (isActive())
    {
        // tell every view to draw itself
        for (TViews::const_iterator iter = mViews.begin(); 
        iter != mViews.end(); 
        ++iter)
        {
            (*iter)->draw();
        }
    }
}

void 
Console::registerView(ConsoleBaseView *view)
{
    mViews.push_back(view);
}

void 
Console::unregisterView(ConsoleBaseView *view)
{
    for (TViews::iterator iter = mViews.begin(); 
    iter != mViews.end(); )
    {
        if (*iter == view)
        {
            iter = mViews.erase(iter);
            break;
        } else {
            ++iter;
        }
    }
}
