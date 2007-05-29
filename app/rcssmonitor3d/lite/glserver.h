/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.h,v 1.11 2007/05/29 09:45:39 jboedeck Exp $

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

#include "camera.h"

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

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

    void InitTexture(const string &tFile);
    bool ReadTexture(const string &tFile);
    bool BMPImageLoad(const char *filename, Image *image) ;
    
    void InitGL();
    void DrawGroundRectangle(salt::Vector3f pos, float szX, float szY,
                             float angleDeg, float height = 0.0f);
    void DrawArbitraryLine(const salt::Vector3f& startPos, const salt::Vector3f& endPos);
    void DrawCircle(const salt::Vector3f& pos, float radius,
                    float start_angle = 0.0, float end_angle = 360.0);
    void ApplyCamera();
    void DrawText(const char* text, salt::Vector2f pos);
    void DrawText3D(const char* text, const salt::Vector3f& pos);
    void DrawText3D(const std::string& text, const salt::Vector3f& pos);
    void DrawTextPix(const char* text, salt::Vector2f pix, ETextAlign = eNONE);
    int GetTextHeight() const;
    int GetTextWidth(const char* text) const;

    void DrawGoal(salt::Vector3f goalPos, salt::Vector3f sz, float barRadius = 0.06);
    void DrawSphere(salt::Vector3f spherePos, float radius, int res = 10, bool wireframe = false);
    void DrawSkyBackground(float x, float y, float z, float width, float height, float length);
    void DrawShadowOfSphere(salt::Vector3f spherePos,float radius);
    void DrawWireBox(salt::Vector3f boxPos, salt::Vector3f sz);
    void Reshape(int width, int height)
    {
        mWidth = width;
        mHeight = height;
    }

    int GetWidth() { return mWidth; }
    int GetHeight() { return mHeight; }

    void SetTextureFile(const string &tfile);

    //inline functions
    inline void SetViewByMouse(const Vector2f& mousePos, const Vector2f& refPos)
    {
        mCamera.SetViewByMouse(mousePos, refPos);
    }
    inline salt::Vector3f GetCameraPos() const
    {
        return mCamPos;
    }
    inline void SetCameraPos(const salt::Vector3f& newPos)
    {
        mCamPos = newPos;
        mCamera.SetCameraPos(newPos);
    }
    inline salt::Vector3f GetUpVector()
    {
        return mUp;
    }
    inline void SetUpVector(const salt::Vector3f& newUp)
    {
        mUp = newUp;
        mCamera.SetUpVector(newUp);
    }
    inline salt::Vector3f GetLookAtPos()
    {
        return mLookAt;
    }
    inline void SetLookAtPos(const salt::Vector3f& newPos)
    {
        mLookAt = newPos;
        mCamera.SetLookAtPos(newPos);
    }
    inline void MoveCamForward(float steps)
    {
        mCamera.MoveCamForward(steps);
        mLookAt = mCamera.GetLookAtPos();
        mCamPos = mCamera.GetCameraPos();
    }
    inline void MoveCamStrafeForward(float steps)
    {
        mCamera.MoveCamStrafeForward(steps);
        mLookAt = mCamera.GetLookAtPos();
        mCamPos = mCamera.GetCameraPos();
    }
    inline void MoveCamStrafe(float steps)
    {
        mCamera.MoveCamStrafe(steps);
        mLookAt = mCamera.GetLookAtPos();
        mCamPos = mCamera.GetCameraPos();
    }
    inline void TurnCamDown()
    {
        mCamera.TurnCamDown();
        mLookAt = mCamera.GetLookAtPos();
        mCamPos = mCamera.GetCameraPos();
        mUp = mCamera.GetUpVector();
    }

    inline void MoveCamUp(float steps)
    {
        mCamera.MoveCamUp(steps);
        mLookAt = mCamera.GetLookAtPos();
        mCamPos = mCamera.GetCameraPos();
    }

protected:
    //typedef GLubyte IconArray[32][32][4];
    

    salt::Vector3f mCamPos;
    salt::Vector3f mLookAt;
    salt::Vector3f mUp;
    Camera mCamera;
    bool mWireframe;
    int mWidth;
    int mHeight;
    GLubyte mTexture[128][128][4];
//    std::map<Monitor::ECameraMode, IconArray> mCamModeIconMap;
    int mTextureWidth;
    int mTextureHeight;
    GLuint mTexName;
    GLuint mTexNames[8];
    //string mTextureFile;
};

#endif // _GLSERVER_H


