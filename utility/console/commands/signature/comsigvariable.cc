/* -*-c++-*- ***************************************************************
                           comsigvariable.cc
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

#include "comsigvariable.h"
#include <variables/convar.h>
#include <variables/convarserver.h>

using namespace std;

ComSigVariable::ComSigVariable(bool optional) : ComSigBase(optional)
{
}

bool
ComSigVariable::validate(const ConVar* parameter) const
{
    if (!parameter->isLabel())
    {
        return false;
    }

    // try to get a variable with this name
    string label;
    parameter->getLabel(label);

    return (ConVarServer::instance().get(label) != 0);
}
