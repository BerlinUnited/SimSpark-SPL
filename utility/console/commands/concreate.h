/* -*-c++-*- ***************************************************************
                           concreate.h
    console command to create new nodes in the entity graph
                           ------------------------
    begin                : Oct 03 2002  Oliver Obst
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
#ifndef UTILITY_CONCREATE_H
#define UTILITY_CONCREATE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

class ConCreate : public ConCommand
{
public:
    ConCreate();
    
protected:
    //! add a new node to the entity graph
    ConExecResult executeSignature(int signature,
                                   ConVar::ConVars& parameter) const;
    
};

#endif
