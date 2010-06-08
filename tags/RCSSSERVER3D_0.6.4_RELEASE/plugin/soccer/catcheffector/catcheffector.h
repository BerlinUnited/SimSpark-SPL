/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef CATCHEFFECTOR_H
#define CATCHEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <ball/ball.h>
#include <ballstateaspect/ballstateaspect.h>

namespace salt
{
    class AABB2;
}

namespace oxygen
{
    class RigidBody;
    class AgentAspect;
}

class AgentState;

class SoccerRuleAspect;

class CatchEffector : public oxygen::Effector
{
public:
    CatchEffector();
    virtual ~CatchEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "catch"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the ball body node */
    virtual void OnLink();

    /** remove the reference to the ball body node */
    virtual void OnUnlink();

    /** set the catch margin (the area within objects are catchable) */
    void SetCatchMargin(float margin);

protected:
    /** moves the ball to pos setting its linear and angular velocity to 0 */
    void MoveBall(const salt::Vector3f& pos);
    /** reference to the soccer rule aspect */
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule;

     /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    /** reference to the body node of the ball */
    boost::shared_ptr<oxygen::RigidBody> mBallBody;
    /** reference to the agent aspect */
    boost::shared_ptr<oxygen::AgentAspect> mAgent;
    /** reference to the agentstate */
    boost::shared_ptr<AgentState> mAgentState;

    /** bounding box for the right penalty area */
    salt::AABB2 mRightPenaltyArea;
    /** bounding box for the left penalty area */
    salt::AABB2 mLeftPenaltyArea;

    /** the field length (in meters) */
    float mFieldLength;
    /** the goal width (in meters) */
    float mGoalWidth;

private:
    /** the margin where objects can be catched */
    float mCatchMargin;
    /** radius of the player */
    float mPlayerRadius;
    /** radius of the ball */
    float mBallRadius;
};

DECLARE_CLASS(CatchEffector);

#endif // CATCHEFFECTOR_H
