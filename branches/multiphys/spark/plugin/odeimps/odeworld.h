/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: world.h 56 2009-03-17 18:03:47Z hedayat $

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
#ifndef ODEWORLD_H
#define ODEWORLD_H

#include "odephysicsobject.h"
#include <oxygen/physicsserver/int/worldint.h>

class WorldImp : public oxygen::WorldInt, public PhysicsObjectImp
{
/** World encapsulates a world object. It is a container for
    rigid bodies and joints. Objects in different worlds can not
    interact, for example rigid bodies from two different worlds can
    not collide. All the objects in a world exist at the same point in
    time, thus one reason to use separate worlds is to simulate
    systems at different rates.
    
    See physicsserver/int/worldint.h for documentation.
*/

public:
    WorldImp();

    void SetGravity(const salt::Vector3f& gravity, long worldID);
    salt::Vector3f GetGravity(long worldID) const;
    void SetERP(float erp, long worldID);
    float GetERP(long worldID) const;
    void SetCFM(float cfm, long worldID);
    float GetCFM(long worldID) const;
    void Step(float deltaTime, long worldID);
    bool GetAutoDisableFlag(long worldID) const;
    void SetAutoDisableFlag(bool flag, long worldID);
    void SetContactSurfaceLayer(float depth, long worldID);
    float GetContactSurfaceLayer(long worldID) const;
    long CreateWorld();
    void DestroyWorld(long worldID);
};

DECLARE_CLASS(WorldImp);

#endif //ODEWORLD_H
