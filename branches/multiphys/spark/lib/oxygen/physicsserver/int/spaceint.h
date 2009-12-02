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

#ifndef OXYGEN_SPACEINT_H
#define OXYGEN_SPACEINT_H

#include <oxygen/physicsserver/ode/odephysicsobject.h>
#include <set>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class Transform;
class RigidBody;
class Collider;

class OXYGEN_API SpaceInt : public ODEPhysicsObject
{

public:
    typedef std::set<dSpaceID> TSpaceIdSet;
    
    //virtual void collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2) = 0;
    virtual long GetSpaceID() const = 0;
    virtual dJointGroupID GetODEJointGroup() const = 0;
    virtual void Collide() = 0;
    virtual void DestroyPhysicsObject() = 0;
    virtual long GetParentSpaceID() = 0;
    virtual bool IsGlobalSpace() = 0;
    virtual void DisableInnerCollision(bool disable) = 0;
    virtual bool GetDisableInnerCollision() const = 0;
    virtual void OnUnlink() = 0;
    virtual void OnLink() = 0;
    virtual void Collide(dSpaceID space) = 0;
    virtual void HandleCollide(dGeomID obj1, dGeomID obj2) = 0;
    virtual void HandleSpaceCollide(dGeomID obj1, dGeomID obj2) = 0;
    virtual bool ConstructInternal() = 0;
    virtual void PostPhysicsUpdateInternal() = 0;
    virtual void DestroySpaceObjects() = 0;
    
    boost::shared_ptr<Scene> scene;
    
protected:
    SpaceInt() : ODEPhysicsObject(){
    };
};

} //namespace oxygen

#endif //OXYGEN_SPACEINT_H
