/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#include <oxygen/physicsserver/int/transformcolliderint.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <iostream>

using namespace oxygen;
using namespace salt;
using namespace boost;

boost::shared_ptr<TransformColliderInt> TransformCollider::mTransformColliderImp;

TransformCollider::TransformCollider() : Collider()
{

}

bool TransformCollider::ConstructInternal()
{    
    if (mTransformColliderImp.get() == 0)
        mTransformColliderImp = shared_dynamic_cast<TransformColliderInt>
            (GetCore()->New("TransformColliderImp"));

    if (! Collider::ConstructInternal())
        {
            return false;
        }

    mGeomID = mTransformColliderImp->CreateTransformCollider();

    if (mGeomID == 0)
        {
            return false;
        }

    //! do not automatically destroy encapsulated geoms
    int cleanupSetting = 0;

    /** report the transform geom in collisions instead of the
        encapsulated geoms
    */
    int infoSetting = 1;
    
    mTransformColliderImp->SetColliderParameters(cleanupSetting, infoSetting, mGeomID);

    return true;
}

