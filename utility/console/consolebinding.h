/* -*-c++-*- ***************************************************************
                             consolebinding.h
    A Keybinding as stored in ConsoleBindings.
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
#ifndef UTILITY_CONSOLEBINDING_H
#define UTILITY_CONSOLEBINDING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

/*! \class ConsoleBinding
  $Id: consolebinding.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    ConsoleBinding

    A Keybinding as stored in ConsoleBindings.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Marcus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleBinding
{
public:
    ConsoleBinding();
    ConsoleBinding(int id, const std::string& key, const std::string& value);

    int getId() const;
    void setId(int id);
    std::string getKey() const;
    void setKey(const std::string& key);
    std::string getValue() const;
    void setValue(const std::string& value);

    bool isIdEqualTo(int id) const;
    bool isKeyEqualTo(const std::string& key) const;

    /*! Check if the two key descriptions overlap.
    
        'a' matches 'a', '+a' and '-a',
        '+a' matches 'a' and '+a',
        '-a' matches 'a' and '-a'.
    */
    bool overlaps(const std::string& key);

protected:
    /*! This binding is registered to an inputdevice under this id.

        If id >0 it's bound to a variable of the group
        ConVar::S_CVG_COMMAND, if id <0 it's bound to a string. For
        more detailed information see ConsoleBindings.
    */
    int M_id;
    //! how the user knows the binding: "a", "escape"
    std::string M_key;
    /*! The bound value. If a command is used, this it is its name, 
        otherwise it's the string to execute.
    */
    std::string M_value;
};

#endif                          // UTILITY_CONSOLEBINDING_H
