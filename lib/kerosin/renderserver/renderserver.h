/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: renderserver.h,v 1.2 2003/08/26 10:57:18 rollmark Exp $

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

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{
#if 0
}
#endif

class SceneServer;
class Scene;


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

protected:
    /** overload this to perform stuff after the object has
        been created and attached to a core
    */
    virtual bool ConstructInternal();

    /** get the active scene node from the sceneServer */
    void RenderServer::GetActiveScene();


    //
    // Members
    //
private:
    boost::shared_ptr<Scene> mActiveScene;
    boost::shared_ptr<SceneServer> mSceneServer;
    unsigned int mAmbientVP;
};

DECLARE_CLASS(RenderServer);

} //namespace kerosin

#endif // KEROSIN_RENDERSERVER_H
