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

#include <oxygen/physicsserver/ode/odeworld.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

World::World() : ODEObject()
{
    shared_ptr<ODEWorld> odeworld(new ODEWorld());
    mWorldImp = (shared_dynamic_cast<WorldImp>(odeworld));
}

World::~World()
{
}

dWorldID World::GetODEWorld() const
{
  return mWorldImp->GetODEWorldImp();
}

void World::SetGravity(const Vector3f& gravity)
{
  mWorldImp->SetGravityImp(gravity);
}

salt::Vector3f World::GetGravity() const
{
  return mWorldImp->GetGravityImp();
}

void World::SetERP(float erp)
{
  mWorldImp->SetERPImp(erp);
}

float World::GetERP() const
{
  return mWorldImp->GetERPImp();
}

void World::SetCFM(float cfm)
{
  mWorldImp->SetCFMImp(cfm);
}

float World::GetCFM() const
{
  return mWorldImp->GetCFMImp();
}

void World::Step(float deltaTime)
{
  mWorldImp->StepImp(deltaTime);
}

bool World::GetAutoDisableFlag() const
{
  return mWorldImp->GetAutoDisableFlagImp();
}

void World::SetAutoDisableFlag(bool flag)
{
  mWorldImp->SetAutoDisableFlagImp(flag);
}

void World::SetContactSurfaceLayer(float depth)
{
  mWorldImp->SetContactSurfaceLayerImp(depth);
}

float World::GetContactSurfaceLayer() const
{
  return mWorldImp->GetContactSurfaceLayerImp();
}

bool World::ConstructInternal()
{
  return mWorldImp->ConstructInternalImp();
}

void World::DestroyODEObject()
{
  mWorldImp->DestroyODEObjectImp();
}
