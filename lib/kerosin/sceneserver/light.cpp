#include "light.h"
#include "../openglserver/openglserver.h"
#include <zeitgeist/logserver/logserver.h>

/*
// define the symbol BOOL to prevent opcode.h to typedef BOOL
#define BOOL BOOL
#include <opcode/Opcode.h>
*/

using namespace boost;
using namespace kerosin;

Light::Light()
{
        mRadius         = 1.0f;
        mDiffuseR       = 1.0f;
        mDiffuseG       = 1.0f;
        mDiffuseB       = 1.0f;

        mVP = 0;
        mFP = 0;

        mCacheFlushed = true;
}

float Light::GetRadius() const
{
        return mRadius;
}

void Light::SetRadius(float radius)
{
        mRadius = radius;
        FlushCache();
}

void Light::SetDiffuseColor(float r, float g, float b)
{
        mDiffuseR = r;
        mDiffuseG = g;
        mDiffuseB = b;
}

void Light::Prepare()
{
/*
        glEnable(GL_VERTEX_PROGRAM_ARB);
        glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mVP);

        glEnable(GL_FRAGMENT_PROGRAM_ARB);
        glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, mFP);

        // publish light color to fragment program
        glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, mDiffuseR, mDiffuseG, mDiffuseB, 1.0f);
        glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, -1.0f/(mRadius*mRadius), 1.0f, 1.0f, 1.0f);
*/
}

void Light::RenderLitMesh(boost::shared_ptr<StaticMesh> mesh)
{
/*
          if (mCacheFlushed || (GetWorldTransform().Pos()-mOldPos).SquareLength() > 0.1 ||
                !mesh->GetWorldTransform().IsEqual(mMeshOldTransform))
        {
                // if the cache wasn't flushed, we will flush it now
                if (!mCacheFlushed) FlushCache();

                // cache the old position
                mOldPos = GetWorldTransform().Pos();

                // cache the mesh old position
                mMeshOldTransform = mesh->GetWorldTransform();

                // give each derived light a chance to find potentially lit triangles
                DeterminePotentiallyLitTriangles(mesh, mTriangles);
                //printf("%d\n", mTriangles.size());
                // fill cache indices
                mesh->FillIndexBuffers(mCachedIndices, mTriangles);

                // mark that we contain valid data
                mCacheFlushed = false;
        }

        mesh->Render(mCachedIndices);

        glDisable(GL_FRAGMENT_PROGRAM_ARB);
        glDisable(GL_VERTEX_PROGRAM_ARB);
*/
}

void Light::FlushCache()
{
        const unsigned int n = mCachedIndices.size();
        for (unsigned int i=0; i<n; ++i)
        {
                mCachedIndices[i].Flush();
        }
        mCacheFlushed = true;
}

void Light::ComputeBoundingBox()
{
        mLocalBoundingBox.minVec.Set(-mRadius,-mRadius,-mRadius);
        mLocalBoundingBox.maxVec.Set(mRadius,mRadius,mRadius);
}

bool Light::ConstructInternal()
{
        // get opengl server
        shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(GetCore()->Get("/sys/server/opengl"));

        mVP = openglServer->LoadARBVertexProgram("sys/program/omnilight.vp");
        if (mVP == 0)
        {
                GetLog()->Error() << "ERROR: Could not load vertex program..." << std::endl;
        }

        mFP = openglServer->LoadARBFragmentProgram("sys/program/omnilight.fp");
        if (mFP == 0)
        {
                GetLog()->Error() << "ERROR: Could not load fragment program..." << std::endl;
        }

        return true;
}

void Light::RenderInternal()
{
}

void Light::DeterminePotentiallyLitTriangles(boost::shared_ptr<StaticMesh>& mesh, std::vector<unsigned int>& triangles)
{
/*
        Opcode::SphereCache cache;
        Sphere sphere;

        sphere.Set(Point(GetWorldTransform().Pos().x(), GetWorldTransform().Pos().y(), GetWorldTransform().Pos().z()), 1.5f*mRadius);
        mesh->FindInfluencedTriangles(cache, sphere, triangles);
*/
}
