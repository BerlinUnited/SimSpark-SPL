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
#ifndef DRIVEEFFECTOR_H
#define DRIVEEFFECTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <agentstate/agentstate.h>

class DriveEffector : public oxygen::Effector
{
public:
    DriveEffector();
    virtual ~DriveEffector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "drive"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** Set the force factor.
     *
     * The drive power vector is multiplied by this factor divided by the maximum length
     * of the drive power vector.
     */
    void SetForceFactor(float force_factor);

    /** Set the error distribution for calculating the applied force.
     *
     * The force applied to a sphere is the drive power vector multiplied by
     * the force factor divided by (maximum length of the drive power vector + Error).
     * The error is normaly distributed around zero with a given sigma. If sigma is <= 0,
     * no error will be applied.
     */
    void SetSigma(float sigma);

    /** Set the maximum length of the drive power vector. */
    void SetMaxPower(float max_power);

    /** Set the battery consumption.
     *  How long can you be driving full speed with a fully charged battery?
     *  \param consume_time time (in milliseconds) the battery works
     *                      driving full speed
     */
    void SetConsumption(float consume_time);

protected:
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

protected:
    typedef boost::shared_ptr<salt::NormalRNG<> > NormalRngPtr;

    /** the reference to the parent transform node */
    boost::shared_ptr<oxygen::Transform> mTransformParent;
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::RigidBody> mBody;
    //! a reference to the agent state
    boost::shared_ptr<AgentState> mAgentState;

    /** the force that should be applied to the agent body */
    salt::Vector3f mForce;

    /** the maximum distance from the plane */
    float mMaxDistance;

    /** The force factor is the force applied to the body if the length of
        the drive power vector is greater or equal to mMaxDrivePower */
    float mForceFactor;

    /** random number generator for the error distribution of the applied force */
    NormalRngPtr mForceErrorRNG;

    /** The maximum length of the drive power vector. */
    float mMaxPower;

    /** The battery consumption for driving one second with full speed */
    double mConsumption;
};

DECLARE_CLASS(DriveEffector);

#endif // DRIVEEFFECTOR_H
