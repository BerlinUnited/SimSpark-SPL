/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkglrender.h,v 1.1 2008/02/24 16:20:24 rollmark Exp $

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
#ifndef SPARKGLRENDER_H__
#define SPARKGLRENDER_H__

#ifdef WIN32
#include <winsock2.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class SimSpark;

namespace oxygen
{
    class Camera;
    class SceneServer;
    class BaseNode;
}

namespace kerosin
{
    class RenderServer;
}

class SparkGLRender
{
protected:
    struct RenderSelectionContext
    {
        RenderSelectionContext(boost::shared_ptr<oxygen::BaseNode> node);
        ~RenderSelectionContext();
    };

public:
    SparkGLRender();
    virtual ~SparkGLRender();

    void Init(boost::shared_ptr<SimSpark> spark);
    void Render(int width, int height);

protected:
    void RenderNodeSelection(boost::shared_ptr<oxygen::BaseNode> node);
    void RenderSelection();

protected:
    boost::shared_ptr<SimSpark> mSpark;
    boost::shared_ptr<oxygen::Camera> mCamera;
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    boost::shared_ptr<kerosin::RenderServer> mRenderServer;
};

#endif // SPARKGLRENDER_H__
