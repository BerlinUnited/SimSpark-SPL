/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ball.h,v 1.1.2.3 2004/02/01 12:23:34 fruit Exp $

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
#include <oxygen/physicsserver/body.h>
#include <salt/vector.h>

/** \class Ball represents the ball on the soccer field ;) */
class Ball : public oxygen::Transform
{
public:
    Ball();
    virtual ~Ball() {};

    /** Set the acceleration of the ball.
     *
     * This method can be used to accelerate the ball "slowly", i.e. over a number
     * of simulation steps. The ball acceleration will be increased over the number
     * of steps from a fraction of the requested force / torque up to the maximum.
     *
     * \param steps number of steps the acceleration should be applied.
     * \param force the maximum force to add to the ball
     * \param torque the maximum torque to add to the ball
     */
    void SetAcceleration(int steps, const salt::Vector3f& force,
                         const salt::Vector3f& torque = salt::Vector3f(0.0,0.0,0.0));

    /** This method is used to add forces and torques to the ball before each simulation
     *  step, if necessary.
     */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

private:
    int mForceTTL;
    double mGamma;
    double mInc;

    salt::Vector3f mForce;
    salt::Vector3f mTorque;
    boost::shared_ptr<oxygen::Body> mBody;
};

DECLARE_CLASS(Ball);


#endif // BALL_H
