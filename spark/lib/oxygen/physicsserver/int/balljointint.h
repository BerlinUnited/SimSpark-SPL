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

#ifndef OXYGEN_BALLJOINTINT_H
#define OXYGEN_BALLJOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class RigidBody;
class Joint;

class OXYGEN_API BallJointInt
{
public:
    virtual ~BallJointInt() {}

    /** Set the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    virtual void SetAnchor(const salt::Vector3f& anchor, long jointID) = 0;

    /** Returns the joint anchor point in local coordinates on the first of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    virtual salt::Vector3f GetAnchor1(long jointID) = 0;

    /** Returns the joint anchor point in local coordinates on the second of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    virtual salt::Vector3f GetAnchor2(long jointID) = 0;

    /** Creates a new BallJoint in the physics world specified
        by \param worldID
    */
    virtual long CreateBallJoint(long worldID) =  0;
};

} //namespace oxygen

#endif //OXYGEN_BALLJOINTINT_H
