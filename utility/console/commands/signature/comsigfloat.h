/* -*-c++-*- ***************************************************************
                           comsigfloat.h
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
#ifndef UTILITY_COMSIGFLOAT_H
#define UTILITY_COMSIGFLOAT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "comsigbase.h"

/*! \class ComSigFloat
  $Id: comsigfloat.h,v 1.1 2002/08/21 08:04:14 fruit Exp $

    ComSigFloat

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ComSigFloat : public ComSigBase
{
public:
    ComSigFloat(bool optional = false);

    virtual bool validate(const ConVar* parameter) const;
};

#endif                           // UTILITY_COMSIGFLOAT_H
