/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: world.cpp,v 1.3.8.1 2004/01/12 18:28:56 rollmark Exp $

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

World::World() : mODEWorld(0)
{
}

World::~World()
{
}

dWorldID World::GetODEWorld() const
{
    return mODEWorld;
}

void World::SetGravity(float x, float y, float z)
{
    dWorldSetGravity(mODEWorld, x, y, z);
}

void World::SetGravity(const salt::Vector3f &v)
{
    SetGravity(v.x(), v.y(), v.z());
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

void World::OnLink()
{
    // create an ode world
    mODEWorld = dWorldCreate();
}

void World::OnUnlink()
{
    // release the ODE world
    if (mODEWorld)
    {
        dWorldDestroy(mODEWorld);
        mODEWorld = 0;
    }
}
