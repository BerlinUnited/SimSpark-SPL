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
#ifndef KICKEFFECTOR_H
#define KICKEFFECTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <ball/ball.h>
#include <ballstateaspect/ballstateaspect.h>

class BallStateAspect;

class KickEffector : public oxygen::Effector
{
public:
    KickEffector();
    virtual ~KickEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "kick"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the ball body node */
    virtual void OnLink();

    /** remove the reference to the ball body node */
    virtual void OnUnlink();

    /** set the kick margin (the area within objects are kickable) */
    void SetKickMargin(float margin);

    /** Set the force factor.
     *
     * The kick power vector is multiplied by this factor.
     */
    void SetForceFactor(float force_factor);

    /** Set the torque factor.
     *
     * The kick torque vector is multiplied by this factor.
     */
    void SetTorqueFactor(float torque_factor);

    /** Set the number of steps the force is applied.
     * \param steps the number of steps to apply force and torque.
     */
    void SetSteps(int steps);

    /** Set the noise parameters.
     *  If used, the noise values are normally distributed around 0.0.
     *  Using this method, the sigmas of the distributions can be set.
     *  Values <= 0 mean that no noise will be added.
     *
     * \param sigma_force for noise of the applied force
     * \param sigma_theta for noise of the angle in the x-y plane
     * \param sigma_phi_end for noise of the latitudal angle at the end of the range
     * \param sigma_phi_mid for noise of the latitudal angle in the middle of the angle range
     */
    void SetNoiseParams(double sigma_force,
                        double sigma_theta,
                        double sigma_phi_end,
                        double sigma_phi_mid);

    /** Set the maximum kick power. */
    void SetMaxPower(float max_power);

    /** Set the latitudal angle range
     * \param min minimum latitudal kick angle in degrees
     * \param max maximum latitudal kick angle in degrees
     *
     * min has to be smaller than max.
     */
    void SetAngleRange(float min, float max);

protected:
    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    typedef boost::shared_ptr<salt::NormalRNG<> > NormalRngPtr;

    /** reference to the body node of the ball */
    boost::shared_ptr<oxygen::RigidBody> mBallBody;
    /** reference to the body node of the ball */
    boost::shared_ptr<Ball> mBall;
    /** reference to the agent aspect */
    boost::shared_ptr<oxygen::AgentAspect> mAgent;

    /** random number generator for the error distribution of the applied force */
    NormalRngPtr mForceErrorRNG;
    /** random number generator for the error distribution of the theta */
    NormalRngPtr mThetaErrorRNG;

    /** reference to the ball state aspect */
    boost::shared_ptr<BallStateAspect> mBallStateAspect;

private:
    /** the margin where objects can be kicked */
    float mKickMargin;
    /** radius of the player */
    float mPlayerRadius;
    /** radius of the ball */
    float mBallRadius;
    /** force factor */
    float mForceFactor;
    /** force factor */
    float mTorqueFactor;
    /** the maximum kick power */
    float mMaxPower;
    /** the minimal kick angle */
    float mMinAngle;
    /** the maximum kick angle */
    float mMaxAngle;
    /** number of simulation steps for applying kick force */
    int mSteps;
    /** sigma for angle error at the end of the range (latitudal angle) */
    double mSigmaPhiEnd;
    /** sigma for angle error in the middle of the range (latitudal angle) */
    double mSigmaPhiMid;
};

DECLARE_CLASS(KickEffector);

#endif // KICKEFFECTOR_H
