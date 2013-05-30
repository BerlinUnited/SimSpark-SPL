/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "sparkglrender.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/sceneserver/camera.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <kerosin/renderserver/renderserver.h>
#include <kerosin/renderserver/rendernode.h>
#include <kerosin/sceneserver/singlematnode.h>
#include <kerosin/sceneserver/axis.h>
#include <kerosin/materialserver/material.h>
#include "constants.h"
#include "simspark.h"
#include "sparkcontext.h"

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

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

SparkGLRender::SparkGLRender()
{
}

SparkGLRender::~SparkGLRender()
{
}

void SparkGLRender::Init(shared_ptr<SimSpark> spark)
{
    mCamera.reset();
    mRenderServer.reset();

    mSpark = spark;
    if (mSpark.get() == 0)
        {
            return;
        }

    mCamera = shared_dynamic_cast<Camera>
        (mSpark->GetCore()->Get("/usr/scene/camera/camera"));

    if (mCamera.get() == 0)
        {
            mSpark->GetLog()->Error()
                << "(SparkGLRender) ERROR: camera node not found\n";
        }

    mRenderServer = shared_dynamic_cast<RenderServer>
        (mSpark->GetCore()->Get("/sys/server/render"));

    if (mRenderServer.get() == 0)
        {
            mSpark->GetLog()->Error()
                << "(SparkGLRender) ERROR: RenderServer not found\n";
        }

    mSceneServer = shared_dynamic_cast<SceneServer>
        (mSpark->GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            mSpark->GetLog()->Error()
                << "(SparkGLRender) ERROR: SceneServer not found\n";
        }
}

void SparkGLRender::Render(int width, int height)
{
    if (
        (mCamera.get() == 0) ||
        (mRenderServer.get() == 0)
        )
        {
            return;
        }

    mCamera->SetViewport(0,0,width,height);
    mRenderServer->Render(true);
    RenderSelection();
}

void SparkGLRender::RenderNodeSelection(shared_ptr<BaseNode> node)
{
    if (node.get() == 0)
        {
            return;
        }

    shared_ptr<SingleMatNode> sMatNode =
        shared_dynamic_cast<SingleMatNode>(node);

    if (sMatNode.get() != 0)
        {

            shared_ptr<Material> material = sMatNode->GetMaterial();
            sMatNode->SetMaterial("matSelected");

            RenderSelectionContext context(sMatNode);
            sMatNode->RenderInternal();

            if (material.get() != 0)
                {
                    sMatNode->SetMaterial(material->GetName());
                }

            return;
        }

    shared_ptr<RenderNode> renderNode =
        shared_dynamic_cast<RenderNode>(node);

    if (renderNode.get() != 0)
        {
            RenderSelectionContext context(renderNode);
            renderNode->RenderInternal();
            return;
        }

    shared_ptr<Joint> jointNode =
        shared_dynamic_cast<Joint>(node);

    if (jointNode.get() != 0)
    {
        for (
             int i=Joint::BI_FIRST;
             i<=Joint::BI_SECOND;
             ++i
             )
        {
            shared_ptr<RigidBody> body =
                jointNode->GetBody(static_cast<Joint::EBodyIndex>(i));

            if (body.get() == 0)
                {
                    continue;
                }

            RenderNodeSelection(body);
        }

        {
            RenderSelectionContext context(jointNode);
            Axis::RenderAxis(0.5);
        }

        return;
    }

    const salt::AABB3& bb = node->GetWorldBoundingBox();
    float size = std::max<float>(GLRENDER_MIN_AXIS_WIDTH, bb.GetRadius());

    {
        RenderSelectionContext context(node);
        Axis::RenderAxis(size);
    }
}

void SparkGLRender::RenderSelection()
{
    shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>
        (SparkContext::GetInstance().GetSelection().lock());

    if (node.get() == 0)
        {
            return;
        }

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    RenderNodeSelection(node);
}
