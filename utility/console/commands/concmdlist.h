/* -*-c++-*- ***************************************************************
                           concmdlist.h
    Console Command Cmdlist
                           ------------------------
    begin                : Aug 23 2002  Oliver Obst
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
#ifndef UTILITY_CONCMDLIST_H
#define UTILITY_CONCMDLIST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

/*! \class ConCmdList
  $Id: concmdlist.h,v 1.1 2002/08/23 14:04:13 fruit Exp $

    ConCmdlist - Console Command Cmdlist

    Display all console commands.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConCmdlist:public ConCommand
{
public:
    ConCmdlist();
    
protected:
    //! displays all commands
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const;
};
#endif                          // UTILITY_CONCMDLIST_H
