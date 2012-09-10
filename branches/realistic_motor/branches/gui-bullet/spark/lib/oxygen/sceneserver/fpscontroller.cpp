/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: fpscontroller.cpp 179 2010-02-28 01:33:40Z marianbuchta $

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

#include <oxygen/sceneserver/fpscontroller.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

// define some magic number here for the static fps controller update
// (while the simulation is paused). These should be made
// configurable.n
static const float STATIC_MIN_ANGLE_DELTA = 0.1f;
static const float STATIC_MIN_VELOCITY = 1e-3f;
static const float STATIC_VELOCITY_DECAY = 0.8f;
static const float STATIC_ACCELERATION = 0.01f;

FPSController::FPSController() : BodyController()
{
    mHAngle   = 0.0f;
    mHAngleDelta = 0.0f;
    mVAngle   = 0.0f;
    mVAngleDelta = 0.0f;
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

void
FPSController::PrepareUpdate(Matrix& matrix, Matrix& fwd, Vector3f& vec)
{
    // determine force direction
    vec = Vector3f(0.0f,0.0f,0.0f);

    if (mForward)  vec.y() += 1.0f;
    if (mBackward) vec.y() -= 1.0f;
    if (mRight)    vec.x() += 1.0f;
    if (mLeft)     vec.x() -= 1.0f;
    if (mUp)       vec.z() += 1.0f;
    if (mDown)     vec.z() -= 1.0f;

    // constrain angles
    if(mVAngle > 88.0f)
        {
            mVAngle = 88.0f;
        } else if(mVAngle<-88.0f)
            {
                mVAngle = -88.0f;
            }

    matrix.Identity();
    float hAngle = gDegToRad(-mHAngle);
    float vAngle = gDegToRad(-mVAngle);
    matrix.RotationZ(hAngle);
    matrix.RotateX(vAngle);

    fwd.Identity();
    fwd.RotationZ(hAngle);
}

void FPSController::PrePhysicsUpdateInternal(float deltaTime)
{
    if (mBody.get() == 0)
    {
        return;
    }

    Matrix matrix;
    Matrix fwd;
    Vector3f vec;
    PrepareUpdate(matrix, fwd, vec);

    mBody->SetRotation(matrix);

    if (vec.SquareLength() > 0)
        {
            //Bullet can't disable gravity for single objects, so the camera has a mass of 0
            //Bullet does not support forces for Bodies without mass, so change the position statically
            if(mBody->GetMass()==0){
                UpdateStatic(deltaTime);
            }
            else
            {
                float force = mBody->GetMass() * mAcceleration;
                vec *= force;

                mBody->AddForce(vec.y() * fwd.Up());
                mBody->AddForce(vec.x() * fwd.Right());
                mBody->AddForce(vec.z() * Vector3f(0,0,1));
            }
        }

    mHAngleDelta = 0.0;
    mVAngleDelta = 0.0;
}

bool FPSController::NeedStaticUpdate() const
{
    if (mBody.get() == 0)
        {
            return false;
        }

    return (
            (mForward) ||
            (mBackward) ||
            (mRight) ||
            (mLeft) ||
            (mUp) ||
            (mDown) ||
            (gAbs(mHAngleDelta) >= STATIC_MIN_ANGLE_DELTA) ||
            (gAbs(mVAngleDelta) >= STATIC_MIN_ANGLE_DELTA) ||
            (mBody->GetVelocity().Length() > STATIC_MIN_VELOCITY)
            );
}

void FPSController::UpdateStatic(float deltaTime)
{
    if (mBody.get() == 0)
        {
            return;
        }

    Matrix matrix;
    Matrix fwd;
    Vector3f vec;
    PrepareUpdate(matrix, fwd, vec);

    mBody->SetRotation(matrix);

    Vector3f bodyVel = mBody->GetVelocity() + (deltaTime * vec * STATIC_ACCELERATION);
    bodyVel *= STATIC_VELOCITY_DECAY;

    Vector3f velocity =
        vec.y() * fwd.Up() +
        vec.x() * fwd.Right() +
        vec.z() * Vector3f(0,0,1);

    matrix.Pos() = mBody->GetPosition() + velocity * bodyVel.Length();

    mBody->SetPosition(matrix.Pos());
    mBody->SetVelocity(velocity);

    boost::shared_ptr<BaseNode> bodyParent = shared_static_cast<BaseNode>
        (mBody->GetParent().lock());

    if (bodyParent.get() != 0)
        {
            mBody->SynchronizeParent();
            bodyParent->UpdateHierarchy();
        }

    mHAngleDelta = 0.0;
    mVAngleDelta = 0.0;
}

void FPSController::AdjustHAngle(const float delta)
{
    mHAngle += delta;
    mHAngleDelta += delta;
}

void FPSController::AdjustVAngle(const float delta)
{
    mVAngle += delta;
    mVAngleDelta += delta;
}

void FPSController::SetHAngleDeg(const float angleDeg)
{
    mHAngle = angleDeg;
    mHAngleDelta = 0.0;
}

void FPSController::SetVAngleDeg(const float angleDeg)
{
    mVAngle = angleDeg;
    mVAngleDelta = 0.0;
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

