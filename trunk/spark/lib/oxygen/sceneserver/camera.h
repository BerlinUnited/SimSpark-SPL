/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef KEROSIN_CAMERA_H
#define KEROSIN_CAMERA_H

#include "basenode.h"
#include <salt/frustum.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

/** Camera encapsualtes all data needed to describe the viewpoint from
 * which a scene is rendered. The active camera is responsible to
 * construct a frustum needed to render the scene.
 */
class OXYGEN_API Camera : public BaseNode
{
    //
    // Functions
    //
public:
    Camera();
    virtual ~Camera();

    // set properties

    /** sets viewpoint properties */
    void SetViewport(int x, int y, int width, int height);

    int GetViewportX() const;
    int GetViewportY() const;
    int GetViewportWidth() const;
    int GetViewportHeight() const;

    /** sets the field of view (FOV) */
    void SetFOV(float fov);

    /** sets the distance of the Z near plane */
    void SetZNear(float zNear);

    /** sets the distance of the Z far plane */
    void SetZFar(float zFar);

    /** adjusts the current FOV, i.e. adds a delta increment */
    void AdjustFOV(float fov);

    /** adjusts the distance of the Z near plane, i.e adds a delta
        increment */
    void AdjustZNear(float zNear);

    /** adjusts the distance of the Z far plane, i.e adds a delta
        increment */
    void AdjustZFar(float zFar);

    /** returns the field of View */
    float GetFOV() const;

    /** returns the distance of the Z near plane */
    float GetZNear() const;

    /** returns the distance of the Z far plane */
    float GetZFar() const;

    /** returns the view transformation matrix */
    const salt::Matrix& GetViewTransform() const;

    /** returns the projection matrix */
    const salt::Matrix& GetProjectionTransform() const;

    /** fills in a frustum object with the correct parameters for this
        camera */
    void DescribeFrustum(salt::Frustum& frustum) const;

    /** turn the camera so that it looks to a given point.
     *  @param toPoint the point to look at
     */
    void LookAt(const salt::Vector3f& toPoint);

    /** sets the view transform to be the inverted WorldTransform and
        sets up the projection transform matrix
    **/
    void Bind();

protected:
    /** gets the right viewport resolution */
    virtual void OnLink();

private:
    /** calculates the view matrix (world->view space
        transformation) */
    virtual void UpdateHierarchyInternal();

    //
    // Members
    //
protected:
    /** horizontal field of view, default is 60 degrees */
    float mFOV;

    /** near clipping plane, default is 1 */
    float mZNear;

    /** far clipping plane, default is 2000 */
    float mZFar;

    /** x-position of upper left viewport corner, default is 0 */
    int mX;

    /** y-position of upper left viewport corner, default is 0 */
    int mY;

    /** width of viewport, default is the engine window width */
    int mWidth;

    /** height of viewport, default is the egine window height */
    int mHeight;
    float mHalfWorldWidth;
    float mHalfWorldHeight;

    /** the view transformation matrix */
    salt::Matrix mViewTransform;

    /** the projection matrix */
    salt::Matrix mProjectionTransform;
};

DECLARE_CLASS(Camera);

} //namespace kerosin

#endif //KEROSIN_CAMERA_H
