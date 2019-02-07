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

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "light.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/openglserver/openglwrapper.h>

using namespace boost;
using namespace kerosin;
using namespace salt;

Light::Light()
    : BaseNode(),
      mDiffuse(1.0f,1.0f,1.0f,1.0f),
      mAmbient(0.0f,0.0f,0.0f,1.0f),
      mSpecular(1.0f,1.0f,1.0f,1.0f),
      mGLLight(-1),
      mVP(0),
      mFP(0),
      mCacheFlushed(true)
{
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

void Light::SetAmbient(const RGBA& ambient)
{
    mAmbient = ambient;
}

const RGBA& Light::GetAmbient()
{
    return mAmbient;
}

void Light::SetDiffuse(const RGBA& diffuse)
{
    mDiffuse = diffuse;
}

const RGBA& Light::GetDiffuse()
{
    return mDiffuse;
}

void Light::SetSpecular(const RGBA& specular)
{
    mSpecular = specular;
}

const RGBA& Light::GetSpecular()
{
    return mSpecular;
}

void Light::Prepare()
{
    if (mGLLight < 0)
        {
            // no light allocated
            return;
        }

    const Vector3f& pos = GetWorldTransform().Pos();

    float light_pos[4];
    light_pos[0]=pos[0];
    light_pos[1]=pos[1];
    light_pos[2]=pos[2];
    light_pos[3]=1.0f;

    glLightfv(mGLLight, GL_AMBIENT, mAmbient);
    glLightfv(mGLLight, GL_DIFFUSE, mDiffuse);
    glLightfv(mGLLight, GL_POSITION,light_pos);
    glEnable(mGLLight);

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

void Light::RenderLitMesh(boost::shared_ptr<StaticMesh> /*mesh*/)
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
    const unsigned int n
        = static_cast<unsigned int>(mCachedIndices.size());

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

void Light::OnLink()
{
    RegisterCachedPath(mOpenGLServer, "sys/server/opengl");

    if (mOpenGLServer.expired())
        {
            GetLog()->Error()
                << "(Light) ERROR: OpenGLServer not found\n";
            return;
        }

    mGLLight = mOpenGLServer->AllocLight();

    if (mGLLight < 0)
        {
            GetLog()->Error()
                << "(Light) ERROR: No more OpenGL lights available\n";
        }
}

void Light::OnUnlink()
{
    if (
        (! mOpenGLServer.expired()) &&
        (mGLLight >= 0)
        )
        {
            mOpenGLServer->PutLight(mGLLight);
            mGLLight = -1;
        }

    BaseNode::OnUnlink();
}

void Light::DeterminePotentiallyLitTriangles(boost::shared_ptr<StaticMesh>& /*mesh*/, std::vector<unsigned int>& /*triangles*/)
{
    /*
      Opcode::SphereCache cache;
      Sphere sphere;

      sphere.Set(Point(GetWorldTransform().Pos().x(), GetWorldTransform().Pos().y(), GetWorldTransform().Pos().z()), 1.5f*mRadius);
      mesh->FindInfluencedTriangles(cache, sphere, triangles);
    */
}
