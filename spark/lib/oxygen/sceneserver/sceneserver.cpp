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
*/

#include "sceneserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/corecontext.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <salt/vector.h>
#include <salt/frustum.h>
#include "transform.h"
#include "scene.h"
#include "sceneimporter.h"
#include "scenedict.h"
#include <oxygen/physicsserver/physicsserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;
using namespace std;

int SceneServer::mTransformMark = 0;

SceneServer::SceneServer() : Node()
{   
}

SceneServer::~SceneServer()
{
}

void SceneServer::OnLink()
{
    shared_ptr<CoreContext> context = GetCore()->CreateContext();

    mPhysicsServer = shared_static_cast<PhysicsServer>
        (context->Get("/sys/server/physics"));
        
    if (mPhysicsServer.get() == 0)
        {
            GetLog()->Error() << 
                "(SceneServer) ERROR: PhysicsServer not found at /sys/server/physics\n";
        }
    else
        {
            GetLog()->Normal() <<
                "(SceneServer) Found PhysicsServer\n";
                
            mPhysicsServer->ConfirmExistence();
        }
}

bool SceneServer::CreateScene(const std::string &location)
{
    shared_ptr<CoreContext> context = GetCore()->CreateContext();

    shared_ptr<Scene> scene = shared_static_cast<Scene>
        (context->New("oxygen/Scene", location));

    ResetCache();
    RegisterCachedPath(mActiveScene, location);
    return (scene.get() != 0);
}

bool SceneServer::SetActiveScene(const std::string &location)
{
    ResetCache();
    RegisterCachedPath(mActiveScene, location);
    return (mActiveScene.get() != 0);
}

void SceneServer::ResetCache()
{
    mPhysicsServer->ResetCache();
}

void SceneServer::UpdateCache()
{
    if (mActiveScene.get() == 0)
        {
            ResetCache();
            return;
        }
        
    mPhysicsServer->UpdateCache(mActiveScene.get());
}

void SceneServer::OnUnlink()
{
    ResetCache();
}

int SceneServer::GetTransformMark()
{
    return mTransformMark;
}

/** Update the scene graph. We do not make the distinction between
    'controllers' and 'scene graph nodes'. This design forces us to
    perform several update passes through the hierarchy. Nevertheless,
    the more homogenuous structure is worth it.

    An example Hierarchy:

    scene
    |-   transform
    |-   body
    |    |-  quakecontroller
    |
    |-   camera

    The following passes are performed:

    1st pass: PrePhysicsUpdate(deltaTime) The first update pass is
    responsible for updating the internal state of an object. For
    example, the quakecontroller has received input from the main
    application in terms of state changes like StrafeLeft, or Forward.
    As the controller is physics driven, it has to translate these
    commands to actual forces, which act on the body. This is done
    during this pass.

    After the first pass pyhsics simulation and collision are
    calculated using ODE.

    2nd pass: PostPhysicsUpdate()
    This pass can be used to affect other nodes. For example, 'Body'
    has undergone physics simulation and collision detection. It has
    reached its final 'simulated' position, which has to be passed to
    the parent Transform node.

    3rd pass: UpdateHierarchy()
    As the second pass affects the position of objects, we have to
    recalculate the transformation hierarchy before we can display the
    scene.
*/
void SceneServer::Update(float deltaTime)
{
    if (
        (deltaTime == 0.0f) ||
        (mActiveScene.get() == 0)
        )
        {
            return;
        }

    UpdateCache();
    ++mTransformMark;

    mActiveScene->PrePhysicsUpdate(deltaTime);

    // determine collisions
    mPhysicsServer->DoCollisions();

    // do physics
    mPhysicsServer->StepSimulation(deltaTime);

    mActiveScene->PostPhysicsUpdate();
    mActiveScene->UpdateHierarchy();
}

void SceneServer::PrePhysicsUpdate(float deltaTime)
{
    if (
        (deltaTime == 0.0f) ||
        (mActiveScene.get() == 0)
        )
        {
            return;
        }

    UpdateCache();
    ++mTransformMark;

    mActiveScene->PrePhysicsUpdate(deltaTime);
}

void SceneServer::PhysicsUpdate(float deltaTime)
{
    boost::recursive_mutex::scoped_lock lock(mMutex);
    // determine collisions
    mPhysicsServer->DoCollisions();

    // do physics
    mPhysicsServer->StepSimulation(deltaTime);
}

void SceneServer::PostPhysicsUpdate()
{
    if ( mActiveScene.get() == 0  )
        {
            return;
        }
    mActiveScene->PostPhysicsUpdate();
    mActiveScene->UpdateHierarchy();
}

bool SceneServer::ImportScene(const string& fileName, shared_ptr<BaseNode> root,
                              shared_ptr<ParameterList> parameter)
{
    string file;
    if (! GetFile()->LocateResource(fileName, file))
        {
            GetLog()->Error() << "(SceneServer) ERROR: cannot locate file '"
                              << fileName << "'\n";

            return false;
        }

    if (root.get() == 0)
        {
            GetLog()->Error()
                << "(SceneServer) ERROR: NULL node given as ImportScene "
                << "root node, fileName was " << fileName << "\n";
        }

    GetLog()->Debug() << "(SceneServer) ImportScene fileName=" << fileName
                      << " root=" << root->GetFullPath() << "\n";

    TLeafList importer;
    ListChildrenSupportingClass<SceneImporter>(importer);

    if (importer.empty())
        {
            GetLog()->Error()
                << "(SceneServer) Warning: no SceneImporter registered\n";
        }

    // because the importer will create ODE objects,
    // so we have to lock the ODE
    boost::recursive_mutex::scoped_lock lock(mMutex);

    for (
         TLeafList::iterator iter = importer.begin();
         iter != importer.end();
         ++iter
         )
        {
            shared_ptr<SceneImporter> importer =
                shared_static_cast<SceneImporter>(*iter);

            importer->SetSceneDict(&SceneDict::GetInstance());

            GetLog()->Debug()
                << "(SceneServer) trying importer " << importer->GetName() << std::endl;

            if (importer->ImportScene(file,root,parameter))
                {
                    GetLog()->Normal()
                        << "(SceneServer) imported scene file '"
                        << file << " with '"
                        << importer->GetName()
                        << " at " << root->GetFullPath() << endl;

                    RemoveTransformPaths(root);

                    // mark the corresponding scene as modified
                    shared_ptr<Scene> scene = root->GetScene();
                    if (scene.get() != 0)
                        {
                            scene->SetModified(true);
                        }

                    return true;
                }
        }

    GetLog()->Error() << "(SceneServer) ERROR: cannot import scene file '"
                      << file << "'\n";

    return false;
}

void SceneServer::ReparentTransformChildren(shared_ptr<Transform> node)
{
    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(node->GetParent().lock());

    // while not empty
    while (node->begin() != node->end())
        {
            shared_ptr<Leaf> child = (*node->begin());

            shared_ptr<Transform> tChild
                = shared_dynamic_cast<Transform>(child);

            if (tChild.get() != 0)
                {
                    tChild->SetLocalTransform
                        (node->GetLocalTransform() * tChild->GetLocalTransform());
                }

            child->Unlink();
            parent->AddChildReference(child);
        }
}

void SceneServer::RemoveTransformPaths(shared_ptr<Leaf> root)
{
    if (root.get() == 0)
        {
            return;
        }

    // go top down and remove Transform nodes that only have Transform
    // node children and reparent them; note that reparented transform
    // nodes are automatically checked in the parent loop, as they are
    // appended to the child list in subsequent calls to
    // RemoveTransformPaths; this process leaves some transform node
    // without children, that are removed in a second pass

    for (
         TLeafList::iterator iter = root->begin();
         iter != root->end();
         ++iter
         )
        {
            RemoveTransformPaths(*iter);
        }

    shared_ptr<Transform> trans = shared_dynamic_cast<Transform>(root);
    bool tRoot = (trans.get() != 0);
    bool tChildOnly = true;

    TLeafList::iterator iter = root->begin();
    while (iter != root->end())
        {
            shared_ptr<Transform> tChild = shared_dynamic_cast<Transform>(*iter);
            if (tChild.get() == 0)
                {
                    tChildOnly = false;
                } else
                {
                    if (tChild->begin() == tChild->end())
                        {
                            // remove a transform node without
                            // children
                            tChild->Unlink();

                            // restart as iter is now invalidated
                            iter = root->begin();
                            continue;
                        }
                }

            ++iter;
        }

    if ( (tRoot) && (tChildOnly))
        {
            ReparentTransformChildren(trans);
        }
}

bool SceneServer::InitSceneImporter(const std::string& importerName)
{
    shared_ptr<SceneImporter> importer
        = shared_dynamic_cast<SceneImporter>(GetCore()->New(importerName));

    if (importer.get() == 0)
        {
            GetLog()->Error() << "ERROR (SceneServer::InitSceneImporter) "
                              << "Unable to create '" << importerName << "'\n";
            return false;
        }

    importer->SetName(importerName);
    AddChildReference(importer);

    GetLog()->Normal()
        << "(SceneServer) SceneImporter '" << importerName << "' registered\n";

    return true;
}
