/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ccylindercollider.h,v 1.3 2003/11/14 14:05:53 fruit Exp $

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

#include "collider.h"

namespace oxygen
{

/** CCylinderCollider encapsulates an ODE capped cylinder geometry object.A
    capped cylinder is like a normal cylinder except it has half-sphere caps
    at its ends. This feature makes the internal collision detection code
    particularly fast and accurate.
 */
class CCylinderCollider : public Collider
{
    //
    // Functions
    //
public:
    CCylinderCollider();

    /* Sets the parameters of the capped cylinder. The cylinder's length,
       not counting the caps, is given by length. The cylinder is aligned
       along the geom's local Z axis. The radius of the caps, and of the
       cylinder itself, is given by radius.
    */
    void SetParams(float radius, float length);

protected:
    /** constructs a default capped cylinder with an radius of 1 and a length of 1 */
    virtual bool ConstructInternal();
};

DECLARE_CLASS(CCylinderCollider);

} //namespace oxygen

#endif //OXYGEN_CCYLINDERCOLLIDER_H
