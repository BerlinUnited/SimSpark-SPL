/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collisionhandler.h,v 1.1.2.1 2004/01/12 14:40:34 rollmark Exp $

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
#include <ode/ode.h>

namespace oxygen
{
class Collider;
class World;
class Space;

/** \class CollisionHandler defines an interface for handlers that
    take action in response to a collision. To each Collider instance
    one ore more CollisionHandlers are registered. When the collision
    system detects a collision between two geoms the HandleCollision()
    method of each registered CollisionHandler is called. An example
    implementation is the derived ContactJoint class that constructs
    an ODE contact joint, covering the most common response to a
    collision.
 */
class CollisionHandler : public BaseNode
{
  //
  // Functions
  //
 public:
  CollisionHandler() : BaseNode() {};
  virtual ~CollisionHandler() {};

  /** HandleCollision is called from the Collider to which this
      CollisionHandler is registered to. Derived classes implement
      this callback to react in response to a collision.

       \param collidee is the geom ID of the colliders collision
       partner

       \param holds the contact points between the two affected geoms
       as returned from the ODE dCollide function
   */
  virtual void HandleCollision(dGeomID collidee, dContact& contact) = 0;

 protected:
  virtual void OnLink();
  virtual void OnUnlink();

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
