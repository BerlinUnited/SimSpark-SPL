/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint_c.cpp,v 1.3 2004/04/12 19:44:12 rollmark Exp $

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
#include "joint.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

FUNCTION(Joint,attach)
{
    string inPath1;
    string inPath2;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inPath1)) ||
        (! in.GetValue(in[1], inPath2))
        )
        {
            return false;
        }

    obj->Attach(inPath1,inPath2);
    return true;
}


void CLASS(Joint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/ODEObject);
    DEFINE_FUNCTION(attach);
}

