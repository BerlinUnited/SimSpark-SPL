/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "monitorframe_sparkglrender.h"
#include "monitorframe_glconstants.h"

#include <carbon.h>
#include <sparkcontext.h>
#include <simspark.h>
#include <openglmanager.h>

#include <zeitgeist/logserver/logserver.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/sceneserver/camera.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <kerosin/renderserver/renderserver.h>
#include <kerosin/renderserver/rendernode.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <kerosin/sceneserver/singlematnode.h>
#include <kerosin/sceneserver/axis.h>
#include <kerosin/materialserver/material.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

namespace MonitorFrameUtil
{

// ---- SparkGLRender::RenderSelectionContext

SparkGLRender::RenderSelectionContext::RenderSelectionContext(shared_ptr<BaseNode> node)
{
    assert(node.get() != 0);
    glPushMatrix();
    glMultMatrixf(node->GetWorldTransform().m);
}

SparkGLRender::RenderSelectionContext::~RenderSelectionContext()
{
    glPopMatrix();
}

// ---- SparkGLRender

SparkGLRender::SparkGLRender(const std::string& cameraPath, const std::string& sceneServerPath,
    const std::string& renderServerPath, const std::string& renderControlPath) :
    mCameraPath(cameraPath),
    mSceneServerPath(sceneServerPath),
    mRenderServerPath(renderServerPath),
    mRenderControlPath(renderControlPath),
    mErr(0)
{
}

SparkGLRender::~SparkGLRender()
{
}

bool SparkGLRender::init(shared_ptr<SimSpark> spark)
{
    mCamera.reset();
    mRenderServer.reset();

    //Check spark
    mSpark = spark;
    if (mSpark.get() == 0)
    {
        LOG_ERROR() << "Spark pointer illegal.";
        return false;
    }

    //Search camera
    mCamera = dynamic_pointer_cast<Camera>
        (mSpark->GetCore()->Get(mCameraPath));

    if (mCamera.get() == 0)
    {
        LOG_ERROR() << "Camera node not found. Expected it to be at '" << mCameraPath.c_str() << "'";
        return false;
    }

    //Search RenderServer
    mRenderServer = dynamic_pointer_cast<RenderServer>
        (mSpark->GetCore()->Get(mRenderServerPath));

    if (mRenderServer.get() == 0)
    {
        LOG_ERROR() << "RenderServer not found. Expected it to be at '" << mRenderServerPath.c_str() << "'";
        return false;
    }

    //Search RenderControl node
    mRenderControl = dynamic_pointer_cast<RenderControl>
        (mSpark->GetCore()->Get(mRenderControlPath));

    if (mRenderControl.get() == 0)
    {
        LOG_ERROR() << "RenderControl node not found. Expected it to be at '" << mRenderControlPath.c_str() << "'";
        return false;
    }

    //Search SceneServer
    mSceneServer = dynamic_pointer_cast<SceneServer>
        (mSpark->GetCore()->Get(mSceneServerPath));

    if (mSceneServer.get() == 0)
    {
        LOG_ERROR() << "SceneServer not found. Expected it to be at '" << mSceneServerPath.c_str() << "'";
        return false;
    }

    return true;
}

void SparkGLRender::setAmbientColor(float r, float g, float b, float a)
{
    mRenderServer->SetAmbientColor(RGBA(r,g,b,a));
}

void SparkGLRender::render(int width, int height)
{
    mWidth = width;
    mHeight = height;

    render();
}

weak_ptr<RenderNode> SparkGLRender::pickNode(int pos_x, int pos_y, int width, int height)
{
    if ((mSpark.get() == 0) || (mRenderServer.get() == 0))
    {
        assert(false);
        return weak_ptr<RenderNode>();
    }

    //Enable Picking
    double pickRange = 0.2;
    mRenderServer->EnablePicking(true, salt::Vector2f(pos_x, pos_y), pickRange);
    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " after EnablePicking";

    //Render in selection mode
    render(width, height);
    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " after Pick-rendering";

    //Disable Picking
    mRenderServer->DisablePicking();
    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " after DisablePicking";

    //Find and return picked node (if any)
    weak_ptr<RenderNode> pickedNode = mRenderServer->GetPickedNode();

    return pickedNode;
}

void SparkGLRender::render()
{
    if ((mCamera.get() == 0) ||
        (mRenderServer.get() == 0))
    {
        return;
    }

    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " at start of rendering.";

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glEnable( GL_DEPTH_TEST );

    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " before SetViewport";

    //Set the camera viewport
    mCamera->SetViewport(0, 0, mWidth, mHeight);

    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " before Render";

    //Update OpenGLSystem
    //mOpenGLServer->Update(); //OpenGLServers is OpenGLSystemQt, which does not do anything

    //Render Scene
    mRenderServer->Render(true);

    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " after Render";

    //Render CustomRender plugins
    mRenderControl->RenderCustom();

    if (mErr = glGetError())
        LOG_ERROR() << OpenGLManager::getErrorMessageStd(mErr).c_str() << " after rendering";
}

} // namespace MonitorFrameUtil
