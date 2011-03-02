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

#include "baserenderserver.h"

#include <salt/frustum.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/glbase.h>
#include <kerosin/openglserver/openglwrapper.h>
#include <oxygen/sceneserver/camera.h>

namespace kerosin
{
    class OpenGLServer;
    class RenderNode;

class KEROSIN_API RenderServer : public BaseRenderServer
{
    //
    // Functions
    //
public:
    typedef std::map<GLuint, boost::weak_ptr<kerosin::RenderNode> > TGLNameMap;

public:
    RenderServer();
    virtual ~RenderServer() {}

    //! display the current active scene
    virtual void Render(bool clean);

    //! @return the width of the current view
    virtual int Width() const;
    //! @return the width of the height view
    virtual int Height() const;
    //! Read the current screen content into a buffer
    bool CopyFrame(char* buffer) const;

    //! render the scene with fancy lighting
    void RenderFancyLighting(const salt::Frustum& frustum,
                             boost::shared_ptr<oxygen::Camera>& camera,
                             TLeafList& myLights, TLeafList& allMeshes,
                             TLeafList& visibleMeshes);

    /** sets the ambient color of the scene */
    void SetAmbientColor(const RGBA& ambient);

    /** update variables from a script */
    virtual void UpdateCached();

    /** If enabled the next render pass is used to pick an object */
    void EnablePicking(bool enable, const salt::Vector2f& pickAt, const double pickRange);

    /** Disables object picking for the next render pass */
    void DisablePicking();

    /** Returns the last picked node */
    boost::weak_ptr<RenderNode> GetPickedNode() const;

    /** Select next camera */
    void NextCamera();

    /** Select previous camera */
    void PreviousCamera();

protected:
    /** render a scene recursively.
        \param node the scene base node
    */
    void RenderScene(boost::shared_ptr<oxygen::BaseNode> node, unsigned pass);

    /** sets the OpenGl view parameters for a camera
     */
    void BindCamera(boost::shared_ptr<oxygen::Camera>& camera);

    /** clears last intermediate picking state and prepares OpenGL
        picking
    */
    void PreparePicking();

    /** processes the collected picking information */
    void ProcessPicks();

    //
    // Members
    //
protected:
    /** reference to the SceneServer */
    //CachedPath<oxygen::SceneServer> mSceneServer;

    /** the OpenGL ambient clear color */
    RGBA mAmbientColor;

    /** OpenGL name to RenderNode map for object picking */
    TGLNameMap mNameMap;

    /** counter to assign OpenGL names to RenderNodes while picking */
    GLuint mNextName;

    /** true, iff picking is enabled */
    bool mEnablePicking;

    /** the picking position in OpenGL window coordinates */
    salt::Vector2f mPickAt;
    /** the pick radius around mPickAt */
    double mPickRange;
    /** the picked result node */
    boost::weak_ptr<RenderNode> mPickedNode;
};

DECLARE_CLASS(RenderServer);

} //namespace kerosin

#endif // KEROSIN_RENDERSERVER_H
