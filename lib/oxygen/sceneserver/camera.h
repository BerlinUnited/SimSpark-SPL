/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: camera.h,v 1.3 2003/09/08 08:58:53 rollmark Exp $

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

#ifndef CAMERA_H__
#define CAMERA_H__

#include "basenode.h"
#include <salt/frustum.h>

namespace kerosin
{

/** Camera encapsualtes all data needed to describe the viewpoint from which a
 * scene is rendered. The active camera is responsible to construct a frustum
 * needed to render the scene.
 */
class Camera : public BaseNode
{
        //
        // Functions
        //
public:
        Camera();
        virtual ~Camera();

        // set properties

        /** sets viewpoint properties */
        f_inline void   SetViewport(int x, int y, int width, int height)        
   	    {       mX = x; mY = y; mWidth = width; mHeight = height;       }

	f_inline int GetViewportX() { return mX; }
	f_inline int GetViewportY() { return mY; }
	f_inline int GetViewportWidth() { return mWidth; }
	f_inline int GetViewportHeight() { return mHeight; }


        /** sets the field of view (FOV) */
        f_inline void   SetFOV(const float fov)         {       mFOV = fov;             }

        /** sets the distance of the Z near plane */
        f_inline void   SetZNear(const float zNear)     {       mZNear = zNear; }

        /** sets the distance of the Z far plane */
        f_inline void   SetZFar(const float zFar)       {       mZFar = zFar;   }

        /** adjusts the current FOV, i.e. adds a delta increment */
        f_inline void   AdjustFOV(const float fov)              {       mFOV+=fov;              }

        /** adjusts the distance of the Z near plane, i.e adds a delta increment */
        f_inline void   AdjustZNear(const float zNear)  {       mZNear+=zNear;  }

        /** adjusts the distance of the Z far plane, i.e adds a delta increment */
        f_inline void   AdjustZFar(const float zFar)    {       mZFar+=zFar;    }

        /** returns the field of View */
        f_inline float                                  GetFOV()const                           {       return mFOV;    }

        /** returns the distance of the Z near plane */
        f_inline float                                  GetZNear()const                         {       return mZNear;  }

        /** returns the distance of the Z far plane */
        f_inline float                                  GetZFar()const                          {       return mZFar;   }

        /** returns the view transformation matrix */
        f_inline const salt::Matrix&    GetViewTransform() const        {       return mViewTransform;  }

        /** returns the projection matrix */
	f_inline const salt::Matrix&    GetProjectionTransform() const { return mProjectionTransform; }

        /** fills in a frustum object with the correct parameters for this camera */
        void DescribeFrustum(salt::Frustum& frustum) const;

        /** sets the view transform to be the inverted WorldTransform and sets
	    up the projection transform matrix
	**/
        void Bind();


protected:

        /** gets the right viewport resolution */
        virtual void OnLink();
private:

        /** calculates the view matrix (world->view space transformation) */
        virtual void UpdateHierarchyInternal();


        //
        // Members
        //
protected:

        /** horizontal field of view, default is 60 degrees */
        float           mFOV;

        /** near clipping plane, default is 1 */
        float           mZNear;

        /** far clipping plane, default is 2000 */
        float           mZFar;

        /** x-position of upper left viewport corner, default is 0 */
        int                     mX;

        /** y-position of upper left viewport corner, default is 0 */
        int                     mY;

        /** width of viewport, default is the engine window width */
        int                     mWidth;

        /** height of viewport, default is the egine window height */
        int                     mHeight;


        float           mHalfWorldWidth;
        float           mHalfWorldHeight;

        /** the view transformation matrix */
        salt::Matrix    mViewTransform;

        /** the projection matrix */
        salt::Matrix    mProjectionTransform;
};

DECLARE_CLASS(Camera);

} //namespace kerosin

#endif //CAMERA_H__
