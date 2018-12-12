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

#ifndef OXYGEN_JOINTINT_H
#define OXYGEN_JOINTINT_H

#include <oxygen/physicsserver/genericphysicsobjects.h>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>
#include <string>

namespace oxygen
{
class RigidBody;
class Joint;

class OXYGEN_API JointInt
{
public:
    virtual ~JointInt() {}

    /** Returns a pointer to the Joint object that manages the joint
        specified by \param jointID.
    */
    virtual Joint* GetJoint(long jointID) = 0;

    /** Checks if two bodies, specified by \param bodyID1 and
        \param bodyID2, are connected by a joint.
    */
    virtual bool AreConnected(long bodyID1, long bodyID2) = 0;

    /** Checks if two bodies, specified by \param bodyID1 and
        \param bodyID2, are connected by a joint that is not of the type
        specified by \param joint_type.
    */
    virtual bool AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type) = 0;

    /** Destroys the Joint specified by \param jointID */
    virtual void DestroyJoint(long jointID,
                              boost::shared_ptr<GenericJointFeedback> feedback) = 0;

    /** attaches the joint to some new bodies. If the joint is already
        attached, it will be detached from the old bodies first. To
        attach this joint to only one body, set body1 or body2 to
        null.  A null body refers to the static environment. Setting
        both bodies to zero puts the joint into "limbo", i.e. it will
        have no effect on the simulation.
    */
    virtual void Attach(long bodyID1, long bodiID2, long jointID) = 0;

    /** returns the type of the managed joint, possible return
        values from ODE are dJointTypeNone, dJointTypeBall, dJointTypeHinge,
        dJointTypeSlider, dJointTypeContact, dJointTypeUniversal,
        dJointTypeHinge2, dJointTypeFixed or dJointTypeAMotor.
     */
    virtual int GetType(long jointID) const = 0;

    /** Returns the ID of one of the bodies the joint specified
        by \param jointID is connected to. Since a joint is always
        connected to two bodies, use \param idx to address the first or
        second body.
    */
    virtual long GetBodyID(int idx, long jointID) = 0;

    /** during the world time step, the forces that are applied by
        each joint are added directly to the joined bodies, and the
        user normally has no way of telling which joint contributed
        how much force. If this information is desired the joint can
        be enabled to collect feedback information. By default a joint
        does not collect any feedback information.
     */
    virtual void EnableFeedback(bool enable, long jointID,
                                boost::shared_ptr<GenericJointFeedback>& feedback) = 0;

    /** returns true if the joint is set to collect feedback
        information
    */
    virtual bool FeedbackEnabled(long jointID) const = 0;

    /** queries the force that the joint applied to one body attached
        to it during the last timestep.
    */
    virtual salt::Vector3f GetFeedbackForce(int idx,
                                            boost::shared_ptr<GenericJointFeedback> feedback) const = 0;

    /** queries the torque that the joint applied to one body attached
        to it during the last timestep.
    */
    virtual salt::Vector3f GetFeedbackTorque(int idx,
                                            boost::shared_ptr<GenericJointFeedback> feedback) const = 0;

    /** sets the fudgefactor parameter, making this value too small can
        prevent the motor from being able to move the joint away from
        a stop
    */
    virtual void SetFudgeFactor(int idx, float fudge_factor, long jointID) = 0;

    /** returns the fudgefactor paramter */
    virtual float GetFudgeFactor(int idx, long jointID) const = 0;

    /** sets the bouncyness of the stops. This is a restitution
        parameter in the range 0..1. 0 means the stops are not bouncy
        at all, 1 means maximum bouncyness.
    */
    virtual void SetBounce(int idx, float bounce, long jointID) = 0;

    /** returns the bouncyness of the stops */
    virtual float GetBounce(int idx, long jointID) const = 0;

    /** sets the low stop angle in degrees, this stop must be greater
        than -180 to be effective
    */
    virtual void SetLowStopDeg(int idx, float deg, long jointID) = 0;

    /** returns the low stop angle in degrees */
    virtual float GetLowStopDeg(int idx, long jointID) const = 0;

    /** sets the high stop angle in degrees, this stop must be less
        than +180 to be effective
    */
    virtual void SetHighStopDeg(int idx, float deg, long jointID) = 0;

    /** returns the high stop angle in degrees */
    virtual float GetHighStopDeg(int idx, long jointID) const = 0;

    /** sets the low stop position */
    virtual void SetLowStopPos(int idx, float deg, long jointID) = 0;

    /** returns the low stop position */
    virtual float GetLowStopPos(int idx, long jointID) const = 0;

    /** sets the high stop position */
    virtual void SetHighStopPos(int idx, float deg, long jointID) = 0;

    /** returns the high stop position */
    virtual float GetHighStopPos(int idx, long jointID) const = 0;

    /** the constraint force mixing (CFM) value used when not at a
        stop
    */
    virtual void SetCFM(int idx, float cfm, long jointID) = 0;

    /** returns the constraint force mixing value used when not a a
        stop
    */
    virtual float GetCFM(int idx, long jointID) const = 0;

    /** sets the constraint force mixing (CFM) value used by the
        stops. Together with the ERP value this can be used to get
        spongy or soft stops. This is intended for unpowered joints,
        it does not really work as expected when a powered joint
        reaches its limit.
    */
    virtual void SetStopCFM(int idx, float cfm, long jointID) = 0;

    /** returns the constraint force mixing value used by the stops */
    virtual float GetStopCFM(int idx, long jointID) const = 0;

    /** sets the error reduction parameter (ERP) used by the stops. */
    virtual void SetStopERP(int idx, float erp, long jointID) = 0;

    /** returns the error reduction parameter used by the stops */
    virtual float GetStopERP(int idx, long jointID) const = 0;

    /** sets the suspension error reduction parameter (ERP). As of ode
        0.039 this is only implemented on the hinge-2 joint.
    */
    virtual void SetSuspensionERP(int idx, float erp, long jointID) = 0;

    /** returns the suspension error reduction parameter (ERP). As of
        ode 0.039 this is only implemented on the hinge-2 joint.
    */
    virtual float GetSuspensionERP(int idx, long jointID) const = 0;

    /** sets the suspension constraint force mixing value. As of ode
        0.039 this is only implemented on the hinge-2 joint.
    */
    virtual void SetSuspensionCFM(int idx, float cfm, long jointID) = 0;

    /** returns the suspension constraint force mixing value. As of
        ode 0.039 this is only implemented on the hinge-2 joint.
    */
    virtual float GetSuspensionCFM(int idx, long jointID) const = 0;

    /** sets the linear motor velocity */
    virtual void SetLinearMotorVelocity(int idx, float vel, long jointID) = 0;

    /** returns the linear motor velocity */
    virtual float GetLinearMotorVelocity(int idx, long jointID) const = 0;

    /** sets the angular motor velocity in degrees */
    virtual void SetAngularMotorVelocity(int idx, float deg, long jointID) = 0;

    /** returns the angular motor velocity in degrees */
    virtual float GetAngularMotorVelocity(int idx, long jointID) const = 0;

    /** sets the maximum force or torque that the motor will use to
        achieve the desired velocity. This must always be greater than
        or equal to zero. Setting this to zero (the default value)
        turns off the motor.
    */
    virtual void SetMaxMotorForce(int idx, float f, long jointID) = 0;

    /** returns the maximum force or torque that the motor will use to
        achieve the desired velocity.
    */
    virtual float GetMaxMotorForce(int idx, long jointID) const = 0;

    /** Sets the joint parameter indexed by \param parameter of the joint
        specified by \param jointID to \param value
    */
    virtual void SetParameter(int parameter, float value, long jointID) = 0;

    /** Returns the joint parameter indexed by \param parameter of the joint
        specified by \param jointID
    */
    virtual float GetParameter(int parameter, long jointID) const = 0;

    /** Links the ID of the joint specified by \param jointID to the Joint
        object specified by \param joint. Doing this allows us to later
        find the Joint object that manages the joint specified
        by \param jointID.
    */
    virtual void OnLink(long jointID, Joint* joint) = 0;
};

} //namespace oxygen

#endif //OXYGEN_JOINTINT_H
