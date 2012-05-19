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

#ifndef OXYGEN_BOXCOLLIDERINT_H
#define OXYGEN_BOXCOLLIDERINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API BoxColliderInt
{

public:
    virtual ~BoxColliderInt() {}

    /** sets the side lengths of the box geom */
    virtual void SetBoxLengths(const salt::Vector3f& extents, long geomID) = 0;

    /** gets the side lengths of the box geom */
    virtual void GetBoxLengths(salt::Vector3f& extents, long geomID) = 0;

    /** returns the depth of the given relative position in the
       managed box geom. Points inside the geom will have positive
       depth, points outside it will have negative depth, and points
       on the surface will have zero depth.
     */
    virtual float GetPointDepth(const salt::Vector3f& pos, long geomID) = 0;

    /** Creates a new box geom and returns the ID of the newly created box */
    virtual long CreateBox() = 0;
};

} //namespace oxygen

#endif //OXYGEN_BOXCOLLIDERINT_H
