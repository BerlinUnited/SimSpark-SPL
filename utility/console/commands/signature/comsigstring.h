/* -*-c++-*- ***************************************************************
                           comsigstring.h
                           ------------------------
    begin                : Aug 21 2002  Oliver Obst
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
#ifndef UTILITY_COMSIGSTRING_H
#define UTILITY_COMSIGSTRING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "comsigbase.h"

class ComSigString : public ComSigBase
{
public:
    ComSigString(bool optional = false);

    virtual bool validate(const ConVar* parameter) const;
};

#endif                           // UTILITY_COMSIGSTRING_H
