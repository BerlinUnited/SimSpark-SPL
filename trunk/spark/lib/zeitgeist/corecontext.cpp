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
#include "corecontext.h"
#include <salt/path.h>
#include "leaf.h"
#include "node.h"
#include "core.h"
#include <iostream>

using namespace boost;
using namespace salt;
using namespace std;
using namespace zeitgeist;

CoreContext::CoreContext(const boost::shared_ptr<Core> &core, const boost::shared_ptr<Leaf> &root) : mCore(core), mObject(root)
{
}

CoreContext::~CoreContext()
{
    //cout << "~CoreContext()" << endl;
}

boost::shared_ptr<Leaf> CoreContext::New(const std::string& className, const std::string& pathStr)
{
    // first, we try to create the class with the core
    shared_ptr<Object> instance = mCore->New(className);

    if (instance.get())
        {
            // now, we check the type by dynamic casting to leaf
            shared_ptr<Leaf> leaf = shared_static_cast<Leaf>(instance);

            if (leaf.get() != NULL)
                {
                    // we have created the instance, now we install it at the location
                    // stored in pathName
                    if (Install(leaf, pathStr) == true)
                        {
                            mObject = leaf;
                            return leaf;
                        }
                }
        }

    // return default constructed 'NULL' object
    return shared_ptr<Leaf>();
}

bool CoreContext::Delete (const std::string &name)
{
    shared_ptr<Leaf> leaf = Get(name);

    if (leaf.get())
        {
            leaf->Unlink();
            return true;
        }

    return false;
}


boost::shared_ptr<Leaf> CoreContext::Select(const std::string &pathStr)
{
    shared_ptr<Leaf> leaf = Get(pathStr);

    if (leaf.get())
        {
            mObject = leaf;
        }

    return leaf;
}

bool CoreContext::Install(const boost::shared_ptr<Leaf> &leaf, const std::string &pathStr, bool isNamed)
{
    //cout << "CoreContext(" << (void*) this << ") Install '" << pathStr << "'" << endl;
    Path path(pathStr);

    if(! isNamed)
        {
            // we need at least one token to 'name' the leaf class
            if (path.IsEmpty())
                {
                    return false;
                }

            leaf->SetName(path.Back());
            path.PopBack();
            // now, we have named the leaf object, so we can install it
        }

    shared_ptr<Leaf> current;

    // check if we have a relative or absolute path
    if (path.IsAbsolute())
        {
            current = mCore->GetRoot();
        }
    else
        {
            current = mObject;
        }

    if (! current.get())
        {
            return false;
        }

    while (! path.IsEmpty())
        {
            current = mCore->GetChild(current, path.Front());

            if (! current.get())
                {
                    return false;
                }

            path.PopFront();
        }

    return current->AddChildReference(leaf);
}

boost::shared_ptr<Leaf> CoreContext::Get(const std::string& pathStr)
{
    return mCore->Get(pathStr, mObject);
}

bool CoreContext::Test(const std::string& pathStr)
{
    return mCore->Test(pathStr, mObject);
}

void CoreContext::ListObjects() const
{
    Leaf::TLeafList::iterator i;

    for (i = mObject->begin(); i != mObject->end(); ++i)
        {
            cout << (*i)->GetName();
            if (!(*i)->IsLeaf())
                {
                    cout << "/";
                }
            cout << endl;
        }
}

void CoreContext::Push()
{
    if (mObject.get() != NULL)
        mObjectStack.push_front(mObject);
}

void CoreContext::Pop()
{
    if (!mObjectStack.empty())
        {
            mObject = mObjectStack.front();
            mObjectStack.pop_front();
        }
}

void CoreContext::Dir() const
{
    for (
         TObjectStack::const_iterator i = mObjectStack.begin();
         i != mObjectStack.end();
         ++i
         )
        {
            cout << (*i)->GetName();
            if (!(*i)->IsLeaf())
                {
                    cout << "/";
                }
            cout << endl;
        }
}
