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

#include "odeplanecollider.h"

using namespace oxygen;
using namespace salt;

PlaneColliderImp::PlaneColliderImp() : ColliderImp()
{
}

void PlaneColliderImp::SetPlaneParams(float a, float b, float c, float d, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    dGeomPlaneSetParams(ODEGeom, a, b, c, d);
}

long PlaneColliderImp::CreatePlane()
{
    // a plane with normal pointing up, going through the origin
   dGeomID ODEGeom = dCreatePlane(0, 0, 1, 0, 0);
   return (long) ODEGeom;
}

void PlaneColliderImp::SetParams(const salt::Vector3f& pos, salt::Vector3f normal, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    normal.Normalize();
    float d = pos.Dot(normal);
    dGeomPlaneSetParams(ODEGeom, normal.x(), normal.y(), normal.z(), d);
}

float PlaneColliderImp::GetPointDepth(const Vector3f& pos, long geomID)
{
    dGeomID ODEGeom = (dGeomID) geomID;
    return dGeomPlanePointDepth
        (ODEGeom,pos[0],pos[1],pos[2]);
}
