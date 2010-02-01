/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: oderaycollider.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_ODERAYCOLLIDER_H
#define OXYGEN_ODERAYCOLLIDER_H

#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/int/raycolliderint.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API ODERayCollider : public RayColliderInt, public Collider
{
public:
    ODERayCollider();
    void SetParams(salt::Vector3f pos, salt::Vector3f dir, float length, long geomID);
    long CreateRay();
};

} //namespace oxygen

#endif //OXYGEN_ODERAYCOLLIDER_H
