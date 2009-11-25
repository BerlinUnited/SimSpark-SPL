/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include <oxygen/physicsserver/dragcontroller.h>
#include <oxygen/physicsserver/rigidbody.h>

using namespace oxygen;
using namespace salt;

DragController::DragController()
    : BodyController()
{
    mLinearDrag = 0.1f;
    mAngularDrag = 0.1f;
}

DragController::~DragController()
{
}

void DragController::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (mBody.get() == 0)
    {
        return;
    }

    if (mLinearDrag > 0)
    {
        // apply linear drag
        Vector3f vel = mBody->GetVelocity() * mLinearDrag * -1;
        mBody->AddForce(vel);
    }

    if (mAngularDrag > 0)
    {
        // apply angular drag
        Vector3f vel = mBody->GetAngularVelocity() * mAngularDrag * -1;
        mBody->AddTorque(vel);
    }
}

float DragController::GetLinearDrag() const
{
    return mLinearDrag;
}

void DragController::SetLinearDrag(float d)
{
    mLinearDrag = d;
}

float DragController::GetAngularDrag() const
{
    return mAngularDrag;
}

void DragController::SetAngularDrag(float d)
{
    mAngularDrag = d;
}


