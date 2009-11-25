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
#ifndef BALL_H
#define BALL_H

#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <salt/vector.h>

class BallStateAspect;

namespace oxygen
{
    class AgentAspect;
}

/** \class Ball represents the ball on the soccer field ;) */
class Ball : public oxygen::Transform
{
public:
    Ball();
    virtual ~Ball() {};

    /** Set the acceleration of the ball.
     *
     * This method can be used to accelerate the ball over a number
     * of simulation steps. The ball acceleration will be applied over the number
     * of steps using a constant force and torque.
     *
     * \param steps number of steps the acceleration should be applied.
     * \param force the maximum force to add to the ball
     * \param torque the maximum torque to add to the ball
     * \param agent the agent kicking the ball
     */
    void SetAcceleration(int steps, const salt::Vector3f& force,
                         const salt::Vector3f& torque,
                         boost::shared_ptr<oxygen::AgentAspect> agent);

    /** This method is used to add forces and torques to the ball before each simulation
     *  step, if necessary.
     */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

private:
    int mForceTTL;

    salt::Vector3f mForce;
    salt::Vector3f mTorque;
    boost::shared_ptr<oxygen::RigidBody> mBody;
    boost::shared_ptr<oxygen::AgentAspect> mKickedLast;
    boost::shared_ptr<BallStateAspect> mBallStateAspect;
};

DECLARE_CLASS(Ball);


#endif // BALL_H
