/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: contactjointhandler.cpp,v 1.3 2004/03/30 09:53:37 rollmark Exp $

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
#include "contactjointhandler.h"
#include "collider.h"
#include "world.h"
#include "space.h"

#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

ContactJointHandler::ContactJointHandler() : CollisionHandler()
{
  // set up default contact surface parameters
  mSurfaceParameter.mode = dContactBounce;
  mSurfaceParameter.mu = dInfinity;
  mSurfaceParameter.bounce = 0.8f;
  mSurfaceParameter.bounce_vel = 2.0f;
}

ContactJointHandler::~ContactJointHandler()
{
}

void
ContactJointHandler::HandleCollision(shared_ptr<Collider> collidee, dContact& contact)
{
  if (
      (mCollider.get() == 0) ||
      (mWorld.get() == 0) ||
      (mSpace.get() == 0)
      )
      {
          return;
      }

  // check if the collidee has a ContactJointHandler registered to it
  shared_ptr<Leaf> handler =
      collidee->GetChildSupportingClass("ContactJointHandler");

  if (handler.get() == 0)
      {
          return;
      }

  dBodyID myBody = dGeomGetBody(mCollider->GetODEGeom());
  dBodyID collideeBody = dGeomGetBody(collidee->GetODEGeom());

  // to create a contact joint it we must have at least one body to
  // attach it to.
  if (
      (myBody == 0) &&
      (collideeBody == 0)
      )
    {
      return;
    }

  // fill in the surface parameters
  contact.surface = mSurfaceParameter;

  // create the contact joint and attach it to the body
  dJointID joint = dJointCreateContact
    (mWorld->GetODEWorld(), mSpace->GetODEJointGroup(), &contact);

  dJointAttach (joint, myBody, collideeBody);
 }

void
ContactJointHandler::SetSurfaceParameter(const dSurfaceParameters& surface)
{
  mSurfaceParameter = surface;
}

void
ContactJointHandler::SetContactMode(int mode, bool set)
{
  if (set)
  {
    mSurfaceParameter.mode |= mode;
  } else
  {
    mSurfaceParameter.mode &= ~mode;
  }
}

void
ContactJointHandler::SetContactBounceMode(bool set)
{
  SetContactMode(dContactBounce,set);
}

void
ContactJointHandler::SetMinBounceVel(float vel)
{
  mSurfaceParameter.bounce_vel = std::max<float>(0.0f,vel);
}

void
ContactJointHandler::SetContactSoftERPMode(bool set)
{
  SetContactMode(dContactSoftERP,set);
}

void
ContactJointHandler::SetContactSoftERP(float erp)
{
  salt::gClamp(erp,0.0f,1.0f);
  mSurfaceParameter.soft_erp = erp;
}

void
ContactJointHandler::SetContactSoftCFMMode(bool set)
{
  SetContactMode(dContactSoftCFM,set);
}

void
ContactJointHandler::SetContactSoftCFM(float cfm)
{
  mSurfaceParameter.soft_cfm = std::max<float>(0.0f,cfm);
}

void ContactJointHandler::SetContactSlipMode (bool set)
{
  SetContactMode(dContactSlip1,set);
  SetContactMode(dContactSlip2,set);
}

void ContactJointHandler::SetContactSlip(float slip)
{
  mSurfaceParameter.slip1 = slip;
  mSurfaceParameter.slip2 = slip;
}









