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

WorldImp::WorldImp() : PhysicsObjectImp(){
}

void WorldImp::SetGravity(const Vector3f& gravity, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldSetGravity(WorldImp,
                     gravity.x(),
                     gravity.y(),
                     gravity.z()
                     );
}

salt::Vector3f WorldImp::GetGravity(long worldID) const
{
    dWorldID WorldImp = (dWorldID) worldID;
    dVector3 dGravity;
    dWorldGetGravity(WorldImp,dGravity);
    return Vector3f(dGravity[0],dGravity[1],dGravity[2]);
}

void WorldImp::SetERP(float erp, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldSetERP(WorldImp, erp);
}

float WorldImp::GetERP(long worldID) const
{
    dWorldID WorldImp = (dWorldID) worldID;
    return dWorldGetERP(WorldImp);
}

void WorldImp::SetCFM(float cfm, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldSetCFM(WorldImp, cfm);
}

float WorldImp::GetCFM(long worldID) const
{
    dWorldID WorldImp = (dWorldID) worldID;
    return dWorldGetCFM(WorldImp);
}

void WorldImp::Step(float deltaTime, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldStep(WorldImp, deltaTime);
}

bool WorldImp::GetAutoDisableFlag(long worldID) const
{
    dWorldID WorldImp = (dWorldID) worldID;
    return (dWorldGetAutoDisableFlag(WorldImp) == 1);
}

void WorldImp::SetAutoDisableFlag(bool flag, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldSetAutoDisableFlag(WorldImp, static_cast<int>(flag));
}

void WorldImp::SetContactSurfaceLayer(float depth, long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldSetContactSurfaceLayer(WorldImp, depth);
}

float WorldImp::GetContactSurfaceLayer(long worldID) const
{
    dWorldID WorldImp = (dWorldID) worldID;
    return dWorldGetContactSurfaceLayer(WorldImp);
}

long WorldImp::CreateWorld()
{
    dWorldID WorldImp = dWorldCreate();
    return (long) WorldImp;
}

void WorldImp::DestroyWorld(long worldID)
{
    dWorldID WorldImp = (dWorldID) worldID;
    dWorldDestroy(WorldImp);
}
