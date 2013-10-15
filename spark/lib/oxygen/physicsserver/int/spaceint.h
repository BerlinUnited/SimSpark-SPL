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
    virtual ~SpaceInt() {}

    virtual long CreateSpace(long spaceID) = 0;

    /** Destroys the space specified by \param spaceID and the
        contact group specified by \param contactGroup
    */
    virtual void DestroySpace(long contactGroup, long spaceID) = 0;

    /** returns the ID of the containing parent space */
    virtual long GetParentSpaceID(long spaceID) = 0;

    /** calls collision detection for this space if internal collision
        detection is enabled for this space.
     */
    virtual void Collide(long space, Space* callee) = 0;

    /** collide all geoms internal to the space \param callee */
    virtual void Collide2(long obj1, long obj2, Space* callee) = 0;

    /** Creates a Contact Group and returns its ID */
    virtual long CreateContactGroup() = 0;

    /** updates internal state after physics calculation */
    virtual void PostPhysicsUpdateInternal(long contactGroup) = 0;

    /** Returns true if the physics object specified
        by \param objectID is a space
    */
    virtual bool ObjectIsSpace(long objectID) = 0;

    /** Retrieves the ID of the body that the geom specified
        by \param geomID is linked with
    */
    virtual long FetchBody(long geomID) = 0;

    /** Retrieves the ID of the space that the geom specified
        by \param geomID is inside of
    */
    virtual long FetchSpace(long geomID) = 0;

    /** Returns true if the two bodies specified by \param bodyID1
        and \param bodyID2 are connected with a joint
    */
    virtual bool AreConnectedWithJoint(long bodyID1, long bodyID2) = 0;

    /** Collides the two geoms managed by \param collider
        and \param collidee */
    virtual void CollideInternal(boost::shared_ptr<Collider> collider,
                                boost::shared_ptr<Collider> collidee,
                                long geomID1, long geomID2) = 0;
};

} //namespace oxygen

#endif //OXYGEN_SPACEINT_H
