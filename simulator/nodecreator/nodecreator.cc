/* -*-c++-*- ***************************************************************
                             nodecreator.h
    abstract base class for objects that create entity graph nodes
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
#include "nodecreator.h"

#include <consoleparser.h>

using namespace rcss::EntityTree;
using namespace rcss::NodeCreator;
using namespace std;

BaseNode* 
NodeCreator::create(const string& parameter)
{
    // try to parse the parameters into a convar list
    ConsoleParser parser;
    pair<bool, ConsoleParser::StatementList> result = parser.scan(parameter);

    if (!result.first) return 0;
	
    // try to create the entity node using the parsed convars
    BaseNode *node = create(result.second.front());
	
    // free the parameter list
    ConsoleParser::deleteStatements(result.second);

    return node;
}
