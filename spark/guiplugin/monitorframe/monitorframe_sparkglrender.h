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

#ifndef MONITORFRAME_SPARKGLRENDER_H
#define MONITORFRAME_SPARKGLRENDER_H
/**
 * \file monitorframe_sparkglrender.h
 * \brief Contains MonitorFrameUtil::SparkGLRender class.
 */

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif

#include <iostream>

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
    class RenderControl;
    class RenderNode;
}

namespace MonitorFrameUtil
{

/*!
 \brief Renderer for SimSpark internal data and custom render nodes.

 The renderer is the external render object for the OpenGlSysQt that is registered to SimSpark.
 Since the rendering here is done in main thread (not the simulation server thread), the renderer does not need to communicate with the opengl system directly.
 
 The renderer uses the camera initialized in the SimSpark scene graph to calculate the rendering position, and it uses the scene server an render server and 
 render control to render the scene and the CustomRender nodes registered to Simspark.
 
 \warning This external renderer can only be used if the used SimSpark simulation does not contain plugins that rely on owning the rendering context, like for example
 vision perceptor plugins who have to render the scene themselves.
*/
class SparkGLRender
{
protected:
    //protected types

    /*!
     \brief Self-releasing rendering context.

     Creation pushes a nodes world transform on the matrix stack.
     Destruction pops the matrix.
    */
    struct RenderSelectionContext
    {
        /*!
         \brief Constructor.

         Creation pushes the given base nodes world transform on OpenGl matrix stack.
         \param node
        */
        RenderSelectionContext(boost::shared_ptr<oxygen::BaseNode> node);
        /*!
         \brief Destructor.

         Destruction pops gl matrix.
        */
        ~RenderSelectionContext();
    };

public:
    //public functions

    /*!
     \brief Complete constructor.

     \param cameraPath path of the camera in the SimSpark scene graph
     \param sceneServerPath path of the scene server in the SimSpark scene graph
     \param renderServerPath path of the render server in the SimSpark scene graph
     \param renderControlPath path of the render control node in the SimSpark scene graph
    */
    SparkGLRender(const std::string& cameraPath, const std::string& sceneServerPath,
        const std::string& renderServerPath, const std::string& renderControlPath);
    /*!
     \brief Destructor.
    */
    virtual ~SparkGLRender();

    /*!
     \brief Initializes rendering.

     \param spark SimSpark object pointer
    */
    bool init(boost::shared_ptr<SimSpark> spark);
    /*!
     \brief Render with a viewport of the given size.

     \param width vieport width
     \param height viewport height
    */
    void render(int width, int height);

    /*!
     \brief Specify ambient color.

     \param r red
     \param g green
     \param b blue
     \param a alpha
    */
    void setAmbientColor(float r, float g, float b, float a);

    /*!
     \brief Try to pick a node at the specified position.

     Picking will render the scene in a small radius around the pick position and return the base node displayed at the frontmost rendered
     pixel.
     Uses OpenGls selection rendering mode.
     \param x x pick position
     \param y y pick position
     \param width viewport width
     \param height viewport height
     \return picked node or null pointer if there is only backgorund
    */
    boost::weak_ptr<kerosin::RenderNode> pickNode(int x, int y, int width, int height);

protected:
    //protected functions

    /*!
     \brief Render the SimSpark scene graphs BaseNodes (calling the renderserver) and renders CustomRender objects.

     \warning Removing the RenderServer without stopping the rendering here might cause a crash.
    */
    void render();

protected:
    //protected members

    boost::shared_ptr<SimSpark> mSpark; /*!< Pointer to SimSpark. */
    boost::shared_ptr<oxygen::Camera> mCamera; /*!< Pointer to Camera. */
    boost::shared_ptr<oxygen::SceneServer> mSceneServer; /*!< Pointer to scene server. */
    boost::shared_ptr<kerosin::RenderServer> mRenderServer; /*!< Pointer to render server. */
    boost::shared_ptr<kerosin::RenderControl> mRenderControl; /*!< Pointer to render control. */

    std::string mCameraPath;        /*!< Absolute path of camera in zeitgeist scene graph. */
    std::string mSceneServerPath;   /*!< Absolute path of scene server in zeitgeist scene graph. */
    std::string mRenderServerPath;  /*!< Absolute path of render server in zeitgeist scene graph. */
    std::string mRenderControlPath; /*!< Absolute path of render control node in zeitgeist scene graph. */

    int mWidth;  /*!< Viewport width. */
    int mHeight; /*!< Viewport height. */

    int mErr; /*!< OpenGl error code storage. */
};

} // namespace MonitorFrameUtil

#endif // MONITORFRAME_SPARKGLRENDER_H
