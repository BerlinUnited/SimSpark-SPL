/* -*-c++-*- ***************************************************************
                                 comsigbase.cc
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

#include "comsigbase.h"

#include <variables/convarserver.h>

using namespace std;

ComSigBase::ComSigBase(bool optional) : M_optional(optional)
{
}

bool
ComSigBase::validateConvertibleTo(const ConVar* parameter,
                                  ConVar::ConVarType type) const
{
    return 
        parameter->isConvertibleTo(type) || 
        validateLabelConvertibleTo(parameter, type);
}

bool
ComSigBase::validateLabelConvertibleTo(const ConVar* parameter,
                                       ConVar::ConVarType type) const
{
    if (!parameter->isLabel())
    {
        return false;
    }

    // try to get a variable with this name
    string label;
    parameter->getLabel(label);
    const ConVar* variable = ConVarServer::instance().get(label);

    if (variable == 0)
    {
        // no, such a variable does not exist
        return false;
    }
    else
    {
        // ok, try to convert the variable to the desired type
        return variable->isConvertibleTo(type);
    }
}

void
ComSigBase::setOptional(bool optional)
{
    M_optional = optional;
}

bool
ComSigBase::isOptional() const
{
    return M_optional;
}

bool
ComSigBase::isElliptic() const
{
    return false;
}
