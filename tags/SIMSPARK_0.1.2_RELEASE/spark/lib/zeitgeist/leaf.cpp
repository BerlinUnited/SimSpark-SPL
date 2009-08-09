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
*/
#include "class.h"
#include "node.h"
#include <iostream>
#include <sstream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Leaf::Leaf(const std::string &name)
    : Object(), mName(name), mCachedFullPath(NULL)
{
}

Leaf::~Leaf()
{
}

boost::weak_ptr<Node>& Leaf::GetParent()
{
    return mParent;
}

const boost::weak_ptr<Node>& Leaf::GetParent() const
{
    return mParent;
}

boost::shared_ptr<Leaf> Leaf::GetChild(const std::string &name, bool /*recursive*/)
{
    if (name.compare("..") == 0)
    {
        return GetParent().lock();
    }

    if (name.compare(".") == 0)
    {

        return shared_static_cast<Leaf>(GetSelf().lock());
    }

    return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Leaf::GetChildOfClass(const std::string &/*name*/, bool /*recursive*/)
{
    return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Leaf::GetChildSupportingClass(const std::string &/*name*/, bool /*recursive*/)
{
    return boost::shared_ptr<Leaf>();
}

void Leaf::GetChildren(const std::string &name, TLeafList &baseList, bool /*recursive*/)
{
    if (name.compare("..") == 0)
    {
        baseList.push_back(GetParent().lock());
    }

    if (name.compare(".") == 0)
    {
        baseList.push_back(shared_static_cast<Leaf>(GetSelf().lock()));
    }
}

void Leaf::GetChildrenOfClass(const std::string &/*name*/, TLeafList &/*baseList*/, bool /*recursive*/)
{
}

void Leaf::GetChildrenSupportingClass(const std::string &/*name*/, TLeafList &/*baseList*/, bool /*recursive*/)
{
}

boost::weak_ptr<Node>
Leaf::GetParentSupportingClass(const std::string &name) const
{
  shared_ptr<Node> node
    = shared_static_cast<Node>(GetParent().lock());

  while
    (
     (node.get() != 0) &&
     (node->GetClass()) &&
     (! node->GetClass()->SupportsClass(name))
     )
    {
      node = node->GetParent().lock();
    }

  return weak_ptr<Node>(node);
}

int
Leaf::GetNumberOfChildren() const
{
    return 0;
}

bool Leaf::IsLeaf() const
{
    return true;
}

void Leaf::RemoveChildReference(const boost::shared_ptr<Leaf> &/*base*/)
{
}

bool Leaf::AddChildReference(const boost::shared_ptr<Leaf> &/*base*/)
{
    return false;
}

void Leaf::Unlink()
{
    // here we lose our reference to the parent
    SetParent(boost::shared_ptr<Node>());
}

void Leaf::UnlinkChildren()
{
}

void Leaf::Dump() const
{
    Object::Dump();
    cout << "Leaf: '" << GetName() << "'" << endl;
}

const std::string& Leaf::GetFullPath() const
{
    //printf("this:getfullpath %x - %s - %x %x\n", this, GetName().c_str(), GetParent(), mCachedFullPath);
    if (mCachedFullPath == NULL)
    {
        std::string parentPath;

        if (shared_ptr<Leaf> p = GetParent().lock())
        {
            if (p)
            {
                shared_ptr<Leaf> blah = GetParent().lock();
                parentPath = blah->GetFullPath();
            }
        }

        // no cached data available
        if (IsLeaf())
            mCachedFullPath = new std::string(parentPath + mName);
        else
            mCachedFullPath = new std::string(parentPath + mName + "/");
    }

    return *mCachedFullPath;
}

void Leaf::ClearCachedData()
{
    delete mCachedFullPath;
    mCachedFullPath = NULL;
    mCachedPaths.clear();
}

Leaf::TLeafList gFakeChildren;

Leaf::TLeafList::iterator Leaf::begin()
{
    return gFakeChildren.begin();
}

Leaf::TLeafList::const_iterator Leaf::begin() const
{
    return gFakeChildren.begin();
}

Leaf::TLeafList::iterator Leaf::end()
{
    return gFakeChildren.end();
}

Leaf::TLeafList::const_iterator Leaf::end() const
{
    return gFakeChildren.end();
}

void Leaf::SetParent(const boost::shared_ptr<Node> &newParent)
{
    shared_ptr<Node> oldParent = GetParent().lock();
    shared_ptr<Leaf> self = shared_static_cast<Leaf>(GetSelf().lock());

    if (oldParent.get() != 0)
        {
            // we have a parent, so update our state
            // here reference count should be > 1 (at least one in the
            // parent, and one in this routine)
            assert(self.use_count() > 1);

            if (newParent.get() == 0)
                {
                    // time to clean up
                    OnUnlink();
                    ClearCachedData();
                    oldParent->RemoveChildReference(self);
                }

            // we remove ourself from the old parent's list of children
            oldParent->RemoveChildReference(self);

            // we add ourself to the new parent's list of children
            if (newParent.get() != 0)
            {
                newParent->AddChildReference(self);
            }
        }

    mParent = newParent;
    if (newParent.get() == 0)
        {
            return;
        }

    // assure a unique name among our siblings
    shared_ptr<Leaf> sibling = newParent->GetChild(mName);

    if (sibling != self)
        {
            stringstream ss;
            ss << mName << "_" << newParent->GetNumberOfChildren() << ">";
            mName = ss.str().c_str();
        }

    // we have been linked, so now we can do something :)
    OnLink();
}

void Leaf::OnLink()
{
}

void Leaf::OnUnlink()
{
}

void Leaf::UpdateCached()
{
    shared_ptr<Core> core(GetCore());

    for (
         TCachedPathSet::iterator iter = mCachedPaths.begin();
         iter != mCachedPaths.end();
         ++iter
         )
        {
            (*iter)->Update(core);
        }
}

void Leaf::RegisterCachedPath(Core::CachedLeafPath& path, const std::string& pathStr)
{
    path.Cache(GetCore(), pathStr);
    mCachedPaths.insert(&path);
}

void Leaf::RegisterCachedPath(Core::CachedLeafPath& path)
{
    mCachedPaths.insert(&path);
}
