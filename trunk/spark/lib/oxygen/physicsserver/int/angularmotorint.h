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

#ifndef OXYGEN_ANGULARMOTORINT_H
#define OXYGEN_ANGULARMOTORINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class RigidBody;
class Joint;

/** An angular motor allows the relative angular velocities of two
    bodies to be controlled. The angular velocity can be controlled on
    up to three axes, allowing torque motors and stops to be set for
    rotation about those axes.
*/

class OXYGEN_API AngularMotorInt
{
public:
    virtual ~AngularMotorInt() {}

    /** Creates a new angular motor within the physics world specified
        by \param worldID
    */
    virtual long CreateAngularMotor(long worldID) = 0;

    /** Sets the mode of this angular motor to user mode */
    virtual void SetModeUserMode(long jointID) = 0;

    /** Sets the mode of this angular motor to euler mode */
    virtual void SetModeEulerMode(long jointID) = 0;

    /** returns the current motor mode - zero if motor is in user mode,
        or one if motor is in euler mode.
    */
    virtual int GetMode(long jointID) = 0;

    /** sets the number of angular axes that will be controlled by the
        angular motor. \param num can range from 0 which effectively
        disables the motor to 3, which are automatically set in euler
        mode.
    */
    virtual void SetNumAxes(int num, long jointID) = 0;

    /** returns the number of angular axes that are controlled by the
        angular motor
    */
    virtual int GetNumAxes(long jointID) = 0;

    /** sets one of the motor axis. \param idx gives the motor axis to
        be set. \param anchor gives the relative anchor mode of the
        axis. \param axis gives the axis vector relative to the joint node
        orientation.

        For Euler mode only axes 0 and 2 need to be set. Axis 1 will
        be determined automatically at each time step.  Axes 0 and 2
        must be perpendicular to each other.  Axis 0 must be anchored
        to the first body, axis 2 must be anchored to the second body.
    */
    virtual void SetMotorAxis(int idx, int anchor, salt::Vector3f axis, long jointID) = 0;

    /** returns the relative anchor mode of the motor axis \param idx */
    virtual int GetAxisAnchor(int idx, long jointID) = 0;

    /** returns the motor axis \param idx */
    virtual salt::Vector3f GetMotorAxis(int idx, long jointID) = 0;

    /** sets the current angle along axis \param idx. This function
        should only be called in 'user' mode, as in this mode the
        motor has no other way of knowing the joint angles.
    */
    virtual void SetAxisAngle(int idx, float degAngle, long jointID) = 0;

    /** sets the current angle along axis \param idx. This function
        should only be called in 'user' mode, as in this mode the
        motor has no other way of knowing the joint angles.
    */
    virtual float GetAxisAngle(int idx, long jointID) = 0;

    /** Return the current angle rate for axis anum. In dAMotorUser
        mode this is always zero, as not enough information is
        available. In dAMotorEuler mode this is the corresponding
        euler angle rate.
    */
    virtual float GetAxisAngleRate(int idx, long jointID) = 0;

    /** returns the torque on the joint */
    virtual float GetTorque(long jointID) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_ANGULARMOTORINT_H
