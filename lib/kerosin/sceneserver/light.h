#ifndef LIGHT_H__
#define LIGHT_H__

#include <oxygen/sceneserver/basenode.h>
#include "staticmesh.h"
#include "indexbuffer.h"

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
        std::vector<IndexBuffer>        mCachedIndices;
};

DECLARE_CLASS(Light);

} //namespace kerosin

#endif //LIGHT_H__
