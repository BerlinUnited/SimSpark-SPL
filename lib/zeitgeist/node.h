/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: node.h,v 1.7 2004/04/22 14:32:26 fruit Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

        Node

        HISTORY:
                15.06.2002 MK
                        - initial version
                02.09.2002 MK
                        - removed quad-link
*/
#ifndef ZEITGEIST_NODE_H
#define ZEITGEIST_NODE_H

#include <salt/defines.h>
#include "class.h"
#include "leaf.h"

namespace zeitgeist
{
#if 0
}
#endif
/** Node makes up the hierarchy. A Node object can reside within the
    hierarchy and have children
*/
class Node : public Leaf
{
    //
    // functions
    //
public:
    /** constructs a node with the given name */
    Node(const std::string &name = "<Unnamed>");
    virtual ~Node();

    // hierarchy

    /** returns a pointer to a child (i.e. node or leaf) with the
        given name, which can be searched recursively. */
    virtual boost::shared_ptr<Leaf> GetChild(const std::string &name, bool recursive = false);

    /** returns a pointer to a child of the given class type,
        which can be searched recursively. The class Leaf will
        always return an empty reference */
    virtual boost::shared_ptr<Leaf> GetChildOfClass(const std::string &name, bool recursive = false);

    /** constructs a list of children supporting the class 'name'
        (i.e. nodes of a type equal to or derived from the class
        'name'), which can be searched recursively. */
    virtual boost::shared_ptr<Leaf> GetChildSupportingClass(const std::string &name, bool recursive = false);

    /** returns an a list of children. */
    virtual void GetChildren(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** constructs a list of all children of type 'name'. */
    virtual void GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** constructs a list of all children supporting a class
        'name' i.e. they are an instance of that class or are
        derived from it.  */
    virtual void GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** returns false to indicate that this node isn't a lead */
    virtual bool IsLeaf() const;

    /** removes base from the set of children. */
    virtual void RemoveChildReference(const boost::shared_ptr<Leaf> &base);

    /** adds base to the set of children and sets the parent of base to be this node */
    virtual bool AddChildReference(const boost::shared_ptr<Leaf> &base);

    /** writes debug data to stdout */
    virtual void Dump() const;

    /** update variables from a script */
    virtual void UpdateCached() {}

    // iterator helpers
    virtual TLeafList::iterator begin();
    virtual TLeafList::const_iterator begin() const;
    virtual TLeafList::iterator end();
    virtual TLeafList::const_iterator end() const;
private:
    Node(const Node &obj);
    Node& operator=(const Node &obj);

    //
    // members
    //
protected:
    // object hierarchy related stuff
    TLeafList mChildren;
};

DECLARE_CLASS(Node);

} //namespace zeitgeist;

#endif //ZEITGEIST_NODE_H
