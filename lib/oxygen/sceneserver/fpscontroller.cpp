/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: fpscontroller.cpp,v 1.4.8.1 2004/01/20 19:03:08 rollmark Exp $

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

using namespace boost;
using namespace oxygen;
using namespace salt;

FPSController::FPSController() : BaseNode()
{
  mHAngle = 0.0f;
  mVAngle = 0.0f;
  mForward = false;
  mBackward = false;
  mLeft = false;
  mRight = false;
  mUp = false;
  mDown = false;
}

FPSController::~FPSController()
{
}

void FPSController::PrePhysicsUpdateInternal(float deltaTime)
{
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

  shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent()));

  if (body.get() != NULL)
    {
      // constrain angles
      if(mVAngle>88.0f)
        {
          mVAngle = 88.0f;
        }
      else if(mVAngle<-88.0f)
        {
          mVAngle = -88.0f;
        }

      Matrix matrix;
      matrix.RotationY(gDegToRad(-mHAngle));
      matrix.RotateX(gDegToRad(-mVAngle));

      dMatrix3 ODEMatrix;
      dRSetIdentity(ODEMatrix);
      ODEMatrix[0]=matrix.m[0];
      ODEMatrix[4]=matrix.m[1];
      ODEMatrix[8]=matrix.m[2];
      ODEMatrix[1]=matrix.m[4];
      ODEMatrix[5]=matrix.m[5];
      ODEMatrix[9]=matrix.m[6];
      ODEMatrix[2]=matrix.m[8];
      ODEMatrix[6]=matrix.m[9];
      ODEMatrix[10]=matrix.m[10];
      dBodySetRotation(body->GetODEBody(), ODEMatrix);

      force *= 20.0f/deltaTime;
      force = matrix.Rotate(force);
      dBodyAddForce(body->GetODEBody(), force.x(), force.y(), force.z());

      //force = - drag_coefficient * mass * velocity
      dMass m;
      dBodyGetMass(body->GetODEBody(), &m);
      const dReal* vel = dBodyGetLinearVel(body->GetODEBody());

      dBodyAddForce(
                    body->GetODEBody(),
                    -vel[0]*m.mass/deltaTime,
                    -vel[1]*m.mass/deltaTime,
                    -vel[2]*m.mass/deltaTime
                    );
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
{       mBackward = state;
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


