/* -*-c++-*- ***************************************************************
                           comsigcommand.cc
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

#include "comsigcommand.h"
#include <variables/convar.h>

using namespace std;

ComSigCommand::ComSigCommand(bool optional) : ComSigBase(optional)
{
}

bool
ComSigCommand::validate(const ConVar* parameter) const
{
    return validateConvertibleTo(parameter, ConVar::S_CVT_COMMAND);
}
