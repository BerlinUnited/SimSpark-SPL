/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sceneserver.h,v 1.1 2003/08/29 14:12:56 fruit Exp $

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

   SceneServer

   The scene server manages displayable subtrees within the object
   hierarchy. Each subtree begins with a Scene node. The scene server
   knows which scene node is currently active and updates/displays
   that node (and its corresponding subtree).

   NOTE:

   HISTORY:
       05.11.02 - MK
                - Initial version

   TODO:

   TOFIX:
*/
#ifndef KEROSIN_SCENESERVER_H
#define KEROSIN_SCENESERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{
#if 0
}
#endif

class Scene;

class SceneServer : public zeitgeist::Leaf
{
    //
    // Functions
    //
public:
    SceneServer();
    ~SceneServer();

    //! create a new scene hierarchy at a specific location, new hierarchy is also made current
    boost::shared_ptr<Scene> CreateScene(const std::string &location);
    //! set the active scene
    bool SetActiveScene(const std::string &location);

    //! returns a reference to the current active scen
    boost::shared_ptr<Scene> GetActiveScene() { mActiveScene; }

    //! update the state of the current active scene (deltaTime is in seconds)
    void Update(float deltaTime);

protected:
    //
    // Members
    //
private:
    boost::shared_ptr<Scene>        mActiveScene;
};

DECLARE_CLASS(SceneServer);

} //namespace kerosin
#endif //KEROSIN_SCENESERVER_H
