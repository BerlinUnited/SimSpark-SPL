/* -*-c++-*- ***************************************************************
                           nodecommands.cc
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
#include "nodecommands.h"
#include "dircreator.h"
#include "planecreator.h"
#include "spherecreator.h"
#include "worldcreator.h"

#include <classserver.h>

#include <algorithm>

using namespace rcss::NodeCreator;
using namespace std;

// create the NodeCreator factories
CS_CreateFactory(DirCreator)
CS_CreateFactory(PlaneCreator)
CS_CreateFactory(SphereCreator)
CS_CreateFactory(WorldCreator)

void
NodeCommands::registerNodeCreators()
{
    CS_FunctionBegin()
        CS_FunctionRegisterToGroup(NodeCreator, DirCreator)
        CS_FunctionRegisterToGroup(NodeCreator, PlaneCreator)
        CS_FunctionRegisterToGroup(NodeCreator, SphereCreator)
        CS_FunctionRegisterToGroup(NodeCreator, WorldCreator)
    CS_FunctionEnd()
}

string
NodeCommands::formatCreator(const string& type)
{
    string formatted;

    // convert first char to upper case
    string::const_iterator iter = type.begin();
    formatted = toupper(*iter);
    ++iter;

    // ... and convert the remaining to lower case
    transform(iter, type.end(), back_inserter(formatted), ::tolower);

    // add the postfix
    return formatted + "Creator";
}
