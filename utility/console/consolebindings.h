/* -*-c++-*- ***************************************************************
                             consolebindings.h
    The key bindings managed by the console.
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
#ifndef UTILITY_CONSOLEBINDINGS_H
#define UTILITY_CONSOLEBINDINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "consolebinding.h"

#include <list>

/*! \class ConsoleBindings
  $Id: consolebindings.h,v 1.1 2002/08/21 14:11:16 fruit Exp $
    
    ConsoleBindings

    The key bindings managed by the console, manipulated with ConBind,
    ConUnbind, ConUnbindAll.

    These bindings are evaluated when the console is active, i.e. in-game.

    A binding attaches an action to a key (keyboard, mouse button,
    mouse axis, ...).

    This action may be
    - a variable of the group ConVar::S_CVG_COMMAND
      The game will execute a function according to the bound variable.
    - a string
      The console will execute the string bound to the key

    The input loop of the game decides according to the id of a
    binding if the binding represents a command to be executed by the
    game (>0) or a string to be passed to the console (<0).  Therefore
    all command bindings need an id >0, all string bindings one <0.
    As the id of the command binding will be the value of the command
    variable, it follows that ConVar::S_CVG_COMMAND variables must be
    >0.  The ids of the string bindings are managed in a variable.

    A key of a binding may be divided in up, down and up+down:
    +a : is activated on the press of button "a"
    -a : is activated on the release of button "a"
     a : is activated on the press and release of button "a"
    A binding to "+a" replaces bindings to  "a" and "+a",
    a binding to "-a" replaces bindings to  "a" and "-a",
    a binding to  "a" replaces bindings to "+a" and "-a".

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleBindings
{
public:
    //! the strings returned by getBindings
    typedef std::list < std::string > StringList;

     ConsoleBindings();
     ~ConsoleBindings();

     /*! binds a key to a string to be executed by the console the
         inputdevice is chosen according to the key, for details see
         BaseInputDevice::TranslateKey.
     */
     bool bind(const std::string& key, const std::string& value);
     /*! binds a key to a command (a variable of the group
         ConVar::S_CVG_COMMAND) to be executed by the game.
         @param id (>0) value of command
         @param name the name of the command

         The inputdevice is chosen according to the key, for details
         see BaseInputDevice::TranslateKey
     */ 
     bool bind(const std::string& key, int id, const std::string& name);
     //! is this key bound?
     bool exists(const std::string& key) const;
     //! is this key bound?
     bool exists(int id) const;
     /*! check with exists if the key is bound.
         @return value of the binding assigned to this key
     */
     std::string get(const std::string& key) const;
     /*! check with exists if the key is bound. 
         @return value of the binding assigned to the id.
     */ 
     std::string get(int id) const;
     /*! returns a list of the string descriptions of all bound keys
         (as used as input to get)
      */
    StringList getBindings() const;
    //! unbinds a key
    bool unbind(const std::string& key);

protected:
    ConsoleBindings(const ConsoleBindings&) {}
    ConsoleBindings& operator =(const ConsoleBindings&) {}

    //! does a binding to a device and stores it
    bool doBind(const std::string& key, int id,
                const std::string& name);
    //! tells an appropriate inputdevice to bind this key to this id
    bool bindToDevice(const std::string& key, int id) const;
    //! stores/replaces this binding
    void store(int id, const std::string& key, const std::string& value);

    /*! this counter is used for the console string bindings
        and therefore negative and decremented on every binding.
    */  
    int M_lastBinding;

    //! the container for the bindings
    typedef std::list < ConsoleBinding > Bindings;
    //! the bindings
    Bindings M_bindings;
};

#endif                          // UTILITY_CONSOLEBINDINGS_H
