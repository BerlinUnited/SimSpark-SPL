/* -*-c++-*- ***************************************************************
                             dirnode.h
    containment node for the entity graph
                           ------------------------
    begin                : Sep 30 2002  Oliver Obst
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
#ifndef RCSS_ENTITY_DIRNODE_H
#define RCSS_ENTITY_DIRNODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "basenode.h"

#include <list>
#include <string>

namespace rcss
{ 
    namespace entity
    { 
#if 0 // fix xemacs indent
}}
#endif

class DirNode : public BaseNode
{
public:
    DirNode(const std::string& name = "<unknown>",
            DirNode* parent = 0);
    ~DirNode();

    void process();

    virtual std::string getPath() const;

    void registerChild(BaseNode* child);
    void unregisterChild(BaseNode* child);
    bool hasChild(const BaseNode* child) const;

    BaseNode* getChild(const std::string& name) const;

    
    void leafNames(std::set<std::string>& names) const;

protected:
    DirNode(NodeType node_type, 
            const std::string& name = "<unknown>",
            DirNode* parent = 0);

    std::list<BaseNode*> M_children;        
        
};

    } // namespace 
} // namespace

#endif
