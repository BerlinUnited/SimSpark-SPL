/* -*-c++-*- ***************************************************************
                           comsigany.h
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
#ifndef UTILITY_COMSIGBOOL_H
#define UTILITY_COMSIGBOOL_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "comsigbase.h"

class ComSigBool : public ComSigBase
{
public:
    ComSigBool(bool optional = false);

    virtual bool validate(const ConVar* parameter) const;
};

#endif                           // UTILITY_COMSIGBOOL_H
