/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: leaf.h,v 1.6 2003/11/14 14:05:54 fruit Exp $

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

   Leaf

        HISTORY:
                04.06.2002 MK
                        - initial version
                13.06.2002 MK
                        - reworked Base to use a quad-linked tree
                15.06.2002 MK
                        - split Base up into Base and Node classes
                02.09.2002 MK
                        - removed quad-link
                22.02.2003 MK
                        - renamed to Leaf

*/
#ifndef ZEITGEIST_LEAF_H
#define ZEITGEIST_LEAF_H

#include <list>
#include <string>
#include "object.h"

namespace zeitgeist
{

/** Leaf defines the beginning of the hierarchy. A Leaf object can
    reside within the hierarchy, but cannot have children!
*/
class Leaf : public Object
{
    friend class Node;
    //
    // types
    //
public:
    typedef std::list< boost::shared_ptr<Leaf> > TLeafList;

    //
    // functions
    //
public:
    /** constructs a leaf with the given name */
    Leaf(const std::string &name = "<unnamed>");
    virtual ~Leaf();

    /** returns a reference to the name of the leaf */
    std::string& GetName()
    {       return mName;   }

    /** returns a constant reference to the name of the leaf */
    const std::string&  GetName() const
    {       return mName;   }

    // hierarchy

    /** returns a pointer to the parent of the leaf */
    boost::weak_ptr<Node>& GetParent();

    /** returns a constant pointer to the parent of the leaf */
    const boost::weak_ptr<Node>& GetParent() const;

    /** defines an interface to get a pointer to a child
        (i.e. node or leaf) with the given name, which can be
        searched recursively. The class Leaf will always return an
        empty reference */
    virtual boost::shared_ptr<Leaf>
    GetChild(const std::string &name, bool recursive = false);

    /** defines an interface to get a pointer to child of the
        given class type, which can be searched recursively. The
        class Leaf will always return an empty reference */
    virtual boost::shared_ptr<Leaf>
    GetChildOfClass(const std::string &name, bool recursive = false);

    /** defines an interface to get a list of children supporting
        the class 'name' (i.e. nodes of a type equal to or derived
        from the class 'name'), which can be searched
        recursively. The class Leaf will always return an empty
        reference */
    virtual boost::shared_ptr<Leaf>
    GetChildSupportingClass(const std::string &name, bool recursive = false);

    /** defines an interface to get a list of children. The Leaf
        class will always return an empty list */
    virtual void
    GetChildren(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** defines an interface to get a list to all children of type
        'name'. The Leaf class will always return an empty list */
    virtual void GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** defines an interface to get a list to all children
        supporting a class 'name' i.e. they are an instance of
        that class or are derived from it. The Leaf class will
        always return an empty list */
    virtual void GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive = false);

    /** defines an interface to test if this node is a leaf. Only
        the TLeaf class will return true */
    virtual bool IsLeaf() const;

    /** removes base from the set of children. */
    virtual void RemoveChildReference(const boost::shared_ptr<Leaf> &base);

    /** adds base to the set of children and sets the parent of base to be this node */
    virtual bool AddChildReference(const boost::shared_ptr<Leaf> &base);

    /** detaches this node and its hierarchy from its parent. The
        virtual method OnUnlink() is called to give derived nodes
        a chance to clean up. */
    void Unlink();

    /** writes debug data to stdout */
    virtual void Dump() const;

    /** constructs the full path of this node by walking up the
        tree. Cosecutive calls return a cached copy of the full
        path to avoid the expensive tree walk. */
    const std::string& GetFullPath() const;

    /** clears any cached data (e.g. the cached full path and
        forces the node to recalculate all values */
    void                                ClearCachedData() const;

    /** sets the name of this node */
    void        SetName(const std::string &name)
    {   mName = name;   }

    // iterator helpers
    virtual TLeafList::iterator begin();
    virtual TLeafList::const_iterator begin() const;
    virtual TLeafList::iterator end();
    virtual TLeafList::const_iterator end() const;
protected:
    /** Sets the parent of this node. It has to be implemented
        'very carefully'. The parent object always holds a
        shared pointer reference to mSelf. What we have to do
        is 'get' the shared reference, remove it from the old
        parent. Insert it into the new parent and change the
        parent pointer.
    */
    void        SetParent(const boost::shared_ptr<Node> &parent);

    /** This method is called, when the hierarchy object has been
        linked to a parent. At that point, traversal can
        commence. It can be overridden to support custom 'link'
        behavior.
    */
    virtual void OnLink();

    /** This rountine is called, before the hierarchy object is
        removed from the parent. It can be overridden to support
        custom 'unlink' behavior.
    */
    virtual void OnUnlink();

private:
    Leaf(const Leaf &obj);
    Leaf& operator=(const Leaf &obj);

    //
    // members
    //
protected:
    /** This pointer holds a link to the parent of this node. It
        has to be at least a Node, as that is the first class,
        which can hold children.  We use a weak pointer to break
        the cyclic dependency. */
    boost::weak_ptr<Node>       mParent;

private:
    /** the name of the node */
    std::string mName;

    /** temporary cached full path of this node in the hierarchy */
    mutable std::string *mCachedFullPath;
};

/** the class object declaration for Leaf has been moved to class.h to break
 * circular inclusion between class.h and leaf.h
 */

} //namespace zeitgeist;

#endif //ZEITGEIST_LEAF_H
