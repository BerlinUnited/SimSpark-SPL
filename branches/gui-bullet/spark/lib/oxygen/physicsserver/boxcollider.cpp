/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: boxcollider.cpp 176 2010-02-25 12:19:37Z a-held $

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

#include <oxygen/physicsserver/boxcollider.h>
#include <oxygen/physicsserver/int/boxcolliderint.h>
#include <iostream>

using namespace oxygen;
using namespace salt;
using namespace boost;

boost::shared_ptr<BoxColliderInt> BoxCollider::mBoxColliderImp;

BoxCollider::BoxCollider() : ConvexCollider()
{
 
}      

void BoxCollider::SetBoxLengths(const Vector3f& extents)
{
    mBoxColliderImp->SetBoxLengths(extents, mGeomID);
}

bool BoxCollider::ConstructInternal()
{
    if (mBoxColliderImp.get() == 0)
        mBoxColliderImp = dynamic_pointer_cast<BoxColliderInt>
            (GetCore()->New("BoxColliderImp"));
            
    if (mBoxColliderImp.get() == 0)
        {
            //we can't use the logserver here
            std::cerr << "(BoxCollider) ERROR: Found no implementation at '/classes/BoxColliderImp'";
            return false;
        }

    if (! Collider::ConstructInternal())
        {
            return false;
        }

    // create a unit box
    mGeomID = mBoxColliderImp->CreateBox();

    return (mGeomID != 0);
}

void BoxCollider::GetBoxLengths(Vector3f& extents)
{
    mBoxColliderImp->GetBoxLengths(extents, mGeomID);
}

float BoxCollider::GetBoxLength(int axis)
{
    if (
        (axis<0) ||
        (axis>2)
        )
        {
            return 0;
        }

    Vector3f extents;
    GetBoxLengths(extents);
    return extents[axis];
}

float BoxCollider::GetPointDepth(const Vector3f& pos)
{
    Vector3f worldPos(GetWorldTransform() * pos);
    return mBoxColliderImp->GetPointDepth(worldPos, mGeomID);
}
