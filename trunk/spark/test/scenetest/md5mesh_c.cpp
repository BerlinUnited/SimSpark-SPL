/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: md5mesh_c.cpp,v 1.3 2004/04/10 11:54:23 fruit Exp $

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
#include "md5mesh.h"

using namespace boost;

FUNCTION(MD5Mesh,load)
{
    std::string name;

    if (in.GetSize() != 1 || ! in.GetValue(in[0], name))
    {
        return false;
    }

    MD5Mesh *m = static_cast<MD5Mesh*>(obj);
    m->Load(name);
    return true;
}

FUNCTION(MD5Mesh,loadAnimation)
{
    std::string name;

    if (in.GetSize() != 1 || ! in.GetValue(in[0], name))
    {
        return false;
    }

    MD5Mesh *m = static_cast<MD5Mesh*>(obj);
    m->LoadAnimation(name);
    return true;
}

void
CLASS(MD5Mesh)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/BaseNode);
    DEFINE_FUNCTION(load);
    DEFINE_FUNCTION(loadAnimation);
}
