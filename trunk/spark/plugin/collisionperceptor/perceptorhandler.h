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
#ifndef PERCEPTORHANDLER_H
#define PERCEPTORHANDLER_H

#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

class CollisionPerceptor;

/** \class PerceptorHandler is a CollisionHandler that passes
    collision information on to a CollisionPerceptor.
 */
class PerceptorHandler : public oxygen::CollisionHandler
{
 public:
  PerceptorHandler() : CollisionHandler() {};
  virtual ~PerceptorHandler() {};

  /** searches for a CollisionPerceptor below the closest Transform
      node on the way up the hierarchy and notifies it that a
      collision has occured.

       \param collidee is the geom ID of the colliders collision
       partner

       \param holds the contact points between the two affected geoms
       as returned from the ODE dCollide function
  */
  virtual void HandleCollision
  (boost::shared_ptr<oxygen::Collider> collidee, oxygen::GenericContact& contact);

protected:

    virtual void OnLink();
    virtual void OnUnlink();

protected:

    //! reference to the collision perecptor
    boost::shared_ptr<CollisionPerceptor> mColPercept;
};

DECLARE_CLASS(PerceptorHandler);

#endif // PERCEPTORHANDLER_H
