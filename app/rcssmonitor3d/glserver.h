/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.h,v 1.3 2003/12/27 17:53:40 fruit Exp $

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
#ifndef _GLSERVER_H
#define _GLSERVER_H

#include <GL/glut.h>
#include "camera.h"

class GLserver
{
 public:
  GLserver(){};
  GLserver(int width, int height,
           salt::Vector3f camPos,
           salt::Vector3f lookAtPos,
           salt::Vector3f up, bool wire);
  ~GLserver() {};

  void InitGL();
  void DrawGround();
  void ApplyCamera();
  void DrawSphere(salt::Vector3f spherePos, float radius);

  //inline functions
  inline void SetViewByMouse(const Vector2f& mousePos, const Vector2f& refPos)
    {
      mCamera.SetViewByMouse(mousePos, refPos);
    }

protected:
  salt::Vector3f mCamPos;
  salt::Vector3f mLookAt;
  salt::Vector3f mUp;
  Camera mCamera;
  bool mWireframe;
  int mWidth;
  int mHeight;
};

#endif // _GLSERVER_H


