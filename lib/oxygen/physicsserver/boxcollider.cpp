/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: boxcollider.cpp,v 1.4 2004/04/15 18:32:24 rollmark Exp $

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

void
BoxCollider::GetBoxLengths(Vector3f& extents)
{
    dVector3 lengths;
    dGeomBoxGetLengths(mODEGeom,lengths);
    extents[0] = lengths[0];
    extents[1] = lengths[1];
    extents[2] = lengths[2];
}

float
BoxCollider::GetBoxLength(int axis)
{
    if (
        (axis<0) ||
        (axis>2)
        )
        {
            return 0;
        }

    Vector3f extents;
    GetBoxLengths(extents);
    return extents[axis];
}

float
BoxCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return dGeomBoxPointDepth
        (mODEGeom,worldPos[0],worldPos[1],worldPos[2]);
}


