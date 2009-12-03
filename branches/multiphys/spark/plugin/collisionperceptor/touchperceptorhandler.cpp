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

#include "touchperceptorhandler.h"
#include <oxygen/physicsserver/collider.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>
#include "forceresistanceperceptor.h"

using namespace oxygen;
using namespace boost;

void TouchPerceptorHandler::OnLink()
{
	ContactJointHandler::OnLink();

    // find the first CollisionPerceptor below our closest Transform node
    shared_ptr<Transform> transformParent = shared_static_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());

    if (transformParent.get() == 0)
    {
        return;
    }

    mForceResistancePercept = shared_dynamic_cast<ForceResistancePerceptor>(
            transformParent->GetChildSupportingClass("ForceResistancePerceptor",
                    true));

    if (mForceResistancePercept.get() == 0)
        GetLog()->Error() << "TouchPerceptorHandler: no suitable child node found!\n";
}

void TouchPerceptorHandler::OnUnlink()
{
	ContactJointHandler::OnUnlink();
	mForceResistancePercept.reset();
}

void TouchPerceptorHandler::HandleCollision(
        boost::shared_ptr<Collider> collidee, dContact& contact)
{
    if (mCollider.get() == 0 || mWorld.get() == 0 || mSpace.get() == 0)
        return;

    // to create a contact joint it we must have at least one body to
    // attach it to.
    dBodyID myBody = dGeomGetBody((dGeomID) mCollider->GetGeomID());
    dBodyID collideeBody = dGeomGetBody((dGeomID) collidee->GetGeomID());

    if (myBody == 0 && collideeBody == 0)
        return;

    shared_ptr<ContactJointHandler> handler =
        collidee->FindChildSupportingClass<ContactJointHandler>();

    if (handler.get() == 0)
        return;

    CalcSurfaceParam(contact.surface,handler->GetSurfaceParameter());

    dJointID joint = dJointCreateContact((dWorldID) mWorld->GetWorldID(),
            mSpace->GetODEJointGroup(), &contact);

    dJointAttach (joint, myBody, collideeBody);

    if (mForceResistancePercept.get() != 0)
        {
            dJointFeedback *feedback =
                    mForceResistancePercept->AddTouchInfo(contact);
            dJointSetFeedback(joint, feedback);
        }
}
