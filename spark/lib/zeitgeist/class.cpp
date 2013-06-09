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
#include "leaf.h"
#include "core.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Class::Class(const std::string &name) : Leaf(name)
{
}

Class::~Class()
{
    if (mInstances.size() > 0)
    {
        cout << "(Class) Leaked "
             << mInstances.size() << " instances..." << endl;

        for (
             TObjectList::iterator i = mInstances.begin();
             i != mInstances.end(); ++i
             )
        {
            if (boost::shared_ptr<Object> j = i->lock())
            {
                cout << "    " << j.get() << endl;
            } else
            {
                cout << "    " << "(expired)" << endl;
            }
        }
    }
}

boost::shared_ptr<Object> Class::Create()
{
    boost::shared_ptr<Object> obj(CreateInstance());

    if (obj.get())
    {
        if (obj->Construct(obj, static_pointer_cast<Class>
                           (GetSelf().lock())) == true)
        {
            // successfully constructed
            AttachInstance(obj);
        }
        else
        {
            obj.reset();
        }
    }

    return obj;
}

boost::shared_ptr<Core> Class::GetCore() const
{
    if (mCore.expired())
        {
            std::cerr << "(Class) ERROR: failed to get zeitgeist Core for class '"
                      << GetName() << "'" << std::endl;

        }

    return mCore.lock();
}

void Class::AttachInstance(const boost::weak_ptr<Object> &instance)
{
    mInstances.push_back(instance);
}

void Class::DetachInstance(const boost::weak_ptr<Object> &instance)
{
    // mInstances.remove() doesn't work in this case because
    // operator== is not implemented for boost::weak_ptr
    TObjectList::iterator first = mInstances.begin();
    TObjectList::iterator last = mInstances.end();

    while (first != last)
    {
        TObjectList::iterator next = first;
        ++next;

        boost::shared_ptr<Object> i = first->lock();
        boost::shared_ptr<Object> j = instance.lock();

        if (i.get() == j.get())
        {
            mInstances.erase(first);
        }
        first = next;
    }
}

Object* Class::CreateInstance() const
{
    return NULL;
}

void Class::AttachTo(const boost::weak_ptr<Core>& core)
{
    mCore = core;
}

void Class::SetBundle(const boost::shared_ptr<salt::SharedLibrary> &bundle)
{
    mBundle = bundle;
}

const Class::TCommandMap& Class::GetCommandMap() const
{
    return mFunctions;
}

Class::TCmdProc Class::GetCmdProc(const std::string &functionName) const
{
    TCommandMap::const_iterator cmd = mFunctions.find(functionName);

    if (cmd != mFunctions.end())
    {
        return (*cmd).second;
    }

    // ok, we don't have the requested function, so we'll try the base
    // class objects
    boost::shared_ptr<Leaf> classDir = GetCore()->Get("/classes");

    for (
         TStringList::const_iterator baseClass = mBaseClasses.begin();
         baseClass != mBaseClasses.end();
         ++baseClass
         )
    {
        // this should get the base class object (it has to live on
        // the same level of the hierarchy as this class object)
        boost::shared_ptr<Class> theClass = static_pointer_cast<Class>
            (GetCore()->Get(*baseClass, classDir));

        if (theClass)
        {
            // now, we ask the class object, if it knows the command
            // in question
            TCmdProc theCmd = theClass->GetCmdProc(functionName);
            //printf("theCmd: %s - %d\n", functionName.c_str(), theCmd);

            if (theCmd != NULL)
            {
                // here we have found the command and return it
                return theCmd;
            }
        }
    }

    return NULL;
}

const Class::TStringList& Class::GetBaseClasses() const
{
    return mBaseClasses;
}

bool Class::SupportsCommand(const std::string & name) const
{
    return (GetCmdProc(name) != 0);
}

bool Class::SupportsClass(const std::string &name) const
{
    if (GetName().compare(name) == 0)
    {
        return true;
    }

    // check base-classes
    boost::shared_ptr<Leaf> classDir = GetCore()->Get("/classes");

    for (
         TStringList::const_iterator i = mBaseClasses.begin();
         i != mBaseClasses.end();
         ++i
         )
    {
        boost::shared_ptr<Class> theClass = static_pointer_cast<Class>
            (GetCore()->Get(*i, classDir));

        if (theClass)
        {
            if (theClass->SupportsClass(name))
            {
                return true;
            }
        }
        else
        {
            cout << "(Class) WARNING: Illegal BaseClass '" << (*i)
                 << "' in Class '" << GetName() << "'" << endl;
        }
    }

    return false;
}

boost::shared_ptr<salt::SharedLibrary> Class::GetBundle() const
{
    return mBundle;
}
