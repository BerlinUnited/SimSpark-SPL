/* -*-c++-*- ***************************************************************
                             entitytree.h
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
#ifndef RCSS_ENTITY_ENTITYTREE_H
#define RCSS_ENTITY_ENTITYTREE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

namespace rcss
{ 
    namespace entity
    { 
#if 0
}}
#endif

class BaseNode;
class DirNode;

class EntityTree
{
public:
    ~EntityTree();

    //! get the singleton instance of the entity tree
    static EntityTree& instance();

    //! Add a child node to the list of child nodes.
    /*! It is checked if the current node is a directory type node
        and if no child with the same name already exists.
        \param node the node to be added
    */
    void addNode(BaseNode& node);

    //! Change to a child or to the parent node.
    /*! If possible change to a child node with the given name or
        to the parent directory (..).
        \param name the name of the new node
        \return true if successful
    */
    bool changeNode(const std::string& name);

    //! \return current node
    BaseNode* getCurrentNode() { return M_current; }

    //! \return current node
    DirNode* getRoot() { return M_root; }

protected:
    EntityTree();
    
    DirNode* M_root;
    BaseNode* M_current;
};

    } // namespace 
} // namespace

#endif
