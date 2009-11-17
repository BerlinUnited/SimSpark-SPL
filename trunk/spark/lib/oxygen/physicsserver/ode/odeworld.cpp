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

#include <oxygen/physicsserver/ode/odeworld.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/sceneserver/scene.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

ODEWorld::ODEWorld() : WorldImp(), mODEWorld(0)
{
}

ODEWorld::~ODEWorld()
{
}

dWorldID ODEWorld::GetODEWorldImp() const
{
  return mODEWorld;
}

void ODEWorld::SetGravityImp(const Vector3f& gravity)
{
  dWorldSetGravity(mODEWorld,
                   gravity.x(),
                   gravity.y(),
                   gravity.z()
                   );
}

salt::Vector3f ODEWorld::GetGravityImp() const
{
  dVector3 dGravity;
  dWorldGetGravity(mODEWorld,dGravity);
  return Vector3f(dGravity[0],dGravity[1],dGravity[2]);
}

void ODEWorld::SetERPImp(float erp)
{
  dWorldSetERP(mODEWorld, erp);
}

float ODEWorld::GetERPImp() const
{
  return dWorldGetERP(mODEWorld);
}

void ODEWorld::SetCFMImp(float cfm)
{
  dWorldSetCFM(mODEWorld, cfm);
}

float ODEWorld::GetCFMImp() const
{
  return dWorldGetCFM(mODEWorld);
}

void ODEWorld::StepImp(float deltaTime)
{
  dWorldStep(mODEWorld, deltaTime);
}

bool ODEWorld::GetAutoDisableFlagImp() const
{
  return (dWorldGetAutoDisableFlag(mODEWorld) == 1);
}

void ODEWorld::SetAutoDisableFlagImp(bool flag)
{
  dWorldSetAutoDisableFlag(mODEWorld, static_cast<int>(flag));
}

void ODEWorld::SetContactSurfaceLayerImp(float depth)
{
  dWorldSetContactSurfaceLayer(mODEWorld, depth);
}

float ODEWorld::GetContactSurfaceLayerImp() const
{
  return dWorldGetContactSurfaceLayer(mODEWorld);
}

bool ODEWorld::ConstructInternalImp()
{
  // create an ode world
  mODEWorld = dWorldCreate();

  return (mODEWorld != 0);
}

void ODEWorld::DestroyODEObjectImp()
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
