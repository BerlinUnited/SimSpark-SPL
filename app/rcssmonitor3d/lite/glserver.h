/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.h,v 1.4 2004/05/10 14:10:46 fruit Exp $

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

class GLServer
{
public:
    enum ETextAlign { eNONE, eLEFT, eRIGHT, eCENTER };

    GLServer(){};
    GLServer(int width, int height,
             salt::Vector3f camPos,
             salt::Vector3f lookAtPos,
             salt::Vector3f up, bool wire);
    ~GLServer() {};

    void InitGL();
    void DrawGroundRectangle(salt::Vector3f pos, float szX, float szY,
                             float angleDeg, float height = 0.0f);
    void DrawCircle(const salt::Vector3f& pos, float radius,
                    float start_angle = 0.0, float end_angle = 360.0);
    void ApplyCamera();
    void DrawText(const char* text, salt::Vector2f pos);
    void DrawText3D(const char* text, const salt::Vector3f& pos);
    void DrawText3D(const std::string& text, const salt::Vector3f& pos);
    void DrawTextPix(const char* text, salt::Vector2f pix, ETextAlign = eNONE);
    int GetTextHeight() const;
    int GetTextWidth(const char* text) const;

    void DrawGoal(salt::Vector3f goalPos, salt::Vector3f sz);
    void DrawSphere(salt::Vector3f spherePos, float radius, int res = 10);
    void DrawShadowOfSphere(salt::Vector3f spherePos,float radius);
    void DrawWireBox(salt::Vector3f boxPos, salt::Vector3f sz);
    void Reshape(int width, int height)
    {
        mWidth = width;
        mHeight = height;
    }

    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }

    //inline functions
    inline void SetViewByMouse(const Vector2f& mousePos, const Vector2f& refPos)
    {
        mCamera.SetViewByMouse(mousePos, refPos);
    }
    inline salt::Vector3f GetCameraPos()
    {
        return mCamPos;
    }
    inline void SetCameraPos(const salt::Vector3f& newPos)
    {
        mCamPos = newPos;
        mCamera.SetCameraPos(newPos);
    }
    inline void SetLookAtPos(const salt::Vector3f& newPos)
    {
        mLookAt = newPos;
        mCamera.SetLookAtPos(newPos);
    }
    inline void MoveCamForward(float steps)
    {
        mCamera.MoveCamForward(steps);
        mCamPos = mCamera.GetLookAtPos();
        mLookAt = mCamera.GetCameraPos();
    }
    inline void MoveCamStrafe(float steps)
    {
        mCamera.MoveCamStrafe(steps);
        mCamPos = mCamera.GetLookAtPos();
        mLookAt = mCamera.GetCameraPos();
    }

    inline void MoveCamUp(float steps)
    {
        mCamera.MoveCamUp(steps);
        mCamPos = mCamera.GetLookAtPos();
        mLookAt = mCamera.GetCameraPos();
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


