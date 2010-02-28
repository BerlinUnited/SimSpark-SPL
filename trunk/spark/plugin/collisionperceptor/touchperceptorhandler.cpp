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
#include <oxygen/physicsserver/int/contactjointhandlerint.h>
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
    boost::shared_ptr<Transform> transformParent = shared_static_cast<Transform>
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
        boost::shared_ptr<Collider> collidee, GenericContact& contact)
{
    if (mCollider.get() == 0 || mWorld.get() == 0 || mSpace.get() == 0)
        return;

    // to create a contact joint we must have at least one body to
    // attach it to.
    long myBody = mContactJointHandlerImp->RetrieveBody(mCollider->GetGeomID());
    long collideeBody = mContactJointHandlerImp->RetrieveBody(collidee->GetGeomID());

    if (myBody == 0 && collideeBody == 0)
        return;

    boost::shared_ptr<ContactJointHandler> handler =
        collidee->FindChildSupportingClass<ContactJointHandler>();

    if (handler.get() == 0)
        return;

    mContactJointHandlerImp->CalcSurfaceParam(
        contact, handler->GetSurfaceParameter(), mSurfaceParameter);

    // create the contact joint and attach it to the body
    long joint = mContactJointHandlerImp->CreateContactJoint(
        mWorld->GetWorldID(), mSpace->GetODEJointGroup(), contact);

    mContactJointHandlerImp->AttachContactJoint(joint, myBody, collideeBody);
    
    if (mForceResistancePercept.get() != 0)
        {
            GenericJointFeedback* feedback =
                    mForceResistancePercept->AddTouchInfo(contact);
            dJointSetFeedback( (dJointID) joint, (dJointFeedback*) feedback);
        }
}
