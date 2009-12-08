/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: ccylindercollider.cpp 108 2009-11-25 10:20:10Z a-held $

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

#include <oxygen/physicsserver/capsulecollider.h>

using namespace oxygen;
using namespace salt;

CapsuleCollider::CapsuleCollider() : ConvexCollider()
{
}

void CapsuleCollider::SetParams(float radius, float length)
{
    dGeomCCylinderSetParams (mODEGeom, radius, length);
}

void CapsuleCollider::SetRadius(float radius)
{
    SetParams(radius,GetLength());
}

void CapsuleCollider::SetLength(float length)
{
    SetParams(GetRadius(),length);
}

void CapsuleCollider::GetParams(float& radius, float& length)
{
    dReal r,l;
    dGeomCCylinderGetParams(mODEGeom,&r,&l);
    radius = r;
    length = l;
}

float CapsuleCollider::GetRadius()
{
    float length;
    float radius;
    GetParams(radius,length);
    return radius;
}

float CapsuleCollider::GetLength()
{
    float radius;
    float length;
    GetParams(radius,length);
    return length;
}

bool CapsuleCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
    {
        return false;
    }

    // create a unit capped cylinder
    mODEGeom = dCreateCCylinder(0, 1.0f, 1.0f);

    return (mODEGeom != 0);
}

float CapsuleCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return dGeomCCylinderPointDepth
        (mODEGeom,worldPos[0],worldPos[1],worldPos[2]);
}

