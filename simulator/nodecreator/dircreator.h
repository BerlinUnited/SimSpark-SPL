/* -*-c++-*- ***************************************************************
                           dircreator.h
    class for objects that create directory nodes
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
#ifndef RCSS_NODECREATOR_DIRCREATOR_H
#define RCSS_NODECREATOR_DIRCREATOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "nodecreator.h"

namespace rcss
{ 
    namespace EntityTree
    { 
        class BaseNode;
    }
    namespace NodeCreator
    {
            
#if 0   // only for indenting
    }}  
#endif

class DirCreator : public NodeCreator
{
    EntityTree::BaseNode* create(const ConVar::ConVars& parameter);

};

    }  // namespace
}  // namespace

#endif
