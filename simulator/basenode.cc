/* -*-c++-*- ***************************************************************
                             basenode.cc
    the base node for the entity graph
                           ------------------------
    begin                : Sep 26 2002  Oliver Obst
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
#include "basenode.h"

#include <algorithm>
#include <iostream>

using namespace rcss::EntityTree;
using namespace std;

BaseNode::BaseNode(NodeType node_type,
                   const std::string& name, 
                   BaseNode* parent)
    : M_type(node_type), M_name(name), M_parent(parent)
{
}

BaseNode::~BaseNode()
{
}

BaseNode::NodeType
BaseNode::getType() const
{
    return M_type;
}

void 
BaseNode::setParent(BaseNode* parent)
{
}

BaseNode* 
BaseNode::getParent()
{
    return M_parent;
}

const BaseNode* 
BaseNode::getParent() const
{
    return M_parent;
}

BaseNode* 
BaseNode::getRoot()
{
    BaseNode* root = this;
    while (root->getParent() != 0)
    {
        root = root->getParent();
    }
    return root;
}

const BaseNode* 
BaseNode::getRoot() const
{
    const BaseNode* root = (const BaseNode*)  this;
    while (root->getParent() != 0)
    {
        root = root->getParent();
    }
    return root;
}

BaseNode* 
BaseNode::getAncestor(NodeType node_type)
{
    BaseNode* node = this;
    while (node != 0 && (node->getType() & node_type) != node_type)
        node = node->getParent();
    return node;
}

const BaseNode* 
BaseNode::getAncestor(NodeType node_type) const
{
    const BaseNode* node = this;
    while (node != 0 && (node->getType() & node_type) != node_type)
        node = node->getParent();
    return node;
}

string
BaseNode::getName() const
{
    return M_name;
}

string 
BaseNode::getPath() const
{
    string s = "";
    if (getParent() != 0) s = getParent()->getPath();
    return s + M_name;
}

void 
BaseNode::leafNames(std::set<std::string>& names) const
{
    names.clear();
}
