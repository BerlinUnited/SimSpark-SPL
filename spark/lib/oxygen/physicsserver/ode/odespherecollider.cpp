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

#include <oxygen/physicsserver/ode/odespherecollider.h>

using namespace oxygen;
using namespace salt;

ODESphereCollider::ODESphereCollider() : ODEConvexCollider()
{
    mODEGeom = 0;
    mGeomID = 0;
}

void ODESphereCollider::SetRadius(float r)
{
    dGeomSphereSetRadius(mODEGeom, r);
}

float ODESphereCollider::GetRadius() const
{
    return dGeomSphereGetRadius(mODEGeom);
}

void ODESphereCollider::CreateSphere()
{
    mODEGeom = dCreateSphere(0, 1.0f);
    mGeomID = (long) mODEGeom;
}

float ODESphereCollider::GetPointDepth(const Vector3f& pos)
{
  return dGeomSpherePointDepth
    (mODEGeom,pos[0],pos[1],pos[2]);
}

long ODESphereCollider::GetGeomID(){
    return mGeomID;
}
