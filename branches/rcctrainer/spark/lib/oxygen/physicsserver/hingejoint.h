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
#ifndef OXYGEN_HINGEJOINT_H
#define OXYGEN_HINGEJOINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/generic6dofjoint.h>

namespace oxygen
{
class RigidBody;
class HingeJointInt;

class OXYGEN_API HingeJoint : public Generic6DOFJoint
{
public:
    HingeJoint();
    virtual ~HingeJoint();

    /** sets the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    void SetAnchor(const salt::Vector3f& anchor);

    /** returns the joint anchor point in local coordinates on one of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    salt::Vector3f GetAnchor(EBodyIndex idx);

    /** Sets the joint axis in the local coordinate system. The
       connected bodies movements will be constrained to move around
       this axis.
    */
    void SetAxis(EAxisIndex idx);

    /** Sets the joint axis in the local coordinate system. The
       connected bodies movements will be constrained to move around
       this axis.
    */
    void SetAxis(const salt::Vector3f& axis);

    /** Returns the hinge axis in the local coordinate system */
    salt::Vector3f GetAxis();

    /** returns the hinge angle in degrees, measured between the two
        bodies, or between the body and the static environment.
    */
    float GetAngle() const;

    /** returns the time derivate of the hinge angle */
    float GetAngleRate() const;
    
    /** returns the torque on the joint */
    float GetTorque() const;
    
    /** Sets a parameter value of this joint*/
    void SetParameter(int parameter, float value);

protected:
    /** creates a new hingejoint joint */
    virtual void OnLink();
    
private:
    static boost::shared_ptr<HingeJointInt> mHingeJointImp;
};

DECLARE_CLASS(HingeJoint);

} // namespace oxygen

#endif // OXYGEN_HINGEJOINT_H
