/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: contactjointhandler.cpp,v 1.6 2004/04/05 14:51:09 rollmark Exp $

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

float
ContactJointHandler::MixValues(const float v1, const float v2, const int n) const
{
    switch(n)
        {
        default:
        case 0:
            // undefined, default 0
            return 0.0f;

        case 1:
            // first one defined
            return v1;

        case 2:
            // second one defined
            return v2;

        case 3:
            // both defined, return average
            return (v1 + v2) / 2.0f;
        }
}

void
ContactJointHandler::CalcSurfaceParam(dSurfaceParameters& surface,
                                      const dSurfaceParameters& collideeParam)
{
    // init surface
    surface.mode = 0;

    // calculate average mu; mu can be dInfinity, so first multiply with
    // 0.5 and the sum up to avoid a range error
    surface.mu = mSurfaceParameter.mu*0.5f + collideeParam.mu*0.5f;

    // soft cfm
    const int nCfm =
        ((mSurfaceParameter.mode & dContactSoftCFM) ? 1:0) +
        ((collideeParam.mode & dContactSoftCFM) ? 2:0);

    if (nCfm>0)
        {
            surface.soft_cfm = MixValues
                (mSurfaceParameter.soft_cfm, collideeParam.soft_cfm, nCfm);
            surface.mode |= dContactSoftCFM;
        }

    // soft_erp
    const int nErp =
        ((mSurfaceParameter.mode & dContactSoftERP) ? 1:0) +
        ((collideeParam.mode & dContactSoftERP) ? 2:0);

    if (nErp>0)
        {
            surface.soft_erp = MixValues
                (mSurfaceParameter.soft_erp, collideeParam.soft_erp, nErp);
            surface.mode |= dContactSoftERP;
        }

    // bounce
    const int nBounce =
        ((mSurfaceParameter.mode & dContactBounce) ? 1:0) +
        ((collideeParam.mode & dContactBounce) ? 2:0);

    if (nBounce>0)
        {
            surface.bounce = MixValues
                (mSurfaceParameter.bounce, collideeParam.bounce, nBounce);

            surface.bounce_vel = MixValues
                (mSurfaceParameter.bounce_vel, collideeParam.bounce_vel, nBounce);

            surface.mode |= dContactBounce;
        }
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
    shared_ptr<ContactJointHandler> handler =
        collidee->FindChildSupportingClass<ContactJointHandler>();

    if (handler.get() == 0)
        {
            return;
        }

    // to create a contact joint it we must have at least one body to
    // attach it to.
    dBodyID myBody = dGeomGetBody(mCollider->GetODEGeom());
    dBodyID collideeBody = dGeomGetBody(collidee->GetODEGeom());

    if (
        (myBody == 0) &&
        (collideeBody == 0)
        )
        {
            return;
        }

    // calculate the resulting surface parameters
    CalcSurfaceParam(contact.surface,handler->mSurfaceParameter);

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
ContactJointHandler::SetBounceValue(float bounce)
{
    mSurfaceParameter.bounce = std::max<float>(0.0f,bounce);
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









