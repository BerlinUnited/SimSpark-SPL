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

#ifndef OXYGEN_UNIVERSALJOINTINT_H
#define OXYGEN_UNIVERSALJOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API UniversalJointInt
{

public:
    virtual ~UniversalJointInt() {}

    /** Creates a Universal Joint and returns its ID */
    virtual long CreateUniversalJoint(long world) = 0;

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

    /** This function sets up the first axis of the joint
        \param axis a vector describing the axis in relative coordinates
    */
    virtual void SetAxis1(const salt::Vector3f& axis, long jointID) = 0;

    /** This function sets up the second axis of the joint
        \param axis a vector describing the axis in local coordinates
    */
    virtual void SetAxis2(const salt::Vector3f& axis, long jointID) = 0;

    /** returns the vector describing the first of the two axis
        (in local coordinates)
        \param idx index of the desired axis
    */
    virtual salt::Vector3f GetAxis1(long jointID) const = 0;

    /** returns the vector describing the second of the two axis
        (in local coordinates)
        \param idx index of the desired axis
    */
    virtual salt::Vector3f GetAxis2(long jointID) const = 0;

    /** returns the first axis' angles in degrees, measured between
        the two bodies, or between the body and the static
        environment.
    */
    virtual float GetAngle1(long jointID) const = 0;

    /** returns the second axis' angles in degrees, measured between
        the two bodies, or between the body and the static
        environment.
    */
    virtual float GetAngle2(long jointID) const = 0;

    /** returns the time derivate of the first of the hinge angles */
    virtual float GetAngleRate1(long jointID) const = 0;

    /** returns the time derivate of the second of the hinge angles */
    virtual float GetAngleRate2(long jointID) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_UNIVERSALJOINTINT_H
