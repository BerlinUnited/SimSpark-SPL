/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: light.h,v 1.5 2003/11/14 14:05:52 fruit Exp $

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
#ifndef KEROSIN_LIGHT_H
#define KEROSIN_LIGHT_H

#include "oxygen/sceneserver/basenode.h"
#include "staticmesh.h"
#include "oxygen/sceneserver/indexbuffer.h"

namespace kerosin
{

class Light : public oxygen::BaseNode
{
    //
    // Function
    //
public:
    Light();

    // radius
    float GetRadius() const;
    void SetRadius(float radius);

    //! set the diffuse color of the light
    void SetDiffuseColor(float r, float g, float b);

    //! Enable render settings of this light
    virtual void    Prepare();

    //! draw lit triangles of mesh
    void                    RenderLitMesh(boost::shared_ptr<StaticMesh> mesh);

    //! flush the internal cached geometry
    void                    FlushCache();

    //! update the bounding box
    void                    ComputeBoundingBox();

private:
    virtual bool    ConstructInternal();

    virtual void    RenderInternal();

    virtual void    DeterminePotentiallyLitTriangles(boost::shared_ptr<StaticMesh>& mesh, std::vector<unsigned int>& triangles);

    //
    // Members
    //
private:
    float   mRadius;
    float   mDiffuseR;
    float   mDiffuseG;
    float   mDiffuseB;

    unsigned int mFP;
    unsigned int mVP;

    //! position used to invalidate the cache
    salt::Vector3f  mOldPos;
    //! mesh transform used to invalidate the cache
    salt::Matrix    mMeshOldTransform;

    //! state of the cache
    bool                    mCacheFlushed;

    // indices of influenced triangles
    std::vector<unsigned int>       mTriangles;

    // per-material cached indices
    std::vector<oxygen::IndexBuffer>        mCachedIndices;
};

DECLARE_CLASS(Light);

} //namespace kerosin

#endif //KEROSIN_LIGHT_H
