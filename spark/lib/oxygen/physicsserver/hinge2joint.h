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
#ifndef OXYGEN_HINGE2JOINT_H
#define OXYGEN_HINGE2JOINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/generic6dofjoint.h>

namespace oxygen
{
class Hinge2JointInt;

class OXYGEN_API Hinge2Joint : public Generic6DOFJoint
{
public:
    Hinge2Joint();
    virtual ~Hinge2Joint();

    /** sets the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    void SetAnchor(const salt::Vector3f& anchor);

    /** returns the joint anchor point in local coordinates on one of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    salt::Vector3f GetAnchor (EBodyIndex idx);

    /** returns one of the hinge2 angles in degrees, measured between
        the two bodies, or between the body and the static
        environment.
    */
    float GetAngle(EAxisIndex idx);

    /** returns the time derivate of one of the hinge2 angles */
    float GetAngleRate(EAxisIndex idx);

protected:
    /** creates a new hinge2 joint */
    virtual void OnLink();
    
private:
    static boost::shared_ptr<Hinge2JointInt> mHinge2JointImp;
};

DECLARE_CLASS(Hinge2Joint);

} // namespace oxygen

#endif // OXYGEN_HINGE2JOINT_H
