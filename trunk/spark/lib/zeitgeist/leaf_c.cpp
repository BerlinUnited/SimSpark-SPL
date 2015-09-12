/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include "leaf.h"
#include "logserver/logserver.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;


FUNCTION(Leaf,setName)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inName))
        )
        {
            return false;
        }

    obj->SetName(inName);
    return true;
}

FUNCTION(Leaf,getName)
{
    return obj->GetName();
}

FUNCTION(Leaf,getFullPath)
{
    return obj->GetFullPath();
}

FUNCTION(Leaf,isLeaf)
{
    return obj->IsLeaf();
}

/* cant use function name '::unlink' because it is already used 
in stdio.h, which is included in some files */
FUNCTION(Leaf,unlinkLeaf)
{
    obj->Unlink();
    return true;
}


void CLASS(Leaf)::DefineClass()
{
    DEFINE_FUNCTION(getFullPath);
    DEFINE_FUNCTION(setName);
    DEFINE_FUNCTION(getName);
    DEFINE_FUNCTION(isLeaf);
    DEFINE_FUNCTION(unlinkLeaf);
}
