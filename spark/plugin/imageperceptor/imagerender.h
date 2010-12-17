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

#ifndef IMAGERENDER_H
#define	IMAGERENDER_H

#include <oxygen/sceneserver/camera.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <kerosin/renderserver/baserenderserver.h>
#include <kerosin/renderserver/customrender.h>
#include <kerosin/openglserver/openglserver.h>
#include <boost/shared_array.hpp>

class ImageRender: public kerosin::CustomRender
{
public:
  ImageRender();

  ~ImageRender();

  void Render();

  void SetCamera(boost::shared_ptr<oxygen::Camera> camera);

  void OnLink();
  
  void OnUnlink();

  const char* getData() const { return mData.get(); }

  int getDataSize() const { return mDataSize; }

  int getWidth() const { return mWidth; }
  
  int getHeight() const { return mHeight; }

private:
  boost::shared_ptr<oxygen::Camera> mCamera;

  /** cached reference to the RenderServer */
  zeitgeist::Leaf::CachedPath<kerosin::BaseRenderServer> mRenderServer;

  /** cached reference to the OpenGLServer */
  zeitgeist::Leaf::CachedPath<kerosin::OpenGLServer> mOpenGLServer;

  boost::shared_array<char> mData;
  int mDataSize;
  int mWidth;
  int mHeight;

  unsigned int mFBOId;
  unsigned int mRBOId;
  unsigned int mDepthBuffer;
};

DECLARE_CLASS(ImageRender);

#endif	/* IMAGERENDER_H */

