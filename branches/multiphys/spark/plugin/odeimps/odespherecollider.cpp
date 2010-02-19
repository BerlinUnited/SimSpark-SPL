/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: spherecollider.cpp 108 2009-11-25 10:20:10Z a-held $

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

#include "odespherecollider.h"

using namespace oxygen;
using namespace salt;

ODESphereCollider::ODESphereCollider() : ODEConvexCollider()
{
}

void ODESphereCollider::SetRadius(float r, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomSphereSetRadius(ODEGeom, r);
}

float ODESphereCollider::GetRadius(long geomID) const
{
    dGeomID ODEGeom = (dGeomID) geomID;
    return dGeomSphereGetRadius(ODEGeom);
}

long ODESphereCollider::CreateSphere()
{
    dGeomID ODEGeom = dCreateSphere(0, 1.0f);
    return (long) ODEGeom;
}

float ODESphereCollider::GetPointDepth(const Vector3f& pos, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    return dGeomSpherePointDepth
        (ODEGeom,pos[0],pos[1],pos[2]);
}
