/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kickeffector.h,v 1.1.2.2 2004/01/26 15:19:52 fruit Exp $

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

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/body.h>

class KickEffector : public oxygen::Effector
{
    //
    // functions
    //
public:
    KickEffector();
    virtual ~KickEffector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

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

    /** Set the noise parameters.
     *  If used, the noise values are normally distributed around 0.0.
     *  Using this method, the sigmas of the distributions can be set.
     *  Values <= 0 mean that no noise will be added.
     *
     * \param sigma_force for noise of the applied force
     * \param sigma_theta for noise of the angle in the x-y plane
     * \param sigma_phi for noise of the latitudal angle
     */
    void SetNoiseParams(double sigma_force,
                        double sigma_theta,
                        double sigma_phi);

protected:
    /** reference to the body nod of the ball */
    boost::shared_ptr<oxygen::Body> mBallBody;

private:
    /** the margin where objects can be kicked */
    float mKickMargin;
    /** sigma for force error */
    double mSigmaForce;
    /** sigma for angle error (x-y angle) */
    double mSigmaTheta;
    /** sigma for angle error (latitudal angle) */
    double mSigmaPhi;
};

DECLARE_CLASS(KickEffector);

#endif // KICKEFFECTOR_H
