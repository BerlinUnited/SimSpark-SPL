/* -*- mode: c++ -*-

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

#include <oxygen/physicsserver/int/worldint.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

boost::shared_ptr<WorldInt> World::mWorldImp;

World::World() : PhysicsObject()
{

}

World::~World()
{
}

long World::GetWorldID() const
{
    return mWorldID;
}

void World::SetGravity(const Vector3f& gravity)
{
    mWorldImp->SetGravity(gravity, mWorldID);
}

salt::Vector3f World::GetGravity() const
{
    return mWorldImp->GetGravity(mWorldID);
}

void World::SetERP(float erp)
{
    mWorldImp->SetERP(erp, mWorldID);
}

float World::GetERP() const
{
    return mWorldImp->GetERP(mWorldID);
}

void World::SetCFM(float cfm)
{
    mWorldImp->SetCFM(cfm, mWorldID);
}

float World::GetCFM() const
{
    return mWorldImp->GetCFM(mWorldID);
}

void World::Step(float deltaTime)
{
    mWorldImp->Step(deltaTime, mWorldID);
}

bool World::GetAutoDisableFlag() const
{
    return mWorldImp->GetAutoDisableFlag(mWorldID);
}

void World::SetAutoDisableFlag(bool flag)
{
    mWorldImp->SetAutoDisableFlag(flag, mWorldID);
}

void World::SetContactSurfaceLayer(float depth)
{
    mWorldImp->SetContactSurfaceLayer(depth, mWorldID);
}

float World::GetContactSurfaceLayer() const
{
    return mWorldImp->GetContactSurfaceLayer(mWorldID);
}

bool World::ConstructInternal()
{
    if (mWorldImp.get() == 0)
        mWorldImp = dynamic_pointer_cast<WorldInt>
            (GetCore()->New("WorldImp"));

    mWorldID = mWorldImp->CreateWorld();

    return (mWorldID != 0);
}

void World::DestroyPhysicsObject()
{
    static bool recurseLock = false;
    if (recurseLock)
        {
            return;
        }

    recurseLock = true;

    if (mWorldID == 0)
        {
            return;
        }

    // first destory everthing in the world
    DestroyWorldObjects();

    // then destory the world
    // otherwise, things in the world will destroied without notifying us
    mWorldImp->DestroyWorld(mWorldID);

    mWorldID = 0;
    recurseLock = false;
}


void World::DestroyWorldObjects()
{
  boost::shared_ptr<Scene> scene = GetScene();

  if (scene.get() == 0)
  {
    return;
  }

  TLeafList objects;
  const bool recursive = true;
  scene->ListChildrenSupportingClass<PhysicsObject>(objects, recursive);

  boost::shared_ptr<World> self = static_pointer_cast<World>(GetSelf().lock());

  for (TLeafList::iterator iter = objects.begin(); iter != objects.end(); ++iter)
  {
    boost::shared_ptr<PhysicsObject> object = static_pointer_cast<PhysicsObject>(*iter);
    if (object == self)
    {
      continue;
    }

    // destroy everthing in the world
    // assume there is only ONE world
    object->DestroyPhysicsObject();
  }
}
