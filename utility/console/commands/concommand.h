/* -*-c++-*- ***************************************************************
                           concommand.h
    the abstract base class of all console commands.
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
#ifndef UTILITY_CONCOMMAND_H
#define UTILITY_CONCOMMAND_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "signature/commandsignatures.h"

#include <variables/convar.h>
#include <variables/convarattributes.h>

class Console;

/*! \class ConCommand
  $Id: concommand.h,v 1.1 2002/08/21 08:40:32 fruit Exp $

    ConCommand - Console Commands

    This is the abstract base class of all console commands. A command
    executes a parameter sequence of ConVars (Execute). Its signature,
    i.e. which type the single parameters should have, has to be
    specified in mSignature and can be accessed with getSignature.
    Furthermore the console provides a help and usage string and hints
    for the tab completion (ConsoleHistory).
  
    M_signature, M_help and M_usage should be set by every subclass in
    its constructor.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConCommand
{
public:
    //! the result of the execute method
    enum ConExecResult
    {
        //! the command could not be executed because of an invalid parameter list
        S_CER_INV_SIGNATURE = 0x00000001,
        //! the command executed correctly
        S_CER_OK = 0x00000002,
        //! the command was aborted with an error
        S_CER_ERROR = 0x00000003,

        //! this makes the enum to be 32-bits
        S_CER_FORCE32BIT = 0xffffffff,
    };

    //! the tab completion hints are communicated as a string list
    typedef std::list < std::string > StringList;

    /*! In the constructor the member variables M_help, M_usage and
        M_signature should be set.
    */
    ConCommand();
    virtual ~ConCommand();

    //! returns a help description for this command 
    std::string getHelp() const;

    //! returns the call signatur of this command
    std::string getUsage() const;

    //! returns the signatures this command accepts
    const CommandSignatures & getSignature() const;

    /*! Does what the command should do.
        This has to be overriden by a subclass in order to be a useful
        command.
     */
    virtual ConExecResult execute(ConVar::ConVars& parameter) const;

    /*! Give a hint for the tab completion (ConsoleHistory).
      
        Which strings should be considered for tab completion into the
        string list arguments.  If the function returns false, no hint
        has been given, and the tab completion function has to figure
        it out for itself.
    */
    virtual bool complete(const StringList& parameters,
                          StringList& arguments) const;

    /*! A helper to the tab completion (ConsoleHistory).

        Should the last parameter be a string? This enables the tab
        completion to complete the parameter ignoring the string
        requirement and finally put quotes around the last
        successfully completed parameter.
    */
    virtual bool shouldStringify(const StringList& parameters) const;

    /*! These prefixes should be ignored by the tab completion (ConsoleHistory).

        E.g if "invoke +cmd_quit" is to be completed "+" would be an
        element of the IgnorePrefixes of ConBind and the tab
        completion would try to complete cmd_quit.  Comparison is done
        case insensitive
    */
    virtual StringList ignorePrefixes() const;

    virtual bool addBlank() const;

protected:
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const 
        = 0;

    /*! The labelToXXX methods are useful to convert the various label tokens
        to the value which they really represent.

        @returns the variable from the ConVarServer which is 
                 represented by the label 
    */
    ConVar* labelToVariable(const ConVar* label) const;
    //! @returns the bool represented by the label
    bool labelToBool(const ConVar* label) const;

    //! the signatures accepted
    CommandSignatures M_signatures;
    //! a help information
    std::string M_help;
    //! a usage information
    std::string M_usage;
};

#endif // UTILITY_CONCOMMAND_H
