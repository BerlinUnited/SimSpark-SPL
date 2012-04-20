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
#include "fontserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace std;

FUNCTION(FontServer,getFont)
{
    string inName;
    int inSize;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inName)) ||
        (! in.GetValue(in[1], inSize))
        )
        {
            return false;
        }

    return (obj->GetFont(inName,inSize).get() != 0);
}

void CLASS(FontServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Leaf);
    DEFINE_FUNCTION(getFont);
}
