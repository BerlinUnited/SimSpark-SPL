/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider.h,v 1.3 2003/08/31 12:16:49 rollmark Exp $

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

   PlaneCollider
*/

#ifndef PLANECOLLIDER_H__
#define PLANECOLLIDER_H__

#include "collider.h"

namespace kerosin
{

/** PlaneCollider encapsulates an ODE plane geometry object. Planes are
    non-placeable geoms, i.e.  unlike placeable geoms, planes do not have an
    assigned position and rotation. This means that the parameters (a,b,c,d)
    are always in global coordinates. In other words it is assumed that the
    plane is always part of the static environment and not tied to any movable
    object.
*/
class PlaneCollider : public Collider
{
    //
        // Functions
        //
public:
    PlaneCollider();

    /** sets the parameters of the plane equation a*x+b*y+c*z = d */
    void SetParams(float a, float b, float c, float d);

protected:
    /** constructs a default plane with normal pointing up, going through the origin */
    virtual bool ConstructInternal();
};

DECLARE_CLASS(PlaneCollider);

} //namespace kerosin

#endif //PLANECOLLIDER_H__
