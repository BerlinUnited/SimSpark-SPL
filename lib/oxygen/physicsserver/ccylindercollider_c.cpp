/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: ccylindercollider_c.cpp,v 1.3 2003/08/31 21:53:45 fruit Exp $

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

#include "ccylindercollider.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setParams)
{
        if (in.size() == 2)
        {
                CCylinderCollider *cc = static_cast<CCylinderCollider*>(obj);
                cc->SetParams(any_cast<float>(in[0]),any_cast<float>(in[1]));
        }
}

void CLASS(CCylinderCollider)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/Collider);
        DEFINE_FUNCTION(setParams);
}
