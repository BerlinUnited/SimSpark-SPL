/* -*-c++-*- ***************************************************************
                             basenode.h
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
#ifndef RCSS_ENTITYGRAPH_BASENODE_H
#define RCSS_ENTITYGRAPH_BASENODE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <functional>
#include <set>
#include <string>

namespace rcss
{ 
    namespace EntityTree
    { 
#if 0   // only for indenting
    }}  
#endif

class BaseNode
{
public:
    enum NodeType { S_NONE = 0, 
                    S_DIRECTORY = 1, 
                    S_CONTAINER = 2,
                    S_GEOM_BASE = 4 };
	
    BaseNode(NodeType node_type,
             const std::string& name = "<unknown>", 
	     BaseNode* parent = 0);
    virtual ~BaseNode();

    NodeType getType() const;

    void setParent(BaseNode* parent);
    BaseNode* getParent();
    const BaseNode* getParent() const;
    BaseNode* getRoot();
    const BaseNode* getRoot() const;

    BaseNode* getAncestor(NodeType node_type);
    const BaseNode* getAncestor(NodeType node_type) const;

    std::string getName() const;
    
    virtual std::string getPath() const;
    
    //! Initialize the node. 
    //! Will be called everytime the node is successfully 
    //! registered to a new parent.
    virtual void init() {}

    //! Create the set of names of leafs from this node.
    /*! For the BaseNode, this is an empty set; for a DirNode,
        it is the set of names of its children. 
        The set created by this method can be used by the console to 
        show the "content" of a node.

        \param names the set of names
    */
    virtual void leafNames(std::set<std::string>& names) const;

protected:
    NodeType M_type;
    std::string M_name;
    BaseNode* M_parent;
};

struct CompareNodeName 
    : public std::binary_function<const BaseNode*, std::string, bool>
{
    bool operator ()(const BaseNode* node, const std::string& name) const
    { return node->getName() == name; }
};


    } // namespace 
} // namespace

#endif
