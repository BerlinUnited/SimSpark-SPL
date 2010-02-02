/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_ODECOLLIDER_H
#define OXYGEN_ODECOLLIDER_H

#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/physicsserver/int/colliderint.h>

namespace oxygen
{
class Collider;

class OXYGEN_API ODECollider : public ColliderInt, public PhysicsObject
{

public:    
    ODECollider();
    virtual ~ODECollider();

    /** Gets the pointer to the Collider object that manages the geom
        specified by geomID.
    */
    Collider* GetColliderPointer(long geomID);

    void SetPosition(const salt::Vector3f& globalPos, long geomID);
    void SetLocalPosition(const salt::Vector3f& pos, long GeomID);
    salt::Vector3f GetPosition(long geomID) const;
    void SetRotation(const salt::Matrix& rot, long GeomID);
    bool Intersect(boost::shared_ptr<Collider> collider, long geomID);
    long GetParentSpaceID(long geomID);
    void DestroyGeom(long GeomID);
    void TransformSetGeom(long parentGeomID, long geomID);
    void SetSpace(long spaceID, long geomID, Collider* collider);
    void SetBody(long bodyID, long geomID);
    void RemoveFromSpace(long geomID, long spaceID);
};

} //namespace oxygen

#endif //OXYGEN_ODECOLLIDER_H
