/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: raycollider_c.cpp,v 1.3 2004/02/21 15:30:44 fruit Exp $

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

#include "raycollider.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setParams)
{
    if (in.size() == 7)
    {
        RayCollider *rc = static_cast<RayCollider*>(obj);
        rc->SetParams(salt::Vector3f(any_cast<float>(in[0]),
                                     any_cast<float>(in[1]),
                                     any_cast<float>(in[2])),
                      salt::Vector3f(any_cast<float>(in[3]),
                                     any_cast<float>(in[4]),
                                     any_cast<float>(in[5])),
                      any_cast<float>(in[6]));
    }
}

void
CLASS(RayCollider)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/Collider);
        DEFINE_FUNCTION(setParams);
}
