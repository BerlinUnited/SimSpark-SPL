/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: boxcollider.cpp,v 1.3 2004/03/22 10:53:54 rollmark Exp $

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

#include "boxcollider.h"

using namespace oxygen;
using namespace salt;

BoxCollider::BoxCollider() : Collider()
{
}

void
BoxCollider::SetBoxLengths(const Vector3f& extents)
{
    dGeomBoxSetLengths(
                       mODEGeom,
                       extents[0],
                       extents[1],
                       extents[2]
                       );
}

bool
BoxCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
      {
        return false;
      }

    // create a unit box
    mODEGeom = dCreateBox (0, 1.0f, 1.0f, 1.0f);

    return (mODEGeom != 0);
}
