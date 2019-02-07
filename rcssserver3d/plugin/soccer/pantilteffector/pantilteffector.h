/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Mon May 9 2005
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
#ifndef PANTILTEFFECTOR_H
#define PANTILTEFFECTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <agentstate/agentstate.h>

class PanTiltEffector : public oxygen::Effector
{
public:
    PanTiltEffector();
    virtual ~PanTiltEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "pantilt"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** Set the maximum pan angle change.
     *  The camera can pan to any angle, but the absolute pan angle
     *  used for one 'pan' action may be restricted. A maximum pan
     *  angle of 0 effectively disables panning, a value of 360 (or
     *  greater) means no restriction.
     * @param max_pan_angle the maximum pan angle (in degrees) for one action
     */
    void SetMaxPanAngleDelta(unsigned char max_pan_angle);

    /** Set the maximum tilt angle change.
     *  Set the maximum absolute tilt angle change for one 'tilt' action.
     * @param max_tilt_angle the maximum tilt angle (in degrees) for one action
     */
    void SetMaxTiltAngleDelta(unsigned char max_tilt_angle);

    /** Set the angle error parameter.
     *  The error is normally distributed around zero.
     * @param sigma the sigma for the random number distribution
     */
    void SetSigma(float sigma);

protected:
    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    typedef boost::shared_ptr<salt::NormalRNG<> > NormalRngPtr;

    /** the reference to the parent transform node */
    boost::shared_ptr<oxygen::Transform> mTransformParent;
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::RigidBody> mBody;
    //! a reference to the agent state
    boost::shared_ptr<AgentState> mAgentState;

    /** random number generator for the error distribution of pan/tilt actions */
    NormalRngPtr mActuatorErrorRNG;

    /** The maximum absolute value of the pan angle change */
    unsigned char mMaxPanAngleDelta;

    /** The maximum absolute value of the tilt angle change */
    unsigned char mMaxTiltAngleDelta;
};

DECLARE_CLASS(PanTiltEffector);

#endif // PANTILTEFFECTOR_H
