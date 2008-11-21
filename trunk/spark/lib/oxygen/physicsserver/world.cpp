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

#include "world.h"
#include "space.h"
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

World::World() : ODEObject(), mODEWorld(0)
{
}

World::~World()
{
}

dWorldID World::GetODEWorld() const
{
  return mODEWorld;
}

void World::SetGravity(const Vector3f& gravity)
{
  dWorldSetGravity(mODEWorld,
                   gravity.x(),
                   gravity.y(),
                   gravity.z()
                   );
}

salt::Vector3f World::GetGravity() const
{
  dVector3 dGravity;
  dWorldGetGravity(mODEWorld,dGravity);
  return Vector3f(dGravity[0],dGravity[1],dGravity[2]);
}

void World::SetERP(float erp)
{
  dWorldSetERP(mODEWorld, erp);
}

float World::GetERP() const
{
  return dWorldGetERP(mODEWorld);
}

void World::SetCFM(float cfm)
{
  dWorldSetCFM(mODEWorld, cfm);
}

float World::GetCFM() const
{
  return dWorldGetCFM(mODEWorld);
}

void World::Step(float deltaTime)
{
  dWorldStep(mODEWorld, deltaTime);
}

bool World::GetAutoDisableFlag() const
{
  return (dWorldGetAutoDisableFlag(mODEWorld) == 1);
}

void World::SetAutoDisableFlag(bool flag)
{
  dWorldSetAutoDisableFlag(mODEWorld, static_cast<int>(flag));
}

void World::SetContactSurfaceLayer(float depth)
{
  dWorldSetContactSurfaceLayer(mODEWorld, depth);
}

float World::GetContactSurfaceLayer() const
{
  return dWorldGetContactSurfaceLayer(mODEWorld);
}

bool World::ConstructInternal()
{
  // create an ode world
  mODEWorld = dWorldCreate();

  return (mODEWorld != 0);
}

void World::DestroyODEObject()
{
  static bool recurseLock = false;
  if (recurseLock)
    {
      return;
    }

  recurseLock = true;

  shared_ptr<Space> space = GetSpace();
  if (space.get() != 0)
    {
      space->DestroyODEObject();
    }

  if (mODEWorld == 0)
    {
      return;
    }

  // release the ODE world
  dWorldDestroy(mODEWorld);
  mODEWorld = 0;

  recurseLock = false;
}
