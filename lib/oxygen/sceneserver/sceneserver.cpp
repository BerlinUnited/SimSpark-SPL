/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sceneserver.cpp,v 1.12 2004/04/11 17:08:02 rollmark Exp $

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
#include "scene.h"
#include "sceneimporter.h"
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;
using namespace std;

SceneServer::SceneServer() : Node()
{
}

SceneServer::~SceneServer()
{
}

boost::shared_ptr<Scene> SceneServer::CreateScene(const std::string &location)
{
    shared_ptr<CoreContext> context = GetCore()->CreateContext();

    shared_ptr<Scene> scene = shared_static_cast<Scene>
        (context->New("oxygen/Scene", location));

    if (scene.get() != 0)
        {
            ResetCache();
            mActiveScene = scene;
        }

    return scene;
}

bool SceneServer::SetActiveScene(const std::string &location)
{
    shared_ptr<Scene> scene =
        shared_dynamic_cast<Scene>(GetCore()->Get(location));

    if (scene.get() != 0)
        {
            ResetCache();
            mActiveScene = scene;
            return true;
        }

    return false;
}

void SceneServer::ResetCache()
{
    mActiveScene.reset();
    mActiveSpace.reset();
    mActiveWorld.reset();
}

void SceneServer::UpdateCache()
{
    if (mActiveScene.get() == 0)
        {
            ResetCache();
            return;
        }

    if (mActiveSpace.get() == 0)
        {
            // cache the space reference
            mActiveSpace = shared_dynamic_cast<Space>
                (mActiveScene->GetChildOfClass("Space"));
        }

    if (mActiveWorld.get() == 0)
        {
            // cache the world reference
            mActiveWorld = shared_dynamic_cast<World>
                (mActiveScene->GetChildOfClass("World"));
        }
}

void SceneServer::OnUnlink()
{
    ResetCache();
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

    mActiveScene->PrePhysicsUpdate(deltaTime);

    // determine collisions
    if (mActiveSpace.get() != 0)
        {
            mActiveSpace->Collide();
        }

    // do physics
    if (mActiveWorld.get() != 0)
        {
            mActiveWorld->Step(deltaTime);
        }

    mActiveScene->PostPhysicsUpdate();
    mActiveScene->UpdateHierarchy();
}

bool SceneServer::ImportScene(const string& fileName, shared_ptr<BaseNode> root,
                              shared_ptr<ParameterList> parameter)
{
    if (! GetFile()->Exist(fileName))
        {
            GetLog()->Error() << "(SceneServer) ERROR: cannot locate file '"
                              << fileName << "'\n";

            return false;
        }

    TLeafList importer;
    ListChildrenSupportingClass<SceneImporter>(importer);

    if (importer.size() == 0)
        {
            GetLog()->Error()
                << "(SceneServer) Warning: no SceneImporter registered\n";
        }

    for (
         TLeafList::iterator iter = importer.begin();
         iter != importer.end();
         ++iter
         )
        {
            shared_ptr<SceneImporter> importer =
                shared_static_cast<SceneImporter>(*iter);

            if (importer->ImportScene(fileName,root,parameter))
                {
                    GetLog()->Normal()
                        << "(SceneServer) imported scene file '"
                        << fileName << " with '"
                        << importer->GetName() << "'\n";
                    return true;
                }
        }

    GetLog()->Error() << "(SceneServer) ERROR: cannot import scene file '"
                      << fileName << "'\n";

    return false;
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


