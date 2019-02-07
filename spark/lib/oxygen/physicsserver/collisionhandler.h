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
#ifndef OXYGEN_COLLISIONHANDLER_H
#define OXYGEN_COLLISIONHANDLER_H

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class Collider;
class World;
class Space;
class GenericContact;

/** \class CollisionHandler defines an interface for handlers that
    take action in response to a collision. To each Collider instance
    one ore more CollisionHandlers are registered. When the collision
    system detects a collision between two geoms the HandleCollision()
    method of each registered CollisionHandler is called. An example
    implementation is the derived ContactJoint class that constructs
    an ODE contact joint, covering the most common response to a
    collision.
 */
class OXYGEN_API CollisionHandler : public BaseNode
{
  //
  // Functions
  //
public:
    CollisionHandler() : BaseNode() {};
    virtual ~CollisionHandler() {};

    /** update variables from a script */
    virtual void UpdateCached();

    /** HandleCollision is called from the Collider to which this
        CollisionHandler is registered to. Derived classes implement
        this callback to react in response to a collision.

        \param collidee is the geom ID of the colliders collision
        partner

        \param holds the contact points between the two affected geoms
        as returned from the ODE dCollide function
    */
    virtual void HandleCollision
    (boost::shared_ptr<Collider> collidee, GenericContact& contact) = 0;

    /** returns true if a class of CollisionHandlers wants to handle
        both- a collision and it's symmetric counterpart.

        ODE reports us the set of colliding geoms, for example
        {(a,b),(b,c)} for two collisions between three involved geoms
        a,b and c. For each collision two symmetric pairs exist, for
        example (a,b) and (b,a). ODE consideres both equal and only
        reports one of them.

        For the first pair (a,b) of the example above the
        CollisionHandlers of a are notified about the collision with
        b. In the symmetric case (b,a) the CollisionHandlers of b are
        notified about the collision with a.

        For some CollisionHandler, e.g. the ContactJointHandler this
        behaviour is fine, as it is required to create exactly one
        contact joint between the two colliding geoms. This can be
        done by either one of the two involved
        CollisionHandlers. Therefore the ContactJointHandler is not a
        symmetric handler.

        For other CollisionHandlers, e.g. the CollisionRecorder this
        behaviour does not suffice and both CollisionHandlers must be
        notified if a collision occurs as both involved colliders want
        to record a collision as it happens. In any case the collision
        should be reported to both CollisionHandlers. The
        CollisionRecorder therefore is a symmetric handler.

        The default implementation returns true to indicate a
        symmetric handler.
     */
    virtual bool IsSymmetricHandler() { return true; }

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    void ResetCache();

    //
    // Members
    //
protected:
    /** the collider this CollisionHandler is associated with */
    boost::shared_ptr<Collider> mCollider;

    /** the world this CollisionHandler is associated with */
    boost::shared_ptr<World> mWorld;

    /** the space this CollisionHandler is associated with */
    boost::shared_ptr<Space> mSpace;
};

DECLARE_ABSTRACTCLASS(CollisionHandler);

} //namespace oxygen


#endif // OXYGEN_COLLISIONHANDLER_H
