/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider.cpp,v 1.3.8.1 2004/01/11 11:59:53 rollmark Exp $

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

#include "planecollider.h"

using namespace oxygen;

PlaneCollider::PlaneCollider() :
Collider()
{
}

void PlaneCollider::SetParams(float a, float b, float c, float d)
{
    if (mODEGeom)
      {
        dGeomPlaneSetParams(mODEGeom, a, b, c, d);
      }
}

bool PlaneCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
      {
        return false;
      }

    // a plane with normal pointing up, going through the origin
    mODEGeom = dCreatePlane(0, 0, 1, 0, 0);

    return (mODEGeom != 0);
}
