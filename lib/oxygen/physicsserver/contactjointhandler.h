/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: contactjointhandler.h,v 1.2 2004/02/12 14:07:23 fruit Exp $

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
#ifndef OXYGEN_CONTACTJOINTHANDLER_H
#define OXYGEN_CONTACTJOINTHANDLER_H

#include "collisionhandler.h"

namespace oxygen
{

/** \class ContactJointHandler is a CollisionHandler that creates an
    ODE contact joint between the two bodies associated with the two
    affected collision geoms.
 */
class ContactJointHandler : public CollisionHandler
{
 public:
  ContactJointHandler();
  virtual ~ContactJointHandler();

  /** Check if the collidee also has a ContactJoint handler registered
      to it. If yes, create a contact joint between the bodies
      corresponding to our Collider and the collidee's geom using the
      surface parameters stored in the mSurfaceParameter member.

       \param collidee is the geom ID of the colliders collision
       partner

       \param holds the contact points between the two affected geoms
       as returned from the ODE dCollide function
  */
  virtual void HandleCollision
  (boost::shared_ptr<Collider> collidee, dContact& contact);

  /** sets the surface parameters for the contact joints that the
      CollisionHandler creates
  */
  virtual void SetSurfaceParameter(const dSurfaceParameters& surface);

  /** the ContactJointHandler is not a symmetric handler. See
      CollisionHandler::IsSymmetricHandler for an explanation
  */
  virtual bool IsSymmetricHandler() { return false; }

 protected:
    /** the ODE surface parameters of the created contact joint */
    dSurfaceParameters mSurfaceParameter;
};

DECLARE_CLASS(ContactJointHandler);

} //namespace oxygen


#endif // OXYGEN_CONTACTJOINTHANDLER_H
