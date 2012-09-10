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

#include "imagerender.h"
#include <zeitgeist/logserver/logserver.h>


// using namespace kerosin;
using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

unsigned int ImageRender::autoScreenPosX = 0;
unsigned int ImageRender::autoScreenPosY = 0;

ImageRender::ImageRender(): kerosin::CustomRender(),
  mDataSize(0), mWidth(0), mHeight(0), mRequested(false),mOffScreen(false),
  mScreenPosX(0), mScreenPosY(0)
{
}

ImageRender::~ImageRender()
{
}
#define REG_GL_EXT_FUN(_ptr, _function)                     \
    static _ptr _function = (_ptr) mOpenGLServer->GetExtension(#_function);\
    if ( !_function ){                                                  \
        GetLog()->Error()<<"(ImageRender) ERROR: can not get "#_function"\n"; \
    }

void ImageRender::SetCamera(boost::shared_ptr<oxygen::Camera> camera)
{
  mCamera = camera;
}

void ImageRender::OnLink()
{
    RegisterCachedPath(mRenderServer, "/sys/server/render");

    if (mRenderServer.expired())
        {
            GetLog()->Error()
                << "(ImageRender) ERROR: RenderServer not found\n";
        }

    RegisterCachedPath(mOpenGLServer,"/sys/server/opengl");

    if (mOpenGLServer.expired())
        {
            GetLog()->Error()
                << "(ImageRender) ERROR: OpenGLServer not found\n";
        }

    // create a framebuffer object
    REG_GL_EXT_FUN( PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT );
    REG_GL_EXT_FUN( PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);

    glGenFramebuffersEXT(1, &mFBOId);
    glGenRenderbuffersEXT(1, &mRBOId);
    glGenRenderbuffersEXT(1, &mDepthBuffer);
}

void ImageRender::OnUnlink()
{
    REG_GL_EXT_FUN( PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT );
    REG_GL_EXT_FUN( PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT );
    glDeleteFramebuffersEXT(1, &mFBOId);
    glDeleteRenderbuffersEXT(1, &mRBOId);
}

void ImageRender::Render()
{
    if ( (!(mRequested || !mOffScreen )) ||
        (mOpenGLServer.expired()) ||
        (mRenderServer.expired())
        )
    {
        return;
    }

    int w = mCamera->GetViewportWidth();
    int h = mCamera->GetViewportHeight();

  if (mOffScreen)
  {
    REG_GL_EXT_FUN(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
    REG_GL_EXT_FUN(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
    REG_GL_EXT_FUN(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT);
    REG_GL_EXT_FUN(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
    REG_GL_EXT_FUN(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT);


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
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
      GetLog()->Error() << "(ImageRender) fbo error!\n";
    }

  }

    // setup the camera
    boost::shared_ptr<Camera> oldCamera = mRenderServer->GetCamera();
    mRenderServer->SetCamera(mCamera);
    mRenderServer->Render(false);

  if (!mOffScreen)
  {
    glReadBuffer(GL_BACK);
  }

    int size = w*h*3;
    if ( size != mDataSize )
    {
      mData = boost::shared_array<char>(new char[size]);
      mDataSize = size;
    }
    
    glReadPixels(mScreenPosX, mScreenPosY,
                 w, h,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 mData.get());
    mWidth = w;
    mHeight = h;

  if (mOffScreen)
  {
    // unbind FBO
    REG_GL_EXT_FUN(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

    mRenderServer->SetCamera(oldCamera);

    mRequested = false;
}

void ImageRender::SetResolution(int w, int h)
{
  if (!mOffScreen) // HACK!
  {
    // asssert this function is called only once per instance
    // auto position on screen (simple)
    mScreenPosX = autoScreenPosX;
    int screenWidth = mRenderServer->GetCamera()->GetViewportWidth();
    if (mScreenPosX + w > screenWidth)
    {
      mScreenPosX = 0;
      autoScreenPosY += h;
    }
    autoScreenPosX = mScreenPosX + w;
    mScreenPosY = autoScreenPosY;
    //
  }

  mCamera->SetViewport(mScreenPosX, mScreenPosY, w, h);
}