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


// check if there's a separating plane in between the selected axes
bool BoxCollider::CheckSeparatingPlane(const Vector3f& RPos, const Vector3f& Plane, boost::shared_ptr<BoxCollider> box2, float tol)
{
    Vector3f halfExtentsB1, halfExtentsB2;

    this->GetBoxLengths(halfExtentsB1);
    halfExtentsB1 *= 0.5;
    box2->GetBoxLengths(halfExtentsB2);
    halfExtentsB2 *= 0.5;

    return (fabs(RPos.Dot(Plane)) + tol > 
             (fabs((this->GetWorldTransform().Right()   * halfExtentsB1.x()).Dot(Plane) ) +
              fabs((this->GetWorldTransform().Up()      * halfExtentsB1.y()).Dot(Plane) ) +
              fabs((this->GetWorldTransform().Forward() * halfExtentsB1.z()).Dot(Plane) ) +
              fabs((box2->GetWorldTransform().Right()   * halfExtentsB2.x()).Dot(Plane) ) + 
              fabs((box2->GetWorldTransform().Up()      * halfExtentsB2.y()).Dot(Plane) ) +
              fabs((box2->GetWorldTransform().Forward() * halfExtentsB2.z()).Dot(Plane) )));
}

bool BoxCollider::CheckCollisions( boost::shared_ptr<BoxCollider> box2, float tol)
{
    Vector3f RPos;

    RPos = box2->GetWorldTransform().Pos() - this->GetWorldTransform().Pos();

    return !(CheckSeparatingPlane(RPos, this->GetWorldTransform().Right()                                                 , box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Up()                                                    , box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Forward()                                               , box2, tol) ||
             CheckSeparatingPlane(RPos, box2->GetWorldTransform().Right()                                                 , box2, tol) ||
             CheckSeparatingPlane(RPos, box2->GetWorldTransform().Up()                                                    , box2, tol) ||
             CheckSeparatingPlane(RPos, box2->GetWorldTransform().Forward()                                               , box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Right()    .Cross(  box2->GetWorldTransform().Right()  ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Right()    .Cross(  box2->GetWorldTransform().Up()     ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Right()    .Cross(  box2->GetWorldTransform().Forward()), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Up()       .Cross(  box2->GetWorldTransform().Right()  ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Up()       .Cross(  box2->GetWorldTransform().Up()     ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Up()       .Cross(  box2->GetWorldTransform().Forward()), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Forward()  .Cross(  box2->GetWorldTransform().Right()  ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Forward()  .Cross(  box2->GetWorldTransform().Up()     ), box2, tol) ||
             CheckSeparatingPlane(RPos, this->GetWorldTransform().Forward()  .Cross(  box2->GetWorldTransform().Forward()), box2, tol));
}

void:: BoxCollider::AddSCFreezeJointEffName(const std::string name)
{
    selfCollisionFreezeJointEffNames.push_back(name);
}

const std::list<std::string> BoxCollider::GetSCFreezeJointEffNames()
{
    return selfCollisionFreezeJointEffNames;
}

