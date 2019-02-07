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
#ifndef OXYGEN_ANGULARMOTOR_H
#define OXYGEN_ANGULARMOTOR_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/physicsserver.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/battery.h>
#include <oxygen/physicsserver/hingejoint.h>

namespace oxygen
{
class AngularMotorInt;

/** An angular motor allows the relative angular velocities of two
    bodies to be controlled. The angular velocity can be controlled on
    up to three axes, allowing torque motors and stops to be set for
    rotation about those axes.
*/
class OXYGEN_API AngularMotor : public Joint
{
public:

    enum EAxisAnchor
        {
            AA_GLOBAL = 0,
            AA_FIRSTBODY = 1,
            AA_SECONDBODY = 2
        };

public:
    AngularMotor();
    virtual ~AngularMotor();

    /** sets the motor mode. The angular motor can be used in
        different modes. In 'user' mode (the default mode), the user
        directly sets the axes that the angular motor controls. In
        'euler' mode, it computes the euler angles corresponding to
        the relative rotation, allowing euler angle torque motors and
        stops to be set.
        Pass zero to this function to set the mode to user mode, or
        a value not equal to zero to set it to Euler mode.
    */
    void SetMode(int mode);

    /** returns the current motor mode - zero if motor is in user mode,
        or one if motor is in euler mode.
    */
    int GetMode();

    /** sets the number of angular axes that will be controlled by the
        angular motor. \param num can range from 0 which effectively
        disables the motor to 3, which are automatically set in euler
        mode.
    */
    void SetNumAxes(int num);

    /** returns the number of angular axes that are controlled by the
        angular motor 
    */
    int GetNumAxes();

    /** sets one of the motor axis. \param idx gives the motor axis to
        be set. \param anchor gives the relative anchor mode of the
        axis. \param axis gives the axis vector relative to the joint node
        orientation.

        For Euler mode only axes 0 and 2 need to be set. Axis 1 will
        be determined automatically at each time step.  Axes 0 and 2
        must be perpendicular to each other.  Axis 0 must be anchored
        to the first body, axis 2 must be anchored to the second body.
    */
    void SetMotorAxis(EAxisIndex idx, EAxisAnchor anchor,
                      const salt::Vector3f& axis);

    /** returns the motor axis \param idx */
    salt::Vector3f GetMotorAxis(EAxisIndex idx);

    /** returns the relative anchor mode of the motor axis \param idx */
    EAxisAnchor GetAxisAnchor(EAxisIndex idx);

    /** sets the current angle along axis \param idx. This function
        should only be called in 'user' mode, as in this mode the
        motor has no other way of knowing the joint angles.
    */
    void SetAxisAngle(EAxisIndex idx, float degAngle);

    /** returns the current angle in degrees for axis \parm idx. In
       'user' mode this is simply the value that was previoulsy set
       with SetAxisAngle(). In 'euler' mode this is the corresponding
       euler angle.
    */
    float GetAxisAngle(EAxisIndex idx);

    /** Return the current angle rate for axis anum. In dAMotorUser
        mode this is always zero, as not enough information is
        available. In dAMotorEuler mode this is the corresponding
        euler angle rate.
    */
    float GetAxisAngleRate(EAxisIndex idx);

    /** sets a joint parameter value */
    virtual void SetParameter(int parameter, float value);

    /** returns a joint parameter value */
    virtual float GetParameter(int parameter) const;

    void SetBattery(const std::string& batteryPath);

    bool Disabled();

    /** get the current of joint */
    float GetCurrent() const { return mI; }

    /** get the tempeature of joint */
    float GetTemperature() const { return mTempMotor; }

    /** constant setters */
    void setSpeedConstant(float v) { mKe = v; }
    void setTorqueConstant(float v) { mKt = v; }
    void setResistance(float v) { mR = v; }
    void setEnvironmentTempeature(float v) { mTempEnvironment = v; mTempMotor = v;}
    void setThermalConductivity(float v) { mThermalConductivity = v; }
    void setHeatCapacity(float v) { mHeatCapacity = v; }
    void setProtectionTempeature(float v) { mTempProtection = v; }
    void setProtectionStiffness(float v) { mProtectionStiffness = v; }
    void setMaxTorque(float v) { mMaxTorque = v; }

    /** returns the torque on the joint */
    float GetTorque() const;

    void SetStiffness(float v) { mStiffness = v; updateStiffnessControl(); }

protected:
    /** creates a new angularmotor joint */
    virtual void OnLink();

    /** update energe comsuption and tempeture */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    void updateStiffnessControl();
    
private:
    static boost::shared_ptr<AngularMotorInt> mAngularMotorImp;

    boost::shared_ptr<PhysicsServer> mPhysicsServer;
    boost::shared_ptr<Battery> mBattery;

    /** parameters of motor */
    float mKe;
    float mKt;
    float mR;
    float mI; // current
    /** parameters of tempeature */
    float mTempEnvironment;
    float mTempMotor;
    float mThermalConductivity;
    float mHeatCapacity;
    float mTempProtection;
    float mProtectionStiffness;
    float mMaxTorque;

    float mStiffness;
    float mActualStiffness;

    boost::shared_ptr<HingeJoint> mHingeJoint; // HACK: assume connected with hinge joint
};

DECLARE_CLASS(AngularMotor);

} // namespace oxygen

#endif // OXYGEN_ANGULARMOTOR_H
