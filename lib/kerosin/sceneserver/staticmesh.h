/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh.h,v 1.6 2003/11/14 14:05:52 fruit Exp $

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

#include "../textureserver/textureserver.h"
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/indexbuffer.h>

// define the symbol BOOL to prevent opcode.h to typedef BOOL
// #define BOOL BOOL
// #include <opcode/Opcode.h>

/* namespace Opcode */
/* { */
/* class OPCODE_Model; */
/* class SphereCollider; */
/* class PlanesCollider; */
/* class RayCollider; */
/* struct SphereCache; */
/* struct PlanesCache; */
/* } */

namespace Meshmerizer
{
class IndexedTriangle;
class Sphere;
}

namespace IceMaths
{
class OPCPlane;
class Matrix4x4;
}

namespace kerosin
{

class Material;

class StaticMesh : public oxygen::BaseNode
{
    //
    // Types
    //
public:
    struct Float3
    {
        float x,y,z;
    };

    struct Face
    {
        unsigned int a, b, c;
    };
    typedef std::vector<oxygen::IndexBuffer>        TIndexBuffers;

    //
    // Function
    //
public:
    StaticMesh();
    ~StaticMesh();

    bool Load(const std::string& fileName);

/*
//! given a 3D sphere, this returns the potentially influenced triangles lying inside the spere
void FindInfluencedTriangles(Opcode::SphereCache &cache, const Meshmerizer::Sphere &sphere, std::vector<unsigned int> &triangles) const;
//! given a set of planes this returns the potentially influenced triangles lying in front of all planes
void FindInfluencedTriangles(Opcode::PlanesCache &cache, const IceMaths::OPCPlane *planes, int numPlanes, std::vector<unsigned int> &triangles) const;


void FillIndexBuffers(TIndexBuffers &iBuffers, const std::vector<unsigned int> &triangles) const;
*/
/*
//! the render function, which is used by the dynamic point-based lights
void Render(const TIndexBuffers &iBuffers) const;
*/

    //! empty! this prevents the bounding box from being updated ... it cannot change!
    virtual void    ComputeBoundingBox();

    //! calculates the local bounding box
    void                    CalcBoundingBox();

    //! given a ray, find closest intersection point
    bool Intersect(const salt::Vector3f& start, const salt::Vector3f& end, salt::Vector3f& ret) const;
private:
    virtual void RenderInternal();
    //   virtual void RenderAmbientInternal();

/*
//! initializes the collision system (Opcode)
void PrepareCollision();

//! converts internal matrix to an Opcode matrix
void ConvertMatrix(IceMaths::Matrix4x4& matrix) const;
*/

    //
    // Members
    //
private:
    int             mVertexCount;
    Float3* mPos;
    Float3* mTexCoords;
    // tangent space basis vectors
    Float3* mBasisX;
    Float3* mBasisY;

    std::vector<boost::shared_ptr<Material> >       mMaterials;
    std::vector<int>                                                        mFaceCount;
    std::vector<Face*>                                                      mFaces;

    // collision related
    std::vector<unsigned int>               mFaceToMaterial;
    Meshmerizer::IndexedTriangle    *mOCFaces;

/*
  Opcode::OPCODE_Model                    *mCollisionModel;
  Opcode::SphereCollider                  *mSphereCollider;
  Opcode::PlanesCollider                  *mPlanesCollider;
  Opcode::RayCollider                     *mRayCollider;
*/
};

DECLARE_CLASS(StaticMesh);

} //namespace kerosin

#endif //KEROSIN_STATICMESH_H
