/* -*-c++-*- ***************************************************************
                           comsigcommand.h
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
#ifndef UTILITY_COMSIGCOMMAND_H
#define UTILITY_COMSIGCOMMAND_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "comsigbase.h"

/*! \class ComSigCommand
  $Id: comsigcommand.h,v 1.1 2002/08/21 08:04:14 fruit Exp $

    ComSigCommand

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ComSigCommand : public ComSigBase
{
public:
    ComSigCommand(bool optional = false);

    virtual bool validate(const ConVar* parameter) const;
};

#endif                           // UTILITY_COMSIGCOMMAND_H
