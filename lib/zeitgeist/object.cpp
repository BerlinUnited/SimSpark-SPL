/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: object.cpp,v 1.4 2003/08/27 22:44:27 fruit Exp $

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
#include "object.h"
#include "class.h"
#include "core.h"
#include <iostream>
#include <boost/version.hpp>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Object::Object()
{
}

Object::~Object()
{
    if (mClass.get())
    {
        mClass->DetachInstance(GetSelf());
    }
}

bool Object::Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator)
{
    mSelf       = self;
    mClass      = creator;

    // give each object a chance to do something
    return ConstructInternal();
}

boost::shared_ptr<Class> Object::GetClass() const
{
    return mClass;
}

boost::weak_ptr<Object>& Object::GetSelf()
{
    return mSelf;
}

const boost::weak_ptr<Object>& Object::GetSelf() const
{
    return mSelf;
}

boost::shared_ptr<Core> Object::GetCore() const
{
    assert(mClass.get() != NULL);
    return mClass->GetCore();
}

void Object::Dump() const
{
    cout << "Object: " << (void*) this;
    if (shared_ptr<Object> self = mSelf.lock())
    {
        cout << " " << (void*) self.get();
    } else {
        cout << " " << "(expired)";
    }
    cout << " - use count = " << mSelf.use_count() << endl;
}

void Object::Invoke(const std::string &functionName)
{
    Class::TParameterList in;

    Class::TCmdProc cmd = mClass->GetCmdProc(functionName);
    if (cmd != NULL)
        if (shared_ptr<Object> self = GetSelf().lock())
            cmd(self.get(), in);
}

const boost::shared_ptr<FileServer>& Object::GetFile() const
{
    return GetCore()->GetFileServer();
}

const boost::shared_ptr<LogServer>& Object::GetLog() const
{
    return GetCore()->GetLogServer();
}

const boost::shared_ptr<ScriptServer>& Object::GetScript() const
{
    return GetCore()->GetScriptServer();
}

bool Object::ConstructInternal()
{
    return true;
}
