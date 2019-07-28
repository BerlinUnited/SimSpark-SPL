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
#include "object.h"
#include "class.h"
#include "core.h"
#include "leaf.h"
#include <iostream>
#ifndef Q_MOC_RUN
#include <boost/version.hpp>
#endif

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
    if (mClass.get() == 0)
    {
        const Leaf* leaf = dynamic_cast<const Leaf*>(this);

        if (leaf != 0)
        {
            if (leaf->GetFullPath() != "ClassClass")
            {
                std::cerr << "(Object) ERROR: failed to get Class object "
                          << "for '" << leaf->GetName()
                          << "' installed at '"
                          << leaf->GetFullPath() << "'\n";
            }
        } else {
            std::cerr << "(Object) ERROR: failed to get Class object for unknown\n";
        }
    }
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
    boost::shared_ptr<Core> core = mClass->GetCore();

    if (core.get() == 0)
      {
          std::cout << "(Object) ERROR: failed to get zeitgeist core ";
          const Leaf* leaf = dynamic_cast<const Leaf*>(this);

          if (leaf != 0)
              {
                  std::cout << "for '" << leaf->GetName()
                            << "' installed at '"
                            << leaf->GetFullPath()
                            << "'";
              }

          std::cout << std::endl;
      }

    return mClass->GetCore();
}

void Object::Dump() const
{
    cout << "Object: " << (void*) this;
    if (boost::shared_ptr<Object> self = mSelf.lock())
    {
        cout << " " << (void*) self.get();
    } else {
        cout << " " << "(expired)";
    }
    cout << " - use count = " << mSelf.use_count() << endl;
}

void Object::Invoke(const std::string &functionName, const ParameterList& parameter)
{
    Class::TCmdProc cmd = mClass->GetCmdProc(functionName);
    if (cmd != NULL)
      {
        if (boost::shared_ptr<Object> self = GetSelf().lock())
          {
            GCValue out = cmd(self.get(), parameter);
          }
      }
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
