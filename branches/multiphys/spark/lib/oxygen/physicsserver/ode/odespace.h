/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: world.h 56 2009-03-17 18:03:47Z hedayat $

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
#ifndef OXYGEN_ODESPACE_H
#define OXYGEN_ODESPACE_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/ode/odephysicsobject.h>
#include <oxygen/physicsserver/int/spaceint.h>

namespace oxygen
{
class Space;

class OXYGEN_API ODESpace : public SpaceInt, public ODEPhysicsObject
{

public:
    ODESpace();
    
    long CreateSpace(long spaceID);
    void DestroySpace(long contactGroup, long spaceID);
    long GetParentSpaceID(long spaceID);
    long ConstructInternal();
    void PostPhysicsUpdateInternal(long contactGroup);
    void Collide(long spaceID, Space* callee);
    void Collide2(long obj1, long obj2, Space* callee);
    bool ObjectIsSpace(long objectID);
    long FetchBody(long geomID);
    long FetchSpace(long geomID);
    bool AreConnectedWithJoint(long bodyID1, long bodyID2);
    void CollideInternal(boost::shared_ptr<Collider> collider, 
                        boost::shared_ptr<Collider> collidee,
                        long geomID1, long geomID2);
    
private:
    static void collisionNearCallback(void* data, dGeomID obj1, dGeomID obj2);
};

}

#endif //OXYGEN_ODESPACE_H
