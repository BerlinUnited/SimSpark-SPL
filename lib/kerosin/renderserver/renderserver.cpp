/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: renderserver.cpp,v 1.1 2003/08/26 09:07:44 fruit Exp $

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

#include "renderserver.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

RenderServer::RenderServer() :
Leaf()
{
}

RenderServer::~RenderServer()
{
}

void
RenderServer::GetActiveScene()
{
    if (mSceneServer.get() != NULL)
    {
        mActiveScene = mSceneServer.GetActiveScene();
    }
}

bool
RenderServer::ConstructInternal()
{
    shared_ptr<Scene> mSceneServer = shared_static_cast<Scene>(GetCore()->Get("/sys/server/scene"));

    shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(GetCore()->Get("/sys/server/opengl"));

    mAmbientVP = openglServer->LoadARBVertexProgram("sys/program/ambient.vp");
    if (mAmbientVP == 0)
    {
        GetLog()->Error() << "ERROR: Could not load vertex program..." << std::endl;
    }

    return true;
}

void RenderServer::Render()
{
    GetActiveScene();

    if (mActiveScene == NULL)
    {
        return;
    }

    // get a camera to render with
    shared_ptr<Camera> camera = shared_static_cast<Camera>(mActiveScene->GetChildOfClass("Camera", true));
    // get opengl server
    shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(GetCore()->Get("/sys/server/opengl"));

    if (camera.get() != NULL && openglServer.get() != NULL)
    {
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glColor3f(1,1,1);

        // set the view transformation
        camera->Bind();
        Frustum frustum;
        camera->DescribeFrustum(frustum);

        // cull lights and geometries against the frustum
        TLeafList myLights;
        TLeafList allMeshes;
        TLeafList visibleMeshes;
        TLeafList::iterator i;

        mActiveScene->GetChildrenSupportingClass("Light", myLights, true);
        mActiveScene->GetChildrenSupportingClass("StaticMesh", allMeshes, true);

        for (i = allMeshes.begin(); i != allMeshes.end(); ++i)
        {
            // try to cull meshes, which are outside the viewing frustum
            if (frustum.Intersects(shared_static_cast<StaticMesh>(*i)->GetWorldBoundingBox())!=Frustum::FS_OUTSIDE)
            {
                visibleMeshes.push_back(*i);
            }
        }

        // actual rendering

        // ambient pass
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        if (openglServer->SupportsFancyLighting())
        {
            glEnable(GL_VERTEX_PROGRAM_ARB);
            glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mAmbientVP);

            glColor3f(0.1f,0.1f,0.1f);
            mActiveScene->RenderAmbient();

            // render lights
            glBlendFunc(GL_ONE, GL_ONE);
            glEnable(GL_BLEND);
            //glEnable(GL_ALPHA_TEST);
            //glAlphaFunc(GL_GREATER, 0.0f);
            glDepthMask(0);
            glDepthFunc(GL_EQUAL);

            for (TLeafList::iterator i=myLights.begin(); i != myLights.end(); ++i)
            {
                shared_ptr<Light> light = shared_static_cast<Light>(*i);

                // only render the light if it is visible
                if (frustum.Intersects(light->GetWorldBoundingBox())!=Frustum::FS_OUTSIDE)
                {
                    for (TLeafList::iterator j=visibleMeshes.begin(); j != visibleMeshes.end(); ++j)
                    {
                        shared_ptr<StaticMesh> mesh = shared_static_cast<StaticMesh>(*j);

                        // we only have to render meshes, whose bounding volume intersects the light volume
                        if (light->GetWorldBoundingBox().Intersects(mesh->GetWorldBoundingBox()))
                        {

                            Matrix   toObjectSpace;
                            toObjectSpace.Identity();
                            toObjectSpace = mesh->GetWorldTransform();
                            toObjectSpace.InvertRotationMatrix();
                            //light->GetWorldTransform().Pos().Dump();
                            light->Prepare();
                            Vector3f lightPos = toObjectSpace.Transform(light->GetWorldTransform().Pos());
                            Vector3f viewPos = toObjectSpace.Transform(camera->GetWorldTransform().Pos());
                            glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, lightPos.x(), lightPos.y(), lightPos.z(), 1.0f);
                            glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 1, viewPos.x(), viewPos.y(), viewPos.z(), 1.0f);
                            light->RenderLitMesh(shared_static_cast<StaticMesh>(*j));
                        }
                    }
                }
            }

            glDisable(GL_BLEND);
            glDepthMask(1);

            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            glActiveTextureARB(GL_TEXTURE1_ARB);
            glDisable(GL_TEXTURE_2D);
            glActiveTextureARB(GL_TEXTURE2_ARB);
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            // ambient lighting only
            glColor3f(1, 1, 1);
            mActiveScene->RenderAmbient();
        }

        if (openglServer->SupportsFancyLighting())
        {
            glEnable(GL_VERTEX_PROGRAM_ARB);
            glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mAmbientVP);
        }
        // standard rendering
        mActiveScene->Render();
        if (openglServer->SupportsFancyLighting())
        {
            glDisable(GL_VERTEX_PROGRAM_ARB);
        }
    }
}




