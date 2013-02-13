/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include <oxygen/physicsserver/physicsserver.h>
#include <oxygen/physicsserver/int/physicsserverint.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <zeitgeist/logserver/logserver.h>
#include <iostream>

using namespace oxygen;
using namespace boost;
using namespace std;

boost::shared_ptr<PhysicsServerInt> PhysicsServer::mPhysicsServerImp;

PhysicsServer::PhysicsServer() : Leaf()
{

}

void PhysicsServer::ResetCache()
{
    mActiveSpace.reset();
    mActiveWorld.reset();
}

void PhysicsServer::OnLink()
{ 
    if (mPhysicsServerImp.get() == 0)
        mPhysicsServerImp = dynamic_pointer_cast<PhysicsServerInt>
            (GetCore()->New("PhysicsServerImp"));
        
    mPhysicsServerImp->InitEngine();  
}

void PhysicsServer::UpdateCache(boost::shared_ptr<Scene> activeScene)
{
    if (mActiveSpace.get() == 0)
        {
            // cache the space reference
            mActiveSpace = dynamic_pointer_cast<Space>
                (activeScene->GetChildOfClass("Space"));
        }

    if (mActiveWorld.get() == 0)
        {
            // cache the world reference
            mActiveWorld = dynamic_pointer_cast<World>
                (activeScene->GetChildOfClass("World"));
        }
}

void PhysicsServer::DoCollisions()
{
    if (mActiveSpace.get() != 0)
        {
            mActiveSpace->Collide();
        }
}

void PhysicsServer::StepSimulation(float deltaTime)
{
    if (mActiveWorld.get() != 0)
        {
            mActiveWorld->Step(deltaTime);
        }
}

void PhysicsServer::SetUpBox(boost::shared_ptr<RigidBody> body, string name)
{
    if (body.get() != 0)
        {
            body->SetName(name);
        }   
}
