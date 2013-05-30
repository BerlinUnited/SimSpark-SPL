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
#ifndef OXYGEN_JOINT_H
#define OXYGEN_JOINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{
class RigidBody;
class JointInt;

/** \class Joint encapsulates a joint object. A joint is a
    relationship (a constraint) that is enforced between two bodies so
    that they can only have certain positions and orientations
    relative to each other.

    Note that the joint geometry parameter setting functions should
    only be called after the joint has been attached to bodies, and
    those bodies have been correctly positioned, otherwise the joint
    may not be initialized correctly. If the joint is not already
    attached, these functions will do nothing.

    The default anchor for all joints is global (0,0,0). The default
    axis for all joints is global (1,0,0).

    There are no functions to set joint angles or positions (or their
    rates) directly, instead you must set the corresponding body
    positions and velocities.
 */
class OXYGEN_API Joint : public PhysicsObject
{
public:
    enum EBodyIndex
        {
            BI_FIRST = 0,
            BI_SECOND = 1
        };

    enum EAxisIndex
        {
            AI_FIRST = 0,
            AI_SECOND = 1,
            AI_THIRD = 2
        };

    Joint();
    virtual ~Joint();

    /** destroy the managed physics object */
    virtual void DestroyPhysicsObject();

    /** attaches the joint to some new bodies. If the joint is already
        attached, it will be detached from the old bodies first. To
        attach this joint to only one body, set body1 or body2 to
        null.  A null body refers to the static environment. Setting
        both bodies to zero puts the joint into "limbo", i.e. it will
        have no effect on the simulation.
    */
    virtual void Attach(boost::shared_ptr<RigidBody> body1,
                        boost::shared_ptr<RigidBody> body2);

    /** attaches the joint to some new bodies, that are given as path
        expressions. These path expressions are allowed to be relative
        to this joint node.
     */
    void Attach(const std::string& path1, const std::string& path2);

    /** returns one of the bodies that this joint connects, according
        to the given EBodyIndex
    */
    boost::shared_ptr<RigidBody> GetBody(EBodyIndex idx);

    /** returns the Joint node corresponding to the given joint */
    static boost::shared_ptr<Joint> GetJoint(long jointID);

    /** returns the type of the managed joint, possible return
        values from ODE are dJointTypeNone, dJointTypeBall, dJointTypeHinge,
        dJointTypeSlider, dJointTypeContact, dJointTypeUniversal,
        dJointTypeHinge2, dJointTypeFixed or dJointTypeAMotor.
     */
    int GetType() const;

    /** returns true if the two given bodies are connected by a
        joint */
    static bool AreConnected (boost::shared_ptr<RigidBody> body1,
                              boost::shared_ptr<RigidBody> body2);

    /** returns true if the two given bodies are connected together by
        a joint that does not have the given joint type. For possible
        joint type constants see GetType()
     */
    static bool AreConnectedExcluding (boost::shared_ptr<RigidBody> body1,
                                       boost::shared_ptr<RigidBody> body2,
                                       int joint_type);

    /** during the world time step, the forces that are applied by
        each joint are added directly to the joined bodies, and the
        user normally has no way of telling which joint contributed
        how much force. If this information is desired the joint can
        be enabled to collect feedback information. By default a joint
        does not collect any feedback information.
     */
    void EnableFeedback(bool enable);

    /** returns true if the joint is set to collect feedback
        information */
    bool FeedBackEnabled() const;

    /** queries the force that the joint applied to one body attached
        to it during the last timestep.
    */
    salt::Vector3f GetFeedbackForce(EBodyIndex idx) const;

    /** queries the torque that the joint applied to one body attached
        to it during the last timestep.
    */
    salt::Vector3f GetFeedbackTorque(EBodyIndex idx) const;

    /** sets the fudgefactor parameter, making this value too small can
        prevent the motor from being able to move the joint away from
        a stop
    */
    void SetFudgeFactor(EAxisIndex idx, float fudge_factor);

    /** returns the fudgefactor paramter */
    float GetFudgeFactor(EAxisIndex idx) const;

    /** sets the bouncyness of the stops. This is a restitution
        parameter in the range 0..1. 0 means the stops are not bouncy
        at all, 1 means maximum bouncyness.
    */
    void SetBounce(EAxisIndex idx, float bounce);

    /** returns the bouncyness of the stops */
    float GetBounce(EAxisIndex idx) const;

    /** sets the low stop angle in degrees, this stop must be greater
        than -180 to be effective
    */
    void SetLowStopDeg(EAxisIndex idx, float deg);

    /** returns the low stop angle in degrees */
    float GetLowStopDeg(EAxisIndex idx) const;

    /** sets the high stop angle in degrees, this stop must be less
        than +180 to be effective
    */
    void SetHighStopDeg(EAxisIndex idx, float deg);

    /** returns the high stop angle in degrees */
    float GetHighStopDeg(EAxisIndex idx) const;

    /** sets the low stop position */
    void SetLowStopPos(EAxisIndex idx, float deg);

    /** returns the low stop position */
    float GetLowStopPos(EAxisIndex idx) const;

    /** sets the high stop position */
    void SetHighStopPos(EAxisIndex idx, float deg);

    /** returns the high stop position */
    float GetHighStopPos(EAxisIndex idx) const;

    /** the constraint force mixing (CFM) value used when not at a
        stop */
    void SetCFM(EAxisIndex idx, float cfm);

    /** returns the constraint force mixing value used when not a a
        stop */
    float GetCFM(EAxisIndex idx) const;

    /** sets the constraint force mixing (CFM) value used by the
        stops. Together with the ERP value this can be used to get
        spongy or soft stops. This is intended for unpowered joints,
        it does not really work as expected when a powered joint
        reaches its limit.
    */
    void SetStopCFM(EAxisIndex idx, float cfm);

    /** returns the constraint force mixing value used by the stops */
    float GetStopCFM(EAxisIndex idx) const;

    /** sets the error reduction parameter (ERP) used by the stops. */
    void SetStopERP(EAxisIndex idx, float erp);

    /** returns the error reduction parameter used by the stops */
    float GetStopERP(EAxisIndex idx) const;

    /** sets the suspension error reduction parameter (ERP). As of ode
        0.039 this is only implemented on the hinge-2 joint.
    */
    void SetSuspensionERP(EAxisIndex idx, float erp);

    /** returns the suspension error reduction parameter (ERP). As of
        ode 0.039 this is only implemented on the hinge-2 joint.
    */
    float GetSuspensionERP(EAxisIndex idx) const;

    /** sets the suspension constraint force mixing value. As of ode
        0.039 this is only implemented on the hinge-2 joint.
    */
    void SetSuspensionCFM(EAxisIndex idx, float cfm);

    /** returns the suspension constraint force mixing value. As of
        ode 0.039 this is only implemented on the hinge-2 joint.
    */
    float GetSuspensionCFM(EAxisIndex idx) const;

    /** sets the linear motor velocity */
    void SetLinearMotorVelocity(EAxisIndex idx, float vel);

    /** returns the linear motor velocity */
    float GetLinearMotorVelocity(EAxisIndex idx) const;

    /** sets the angular motor velocity in degrees */
    void SetAngularMotorVelocity(EAxisIndex idx, float deg);

    /** returns the angular motor velocity in degrees */
    float GetAngularMotorVelocity(EAxisIndex idx) const;

    /** sets the maximum force or torque that the motor will use to
        achieve the desired velocity. This must always be greater than
        or equal to zero. Setting this to zero (the default value)
        turns off the motor.
    */
    void SetMaxMotorForce(EAxisIndex idx, float f);

    /** returns the maximum force or torque that the motor will use to
        achieve the desired velocity.
    */
    float GetMaxMotorForce(EAxisIndex idx) const;

    /** Set the maximum joint speed1, valid for both hingejoint and universaljoint */
    virtual void SetJointMaxSpeed1(float rad);

    /** Set the maximum joint speed2, valid for only universaljoint */
    virtual void SetJointMaxSpeed2(float rad);

    /** Get the maximum joint speed1, valid for both hingejoint and universaljoint */
    virtual float GetJointMaxSpeed1() const;

    /** Get the maximum joint speed2, valid for only universaljoint */
    virtual float GetJointMaxSpeed2() const;

    /** Whether limit jointMaxSpeed1 */
    bool IsLimitJointMaxSpeed1() const;

    /** Whether limit jointMaxSpeed2 */
    bool IsLimitJointMaxSpeed2() const;
    
    /** This has to be declared here because rsgedit upcasts to Joint 
        at one point and then calls this.  
    */
    virtual void SetParameter(int parameter, float value){}

protected:
    /** associates the created joint with this node */
    virtual void OnLink();

    /** gets the node at 'path' and tries a cast to Body */
    boost::shared_ptr<RigidBody> GetBody(const std::string& path);

    //
    // Members
    //
    
    /** the ID of the managed joint */
    long mJointID;
    
    boost::shared_ptr<GenericJointFeedback> mFeedback;

    /** The maximum joint speed in rad, valid for both hingejoint and universaljoint */
    float mJointMaxSpeed1;
    bool mIsLimitJointMaxSpeed1;

    /** The maximum joint speed in rad, valid only for universaljoint */
    float mJointMaxSpeed2;
    bool mIsLimitJointMaxSpeed2;
 
    static boost::shared_ptr<JointInt> mJointImp;
};

DECLARE_ABSTRACTCLASS(Joint);

} // namespace oxygen

#endif // OXYGEN_JOINT_H
