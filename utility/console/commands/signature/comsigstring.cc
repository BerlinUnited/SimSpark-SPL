/* -*-c++-*- ***************************************************************
                           comsigstring.cc
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

#include "comsigstring.h"
#include <variables/convar.h>

using namespace std;

ComSigString::ComSigString(bool optional) : ComSigBase(optional)
{
}

bool
ComSigString::validate(const ConVar* parameter) const
{
    return validateConvertibleTo(parameter, ConVar::S_CVT_STRING);
}
