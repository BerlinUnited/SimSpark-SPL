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

#include <set>
#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class Transform;
class RigidBody;
class Collider;
class Space;

class OXYGEN_API SpaceInt
{

public:
    virtual long CreateSpace(long spaceID) = 0;
    virtual void DestroySpace(long contactGroup, long spaceID) = 0;
    virtual long GetParentSpaceID(long spaceID) = 0;
    virtual void Collide(long space, Space* callee) = 0;
    virtual void Collide2(long obj1, long obj2, Space* callee) = 0; 
    virtual long CreateContactGroup() = 0;
    virtual void PostPhysicsUpdateInternal(long contactGroup) = 0;
    virtual bool ObjectIsSpace(long objectID) = 0;
    virtual long FetchBody(long geomID) = 0;
    virtual long FetchSpace(long geomID) = 0;
    virtual bool AreConnectedWithJoint(long bodyID1, long bodyID2) = 0;
    virtual void CollideInternal(boost::shared_ptr<Collider> collider, 
                                boost::shared_ptr<Collider> collidee,
                                long geomID1, long geomID2) = 0;
};

} //namespace oxygen

#endif //OXYGEN_SPACEINT_H
