/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: class.cpp,v 1.4 2003/08/27 22:39:24 fruit Exp $

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

Class::Class(const std::string &name) :
Leaf(name)
{
}

Class::~Class()
{
    //cout << "~Class() '" << GetName() << "'\n";

    /*if (GetName().compare("ClassClass") == 0)
      {
      FILE *f = fopen("class.log", "wb");
      fclose(f);
      }*/

    if (mInstances.size() > 0)
    {
        cout << "  Leaked " << mInstances.size() << " instances..." << endl;
        for (TObjectList::iterator i = mInstances.begin(); i != mInstances.end(); ++i)
        {
            // weak_ptr doesn't support get anymore. The correct
            // way to get the address is to use lock() to get
            // a shared_ptr from the weak_ptr
            if (shared_ptr<Object> j = i->lock())
            {
                cout << "    " << j.get() << endl;
            } else {
                cout << "    " << "(expired)" << endl;
            }
        }
    }
}

boost::shared_ptr<Object> Class::Create()
{
    //printf("Class %s: %s\n", GetName().c_str(), name.c_str());
    shared_ptr<Object> obj(CreateInstance());

    if (obj.get())
    {
        if (obj->Construct(obj, shared_static_cast<Class>(make_shared(GetSelf()))) == true)
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
    return make_shared(mCore);
}

void Class::AttachInstance(const boost::weak_ptr<Object> &instance)
{
    mInstances.push_back(instance);
}

void Class::DetachInstance(const boost::weak_ptr<Object> &instance)
{
    // mInstances.remove() doesn't work in this case because
    // operator== is not implemented for weak_ptr
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

Class::TCmdProc Class::GetCmdProc(const std::string &functionName)
{
    TCommandMap::iterator cmd = mFunctions.find(functionName);

    if (cmd != mFunctions.end())
    {
        return (*cmd).second;
    }

    shared_ptr<Leaf> classDir = GetCore()->Get("/classes");
    // ok, we don't have the requested function, so we'll try the base class
    // objects
    for (TStringList::iterator baseClass = mBaseClasses.begin(); baseClass != mBaseClasses.end(); ++baseClass)
    {
        // this should get the base class object (it has to live on the same level of the hierarchy
        // as this class object)
        shared_ptr<Class> theClass = shared_static_cast<Class>(GetCore()->Get(*baseClass, classDir));
        //printf("%x:'%s'.theClass: %s - %d\n", GetCore(), GetName().c_str(), (*baseClass).c_str(), theClass);
        if (theClass)
        {
            // now, we ask the class object, if it knows the command in question
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

bool Class::Supports(const std::string &name) const
{
    if (GetName().compare(name) == 0) return true;

    // check base-classes
    shared_ptr<Leaf> classDir = GetCore()->Get("/classes");

    for (TStringList::const_iterator i = mBaseClasses.begin(); i != mBaseClasses.end(); ++i)
    {
        shared_ptr<Class> theClass = shared_static_cast<Class>(GetCore()->Get(*i, classDir));

        if (theClass)
        {
            if (theClass->Supports(name)) return true;
        }
        else
        {
            cout << "WARNING: Illegal BaseClass '" << (*i) << "' in Class '" << GetName() << "'" << endl;
        }
    }

    return false;
}

void CLASS(Class)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Leaf);
}
