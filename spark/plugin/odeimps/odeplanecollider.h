/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeplanecollider.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef ODEPLANECOLLIDER_H
#define ODEPLANECOLLIDER_H

#include "odecollider.h"
#include <oxygen/physicsserver/int/planecolliderint.h>

class PlaneColliderImp : public oxygen::PlaneColliderInt, public ColliderImp
{
/** PlaneCollider encapsulates an ODE plane geometry object. Planes
    are non-placeable geoms, i.e.  unlike placeable geoms, planes do
    not have an assigned position and rotation. This means that the
    parameters (a,b,c,d) are always in global coordinates. In other
    words it is assumed that the plane is always part of the static
    environment and not tied to any movable object.
    
    See physicsserver/int/planecolliderint.h for documentation
*/
public:
    PlaneColliderImp();
    void SetPlaneParams(float a, float b, float c, float d, long geomID);
    void SetParams(const salt::Vector3f& pos, salt::Vector3f normal, long geomID);
    float GetPointDepth(const salt::Vector3f& pos, long geomID);
    long CreatePlane();
};

DECLARE_CLASS(PlaneColliderImp);

#endif //ODEPLANECOLLIDER_H
