/* -*-c++-*- ***************************************************************
                           conquit.h
    console command to quit the server
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
#ifndef UTILITY_CONQUIT_H
#define UTILITY_CONQUIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

/*! \class ConQuit
  $Id: conquit.h,v 1.1 2002/08/21 08:39:59 fruit Exp $

    ConQuit - Console Command Quit

    Quits the engine.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/

class ConQuit : public ConCommand
{
public:
    ConQuit();

protected:
    //! quits the game 
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const;
};

#endif                          // UTILITY_CONQUIT_H
