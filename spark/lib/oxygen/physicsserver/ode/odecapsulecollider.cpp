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

#include <oxygen/physicsserver/ode/odewrapper.h>
#include <oxygen/physicsserver/ode/odecapsulecollider.h>

using namespace oxygen;
using namespace salt;

ODECapsuleCollider::ODECapsuleCollider() : ConvexCollider()
{
}

void ODECapsuleCollider::SetParams(float radius, float length, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomCapsuleSetParams (ODEGeom, radius, length);
}

void ODECapsuleCollider::SetRadius(float radius, long geomID)
{
    SetParams(radius, GetLength(geomID), geomID);
}

void ODECapsuleCollider::SetLength(float length, long geomID)
{
    SetParams(GetRadius(geomID), length, geomID);
}

void ODECapsuleCollider::GetParams(float& radius, float& length, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dReal r,l;
    dGeomCapsuleGetParams(ODEGeom,&r,&l);
    radius = r;
    length = l;
}

float ODECapsuleCollider::GetRadius(long geomID)
{
    float length;
    float radius;
    GetParams(radius, length, geomID);
    return radius;
}

float ODECapsuleCollider::GetLength(long geomID)
{
    float radius;
    float length;
    GetParams(radius, length, geomID);
    return length;
}

long ODECapsuleCollider::CreateCapsule()
{
    dGeomID ODEGeom = dCreateCapsule(0, 1.0f, 1.0f);
    return (long) ODEGeom;
}

float ODECapsuleCollider::GetPointDepth(const Vector3f& pos, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    return dGeomCapsulePointDepth
        (ODEGeom,pos[0],pos[1],pos[2]);
}
