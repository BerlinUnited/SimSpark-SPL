/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: camera.cpp,v 1.2 2003/08/31 13:08:42 rollmark Exp $

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
#include <kerosin/openglserver/openglserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace salt;

Camera::Camera() :
BaseNode()
{
        mViewTransform.Identity();

        mFOV    = 60.0f;
        mZNear  = 1.0f;
        mZFar   = 2000.0f;
        mX              = 0;
        mY              = 0;
        mWidth  = 640;
        mHeight = 480;

        SetName("camera");

        //gSceneNode.AddCamera(this);
}

Camera::~Camera()
{
        //gSceneNode.RemoveCamera(this);
}

/*!
        This is some dangerous looking code, but it's not as bad as it seems.
        The code extracts a frustum given the projection and view transforms.
        It is really fast and very generic because of that.
*/
void Camera::DescribeFrustum(Frustum& frustum) const
{
        // concatenate projection and view transform
        Matrix frustumMatrix = mProjectionTransform * mViewTransform;

        // Get plane parameters
    float *m= frustumMatrix.m;

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

void Camera::Bind()
{
        mViewTransform          = GetWorldTransform();
        mViewTransform.InvertRotationMatrix();

        // adjust the viewport
        glViewport(mX, mY, mWidth, mHeight);

        // set depth range
        glDepthRange(0, 1);

        // setup the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        mProjectionTransform.CalcInfiniteFrustum(-mHalfWorldWidth, mHalfWorldWidth, -mHalfWorldHeight, mHalfWorldHeight, mZNear);
        glMultMatrixf(mProjectionTransform.m);

        // initialize the modelview stack
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMultMatrixf(mViewTransform.m);
}

void Camera::OnLink()
{
        int xRes, yRes;
        GetScript()->GetVariable("Viewport.xRes", xRes);
        GetScript()->GetVariable("Viewport.yRes", yRes);

        mWidth = xRes;
        mHeight = yRes;
}

void Camera::UpdateHierarchyInternal()
{
        // make sure values are within bounds
        gClamp(mFOV, 10.0f, 170.0f);

        mHalfWorldWidth         = mZNear * (float)tan(gDegToRad(mFOV*0.5f));
        mHalfWorldHeight        = mHalfWorldWidth * (mHeight/(float)mWidth);
}
