/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: fpscontroller.cpp,v 1.8 2004/04/05 08:47:56 rollmark Exp $

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

FPSController::FPSController() : BodyController()
{
    mHAngle   = 0.0f;
    mVAngle   = 0.0f;
    mForward  = false;
    mBackward = false;
    mLeft     = false;
    mRight    = false;
    mUp       = false;
    mDown     = false;
    mAcceleration = 10;
}

FPSController::~FPSController()
{
}

void FPSController::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (mBody.get() == 0)
        {
            return;
        }

    // determine force direction
    Vector3f vec(0.0f,0.0f,0.0f);

    if (mForward)  vec.z() -= 1.0f;
    if (mBackward) vec.z() += 1.0f;
    if (mRight)    vec.x() += 1.0f;
    if (mLeft)     vec.x() -= 1.0f;
    if (mUp)       vec.y() += 1.0f;
    if (mDown)     vec.y() -= 1.0f;

    // constrain angles
    if(mVAngle > 88.0f)
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

    if (vec.SquareLength() > 0)
        {
            // accelerate the body
            vec.Normalize();
            Vector3f force = matrix.Rotate
                (vec * mBody->GetMass() * mAcceleration);
            mBody->AddForce(force);
        }
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

void FPSController::SetAcceleration(const float accel)
{
    mAcceleration = accel;
}

float FPSController::GetAcceleration() const
{
    return mAcceleration;
}

