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

   Class

   HISTORY:
                04.06.2002 MK
                        - initial version

*/
#ifndef Q_MOC_RUN
#include <boost/tokenizer.hpp>
#endif
#include "node.h"
#include <salt/path.h>
#include <iostream>

/*
std::ostream& zeitgeist::operator<<(std::ostream& output, const Node& node)
{
        output << (int)&node << " '" << node.GetName() << "'" << std::endl;

        //for (Node::TLeafList::const_iterator i=leaf.mChildren.begin(); i!=leaf.mChildren.end(); ++i)
        //{
        //      output << *(*i);
        //}
        return output;
}*/

//=============================================================================
using namespace boost;
using namespace salt;
using namespace std;
using namespace zeitgeist;


Node::Node(const std::string& name) : Leaf(name)
{
}

Node::~Node()
{
}

boost::shared_ptr<Leaf>
Node::GetChild(const std::string& name, bool recursive)
{
    boost::shared_ptr<Leaf> leaf   = Leaf::GetChild(name, recursive);

    if (leaf.get() != NULL) return leaf;

    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and return it
        if ((*i)->GetName().compare(name) == 0)
            return (*i);

        if (recursive)
        {
            boost::shared_ptr<Leaf> ret = (*i)->GetChild(name, recursive);
            if (ret.get() != NULL) return ret;
        }
    }

    return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf>
Node::GetChildOfClass(const std::string& name, bool recursive)
{
    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and add it
        boost::shared_ptr<Class> theClass = (*i)->GetClass();
        if (theClass.get() != NULL && theClass->GetName().compare(name) == 0)
        {
            return (*i);
        }

        if (recursive)
        {
            boost::shared_ptr<Leaf> ret = (*i)->GetChildOfClass(name, recursive);
            if (ret.get() != NULL) return ret;
        }
    }

    return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf>
Node::GetChildSupportingClass(const std::string& name, bool recursive)
{
    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and add it
        boost::shared_ptr<Class> theClass = (*i)->GetClass();
        if (theClass.get() != NULL && theClass->SupportsClass(name))
        {
            return (*i);
        }

        if (recursive)
        {
            boost::shared_ptr<Leaf> ret = (*i)->GetChildSupportingClass(name, recursive);
            if (ret.get() != NULL) return ret;
        }
    }

    return boost::shared_ptr<Leaf>();
}

void
Node::GetChildren(const std::string& name, TLeafList& baseList, bool recursive)
{
    Leaf::GetChildren(name, baseList, recursive);

    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and add it
        if ((*i)->GetName().compare(name) == 0)
        {
            baseList.push_back(*i);
        }

        if (recursive)
            (*i)->GetChildren(name, baseList, recursive);
    }
}

void
Node::GetChildrenOfClass(const std::string& name, TLeafList& baseList, bool recursive)
{
    Leaf::GetChildrenOfClass(name, baseList, recursive);

    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and add it
        boost::shared_ptr<Class> theClass = (*i)->GetClass();
        if (theClass.get() != NULL && theClass->GetName().compare(name) == 0)
        {
            baseList.push_back(*i);
        }

        if (recursive)
            (*i)->GetChildrenOfClass(name, baseList, recursive);
    }
}

void
Node::GetChildrenSupportingClass(const std::string& name, TLeafList& baseList, bool recursive)
{
    Leaf::GetChildrenSupportingClass(name, baseList, recursive);

    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        // check if we have found a match and add it
        boost::shared_ptr<Class> theClass = (*i)->GetClass();
        if (theClass.get() != NULL && theClass->SupportsClass(name))
        {
            baseList.push_back(*i);
        }

        if (recursive)
        {
            (*i)->GetChildrenSupportingClass(name, baseList, recursive);
        }
    }
}

int
Node::GetNumberOfChildren() const
{
    return static_cast<int>(mChildren.size());
}

bool
Node::IsLeaf() const
{
    return false;
}

void
Node::RemoveChildReference(const boost::shared_ptr<Leaf>& leaf)
{
    mChildren.remove(leaf);
}

void
Node::UnlinkChildren()
{
    while (! mChildren.empty())
    {
        boost::shared_ptr<Leaf> node = mChildren.front();
        node->UnlinkChildren();
        node->Unlink();
    }
}

bool
Node::AddChildReference(const boost::shared_ptr<Leaf>& leaf)
{
    if (leaf.get() == 0)
    {
        return false;
    }

    if (leaf->GetClass() == 0)
    {
        if (leaf->GetName() != "ClassClass")
        {
            cerr << "(Node::AddChildReference) ERROR: object "
                 << leaf->GetName()
                 << " has no assigned class object." << endl;
        }
        return false;
    }

    mChildren.push_back(leaf);
    leaf->SetParent(static_pointer_cast<Node>(GetSelf().lock()));
    return true;
}

void
Node::Dump() const
{
    Leaf::Dump();
    cout << "Node: numChildren = " << mChildren.size() << endl;

    for (TLeafList::const_iterator i = mChildren.begin(); i!=mChildren.end(); ++i)
    {
        (*i)->Dump();
        cout << endl;
    }

    cout << "End Node" << endl;
}

Leaf::TLeafList::iterator
Node::begin()
{
    return mChildren.begin();
}

Leaf::TLeafList::const_iterator
Node::begin() const
{
    return mChildren.begin();
}

Leaf::TLeafList::iterator
Node::end()
{
    return mChildren.end();
}

Leaf::TLeafList::const_iterator
Node::end() const
{
    return mChildren.end();
}

void
Node::UpdateCached()
{
    Leaf::UpdateCached();

    // update all Leaves found
    for (TLeafList::iterator iter = begin();
         iter != end();
         ++iter
        )
    {
        // node specific update
        (*iter)->UpdateCachedInternal();

        // recurse
        (*iter)->UpdateCached();
    }
}

