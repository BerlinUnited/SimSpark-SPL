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

#ifndef OXYGEN_COLLIDERINT_H
#define OXYGEN_COLLIDERINT_H

#include <oxygen/physicsserver/ode/odewrapper.h>
#include <oxygen/oxygen_defines.h>
#include <set>
#include <string>

namespace oxygen
{
class Collider;

class OXYGEN_API ColliderInt
{

public:    
    enum ECollisionType
    {
         CT_DIRECT,
         CT_SYMMETRIC
    };
     
    typedef std::set<std::string> TColliderNameSet;

    static boost::shared_ptr<Collider> GetCollider(long GeomID){};

    virtual void OnCollision(boost::shared_ptr<Collider> collidee, dContact& contact, ECollisionType type) = 0;
    virtual bool AddCollisionHandler(const std::string& handlerName) = 0;
    virtual long GetGeomID() = 0;
    virtual void SetPosition(const salt::Vector3f& pos) = 0;
    virtual void SetLocalPosition(const salt::Vector3f& pos) = 0;
    virtual salt::Vector3f GetPosition() const = 0;
    virtual void SetRotation(const salt::Matrix& rot) = 0;
    virtual bool Intersects(boost::shared_ptr<Collider> collider) = 0;
    virtual long GetParentSpaceID() = 0;
    virtual void AddNotCollideWithColliderName(const std::string& colliderName, bool isAdd) = 0;
    virtual const TColliderNameSet& GetNotCollideWithSet() const = 0;
    virtual void OnLink() = 0;
    virtual void OnUnlink() = 0;
    virtual void PrePhysicsUpdateInternal(float deltaTime) = 0;
    virtual void DestroyPhysicsObject() = 0;
    
protected:
    long mGeomID;
    TColliderNameSet mNotCollideWithSet;
};

} //namespace oxygen

#endif //OXYGEN_COLLIDERINT_H
