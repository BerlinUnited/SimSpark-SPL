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

#ifndef OXYGEN_CAPSULECOLLIDERINT_H
#define OXYGEN_CAPSULECOLLIDERINT_H

#include <oxygen/oxygen_defines.h>
#include <salt/vector.h>

namespace oxygen
{

class OXYGEN_API CapsuleColliderInt
{
public:
    virtual ~CapsuleColliderInt() {}

    /** sets the parameters of the capsule.

       \param radius is the radius of the caps, and of the cylinder itself
       \param length is the height of the cylinder, not counting the caps
    */
    virtual void SetParams(float radius, float length, long geomID) = 0;

    /** sets the radius of the capsule */
    virtual void SetRadius(float radius, long geomID) = 0;

    /** sets the length of the capsule */
    virtual void SetLength(float length, long geomID) = 0;

    /** gets the radius and the length of the capsule */
    virtual void GetParams(float& radius, float& length, long geomID) = 0;

    /** returns the radius of the capsule */
    virtual float GetRadius(long geomID) = 0;

    /** return the length of the capsule */
    virtual float GetLength(long geomID) = 0;

    /** returns the depth of the given relative position in the
       managed capsule geom. Points inside the geom will have
       positive depth, points outside it will have negative depth, and
       points on the surface will have zero depth.
     */
    virtual float GetPointDepth(const salt::Vector3f& pos, long geomID) = 0;

    /** Creates a new Capsule and returns the ID of the newly created capsule */
    virtual long CreateCapsule() = 0;
};

} //namespace oxygen

#endif //OXYGEN_CAPSULECOLLIDERINT_H
