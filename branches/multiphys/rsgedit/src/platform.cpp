/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "platform.h"
#include "main.h"

//! wxWidgets and zeitgeist both use a 'DECLARE_CLASS' macro
#undef DECLARE_CLASS

#include <zeitgeist/logserver/logserver.h>
#include "simspark.h"

using namespace boost;
using namespace spark;


wxString Platform::GetDefaultAgentOpenPath()
{
#ifdef WIN32
    return wxT(".");
#else
    return GetHomeDirectory();
#endif
}

wxString Platform::GetDefaultFileOpenPath()
{
#ifdef WIN32
    return wxT("./rsg");
#else
    return GetHomeDirectory();
#endif
}

wxString Platform::GetDefaultAgentOpenWildcard()
{
#ifdef WIN32
    return wxT("agent executables (*.exe)|*.exe");
#else
    return wxT("agent executables|*");
#endif
}

wxString Platform::GetHomeDirectory()
{
    shared_ptr<SimSpark> spark = wxGetApp().GetSpark();
    if (spark.get() == 0)
        {
            return wxT(".");
        }

    const char* envName =
#ifdef WIN32
        "USERPROFILE";
#else
        "HOME";
#endif

    char* home = getenv(envName);

    if (! home)
    {
        spark->GetLog()->Warning() << "(Platform) WARNING: cannot get home directory.\n";
        return wxT(".");
    }

    return wxString(home, wxConvUTF8);
}
