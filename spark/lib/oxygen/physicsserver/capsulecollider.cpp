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

#include <oxygen/physicsserver/capsulecollider.h>
#include <oxygen/physicsserver/ode/odecapsulecollider.h>

using namespace oxygen;
using namespace salt;

CapsuleCollider::CapsuleCollider() : ConvexCollider()
{
    mCapsuleColliderImp = boost::shared_ptr<ODECapsuleCollider>(new ODECapsuleCollider());
}

void CapsuleCollider::SetParams(float radius, float length)
{
    mCapsuleColliderImp->SetParams(radius, length);
}

void CapsuleCollider::SetRadius(float radius)
{
    mCapsuleColliderImp->SetRadius(radius);
}

void CapsuleCollider::SetLength(float length)
{
    mCapsuleColliderImp->SetLength(length);
}

void CapsuleCollider::GetParams(float& radius, float& length)
{
    mCapsuleColliderImp->GetParams(radius, length);
}

float CapsuleCollider::GetRadius()
{
    return mCapsuleColliderImp->GetRadius();
}

float CapsuleCollider::GetLength()
{
    return mCapsuleColliderImp->GetLength();
}

bool CapsuleCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
    {
        return false;
    }

    mCapsuleColliderImp->CreateCapsule();
    mODEGeom = (dGeomID) mCapsuleColliderImp->GetGeomID();

    return (mCapsuleColliderImp->GetGeomID() != 0);
}

float CapsuleCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return mCapsuleColliderImp->GetPointDepth(worldPos);
}
