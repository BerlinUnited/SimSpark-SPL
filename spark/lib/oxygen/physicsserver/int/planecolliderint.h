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

#ifndef OXYGEN_PLANECOLLIDERINT_H
#define OXYGEN_PLANECOLLIDERINT_H

#include <salt/matrix.h>
#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API PlaneColliderInt
{
public:
    virtual ~PlaneColliderInt() {}

    /** sets the parameters of the plane equation a*x+b*y+c*z = d ,
        all parameters are given in global coordinates
     */
    virtual void SetPlaneParams(float a, float b, float c, float d, long geomID) = 0;

    /** set the parameters of the plane given a position and a normal
        vector; all parameters are given in global coordinates
    */
    virtual void SetParams(const salt::Vector3f& pos, salt::Vector3f normal, long geomID) = 0;

    /** returns the depth of the given relative position in the
        managed plane. Points inside the geom will have positive
        depth, points outside it will have negative depth, and points
        on the surface will have zero depth.
    */
    virtual float GetPointDepth(const salt::Vector3f& pos, long geomID) = 0;

    /** Creates a plane collider and returns the ID of the newly created plane */
    virtual long CreatePlane() = 0;
};

} //namespace oxygen

#endif //OXYGEN_PLANECOLLIDERINT_H
