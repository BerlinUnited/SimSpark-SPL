/* -*-c++-*- ***************************************************************
                             dirnode.cc
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
#include "dirnode.h"

#include <algorithm>
#include <functional>

using namespace rcss::EntityTree;
using namespace std;

DirNode::DirNode(const string& name, BaseNode* parent)
    : BaseNode(BaseNode::S_DIRECTORY, name, parent)
{
}

DirNode::DirNode(NodeType node_type, const string& name, BaseNode* parent)
    : BaseNode(node_type, name, parent)
{
}

DirNode::~DirNode()
{
    list<BaseNode*>::iterator i;
    for (i = M_children.begin(); i != M_children.end(); ++i)
    {
        delete *i;
    }
    
    M_children.clear();
}

string 
DirNode::getPath() const
{
    string s = "";
    if (getParent() != 0) s = getParent()->getPath();
    return s + M_name + "/";
}

void 
DirNode::registerChild(BaseNode* child)
{
    M_children.push_back(child);
    child->setParent(this);
}

void 
DirNode::unregisterChild(BaseNode* child)
{
    child->setParent(0);
    M_children.remove(child);
}

bool 
DirNode::hasChild(const BaseNode* child) const
{
    list<BaseNode*>::const_iterator i;
    i = find(M_children.begin(), M_children.end(), child);
    return (i != M_children.end());
}

BaseNode*
DirNode::getChild(const string& name) const
{
    list<BaseNode*>::const_iterator i;
    i = find_if(M_children.begin(), M_children.end(), 
                bind2nd(CompareNodeName(), name));
    return i != M_children.end() ? *i : 0;
}

void
DirNode::leafNames(set<string>& names) const
{
    BaseNode::leafNames(names);
    list<BaseNode*>::const_iterator i;
    for (i = M_children.begin(); i != M_children.end(); ++i)
    {
        names.insert((*i)->getName());
    }
}
