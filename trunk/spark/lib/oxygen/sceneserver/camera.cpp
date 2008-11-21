/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: camera.cpp,v 1.10 2008/05/02 19:21:05 sgvandijk Exp $

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
#include <salt/matrix.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace oxygen;
using namespace salt;

Camera::Camera() : BaseNode()
{
    mViewTransform.Identity();
    mFOV    = 60.0f;
    mZNear  = 1.0f;
    mZFar   = 100000.0f;
    mX      = 0;
    mY      = 0;
    mWidth  = 640;
    mHeight = 480;

    SetName("camera");
}

Camera::~Camera()
{
}

/** This is some dangerous looking code, but it's not as bad as it
    seems.  The code extracts a frustum given the projection and view
    transforms.  It is really fast and very generic because of that.
*/
void
Camera::DescribeFrustum(Frustum& frustum) const
{
    // concatenate projection and view transform
    Matrix frustumMatrix = mProjectionTransform * mViewTransform;

    // Get plane parameters
    float* m = frustumMatrix.m;

    Plane *p = &frustum.mPlanes[Frustum::PI_RIGHT];
    p->normal.Set(m[3]-m[0], m[7]-m[4], m[11]-m[8]);
    p->d = m[15]-m[12];

    p = &frustum.mPlanes[Frustum::PI_LEFT];
    p->normal.Set(m[3]+m[0], m[7]+m[4], m[11]+m[8]);
    p->d = m[15]+m[12];

    p = &frustum.mPlanes[Frustum::PI_BOTTOM];
    p->normal.Set(m[3]+m[1], m[7]+m[5], m[11]+m[9]);
    p->d = m[15]+m[13];

    p = &frustum.mPlanes[Frustum::PI_TOP];
    p->normal.Set(m[3]-m[1], m[7]-m[5], m[11]-m[9]);
    p->d = m[15]-m[13];

    p = &frustum.mPlanes[Frustum::PI_NEAR];
    p->normal.Set(m[3]-m[2], m[7]-m[6], m[11]-m[10]);
    p->d = m[15]-m[14];

    p = &frustum.mPlanes[Frustum::PI_FAR];
    p->normal.Set(m[3]+m[2], m[7]+m[6], m[11]+m[10]);
    p->d = m[15]+m[14];

    // Normalize all plane normals
    for(int i=0;i<6;++i)
        {
            frustum.mPlanes[i].Normalize();
        }

    // set base position
    frustum.mBasePos = GetWorldTransform().Pos();
}

void
Camera::Bind()
{
    mViewTransform = GetWorldTransform();
    mViewTransform.RotateX(M_PI / 2);
    mViewTransform.InvertRotationMatrix();

    // setup the projection matrix
    mProjectionTransform.CalcInfiniteFrustum(
                                             -mHalfWorldWidth,
                                             mHalfWorldWidth,
                                             -mHalfWorldHeight,
                                             mHalfWorldHeight,
                                             mZNear);
}

void
Camera::OnLink()
{
    bool gotSetup =
        (
         GetScript()->GetVariable("Viewport.XRes", mWidth) &&
         GetScript()->GetVariable("Viewport.YRes", mHeight)
         );

    if (! gotSetup)
        {
            GetLog()->Error()
                << "(Camera) unable to read setup from ScriptServer\n";
        }
}

void
Camera::UpdateHierarchyInternal()
{
    // make sure values are within bounds
    gClamp(mFOV, 10.0f, 170.0f);

    mHalfWorldWidth =  mZNear * (float)tan(gDegToRad(mFOV*0.5f));
    mHalfWorldHeight = mHalfWorldWidth * (mHeight/(float)mWidth);
}

void
Camera::SetViewport(int x, int y, int width, int height)
{
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
}

int
Camera::GetViewportX() const
{
    return mX;
}

int
Camera::GetViewportY() const
{
    return mY;
}

int
Camera::GetViewportWidth() const
{
    return mWidth;
}

int
Camera::GetViewportHeight() const
{
    return mHeight;
}

void
Camera::SetFOV(float fov)
{
    mFOV = fov;
}

void
Camera::SetZNear(float zNear)
{
    mZNear = zNear;
}

void
Camera::SetZFar(float zFar)
{
    mZFar = zFar;
}

void
Camera::AdjustFOV(float fov)
{
    mFOV+=fov;
}

void
Camera::AdjustZNear(float zNear)
{
    mZNear+=zNear;
}

void
Camera::AdjustZFar(float zFar)
{
    mZFar+=zFar;
}

float
Camera::GetFOV() const
{
    return mFOV;
}

float
Camera::GetZNear() const
{
    return mZNear;
}

float
Camera::GetZFar() const
{
    return mZFar;
}

const salt::Matrix&
Camera::GetViewTransform() const
{
    return mViewTransform;
}

const salt::Matrix&
Camera::GetProjectionTransform() const
{
    return mProjectionTransform;
}

void
Camera::LookAt(const salt::Vector3f& toPoint)
{
    Matrix m;
    salt::Vector3f fromPoint;
    fromPoint = GetWorldTransform().Pos();
    //    std::cerr << "Camera: look from " << fromPoint << " at " << toPoint << "\n";
    salt::Vector3f up(0,0,1);
    m.LookAt(fromPoint, toPoint, up);
    SetWorldTransform(m);
}
