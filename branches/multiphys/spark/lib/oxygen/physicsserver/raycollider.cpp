/* -*- mode: c++ -*-

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

#include <../plugin/odeimps/oderaycollider.h>
#include <oxygen/physicsserver/raycollider.h>

using namespace oxygen;

boost::shared_ptr<RayColliderInt> RayCollider::mRayColliderImp;

RayCollider::RayCollider() : Collider()
{
    mRayColliderImp = boost::shared_ptr<ODERayCollider>(new ODERayCollider());
}

void RayCollider::SetParams(salt::Vector3f pos,
                            salt::Vector3f dir, float length)
{
    mRayColliderImp->SetParams(pos, dir, length, mGeomID);
}

bool RayCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
    {
        return false;
    }

    // create a unit ray
    mGeomID = mRayColliderImp->CreateRay();

    return (mGeomID != 0);
}
