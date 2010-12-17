/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Tue May 9 2006
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2007 RoboCup Soccer Server 3D Maintenance Group
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

   BaseRenderServer

   NOTE: This is an abstract renderserver in order to support different render servers

   HISTORY:
       29/03/07 - OO  - Initial version

*/
#ifndef KEROSIN_BASERENDERSERVER_H
#define KEROSIN_BASERENDERSERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>

namespace oxygen
{
    class Scene;
    class SceneServer;
    class Camera;
    class BaseNode;
}

namespace kerosin
{

class KEROSIN_API BaseRenderServer : public zeitgeist::Leaf
{
public:
    virtual ~BaseRenderServer() {}

    //! display the current active scene
    virtual void Render(bool clean) = 0;

    //! width of the render target
    virtual int Width() const { return 0; }
    //! height of the render target
    virtual int Height() const { return 0; }
    //! get a shot of the current frame
    virtual bool CopyFrame(char* /* buffer */) const { return false; }

    void SetCamera( boost::shared_ptr<oxygen::Camera> c )
        {
            mCamera = c;
        }

    boost::shared_ptr<oxygen::Camera> GetCamera()
        {
            return mCamera;
        }

protected:
    /** get the active scene node from the sceneServer */
    virtual bool GetActiveScene();

    /** set up SceneServer reference */
    virtual void OnLink();

    /** reset SceneServer reference */
    virtual void OnUnlink();

    /** update variables from a script */
    virtual void UpdateCached();

    //
    // Members
    //
protected:
    /** reference to the current active scene */
    boost::shared_ptr<oxygen::Scene> mActiveScene;

    /** reference to the SceneServer */
    CachedPath<oxygen::SceneServer> mSceneServer;

    /** the currently selected camera */
    boost::shared_ptr<oxygen::Camera> mCamera;
};

DECLARE_ABSTRACTCLASS(BaseRenderServer);

} //namespace kerosin

#endif // KEROSIN_BASERENDERSERVER_H
