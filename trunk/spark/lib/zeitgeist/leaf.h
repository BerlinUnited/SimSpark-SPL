/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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

#include <set>
#include <string>
#include "object.h"

namespace zeitgeist
{

/** Leaf defines the beginning of the hierarchy. A Leaf object can
    reside within the hierarchy, but cannot have children!
*/
class ZEITGEIST_API Leaf : public Object
{
    friend class Node;
    //
    // types
    //
public:
    template <typename _CLASS>
    struct CachedPath : public Core::CachedPath<_CLASS>
    {
    };

    typedef std::list< boost::shared_ptr<Leaf> > TLeafList;
    typedef std::set<Core::CachedLeafPath*> TCachedPathSet;

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

    /** defines an interface to get the fist child supporting the
        class 'name' (i.e. nodes of a type equal to or derived from
        the class 'name'), which can be searched recursively. The
        class Leaf will always return an empty reference */
    virtual boost::shared_ptr<Leaf>
    GetChildSupportingClass(const std::string &name, bool recursive = false);

    /** defines an interface to get the fist child supporting the
        class 'name' (i.e. nodes of a type equal to or derived from
        the class 'name'), which can be searched recursively. The
        class Leaf will always return an empty reference. This
        implementation of FindChildSupportingClass does not rely on the
        associated zeitgeist class name but uses the c++ typeid
        system. */
    template<class CLASS>
    boost::shared_ptr<CLASS>
    FindChildSupportingClass(bool recursive = false)
    {
        TLeafList::iterator lstEnd = end(); // avoid repeated virtual calls
        for (TLeafList::iterator i = begin(); i != lstEnd; ++i)
            {
                // check if we have found a match and return it
                boost::shared_ptr<CLASS> child = boost::shared_dynamic_cast<CLASS>(*i);
                if (child.get() != 0)
                    {
                        return child;
                    }

                if (recursive)
                    {
                        child = (*i)->FindChildSupportingClass<CLASS>(recursive);
                        if (child.get() != 0)
                        {
                            return child;
                        }
                    }
            }

        return boost::shared_ptr<CLASS>();
    }

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

    /** constructs a list of all children supporting a class 'name'
        i.e. they are an instance of that class or are derived from
        it. This implementation of GetChildrenSupportingClass does not
        rely on the associated zeitgeist class name but uses the c++
        typeid system.
    */
    template<class CLASS>
    void ListChildrenSupportingClass(TLeafList& list, bool recursive = false)
    {
        ListChildrenSupportingClass<CLASS>(list, recursive, false);
    }
    
    /** constructs a list of all children supporting a class 'name'
        i.e. they are an instance of that class or are derived from
        it by doing a recursive search, backtracking when a child is
        found. This implementation of GetChildrenSupportingClass does not
        rely on the associated zeitgeist class name but uses the c++
        typeid system. 
    */
    template<class CLASS>
    void ListShallowChildrenSupportingClass(TLeafList& list)
    {
        ListChildrenSupportingClass<CLASS>(list, true, true);
    }
    
    /** defines an interface to get the first parent node on the way
        up the hierarchy that supports a class 'name', i.e. is an
        instance of that class or is derived from it.
     */
    virtual boost::weak_ptr<Node>
    GetParentSupportingClass(const std::string &name) const;

     /** defines an interface to get the first parent node on the way
         up the hierarchy that supports a class 'name', i.e. is an
         instance of that class or is derived from it. This
         implementation of GetParentSupportingClass does not rely on
         the associated zeitgeist class name but uses the c++ typeid
         system.
      */
    template<class CLASS>
    boost::weak_ptr<CLASS>
    FindParentSupportingClass() const;
//     {
//         boost::shared_ptr<Node> node
//             = boost::shared_static_cast<Node>(GetParent().lock());

//         while (node.get() != 0)
//         {
//             boost::shared_ptr<CLASS> test =
//                 boost::shared_dynamic_cast<CLASS>(node);

//             if (test.get() != 0)
//             {
//                 return test;
//             }

//             node = boost::shared_static_cast<Node>(node->GetParent().lock());
//         }

//         return boost::shared_ptr<CLASS>();
//     }


    /** defines an interface to test if this node is a leaf. Only
        the TLeaf class will return true */
    virtual bool IsLeaf() const;

    /** returns the total number of children */
    virtual int GetNumberOfChildren() const;

    /** removes base from the set of children. */
    virtual void RemoveChildReference(const boost::shared_ptr<Leaf> &base);

    /** adds base to the set of children and sets the parent of base to be this node */
    virtual bool AddChildReference(const boost::shared_ptr<Leaf> &base);

    /** detaches this node and its hierarchy from its parent.  */
    void Unlink();

    /** unlinks all child nodes */
    virtual void UnlinkChildren();

    /** writes debug data to stdout */
    virtual void Dump() const;

    /** update and register a path reference */
    void RegisterCachedPath(Core::CachedLeafPath& path, const std::string& pathStr);

    /** register a path reference */
    void RegisterCachedPath(Core::CachedLeafPath& path);

    /** update variables from a script */
    virtual void UpdateCached();

    /** constructs the full path of this node by walking up the
        tree. Cosecutive calls return a cached copy of the full
        path to avoid the expensive tree walk. */
    const std::string& GetFullPath() const;

    /** clears any cached data (e.g. the cached full path and
        forces the node to recalculate all values */
    void ClearCachedData();

    /** sets the name of this node */
    void        SetName(const std::string &name)
        {   mName = name;  ClearCachedData(); }

    // iterator helpers
    virtual TLeafList::iterator begin();
    virtual TLeafList::const_iterator begin() const;
    virtual TLeafList::iterator end();
    virtual TLeafList::const_iterator end() const;
protected:
    /** called from within UpdatCached; override to perform node
        specific updates
    */
    virtual void UpdateCachedInternal() {}

    /** Sets the parent of this node. It has to be implemented
        'very carefully'. The parent object always holds a
        shared pointer reference to mSelf. What we have to do
        is 'get' the shared reference, remove it from the old
        parent. Insert it into the new parent and change the
        parent pointer.
    */
    void SetParent(const boost::shared_ptr<Node> &parent);

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

    /** constructs a list of all children supporting a class 'name'
        i.e. they are an instance of that class or are derived from
        it. This implementation of GetChildrenSupportingClass does not
        rely on the associated zeitgeist class name but uses the c++
        typeid system.
    */
    template<class CLASS>
    void ListChildrenSupportingClass(TLeafList& list, bool recursive, bool shallow)
    {
        TLeafList::iterator lstEnd = end(); // avoid repeated virtual calls
        for (TLeafList::iterator i = begin(); i != lstEnd; ++i)
            {
                // check if we have found a match and add it
                boost::shared_ptr<CLASS> child = boost::shared_dynamic_cast<CLASS>(*i);
                if (child.get() != 0)
                    {
                        list.push_back(child);
                        if (shallow)
                            recursive = false;
                    }

                if (recursive)
                    {
                        (*i)->ListChildrenSupportingClass<CLASS>(list,recursive, shallow);
                    }
            }
    }

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
    boost::weak_ptr<Node> mParent;

private:
    /** the name of the node */
    std::string mName;

    /** temporary cached full path of this node in the hierarchy */
    mutable std::string *mCachedFullPath;

    /** list of cached path references to other nodes */
    TCachedPathSet mCachedPaths;
};

/** the class object declaration for Leaf has been moved to class.h to break
 * circular inclusion between class.h and leaf.h
 */

} //namespace zeitgeist;

#endif //ZEITGEIST_LEAF_H
