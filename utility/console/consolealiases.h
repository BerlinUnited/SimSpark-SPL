/* -*-c++-*- ***************************************************************
                             consolealiases.h
    Aliases of the console, a simple mapping from a string to another string
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
#ifndef UTILITY_CONSOLEALIASES_H
#define UTILITY_CONSOLEALIASES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <list>
#include <map>
#include <string>

/*! \class ConsoleAliases   
  $Id: consolealiases.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    ConsoleAliases   

    The aliases of the console, a simple mapping from a string to
    another string.

    Manipulated with the commands ConAlias, ConUnalias and
    ConUnaliasall.

    The aliases are evaluated and replaced by ConsoleParser during the
    scan process.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleAliases
{
public:
    //! this type is used by getAliases to return a list of the defined aliases
    typedef std::list < std::string > StringList;

    ConsoleAliases() {}

    //! exists this alias?
    bool exists(const std::string& alias) const;
    //! creates/sets this alias to this value
    void set(const std::string& alias, const std::string& value);
    /*! @return the value of the alias. 
                If the alias does not exist, an empty string is
                returned - the alias is not created
    */  
    std::string get(const std::string& alias) const;
    ///! returns a list of the names of all defined aliases
    StringList getAliases() const;
    //! removes the alias if it exists
    void remove(const std::string& alias);

protected:
    //! we don't want any copies of the aliases
    ConsoleAliases(const ConsoleAliases&) {}
    ConsoleAliases& operator =(const ConsoleAliases&) {}

    //! the container in which the aliases are stored
    typedef std::map < std::string, std::string > Aliases;
    //! the defined aliases
    Aliases M_aliases;

};

#endif                          // UTILITY_CONSOLEALIASES_H
