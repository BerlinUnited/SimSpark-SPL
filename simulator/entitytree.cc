/* -*-c++-*- ***************************************************************
                             entitytree.cc
    the thing where all the simulation objects are stored
                           ------------------------
    begin                : Sep 27 2002  Oliver Obst
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
#include "entitytree.h"

#include "basenode.h"
#include "dirnode.h"

using namespace rcss::EntityTree;
using namespace std;

EntityTree::EntityTree()
{
    M_root = new DirNode("");
    M_current = M_root;
}

EntityTree::~EntityTree()
{
    delete M_root;
}

EntityTree&
EntityTree::instance()
{
    static EntityTree S_instance;
    return S_instance;
}

void
EntityTree::addNode(BaseNode& node)
{
    if (M_current->getType() & BaseNode::S_DIRECTORY)
    {
        DirNode* current = dynamic_cast<DirNode*>(M_current);
        if (current->getChild(node.getName()) == 0)
        {
            current->registerChild(&node);
        }
        /* FIXME:
           else throw some exception (child with that name already exists)
        */
    }   
    /* FIXME:
       else throw some exception (current node is not a directory)
    */
}

bool 
EntityTree::changeNode(const std::string& name)
{
    BaseNode* current = M_current;
    string::size_type idx = name.find('/');
    if (idx == string::npos)
    {
        if (name == ".") return true;
        if (name == "..")
        {
            if (M_current->getParent() != 0)
                M_current = M_current->getParent();
            return true;
        }
        if (M_current->getType() & BaseNode::S_DIRECTORY)
        {
            current = dynamic_cast<DirNode*>(M_current)->getChild(name);
            if (current == 0) return false;
            M_current = current;
            return true;
        }
        return false;
    }
    ++idx;

    // change to the next part of the path (up to behind the next '/'),
    // e.g. '/' or 'somedir/'
    string next_node = name.substr(0,idx);
    if (next_node == "/") M_current = M_current->getRoot();
    else if (!changeNode(next_node)) return false;

    // change to the remaining part of the path, i.e. everything after
    // the next '/'
    if (changeNode(name.substr(idx))) return true;
    
    // if change didn't work, we restore the original node
    M_current = current;
    return false;
}


