/* -*-c++-*- ***************************************************************
                           comsigbool.cc
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

#include "comsigbool.h"

#include <variables/convar.h>

using namespace std;

ComSigBool::ComSigBool(bool optional) : ComSigBase(optional)
{
}

bool
ComSigBool::validate(const ConVar* parameter) const
{
    if (!parameter->isLabel())
    {
        return false;
    }

    string value;
    parameter->getLabel(value);

    if ((value == "true") || (value == "false"))
    {
        return true;
    }

    return false;
}
