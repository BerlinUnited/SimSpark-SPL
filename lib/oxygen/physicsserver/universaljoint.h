/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: universaljoint.h,v 1.2 2004/04/15 18:31:24 rollmark Exp $

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
#ifndef OXYGEN_UNIVERSALJOINT_H
#define OXYGEN_UNIVERSALJOINT_H

#include "joint.h"

namespace oxygen
{
class Body;

class UniversalJoint : public Joint
{
public:
    UniversalJoint();
    virtual ~UniversalJoint();

    /** Set the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    void SetAnchor(const salt::Vector3f& anchor);

    /** Returns the joint anchor point in local coordinates on one of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    salt::Vector3f GetAnchor (EBodyIndex idx);

    /** returns one of the hinge angles, measured between the two
        bodies, or between the body and the static environment. The
        angle will be between -pi..pi.
    */
    float GetAngle(EAxisIndex idx);

    /** returns the time derivate of one of the hinge angles */
    float GetAngleRate(EAxisIndex idx);

protected:
    /** creates a new contact joint */
    virtual void OnLink();
};

DECLARE_CLASS(UniversalJoint);

} // namespace oxygen

#endif // OXYGEN_UNIVERSALJOINT_H
