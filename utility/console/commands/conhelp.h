/* -*-c++-*- ***************************************************************
                           conhelp.h
    console command to get help
                           ------------------------
    begin                : Aug 20 2002  Oliver Obst
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
#ifndef UTILITY_CONHELP_H
#define UTILITY_CONHELP_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

/*! \class ConHelp
  $Id: conhelp.h,v 1.1 2002/08/23 14:04:13 fruit Exp $

    ConHelp - Console Command Help

    Prints a help information for a command or a variable.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConHelp : public ConCommand
{
public:
    ConHelp();

    //! returns a list of all commands
    virtual bool complete(const StringList& parameters,
                          StringList& arguments) const;

protected:
    //! prints help on a command or variable
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const;

    virtual ConExecResult displayGeneralHelp() const;
    virtual ConExecResult 
        displayCommandHelp(const ConVar::ConVars& parameter) const;
    virtual ConExecResult 
        displayVariableHelp(const ConVar::ConVars& parameter) const;
};

#endif                          // UTILITY_CONHELP_H
