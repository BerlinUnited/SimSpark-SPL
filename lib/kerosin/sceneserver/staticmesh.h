/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh.h,v 1.9 2004/04/19 16:03:23 rollmark Exp $

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
#ifndef KEROSIN_STATICMESH_H
#define KEROSIN_STATICMESH_H

#include <kerosin/textureserver/textureserver.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/indexbuffer.h>
#include <boost/shared_array.hpp>

namespace kerosin
{

class Material;

class StaticMesh : public oxygen::BaseNode
{
    //
    // Types
    //
public:
    typedef std::vector<oxygen::IndexBuffer> TIndexBuffers;

    //
    // Function
    //
public:
    StaticMesh();
    ~StaticMesh();

    bool Load(const std::string& fileName);

    /** empty! this prevents the bounding box from being updated, as
        it cannot change
    */
    virtual void ComputeBoundingBox();

    //! calculates the local bounding box
    void CalcBoundingBox();

protected:
    virtual void RenderInternal();

    //
    // Members
    //
protected:
    int mVertexCount;
    boost::shared_array<float> mPos;
    boost::shared_array<float> mTexCoords;
    boost::shared_array<float> mNormal;
#if 0
    // tangent space basis vectors
    boost::shared_array<float> mBasisX;
    beoost::shared_array<float> mBasisY;
#endif
    std::vector<boost::shared_ptr<Material> >  mMaterials;
    std::vector<int> mFaceCount;
    std::vector<boost::shared_array<unsigned int> > mFaces;
    std::vector<unsigned int> mFaceToMaterial;
};

DECLARE_CLASS(StaticMesh);

} //namespace kerosin

#endif //KEROSIN_STATICMESH_H
