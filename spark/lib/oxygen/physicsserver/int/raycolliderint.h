/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef OXYGEN_RAYCOLLIDERINT_H
#define OXYGEN_RAYCOLLIDERINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API RayColliderInt
{
/** RayCollider encapsulates an ODE ray geometry "object".

    A ray is different from all the other geom classes in that it does not
    represent a solid object. It is an infinitely thin line that starts
    from the geom's position and extends in the direction of the geom's
    local Z-axis.
*/

public:
    virtual ~RayColliderInt() {}

    /** Sets the parameters of the ray.
     *
     * \param pos starting position of the ray
     * \param dir new direction of the ray
     * \param length length of the ray starting at its position
    */
    virtual void SetParams(salt::Vector3f pos, salt::Vector3f dir, float length, long geomID) = 0;

    /* Creates a ray and returns the ID of the newly created ray */
    virtual long CreateRay() = 0;
};

} //namespace oxygen

#endif //OXYGEN_RAYCOLLIDERINT_H
