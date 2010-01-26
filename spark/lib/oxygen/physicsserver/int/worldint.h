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
#ifndef OXYGEN_WORLDINT_H
#define OXYGEN_WORLDINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API WorldInt
{
    
public:
    virtual void SetGravity(const salt::Vector3f& gravity, long worldID) = 0;
    virtual salt::Vector3f GetGravity(long worldID) const = 0;
    virtual void SetERP(float erp, long worldID) = 0;
    virtual float GetERP(long worldID) const = 0;
    virtual void SetCFM(float cfm, long worldID) = 0;
    virtual float GetCFM(long worldID) const = 0;
    virtual void Step(float deltaTime, long worldID) = 0;
    virtual bool GetAutoDisableFlag(long worldID) const = 0;
    virtual void SetAutoDisableFlag(bool flag, long worldID) = 0;
    virtual void SetContactSurfaceLayer(float depth, long worldID) = 0;
    virtual float GetContactSurfaceLayer(long worldID) const = 0;
    virtual long CreateWorld() = 0;
    virtual void DestroyWorld(long worldID) = 0;
};

}

#endif //OXYGEN_WORLDINT_H
