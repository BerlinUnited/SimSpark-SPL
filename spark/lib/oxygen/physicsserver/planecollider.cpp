/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include <oxygen/physicsserver/planecollider.h>

using namespace oxygen;
using namespace salt;

PlaneCollider::PlaneCollider() : Collider()
{
}

void PlaneCollider::SetParams(float a, float b, float c, float d)
{
    // The plane's normal vector is (a,b,c), and it must have length 1
    float l = Vector3f(a,b,c).Length();
    if (l != 0)
        {
            a /= l;
            b /= l;
            c /= l;
            d /= l;
        }

    dGeomPlaneSetParams(mODEGeom, a, b, c, d);
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

void
PlaneCollider::SetParams(const salt::Vector3f& pos, salt::Vector3f normal)
{
    normal.Normalize();
    float d = pos.Dot(normal);
    dGeomPlaneSetParams(mODEGeom, normal.x(), normal.y(), normal.z(), d);
}

void
PlaneCollider::SetPosition(const salt::Vector3f& /*pos*/)
{
    // planes are non placeable geoms
}

void
PlaneCollider::SetRotation(const Matrix& /*rot*/)
{
    // planes are non placeable geoms
}

float
PlaneCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return dGeomPlanePointDepth
        (mODEGeom,worldPos[0],worldPos[1],worldPos[2]);
}



