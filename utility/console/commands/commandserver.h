/* -*-c++-*- ***************************************************************
                             commandserver.h
    This class provides a pool of Commands.
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
#ifndef UTILITY_COMMANDSERVER_H
#define UTILITY_COMMANDSERVER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

#include <map>

/*! \class CommandServer

    This class provides a pool of Commands.

    It assures that every command (ConCommand*) is created only once.
    It is a singleton and should be retrieved with instance().

    For the creation of a command the ClassServer is used. Therefore
    all ClassServer registration for the commands is done in
    commandserver.cpp

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.

  $Id: commandserver.h,v 1.2 2002/10/07 15:57:56 fruit Exp $
*/
class CommandServer
{
public:
    //! the return type of the method listNames ()
    typedef std::list < std::string > ComNames;

    ~CommandServer();

    //! returns the singleton instance
    static CommandServer& instance();

    /*! returns/creates the command named name.

        if no such command is createable, 0 is returned.
    */
    const ConCommand* get(const std::string& name);

    //! retrieves the names of all known commands
    ComNames listNames() const;

private:
    //! CommandServer is a singleton, so the constructor should be private
    CommandServer();
    CommandServer(const CommandServer&) {}
    CommandServer& operator = (const CommandServer&) {}

    /*! creates an instance of the command and adds it to the command pool.

        returns 0 if the instance can not be created.
    */
    const ConCommand* createCommand(const std::string& command);

    /*! To create a command with the ClassServer its class name must
        be known. This method formats a command as it is used in the
        console to its class name, e.g. help -> ConHelp, HELP ->
        ConHelp.
    */
    std::string formatCommand(const std::string& command) const;

    /*! The opposite of formatCommand, the returned string will start
        with an uppercase letter followed by lowercase ones,
        e.g. ConHelp -> Help
    */
    std::string unformatCommand(const std::string& command) const;

    //! the container type for the command pool
    typedef std::map < std::string, const ConCommand* > Commands;
    /*! The command pool.

        An instance of every command is created at most once (on its
        first use), used every time the command is needed and finally
        deleted in the destructor of the console.
    */
    Commands M_commands;
};

#endif                          // UTILITY_COMMANDSERVER_H
