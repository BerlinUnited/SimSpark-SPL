/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: camera.h,v 1.3 2004/06/16 13:20:59 jamu Exp $

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
#ifndef _CAMERA_H
#define _CAMERA_H

#include <math.h>
#include <GL/glut.h>
#include <salt/gmath.h>
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;
using namespace salt;

class Camera
{
 public:
  Camera() {};
  Camera(const Vector3f& camPos,
         const Vector3f& lookAtPos,
         const Vector3f& upVector);

  ~Camera() {};

  void Look();
  void RefreshCam();
  void MoveCamStrafe(float steps);
  void MoveCamForward(float steps);
  void MoveCamStrafeForward(float steps);
  void TurnCamDown();
  void MoveCamUp(float steps);
  void SetCameraPos(const Vector3f& newCamPos);
  void SetLookAtPos(const Vector3f& lookAtPos);
  void SetUpVector(const Vector3f& upVector);
  void SetViewByMouse(const Vector2f& mousePos,
                      const Vector2f& refPos);

  Vector3f GetCameraPos();
  Vector3f GetLookAtPos();
  Vector3f GetUpVector();
    
 protected:
  double mMouseSensitivity;
  double mPhi, mTheta, mFixDist;
  Vector3f mPosition;
  Vector3f mLookAtPos;
  Vector3f mUpVector;
};

#endif // _CAMERA_H
