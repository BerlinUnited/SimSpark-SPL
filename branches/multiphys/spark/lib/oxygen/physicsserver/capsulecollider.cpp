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
#include <oxygen/physicsserver/int/capsulecolliderint.h>
#include <oxygen/physicsserver/impfactory.h>

using namespace oxygen;
using namespace salt;

boost::shared_ptr<CapsuleColliderInt> CapsuleCollider::mCapsuleColliderImp;

CapsuleCollider::CapsuleCollider() : ConvexCollider()
{
    mCapsuleColliderImp = ImpFactory::GetInstance()->GetCapsuleColliderImp();
}

void CapsuleCollider::SetParams(float radius, float length)
{
    mCapsuleColliderImp->SetParams(radius, length, mGeomID);
}

void CapsuleCollider::SetRadius(float radius)
{
    mCapsuleColliderImp->SetRadius(radius, mGeomID);
}

void CapsuleCollider::SetLength(float length)
{
    mCapsuleColliderImp->SetLength(length, mGeomID);
}

void CapsuleCollider::GetParams(float& radius, float& length)
{
    mCapsuleColliderImp->GetParams(radius, length, mGeomID);
}

float CapsuleCollider::GetRadius()
{
    return mCapsuleColliderImp->GetRadius(mGeomID);
}

float CapsuleCollider::GetLength()
{
    return mCapsuleColliderImp->GetLength(mGeomID);
}

bool CapsuleCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
    {
        return false;
    }

    mGeomID = mCapsuleColliderImp->CreateCapsule();

    return (mGeomID != 0);
}

float CapsuleCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return mCapsuleColliderImp->GetPointDepth(worldPos, mGeomID);
}
