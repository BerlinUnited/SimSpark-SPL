/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: fpscontroller.cpp,v 1.6 2004/03/09 12:13:57 rollmark Exp $

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

#include "fpscontroller.h"
#include <oxygen/physicsserver/body.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

FPSController::FPSController() : BaseNode()
{
    mHAngle   = 0.0f;
    mVAngle   = 0.0f;
    mForward  = false;
    mBackward = false;
    mLeft     = false;
    mRight    = false;
    mUp       = false;
    mDown     = false;
}

FPSController::~FPSController()
{
}

void FPSController::OnLink()
{
    mBody = shared_dynamic_cast<Body>(make_shared(GetParent()));

    if (mBody.get() == 0)
        {
            GetLog()->Error()
                << "(FPSController) ERROR: found no parent body.\n";
        }
}

void FPSController::OnUnlink()
{
    mBody.reset();
}

void FPSController::PrePhysicsUpdateInternal(float deltaTime)
{
    if (mBody.get() == 0)
        {
            return;
        }

    // determine force direction
    Vector3f force(0,0,0);

    if (mForward)  force.z() -= 1.0f;
    if (mBackward) force.z() += 1.0f;
    if (mRight)    force.x() += 1.0f;
    if (mLeft)     force.x() -= 1.0f;
    if (mUp)       force.y() += 1.0f;
    if (mDown)     force.y() -= 1.0f;

    if (force.Length() > 1.0f)
        {
            force.Normalize();
        }

    // constrain angles
    if(mVAngle>88.0f)
        {
            mVAngle = 88.0f;
        } else if(mVAngle<-88.0f)
            {
                mVAngle = -88.0f;
            }

    Matrix matrix;
    matrix.RotationY(gDegToRad(-mHAngle));
    matrix.RotateX(gDegToRad(-mVAngle));
    mBody->SetRotation(matrix);

    force *= 20.0f/deltaTime;
    force = matrix.Rotate(force);
    mBody->AddForce(force);

    float mass = mBody->GetMass();
    Vector3f vel = mBody->GetVelocity();

    mBody->AddForce(
                    Vector3f(
                             -vel[0]*mass/deltaTime,
                             -vel[1]*mass/deltaTime,
                             -vel[2]*mass/deltaTime
                             ));
}

void FPSController::AdjustHAngle(const float delta)
{
    mHAngle += delta;
}

void FPSController::AdjustVAngle(const float delta)
{
    mVAngle += delta;
}

void FPSController::Forward(const bool state)
{
    mForward = state;
}

void FPSController::Backward(const bool state)
{
    mBackward = state;
}

void FPSController::StrafeLeft(const bool state)
{
    mLeft = state;
}

void FPSController::StrafeRight(const bool state)
{
    mRight= state;
}

void FPSController::Up(const bool state)
{
    mUp = state;
}

void FPSController::Down(const bool state)
{
    mDown = state;
}
