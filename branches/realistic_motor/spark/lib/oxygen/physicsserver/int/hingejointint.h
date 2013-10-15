/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef OXYGEN_HINGEJOINTINT_H
#define OXYGEN_HINGEJOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen{

class OXYGEN_API HingeJointInt{

public:
    virtual ~HingeJointInt() {}

    /** Creates a new hingejoint within the physics world specified
        by \param worldID
    */
    virtual long CreateHingeJoint(long worldID) = 0;

    /** sets the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    virtual void SetAnchor(const salt::Vector3f& anchor, long jointID) = 0;

    /** returns the joint anchor point in local coordinates on the first of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    virtual salt::Vector3f GetAnchor1(long jointID) = 0;

    /** returns the joint anchor point in local coordinates on the second of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    virtual salt::Vector3f GetAnchor2(long jointID) = 0;

    /** Sets the joint axis in the local coordinate system. The
       connected bodies movements will be constrained to move around
       this axis.
    */
    virtual void SetAxis(const salt::Vector3f& axis, long jointID) = 0;

    /** Returns the hinge axis in the local coordinate system */
    virtual salt::Vector3f GetAxis(long jointID) = 0;

    /** returns the hinge angle in degrees, measured between the two
        bodies, or between the body and the static environment.
    */
    virtual float GetAngle(long jointID) const = 0;

    /** returns the time derivate of the hinge angle */
    virtual float GetAngleRate(long jointID) const = 0;

    /** returns the torque on the joint */
    virtual float GetTorque(long jointID) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_HINGEJOINTINT_H
