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
#ifndef KEROSIN_LIGHT_H
#define KEROSIN_LIGHT_H

#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/glbase.h>
#include <kerosin/openglserver/openglserver.h>
#include "staticmesh.h"
#include <oxygen/geometryserver/indexbuffer.h>
#include <oxygen/sceneserver/basenode.h>

namespace kerosin
{
class OpenGLServer;

class KEROSIN_API Light : public oxygen::BaseNode
{
    //
    // Function
    //
public:
    Light();

    //! gets the light radius
    float GetRadius() const;

    //! sets the light radius
    void SetRadius(float radius);

    //! Enable render settings of this light
    virtual void Prepare();

    //! draw lit triangles of mesh
    void RenderLitMesh(boost::shared_ptr<StaticMesh> mesh);

    //! flush the internal cached geometry
    void FlushCache();

    //! update the bounding box
    void ComputeBoundingBox();

    //! sets the ambient light intensity
    void SetAmbient(const RGBA& ambient);

    //! returns the ambient light intensity
    const RGBA& GetAmbient();

    //! sets the diffuse light intensity
    void SetDiffuse(const RGBA& diffuse);

    //! returns the diffuse light intensitye
    const RGBA& GetDiffuse();

    //! sets the specular light intensity
    void SetSpecular(const RGBA& specular);

    //! returns the specular light intensity
    const RGBA& GetSpecular();

protected:
    /** set up RenderServer reference */
    virtual void OnLink();

    /** reset RenderServer reference */
    virtual void OnUnlink();

    virtual void DeterminePotentiallyLitTriangles(boost::shared_ptr<StaticMesh>& mesh,
                                                  std::vector<unsigned int>& triangles);

    //
    // Members
    //
protected:
    /** the diffuse light intensity */
    RGBA mDiffuse;

    /** the ambient light intensity */
    RGBA mAmbient;

    /** the specular light intensity */
    RGBA mSpecular;

    /** the allocated OpenGL light constant */
    int mGLLight;

    float   mRadius;
    unsigned int mVP;
    unsigned int mFP;

    //! position used to invalidate the cache
    salt::Vector3f mOldPos;

    //! mesh transform used to invalidate the cache
    salt::Matrix mMeshOldTransform;

    //! state of the cache
    bool mCacheFlushed;

    // indices of influenced triangles
    std::vector<unsigned int> mTriangles;

    // per-material cached indices
    std::vector<oxygen::IndexBuffer> mCachedIndices;

    /** reference to the OpenGLServer */
    CachedPath<OpenGLServer> mOpenGLServer;
};

DECLARE_CLASS(Light);

} //namespace kerosin

#endif //KEROSIN_LIGHT_H
