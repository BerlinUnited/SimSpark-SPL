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
#ifndef UTILITY_CONSOLE_H
#define UTILITY_CONSOLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "consolesettings.h"
#if 0
#include "consolehistory.h"
#include "consolebindings.h"
#endif
#include "consolealiases.h"
#include "consoleparser.h"
#include "variables/convar.h"
#include "commands/concommand.h"

// #include "../forwarder/consoleforwarder.h"
// #include "../inputserver/baseinputdevice.h"

class ConsoleBaseView;

/*! \class Console
  $Id: console.h,v 1.4 2002/08/21 14:11:16 fruit Exp $

    Console

    The console is a in game console as known from many games (e.g quake).
    Provided are :
    - Variables (ConVarServer + ConVar)
    - Commands (ConCommand)
    - Aliases (ConAliases)
    - Key Bindings (ConsoleBindings)

    Two modi are discerned
    - Active
      the console displays itself (ConsoleBaseView) and provides 
      an interactive input line for the user (ConsoleInputLine) 
      which handles user input.
    - Inactive
      the console does not draw anything, but the users input is checked 
      against the key bindings defined by the console (InputServer, 
      BaseInputDevice).

    The commands are classes inherited from ConCommand which are on
    need created by the ClassServer.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.

    TODO:
    - memory leaks ;-) (?)
*/

class Console
{
public:
    //! get the singleton instance of the console
    static Console& instance();

    //! sets the consoles activity state
    void setActive(bool active = true);
    //! is the console active?
    bool isActive() const;
#if 0
    /*! If the console is active the input is processed by the input
       line (ConsoleInputLine). Otherwise the input bindings
       (ConsoleBindings) are evaluated. 
     */
    void processInput(const BaseInputDevice::Input& input);
#endif
    /*! execute a string as hacked into the console or 
       read from a configuration file
     */
    bool execute(const std::string& command);
    //! the stream associated with the console
    std::ostream* getStream();
    /*! the variable settings of this console which are 
       accessible through the console input line
     */
    ConsoleSettings& getSettings();
#if 0
    /*! the history contains everything was has been displayed 
       by the console plus the input line (ConsoleInputLine)
     */
    ConsoleHistory& getHistory();
    //! the input bindings which are valid when the console is inactive
    ConsoleBindings& getBindings();
#endif
    //! the aliases
    ConsoleAliases& getAliases();
    //! the parser used to analyse the string given to execute
    ConsoleParser& getParser();
    /*! draw the console if active: 
       this will pass the call to every registered view
     */
    void draw() const;
    /*! Register a view to the console.  
       The console will not delete the view, neither on
       unregistration or in the destructor.
     */
    void registerView(ConsoleBaseView* view);
    //! unregister a view; the view will not be deleted 
    void unregisterView(ConsoleBaseView* view);

protected:
    //! a container for the views
    typedef std::list < ConsoleBaseView* >TViews;
    //! this is a singleton: we don't want no copies
    Console();
    Console(const Console&);
    Console& operator = (const Console &);
#if 0
    //! execute the string bound to the id of the input event
    bool invokeBinding(const BaseInputDevice::Input& input);
#endif
    //! executes a statement returned by the parser and returns the success
    bool perform(ConVar::ConVars& conVars);

    //! is the console active?
    bool M_active;
    //! the registered views
    TViews M_views;
    /*! The console stream.
       Should exist before the M_settings, as the settings will
       reconfigure the streams message interests.
     */
    std::ostream* M_stream;
    //! the variable settings accessable through the console input line
    ConsoleSettings M_settings;
#if 0
    /*! the history contains everything what has been displayed by the
       console and the input line (ConsoleInputLine)
     */
    ConsoleHistory M_history;
    //! the key bindings
    ConsoleBindings M_bindings;
#endif
    //! the aliases
    ConsoleAliases M_aliases;
    //! the parser used to analyse the strings given to execute
    ConsoleParser M_parser;
};

#endif                          // UTILITY_CONSOLE_H
