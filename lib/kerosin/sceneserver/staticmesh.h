#ifndef STATICMESH_H__
#define STATICMESH_H__

#include "basenode.h"
#include "../textureserver/textureserver.h"
#include "indexbuffer.h"

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

class StaticMesh : public BaseNode
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
        typedef std::vector<IndexBuffer>        TIndexBuffers;

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
        virtual void RenderAmbientInternal();

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

#endif //STATICMESH_H__
