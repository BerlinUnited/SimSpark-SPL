/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: basenode_c.cpp 179 2010-02-28 01:33:40Z marianbuchta $

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

#include "basenode.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

FUNCTION(BaseNode,importScene)
{
    string inFileName;

    // importScene <fileName> parameter ...

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in[0],inFileName))
        )
        {
            return false;
        }

    boost::shared_ptr<ParameterList> parameter(new ParameterList(in));

    // remove filename argument
    parameter->Pop_Front();

    return obj->ImportScene(inFileName, parameter);
}

void CLASS(BaseNode)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(importScene);
}
