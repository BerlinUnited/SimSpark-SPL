/* -*-c++-*- ***************************************************************
                           comsigany.cc
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

#include "comsigany.h"

using namespace std;

ComSigAny::ComSigAny(bool optional) : ComSigBase(optional)
{
}

bool
ComSigAny::validate(const ConVar* parameter) const
{
    return true;
}
