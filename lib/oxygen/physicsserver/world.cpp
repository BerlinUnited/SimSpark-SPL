/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: world.cpp,v 1.5 2004/03/22 10:59:02 rollmark Exp $

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

using namespace oxygen;
using namespace salt;

World::World() : mODEWorld(0)
{
}

World::~World()
{
  // release the ODE world
  if (mODEWorld)
    {
      dWorldDestroy(mODEWorld);
      mODEWorld = 0;
    }
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

bool World::ConstructInternal()
{
  // create an ode world
  mODEWorld = dWorldCreate();

  return (mODEWorld != 0);
}
