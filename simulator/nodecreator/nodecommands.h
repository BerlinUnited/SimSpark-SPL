/* -*-c++-*- ***************************************************************
                           nodecommands.h
    code for entity graph nodes commands
                           ------------------------
    begin                : Oct 02 2002  Oliver Obst
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
#ifndef RCSS_NODECREATOR_NODECOMMANDS_H
#define RCSS_NODECREATOR_NODECOMMANDS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

namespace rcss
{ 
    namespace NodeCreator
    {
#if 0   // only for indenting
    }}  
#endif

class NodeCommands
{
public:
    static std::string formatCreator(const std::string& type);
    static void registerNodeCreators();
};

    }
}

#endif
