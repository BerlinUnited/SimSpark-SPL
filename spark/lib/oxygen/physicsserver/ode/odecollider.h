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

#include <oxygen/physicsserver/ode/odephysicsobject.h>
#include <oxygen/physicsserver/int/colliderint.h>

namespace oxygen
{

class OXYGEN_API ODECollider : public ColliderInt, public ODEPhysicsObject
{

public:    
    ODECollider();
    virtual ~ODECollider();

    void OnCollision(boost::shared_ptr<Collider> collidee, dContact& contact, ECollisionType type);
    bool AddCollisionHandler(const std::string& handlerName);
    long GetGeomID();
    void SetPosition(const salt::Vector3f& pos);
    void SetLocalPosition(const salt::Vector3f& pos);
    salt::Vector3f GetPosition() const;
    void SetRotation(const salt::Matrix& rot);
    bool Intersects(boost::shared_ptr<Collider> collider);
    long GetParentSpaceID();
    void AddNotCollideWithColliderName(const std::string& colliderName, bool isAdd);
    const TColliderNameSet& GetNotCollideWithSet() const;
    void OnLink();
    void OnUnlink();
    void PrePhysicsUpdateInternal(float deltaTime);
    void DestroyPhysicsObject();
    
protected:
    dGeomID mODEGeom;
    TColliderNameSet mNotCollideWithSet;
};

} //namespace oxygen

#endif //OXYGEN_ODECOLLIDER_H
