#include "camera.h"

Camera::Camera(const salt::Vector3f& camPos,
	       const salt::Vector3f& lookAtPos,
	       const salt::Vector3f& upVector)
{
  mPosition  = camPos;
  mLookAtPos = lookAtPos;
  mUpVector  = upVector;

  //be aware of how to choose 
  //the initial theta/phi
  Vector3f tmp = Vector3f(camPos[0],camPos[1],camPos[2]).Normalized(); //camPos.Normalized();
  mTheta     = PI - acos( tmp.Dot(salt::Vector3f(0.0, 1.0, 0.0)));
  mPhi       = 2*PI - acos(tmp.Dot(salt::Vector3f(-1.0, 0.0, 0.0)));
  
  mFixDist = 8.0;
  mMouseSensitivity = 0.01;
}

Vector3f
Camera::GetCameraPos()
{
  return mPosition;
}

void 
Camera::SetCameraPos(const salt::Vector3f& newCamPos)
{
  mPosition = newCamPos;
}

void
Camera::SetLookAtPos(const salt::Vector3f& lookAtPos)
{
  mLookAtPos = lookAtPos;
}

void
Camera::SetUpVector(const salt::Vector3f& upVector)
{
  mUpVector = upVector;
} 

void
Camera::Look()
{
  gluLookAt( mPosition[0], mPosition[1], mPosition[2], 
	     mLookAtPos[0], mLookAtPos[1], mLookAtPos[2],
	     mUpVector[0], mUpVector[1], mUpVector[2]);
}

//reset the Camera by taking the difference
//between the current mousePos and the given 
//reference pos, a simple refPos could be the 
//middle of the screen
void
Camera::SetViewByMouse(const salt::Vector2f& mousePos,
		       const salt::Vector2f& refPos)
{
  float deltaX = mousePos[0] - refPos[0];
  float deltaY = mousePos[1] - refPos[1];

  mTheta += mMouseSensitivity * deltaY;
  if(mTheta < mMouseSensitivity) mTheta = mMouseSensitivity;
  else if (mTheta >  PI - mMouseSensitivity) mTheta = PI - mMouseSensitivity;

  mPhi += mMouseSensitivity * deltaX;
  if(mPhi < 0) mPhi += 2*PI;
  else if (mPhi > 2*PI) mPhi -= 2*PI;

  mLookAtPos[0] = mPosition[0] + mFixDist * sin(mTheta) * cos(mPhi);
  mLookAtPos[1] = mPosition[1] + mFixDist * cos(mTheta);
  mLookAtPos[2] = mPosition[2] + mFixDist * sin(mTheta) * sin(mPhi);
}

