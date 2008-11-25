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
#include "renderserver.h"
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/camera.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <kerosin/openglserver/openglwrapper.h>
#include <kerosin/sceneserver/staticmesh.h>
#include <kerosin/sceneserver/light.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

/** OpenGL picking result buffer */
static GLuint gSelectBuffer[4096];

RenderServer::RenderServer() : BaseRenderServer()
{
    mAmbientColor = RGBA(0.0,0.0,0.0,0.0);
    mEnablePicking = false;
    mPickAt = Vector2f(0.0,0.0);
    mPickRange = 10.0;
    mNextName = 1;
}

void
RenderServer::PreparePicking()
{
    mNameMap.clear();
    mNextName = 1;
    glSelectBuffer(sizeof(gSelectBuffer), gSelectBuffer);
    mPickedNode.reset();
}

void
RenderServer::Render()
{
    PreparePicking();

    if (! GetActiveScene())
    {
        return;
    }

    // if no camera is selected yet, select the first one
    if (mCamera.get() == 0)
      mCamera = shared_static_cast<Camera>(mActiveScene->GetChildOfClass("Camera", true));
    
    if (mCamera.get() == 0)
    {
        GetLog()->Error()
            << "(RenderServer) ERROR: found no camera node in the active scene\n";
        return;
    }

    glClearColor(
                 mAmbientColor.r(),
                 mAmbientColor.g(),
                 mAmbientColor.b(),
                 mAmbientColor.a()
                 );

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1,1,1);

    // set the view transformation
    BindCamera(mCamera);

    // actual rendering

    // ambient pass
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    TLeafList lights;
    mActiveScene->ListChildrenSupportingClass<Light>(lights,true);

    if (lights.size() == 0)
    {
        // no lights in the scene, disable lighting
        glDisable(GL_LIGHTING);
    }
    else
    {
        glEnable(GL_LIGHTING);
        glShadeModel (GL_SMOOTH);

        // prepare all lights
        for (
             TLeafList::iterator iter = lights.begin();
             iter != lights.end();
             ++iter
             )
        {
            (shared_static_cast<Light>(*iter))->Prepare();
        }
    }

    // standard rendering
    RenderScene(mActiveScene);

    // reset GL lights
    glDisable(GL_LIGHTING);

    if (mEnablePicking)
    {
        ProcessPicks();
    }
}


int
RenderServer::Width() const
{
    if (mActiveScene.get() == 0) return 0;

    if (mCamera.get() == 0)
    {
        GetLog()->Error()
            << "(RenderServer) ERROR: found no camera node in the active scene\n";
        return 0;
    }
    return mCamera->GetViewportWidth();
}

int
RenderServer::Height() const
{
    if (mActiveScene.get() == 0) return 0;

    if (mCamera.get() == 0)
    {
        GetLog()->Error()
            << "(RenderServer) ERROR: found no camera node in the active scene\n";
        return 0;
    }
    return mCamera->GetViewportHeight();
}

bool
RenderServer::CopyFrame(char* buffer) const
{
    if (mActiveScene.get() == 0) return false;
    glReadPixels(0,0,Width()-1,Height()-1,GL_RGB,GL_UNSIGNED_BYTE,buffer);

    size_t len = Width() * 3;
    unsigned char* line = new unsigned char[len]; /* one line of packed RGB pixels */

    unsigned char* a;
    unsigned char* b;

    for (int y = 0; y < Height() >> 1; ++y)
    {
        a = ( unsigned char * ) & buffer[y * len];
        b = ( unsigned char * ) & buffer[(Height() - ( y + 1 )) * len];

        memcpy(line, a, len);
        memcpy(a, b, len);
        memcpy(b, line, len);
    }
    delete[] line;
    return true;
}

void
RenderServer::ProcessPicks()
{
    // restoring the original projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    // returning to normal rendering mode
    int hits = glRenderMode(GL_RENDER);

    // if there are hits process them
    GLuint numberOfNames = 0;
    GLuint minZ = 0xffffffff;
    GLuint *ptrNames = 0;

    GLuint* ptr = gSelectBuffer;

    for (int i = 0; i < hits; i++)
        {
            // read number of stored names in the current record
            GLuint names = (*ptr);
            ptr++;

            if ((*ptr) < minZ)
                {
                    numberOfNames = names;
                    minZ = *ptr;
                    ptrNames = ptr+2;
                }

        ptr += names+2;
    }
    mPickedNode.reset();
    for (;;)
        {
            if (ptrNames == 0)
                {
                    break;
                }

            TGLNameMap::const_iterator iter = mNameMap.find(*ptrNames);
            if (iter == mNameMap.end())
                {
                    break;
                }

            mPickedNode = (*iter).second;
            break;
        }

    mNameMap.clear();
}

void
RenderServer::RenderScene(boost::shared_ptr<BaseNode> node)
{
#if 0
    shared_ptr<SphereCollider> collider = shared_dynamic_cast<SphereCollider>(node);
    if (collider != 0)
    {
        std::cerr << "RenderScene (spherecollider radius: " << collider->GetRadius() << ")\n";
    }
#endif
    shared_ptr<RenderNode> renderNode = shared_dynamic_cast<RenderNode>(node);
    if (renderNode.get() != 0)
        {
            glPushMatrix();

            if (mEnablePicking)
                {
                    // assign an OpenGL name to the RenderNode
                    GLuint name = (mNextName++);
                    mNameMap[name] = renderNode;
                    glPushName(name);
                }

            glMultMatrixf(node->GetWorldTransform().m);

        renderNode->RenderInternal();

            if (mEnablePicking)
                {
                    // pop name from OpenGL name stack
                    glPopName();
                }

            if (mEnablePicking)
                {
                    // pop name from OpenGL name stack
                    glPopName();
                }

            glPopMatrix();
        }

    // traverse the the hierarchy
    for (TLeafList::iterator i = node->begin(); i!= node->end(); ++i)
    {
        shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>(*i);
        if (node.get() == 0)
        {
            continue;
        }

        RenderScene(node);
    }
}

void
RenderServer::BindCamera(boost::shared_ptr<Camera>& camera)
{
    camera->Bind();

    // adjust the viewport
    glViewport(
               camera->GetViewportX(),
               camera->GetViewportY(),
               camera->GetViewportWidth(),
               camera->GetViewportHeight()
               );

    // set depth range
    glDepthRange(0, 1);

    if (mEnablePicking)
        {
            glRenderMode(GL_SELECT);

            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();

            gluPickMatrix(mPickAt[0],viewport[3]-mPickAt[1],mPickRange,mPickRange,viewport);

            glMultMatrixf(camera->GetProjectionTransform().m);
            glMatrixMode(GL_MODELVIEW);

            glInitNames();
        } else
        {
            // setup the projection matrix
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMultMatrixf(camera->GetProjectionTransform().m);

            // initialize the modelview stack
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMultMatrixf(camera->GetViewTransform().m);
        }
}

void
RenderServer::SetAmbientColor(const RGBA& ambient)
{
    mAmbientColor = ambient;
}

void RenderServer::UpdateCached()
{
    mActiveScene.reset();
}

void RenderServer::DisablePicking()
{
    mEnablePicking = false;
}

void RenderServer::EnablePicking(bool enable, const Vector2f& pickAt, const double pickRange)
{
    mEnablePicking = enable;
    mPickAt = pickAt;
    mPickRange = pickRange;
}

weak_ptr<RenderNode> RenderServer::GetPickedNode() const
{
    return mPickedNode;
}

void RenderServer::NextCamera()
{
    TLeafList cameras;
    mActiveScene->GetChildrenOfClass("Camera", cameras, true);
    for (TLeafList::iterator iter = cameras.begin(); iter != cameras.end(); ++iter)
        if (*iter == mCamera)
        {
            ++iter;
            mCamera = iter != cameras.end() ? shared_static_cast<Camera>(*iter) : shared_static_cast<Camera>(cameras.front());
            break;
        }
}

void RenderServer::PreviousCamera()
{
    TLeafList cameras;
    mActiveScene->GetChildrenOfClass("Camera", cameras, true);
    for (TLeafList::iterator iter = cameras.begin(); iter != cameras.end(); ++iter)
        if (*iter == mCamera)
        {
            mCamera = iter != cameras.begin() ? shared_static_cast<Camera>(*(--iter)) : shared_static_cast<Camera>(cameras.back());
            break;
        }
}

