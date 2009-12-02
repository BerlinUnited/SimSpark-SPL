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
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

World::World() : PhysicsObject()
{
    mWorldImp = shared_ptr<ODEWorld>(new ODEWorld());
}

World::~World()
{
}

long World::GetWorldID() const
{
    return mWorldImp->GetWorldID();
}

void World::SetGravity(const Vector3f& gravity)
{
    mWorldImp->SetGravity(gravity);
}

salt::Vector3f World::GetGravity() const
{
    return mWorldImp->GetGravity();
}

void World::SetERP(float erp)
{
    mWorldImp->SetERP(erp);
}

float World::GetERP() const
{
    return mWorldImp->GetERP();
}

void World::SetCFM(float cfm)
{
    mWorldImp->SetCFM(cfm);
}

float World::GetCFM() const
{
    return mWorldImp->GetCFM();
}

void World::Step(float deltaTime)
{
    mWorldImp->Step(deltaTime);
}

bool World::GetAutoDisableFlag() const
{
    return mWorldImp->GetAutoDisableFlag();
}

void World::SetAutoDisableFlag(bool flag)
{
    mWorldImp->SetAutoDisableFlag(flag);
}

void World::SetContactSurfaceLayer(float depth)
{
    mWorldImp->SetContactSurfaceLayer(depth);
}

float World::GetContactSurfaceLayer() const
{
    return mWorldImp->GetContactSurfaceLayer();
}

bool World::ConstructInternal()
{
    mWorldImp->CreateWorld();

    return (mWorldImp->GetWorldID() != 0);
}

void World::DestroyPhysicsObject()
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
            space->DestroyPhysicsObject();
        }

    if (mWorldImp->GetWorldID() == 0)
        {
            return;
        }

    mWorldImp->DestroyWorld();

    recurseLock = false;
}
