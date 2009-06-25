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
#include "zeitgeist.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace zeitgeist;

Zeitgeist::Zeitgeist(string dotName)
{
    ConstructCore();
    RunInitScript(dotName);
}

Zeitgeist::Zeitgeist(string dotName, string relPathPrefix)
{
    ConstructCore();

    if (mCore->GetScriptServer() == 0)
    {
        return;
    }

    mCore->GetScriptServer()->SetInitRelPathPrefix(relPathPrefix);
    RunInitScript(dotName);
}

Zeitgeist::~Zeitgeist()
{
    // this Zeitgeist object owns the only shared_ptr to the
    // core. Class objects only own weak_ptrs to the core. Destructing
    // the core implicitly after this destructor finishes, invalidates
    // our shared_ptr prior to the call to Core::~Core() and all
    // instances are left without a valid core reference on shutdown
    // (calls to OnUnlink). Therefore we destruct the core and the hierarchy explicitly
    // with the mCore reference intact.

    mCore->Desctruct();
}

void Zeitgeist::ConstructCore()
{
    mCore = boost::shared_ptr<Core>(new Core());
    mCore->Construct(mCore);
}

void Zeitgeist::RunInitScript(string dotName)
{
    if (mCore->GetScriptServer() == 0)
        {
            return;
        }

    // setup the dot directory in the script server
    mCore->GetScriptServer()->SetDotName(dotName);

    // run the zeitgeist init script
    mCore->GetScriptServer()->RunInitScript
        (
         "zeitgeist.rb",
         "lib/zeitgeist",
         ScriptServer::IS_COMMON
         );
}


boost::shared_ptr<CoreContext>  Zeitgeist::CreateContext()
{
    return mCore->CreateContext();
}

boost::shared_ptr<Core>& Zeitgeist::GetCore()
{
    return mCore;
}
