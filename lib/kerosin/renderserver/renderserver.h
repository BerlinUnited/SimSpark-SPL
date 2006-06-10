/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: renderserver.h,v 1.10 2006/06/10 16:36:15 jboedeck Exp $

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

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   RenderServer

   NOTE:

   HISTORY:
       25.08.03 - MR
       - Initial version

   TODO:

   TOFIX:
*/
#ifndef KEROSIN_RENDERSERVER_H
#define KEROSIN_RENDERSERVER_H

#include <salt/frustum.h>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace oxygen
{
    class Scene;
    class SceneServer;
    class Camera;
    class BaseNode;
}

namespace kerosin
{
    class OpenGLServer;

class RenderServer : public zeitgeist::Leaf
{
    //
    // Functions
    //
public:
    RenderServer();
    ~RenderServer();

    //! display the current active scene
    void Render();

    //! render the scene with fancy lighting
    void RenderFancyLighting(const salt::Frustum& frustum,
                             boost::shared_ptr<oxygen::Camera>& camera,
                             TLeafList& myLights, TLeafList& allMeshes,
                             TLeafList& visibleMeshes);

protected:
    /** get the active scene node from the sceneServer */
    bool GetActiveScene();

    /** set up SceneServer reference */
    virtual void OnLink();

    /** reset SceneServer reference */
    virtual void OnUnlink();

    /** render a scene recursively.
        \param node the scene base node
    */
    void RenderScene(boost::shared_ptr<oxygen::BaseNode> node);

    /** sets the OpenGl view parameters for a camera
     */
    void BindCamera(boost::shared_ptr<oxygen::Camera>& camera);

    //
    // Members
    //
protected:
    /** reference to the current active scene */
    boost::shared_ptr<oxygen::Scene> mActiveScene;

    /** reference to the SceneServer */
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;

    /** reference to the OpenGLServer */
    boost::shared_ptr<kerosin::OpenGLServer> mOpenGLServer;

    unsigned int mAmbientVP;
};

DECLARE_CLASS(RenderServer);

} //namespace kerosin

#endif // KEROSIN_RENDERSERVER_H
