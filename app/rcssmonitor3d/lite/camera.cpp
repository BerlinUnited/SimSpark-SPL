/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: camera.cpp,v 1.3 2004/06/08 09:22:27 jamu Exp $

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
#include "camera.h"

using namespace salt;

Camera::Camera(const salt::Vector3f& camPos,
               const salt::Vector3f& lookAtPos,
               const salt::Vector3f& upVector)
{
  mPosition  = camPos;
  mLookAtPos = lookAtPos;
  mUpVector  = upVector;

  //resets the camera
  RefreshCam();

  mFixDist = 8.0;
  mMouseSensitivity = 0.01;
}

Vector3f
Camera::GetCameraPos()
{
  return mPosition;
}

Vector3f
Camera::GetLookAtPos()
{
  return mLookAtPos;
}

void
Camera::SetCameraPos(const Vector3f& newCamPos)
{
  mPosition = newCamPos;
  RefreshCam();
}

void
Camera::SetLookAtPos(const Vector3f& lookAtPos)
{
  mLookAtPos = lookAtPos;
  RefreshCam();
}

void
Camera::SetUpVector(const Vector3f& upVector)
{
  mUpVector = upVector;
  RefreshCam();
}

void
Camera::Look()
{
  gluLookAt( mPosition[0], mPosition[1], mPosition[2],
             mLookAtPos[0], mLookAtPos[1], mLookAtPos[2],
             mUpVector[0], mUpVector[1], mUpVector[2]);
}

void
Camera::RefreshCam()
{
  //resets the camera attributes mTheta and mPhi
  Vector3f tmp = Vector3f(mPosition[0],mPosition[1],mPosition[2]).Normalized();
  mTheta     = gPI - acos( tmp.Dot(Vector3f(0.0, 1.0, 0.0)));
  mPhi       = 2*gPI - acos(tmp.Dot(Vector3f(-1.0, 0.0, 0.0)));
}

void
Camera::MoveCamForward(float steps)
{
  //move camera 'steps' meters into direction we are facing
  Vector3f dir = steps*(mLookAtPos - mPosition).Normalized();
  mPosition   += dir;
  mLookAtPos  += dir;
}

void
Camera::MoveCamStrafeForward(float steps)
{
  //move camera 'steps' meters into direction we are facing
  Vector3f dir = steps*(mLookAtPos - mPosition).Normalized();
  //remove z-coord
  dir[2]=0.0f;
  
  mPosition   += dir;
  mLookAtPos  += dir;
}

void Camera::MoveCamUp(float steps)
{
    mPosition[2] += steps;
    mLookAtPos[2] += steps;
}

void
Camera::MoveCamStrafe(float steps)
{
  //move cam perpendicular to our current direction
  Vector3f tmp = (mLookAtPos - mPosition);
  Vector3f dir = steps*(mUpVector.Cross(tmp)).Normalized();
  mPosition   += dir;
  mLookAtPos  += dir;
}

//reset the Camera by taking the difference between the current
//mousePos and the given reference pos, a simple refPos could be the
//middle of the screen
void
Camera::SetViewByMouse(const Vector2f& mousePos,
                       const Vector2f& refPos)
{
  float deltaX = mousePos[0] - refPos[0];
  float deltaY = mousePos[1] - refPos[1];

  mTheta += mMouseSensitivity * deltaY;
  if(mTheta < mMouseSensitivity) mTheta = mMouseSensitivity;
  else if (mTheta >  gPI - mMouseSensitivity) mTheta = gPI - mMouseSensitivity;

  mPhi -= mMouseSensitivity * deltaX;
  if(mPhi < 0) mPhi += 2*gPI;
  else if (mPhi > 2*gPI) mPhi -= 2*gPI;

  mLookAtPos[0] = mPosition[0] + mFixDist * sin(mTheta) * cos(mPhi);
  mLookAtPos[1] = mPosition[1] + mFixDist * sin(mTheta) * sin(mPhi);
  mLookAtPos[2] = mPosition[2] + mFixDist * cos(mTheta);
}

