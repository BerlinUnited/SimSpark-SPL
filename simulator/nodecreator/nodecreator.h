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
#ifndef RCSS_NODECREATOR_NODECREATOR_H
#define RCSS_NODECREATOR_NODECREATOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <variables/convar.h>

#include <string>

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

/*!	\class NodeCreator

	This (abstract base) class serves as an interface for objects
	which have the ability to create entity graph nodes. This can
	(pretty much) be anything we want. A model loader, something
	which generates geometry procedurally, etc.. It should also be
	noted, that this interface is not limited to only being able
	to create a single node. It can create an entire hierarchy! 
	So, we might have a node creator which can load an entire
	entity graph!

	The interface itself is really slim.
	
	HISTORY:
        The NodeCreator stuff is based on parts of a student project
        at the AI Research Group, Koblenz University. Original
        development by Marco Koegler <koegler@uni-koblenz.de>, Markus
        Rollmann <rollmark@uni-koblenz.de>, Alexander Fuchs
        <alexf@uni-koblenz.de>, et.al.
*/
class NodeCreator
{
public:
    virtual ~NodeCreator()	{}

    virtual EntityTree::BaseNode* 
    create(const std::string& parameter);

    virtual EntityTree::BaseNode* 
    create(const ConVar::ConVars& parameter) = 0;
};

    }  // namespace
}  // namespace

#endif 
