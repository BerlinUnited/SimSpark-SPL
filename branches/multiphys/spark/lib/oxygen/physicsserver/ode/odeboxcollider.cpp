/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: boxcollider.cpp 108 2009-11-25 10:20:10Z a-held $

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

#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/ode/odeboxcollider.h>

using namespace oxygen;
using namespace salt;

ODEBoxCollider::ODEBoxCollider() : ODEConvexCollider()
{
}

void ODEBoxCollider::SetBoxLengths(const Vector3f& extents)
{
    dGeomBoxSetLengths(
                       mODEGeom,
                       extents[0],
                       extents[1],
                       extents[2]
                       );
}

long ODEBoxCollider::CreateBox()
{
    mODEGeom = dCreateBox (0, 1.0f, 1.0f, 1.0f);
    return (long) mODEGeom;
}

void ODEBoxCollider::GetBoxLengths(Vector3f& extents)
{
    dVector3 lengths;
    dGeomBoxGetLengths(mODEGeom,lengths);
    extents[0] = lengths[0];
    extents[1] = lengths[1];
    extents[2] = lengths[2];
}

float ODEBoxCollider::GetPointDepth(const Vector3f& pos)
{
    return dGeomBoxPointDepth
        (mODEGeom,pos[0],pos[1],pos[2]);
}
