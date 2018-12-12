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

   SceneServer

   NOTE:

   HISTORY:
       05.11.02 - MK
                - Initial version

   TODO:

   TOFIX:
*/
#ifndef OXYGEN_SCENESERVER_H
#define OXYGEN_SCENESERVER_H

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include <oxygen/oxygen_defines.h>
#ifndef Q_MOC_RUN
#include <boost/thread/recursive_mutex.hpp>
#endif
#include "scene.h"

namespace oxygen
{
#if 0
}
#endif

class Scene;
class BaseNode;
class Transform;
class PhysicsServer;

/** The scene server manages displayable subtrees within the object
   hierarchy. Each subtree begins with a Scene node. The scene server knows
   which scene node is currently active and updates that node (and its
   corresponding subtree).
 */
class OXYGEN_API SceneServer : public zeitgeist::Node
{
    //
    // Functions
    //
public:
    SceneServer();
    ~SceneServer();

    /** creates a new scene hierarchy at a specific location, new
        hierarchy is also made current */
    bool CreateScene(const std::string &location);

    /** sets the active scene */
    bool SetActiveScene(const std::string &location);

    /** returns a reference to the current active scene */
    boost::shared_ptr<Scene> GetActiveScene() { return mActiveScene.get(); }

    /** updates the state of the current active scene (deltaTime is in
        seconds) */
    void Update(float deltaTime);

    void PrePhysicsUpdate(float deltaTime);

    void PhysicsUpdate(float deltaTime);

    void PostPhysicsUpdate();

    /** imports a scene from a file below the given BaseNode */
    bool ImportScene(const std::string& fileName,
                     boost::shared_ptr<BaseNode> root,
                     boost::shared_ptr<zeitgeist::ParameterList> parameter);

    /** creates an instance of \param importerName and registers it as
        a SceneImporter to the SceneServer
    */
    bool InitSceneImporter(const std::string& importerName);

    /** returns the current label for modified transform nodes */
    static int GetTransformMark();

protected:
    //
    // Members
    //

    /** gets missing references */
    void UpdateCache();

    /** resets all cached references */
    void ResetCache();
    
    /** Retrieves the PhysicsServer from the SceneGraph */
    virtual void OnLink();

    /** resets all cached references */
    virtual void OnUnlink();

    /** reparents all children of node to the parent of node; Their
        local transform matrix is multiplied with the local transform
        matrix of node
     */
    void ReparentTransformChildren(boost::shared_ptr<Transform> node);

    /** recursively reparents all transform nodes whose only children
        are also transform node
     */
    void RemoveTransformPaths(boost::shared_ptr<zeitgeist::Leaf> root);

    /** Pointer to the instance of the physicsserver */
    //causes crash at runtime if declared
    boost::shared_ptr<PhysicsServer> mPhysicsServer;

private:
    /** the current active scene */
    CachedPath<Scene> mActiveScene;

    /** modified transform nodes are labeled with this value, the
        value is incremented each cycle to avoid a resetting
     */
    static int mTransformMark;

    // mutex of the ODE
    boost::recursive_mutex mMutex;
};

DECLARE_CLASS(SceneServer);

} //namespace oxygen

#endif //OXYGEN_SCENESERVER_H
