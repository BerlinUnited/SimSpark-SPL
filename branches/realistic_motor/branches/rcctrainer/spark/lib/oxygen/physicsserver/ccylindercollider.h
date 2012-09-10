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
#ifndef OXYGEN_CCYLINDERCOLLIDER_H
#define OXYGEN_CCYLINDERCOLLIDER_H

#include <oxygen/oxygen_defines.h>
#include "collider.h"

namespace oxygen
{

/** CCylinderCollider encapsulates an ODE capped cylinder geometry
    object. A capped cylinder is like a normal cylinder except it has
    half-sphere caps at its ends. This feature makes the internal
    collision detection code particularly fast and accurate.
 */
class OXYGEN_API CCylinderCollider : public Collider
{
    //
    // Functions
    //
public:
    CCylinderCollider();

    /** sets the parameters of the capped cylinder.

       \param radius is the radius of the caps, and of the cylinder itself
       \param length is the height of the cylinder, not counting the caps
    */
    void SetParams(float radius, float length);

    /** sets the radius of the capped cylinder */
    void SetRadius(float radius);

    /** sets the length of the capped cylinder */
    void SetLength(float length);

    /** gets the radius and the length of the capped cylinder */
    void GetParams(float& radius, float& length);

    /** returns the radius of the capped cylinder */
    float GetRadius();

    /** return the length of the capped cylinder */
    float GetLength();

    /** returns the depth of the given relative position in the
       managed capped cylinder geom. Points inside the geom will have
       positive depth, points outside it will have negative depth, and
       points on the surface will have zero depth.
     */
    float GetPointDepth(const salt::Vector3f& pos);

protected:
    /** constructs a default capped cylinder with an radius of 1 and a
        length of 1 */
    virtual bool ConstructInternal();
};

DECLARE_CLASS(CCylinderCollider);

} //namespace oxygen

#endif //OXYGEN_CCYLINDERCOLLIDER_H
