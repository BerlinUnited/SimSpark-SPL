/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: capsulecollider.cpp 108 2009-11-25 10:20:10Z a-held $

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

#include <oxygen/physicsserver/ode/odecapsulecollider.h>

using namespace oxygen;
using namespace salt;

ODECapsuleCollider::ODECapsuleCollider() : ODEConvexCollider()
{
}

void ODECapsuleCollider::SetParams(float radius, float length)
{
    dGeomCapsuleSetParams (mODEGeom, radius, length);
}

void ODECapsuleCollider::SetRadius(float radius)
{
    SetParams(radius,GetLength());
}

void ODECapsuleCollider::SetLength(float length)
{
    SetParams(GetRadius(),length);
}

void ODECapsuleCollider::GetParams(float& radius, float& length)
{
    dReal r,l;
    dGeomCapsuleGetParams(mODEGeom,&r,&l);
    radius = r;
    length = l;
}

float ODECapsuleCollider::GetRadius()
{
    float length;
    float radius;
    GetParams(radius,length);
    return radius;
}

float ODECapsuleCollider::GetLength()
{
    float radius;
    float length;
    GetParams(radius,length);
    return length;
}

void ODECapsuleCollider::CreateCapsule()
{
    mODEGeom = dCreateCapsule(0, 1.0f, 1.0f);
    mGeomID = (long) mODEGeom;
}

float ODECapsuleCollider::GetPointDepth(const Vector3f& pos)
{
    return dGeomCapsulePointDepth
        (mODEGeom,pos[0],pos[1],pos[2]);
}

long ODECapsuleCollider::GetGeomID(){
    return mGeomID;
}
