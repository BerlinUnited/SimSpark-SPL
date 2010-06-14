/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

// #include <IL/il.h>
// #include <IL/ilu.h>
// #include <sstream>
#ifndef WIN32
#if __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#else
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "imageperceptor.h"
#include <zeitgeist/logserver/logserver.h>

#define USE_FBO

// using namespace kerosin;
using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

ImagePerceptor::ImagePerceptor() : oxygen::Perceptor()
{
    mFramesRendered = 0;
}

ImagePerceptor::~ImagePerceptor()
{
}

#define REG_GL_EXT_FUN(_ptr, _function)                     \
    static _ptr _function = (_ptr) mOpenGLServer->GetExtension(#_function); \
    if ( !_function ){                                                  \
        GetLog()->Error()<<"(Image Perceptor) ERROR: can not get "#_function"\n"; \
    }

void ImagePerceptor::OnLink()
{
    mCamera = shared_dynamic_cast<Camera>( GetCore()->New("oxygen/Camera") );
    if ( 0 != mCamera.get() )
    {
        AddChildReference(mCamera);
    }
    else
    {
        GetLog()->Error()
            <<"(ImagePerceptor) ERROR: can not create camera\n";
    }
    
    RegisterCachedPath(mRenderServer, "/sys/server/render");

    if (mRenderServer.expired())
        {
            GetLog()->Error()
                << "(ImagePerceptor) ERROR: RenderServer not found\n";
        }

    RegisterCachedPath(mOpenGLServer,"/sys/server/opengl");

    if (mOpenGLServer.expired())
        {
            GetLog()->Error()
                << "(ImagePerceptor) ERROR: OpenGLServer not found\n";
        }
    
    // create a framebuffer object
    REG_GL_EXT_FUN( PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT );
    REG_GL_EXT_FUN( PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);

    glGenFramebuffersEXT(1, &mFBOId);
    glGenRenderbuffersEXT(1, &mRBOId);
    glGenRenderbuffersEXT(1, &mDepthBuffer);

}

void ImagePerceptor::OnUnlink()
{
    REG_GL_EXT_FUN( PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT );
    REG_GL_EXT_FUN( PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT );
    glDeleteFramebuffersEXT(1, &mFBOId);
    glDeleteRenderbuffersEXT(1, &mRBOId);
}

bool ImagePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if ( !Render() )
        return false;
    
    Predicate &predicate = predList->AddPredicate();
    predicate.name = "Image";
    predicate.parameter.Clear();
    
    // ParameterList &sizeElement = predicate.parameter.AddList();
    // sizeElement.AddValue(std::string("s"));
    // sizeElement.AddValue(mCamera->GetViewportWidth());
    // sizeElement.AddValue(mCamera->GetViewportHeight());

    predicate.parameter.AddValue(mData);
    return true;
}

bool ImagePerceptor::Render()
{
    if (
        (mOpenGLServer.expired()) ||
        (mRenderServer.expired())
        )
    {
        return false;
    }
    
    int w = mCamera->GetViewportWidth();
    int h = mCamera->GetViewportHeight();

#ifdef USE_FBO
    REG_GL_EXT_FUN( PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT );
    REG_GL_EXT_FUN( PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT );
    REG_GL_EXT_FUN( PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT );
    REG_GL_EXT_FUN( PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
    REG_GL_EXT_FUN( PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT );

    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOId);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRBOId);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB, w, h);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                 GL_RENDERBUFFER_EXT, mRBOId);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mDepthBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mDepthBuffer);


    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
        GetLog()->Error()<<"fbo error!\n";
    }
#endif
    
    // setup the camera
    boost::shared_ptr<Camera> oldCamera = mRenderServer->GetCamera();
    mRenderServer->SetCamera(mCamera);
    mRenderServer->Render();

#ifdef USE_FBO
#else
    glReadBuffer(GL_BACK);
#endif
    int size = 3*w*h;
    if ( mData.size() != size )
        mData.resize(size);
    glReadPixels(0, 0,
                 w, h,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 const_cast<char*>(mData.data()));
    
    ++mFramesRendered;
#ifdef USE_FBO
    // unbind FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
    mRenderServer->SetCamera(oldCamera);

    return true;
}

void ImagePerceptor::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    if ( 0!= mCamera ){
        mCamera->SetViewport(x,y,w,h);
    }
}

void ImagePerceptor::SetFOV(float fov)
{
    if ( 0!= mCamera ){
        mCamera->SetFOV(fov);
    }
}

void ImagePerceptor::SetZNear(float zNear)
{
    if ( 0!= mCamera ){
        mCamera->SetZNear(zNear);
    }
}

void ImagePerceptor::SetZFar(float zFar)
{
    if ( 0!= mCamera ){
        mCamera->SetZFar(zFar);
    }
}
