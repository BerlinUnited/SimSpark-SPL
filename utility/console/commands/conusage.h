/* -*-c++-*- ***************************************************************
                           conusage.h
    console command usage
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
#ifndef UTILITY_CONUSAGE_H
#define UTILITY_CONUSAGE_H

#include "concommand.h"

/*! \class ConUsage
  $Id: conusage.h,v 1.1 2002/08/23 14:04:13 fruit Exp $

    ConUsage - Console Command Usage

    Prints the signature of a console command.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConUsage : public ConCommand
{
public:
    ConUsage();

    //! returns a list of all commands
    virtual bool complete(const StringList& parameters,
                          StringList& arguments) const;

protected:
    //! prints the signature of a console command.
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const;
};

#endif                          // UTILITY_CONUSAGE_H
