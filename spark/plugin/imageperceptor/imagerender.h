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
#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif

class ImageRender: public kerosin::CustomRender
{
public:
  ImageRender();

  ~ImageRender();

  void Render();

  void SetCamera(boost::shared_ptr<oxygen::Camera> camera);

  void OnLink();
  
  void OnUnlink();

  const char* GetData() const { return mData.get(); }

  int GetDataSize() const { return mDataSize; }

  int GetWidth() const { return mWidth; }
  
  int GetHeight() const { return mHeight; }

  void RequestRender() { mRequested = true; }

  void SetResolution(int w, int h);

  void SetOffScreen(bool offScreen) { mOffScreen = offScreen; }

  void SetScreenPos(int x, int y) { mScreenPosX = x; mScreenPosY = y; }

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
  bool mRequested;
  bool mOffScreen;
  int mScreenPosX, mScreenPosY;

  unsigned int mFBOId;
  unsigned int mRBOId;
  unsigned int mDepthBuffer;

  static unsigned int autoScreenPosX;
  static unsigned int autoScreenPosY;
};

DECLARE_CLASS(ImageRender);

#endif	/* IMAGERENDER_H */

