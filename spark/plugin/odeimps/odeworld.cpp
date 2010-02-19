/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: world.cpp 3 2008-11-21 02:38:08Z hedayat $

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

#include "odeworld.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

ODEWorld::ODEWorld() : ODEPhysicsObject(){
}

void ODEWorld::SetGravity(const Vector3f& gravity, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldSetGravity(ODEWorld,
                     gravity.x(),
                     gravity.y(),
                     gravity.z()
                     );
}

salt::Vector3f ODEWorld::GetGravity(long worldID) const
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dVector3 dGravity;
    dWorldGetGravity(ODEWorld,dGravity);
    return Vector3f(dGravity[0],dGravity[1],dGravity[2]);
}

void ODEWorld::SetERP(float erp, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldSetERP(ODEWorld, erp);
}

float ODEWorld::GetERP(long worldID) const
{
    dWorldID ODEWorld = (dWorldID) worldID;
    return dWorldGetERP(ODEWorld);
}

void ODEWorld::SetCFM(float cfm, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldSetCFM(ODEWorld, cfm);
}

float ODEWorld::GetCFM(long worldID) const
{
    dWorldID ODEWorld = (dWorldID) worldID;
    return dWorldGetCFM(ODEWorld);
}

void ODEWorld::Step(float deltaTime, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldStep(ODEWorld, deltaTime);
}

bool ODEWorld::GetAutoDisableFlag(long worldID) const
{
    dWorldID ODEWorld = (dWorldID) worldID;
    return (dWorldGetAutoDisableFlag(ODEWorld) == 1);
}

void ODEWorld::SetAutoDisableFlag(bool flag, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldSetAutoDisableFlag(ODEWorld, static_cast<int>(flag));
}

void ODEWorld::SetContactSurfaceLayer(float depth, long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldSetContactSurfaceLayer(ODEWorld, depth);
}

float ODEWorld::GetContactSurfaceLayer(long worldID) const
{
    dWorldID ODEWorld = (dWorldID) worldID;
    return dWorldGetContactSurfaceLayer(ODEWorld);
}

long ODEWorld::CreateWorld()
{
    dWorldID ODEWorld = dWorldCreate();
    return (long) ODEWorld;
}

void ODEWorld::DestroyWorld(long worldID)
{
    dWorldID ODEWorld = (dWorldID) worldID;
    dWorldDestroy(ODEWorld);
}
