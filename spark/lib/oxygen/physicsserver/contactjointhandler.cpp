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
#include <oxygen/physicsserver/contactjointhandler.h>
#include <../plugin/odeimps/odecontactjointhandler.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

boost::shared_ptr<ContactJointHandlerInt> ContactJointHandler::mContactJointHandlerImp;

ContactJointHandler::ContactJointHandler() : CollisionHandler()
{
    mContactJointHandlerImp = boost::shared_ptr<ODEContactJointHandler>(new ODEContactJointHandler());
    mSurfaceParameter = mContactJointHandlerImp->Initialize();
}

ContactJointHandler::~ContactJointHandler()
{
    delete mSurfaceParameter;
}

void
ContactJointHandler::HandleCollision(shared_ptr<Collider> collidee, GenericContact& contact)
{
    if (
        (mCollider.get() == 0) ||
        (mWorld.get() == 0) ||
        (mSpace.get() == 0)
        )
        {
            return;
        }

    // to create a contact joint it we must have at least one body to
    // attach it to.
    long myBody = mContactJointHandlerImp->RetrieveBody(mCollider->GetGeomID());
    long collideeBody = mContactJointHandlerImp->RetrieveBody(collidee->GetGeomID());

    if (
        (myBody == 0) &&
        (collideeBody == 0)
        )
        {
            return;
        }

    // check if the collidee has a ContactJointHandler registered to it
    shared_ptr<ContactJointHandler> handler =
        collidee->FindChildSupportingClass<ContactJointHandler>();

    if (handler.get() == 0)
        {
            return;
        }

    // calculate the resulting surface parameters    
    mContactJointHandlerImp->CalcSurfaceParam(
        contact, handler->GetSurfaceParameter(), mSurfaceParameter);

    // create the contact joint and attach it to the body
    long joint = mContactJointHandlerImp->CreateContactJoint(
        mWorld->GetWorldID(), mSpace->GetODEJointGroup(), contact);

    mContactJointHandlerImp->AttachContactJoint(joint, myBody, collideeBody);
}

void
ContactJointHandler::SetSurfaceParameter(const GenericSurfaceParameter& surface)
{
    GenericSurfaceParameter* newSurface = (GenericSurfaceParameter*) &surface;
    mSurfaceParameter = newSurface;
}

GenericSurfaceParameter&
ContactJointHandler::GetSurfaceParameter() const
{
    return (GenericSurfaceParameter&) *mSurfaceParameter;
}

int
ContactJointHandler::GetContactMode() const
{
    return mContactJointHandlerImp->GetContactMode(mSurfaceParameter);
}

void
ContactJointHandler::SetContactBounceMode(bool set)
{
    mContactJointHandlerImp->SetContactBounceMode(set, mSurfaceParameter);
}

void
ContactJointHandler::SetMinBounceVel(float vel)
{
    mContactJointHandlerImp->SetMinBounceVel(vel, mSurfaceParameter);
}

float
ContactJointHandler::GetMinBounceVel() const
{
    return mContactJointHandlerImp->GetMinBounceVel(mSurfaceParameter);
}

void
ContactJointHandler::SetBounceValue(float bounce)
{
    mContactJointHandlerImp->SetBounceValue(bounce, mSurfaceParameter);
}

float
ContactJointHandler::GetBounceValue() const
{
    return mContactJointHandlerImp->GetBounceValue(mSurfaceParameter);
}

void
ContactJointHandler::SetContactSoftERPMode(bool set)
{
    mContactJointHandlerImp->SetContactSoftERPMode(set, mSurfaceParameter);
}

void
ContactJointHandler::SetContactSoftERP(float erp)
{
    mContactJointHandlerImp->SetContactSoftERP(erp, mSurfaceParameter);
}

float
ContactJointHandler::GetContactSoftERP() const
{
    return mContactJointHandlerImp->GetContactSoftERP(mSurfaceParameter);
}

void
ContactJointHandler::SetContactSoftCFMMode(bool set)
{
    mContactJointHandlerImp->SetContactSoftCFMMode(set, mSurfaceParameter);
}

void
ContactJointHandler::SetContactSoftCFM(float cfm)
{
    mContactJointHandlerImp->SetContactSoftCFM(cfm, mSurfaceParameter);
}

float ContactJointHandler::GetContactSoftCFM() const
{
    return mContactJointHandlerImp->GetContactSoftCFM(mSurfaceParameter);
}

void ContactJointHandler::SetContactSlipMode (bool set)
{
    mContactJointHandlerImp->SetContactSlipMode(set, mSurfaceParameter);
}

void ContactJointHandler::SetContactSlip(float slip)
{
    mContactJointHandlerImp->SetContactSlip(slip, mSurfaceParameter);
}

float
ContactJointHandler::GetContactSlip1() const
{
    return mContactJointHandlerImp->GetContactSlip1(mSurfaceParameter);
}

float
ContactJointHandler::GetContactSlip2() const
{
    return mContactJointHandlerImp->GetContactSlip2(mSurfaceParameter);
}

void ContactJointHandler::SetContactMu(float mu)
{
    mContactJointHandlerImp->SetContactMu(mu, mSurfaceParameter);
}

float ContactJointHandler::GetContactMu() const
{
    return mContactJointHandlerImp->GetContactMu(mSurfaceParameter);
}
