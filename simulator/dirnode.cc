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

#include <forwarder.h>

#include <algorithm>
#include <functional>

using namespace Utility;
using namespace rcss::entity;
using namespace std;

DirNode::DirNode(const string& name, DirNode* parent)
    : BaseNode(BaseNode::S_DIRECTORY, name, parent)
{
}

DirNode::DirNode(NodeType node_type, const string& name, DirNode* parent)
    : BaseNode(node_type, name, parent)
{
}

DirNode::~DirNode()
{
    cerr << "~DirNode " << M_name << " 0\n";
    list<BaseNode*>::iterator i;
    for (i = M_children.begin(); i != M_children.end(); ++i)
    {
        cout << "Dirnode has " << (*i)->getName() << endl;
        // delete *i;
        // *i = 0;
    }
    cerr << "~DirNode " << M_name << " 1\n";    
    M_children.clear();
}

void
DirNode::process() 
{
    list<BaseNode*>::iterator i;
    for (i = M_children.begin(); i != M_children.end(); ++i)
    {
        (*i)->process();
    }
}

string 
DirNode::getPath() const
{
    smux.debug() << "DirNode::getPath()" << endl;
    
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
    cerr << "unregister child  from " << M_name 
         << "(" << M_children.size() << ")\n";
    child->setParent(0);
    M_children.remove(child);
    cerr << "unregister child (" <<  M_children.size() << ")\n" << flush;
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
