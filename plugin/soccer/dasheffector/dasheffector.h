/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: dasheffector.h,v 1.1.2.2 2004/01/31 14:10:11 fruit Exp $

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
#ifndef DASHEFFECTOR_H
#define DASHEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/body.h>

class DashEffector : public oxygen::Effector
{
public:
    DashEffector();
    virtual ~DashEffector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "dash"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** Set the force factor.
     *
     * The dash power vector is multiplied by this factor divided by the maximum length
     * of the dash power vector.
     */
    void SetForceFactor(float force_factor);

    /** Set the error distribution for calculating the applied force.
     *
     * The force applied to a sphere is the dash power vector multiplied by
     * the force factor divided by (maximum length of the dash power vector + Error).
     * The error is normaly distributed around zero with a given sigma. If sigma is <= 0,
     * no error will be applied.
     */
    void SetSigma(float sigma);

    /** Set the maximum length of the dash power vector. */
    void SetMaxPower(float max_power);

    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

protected:
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::Body> mBody;

    /** The force factor is the force applied to the body if the length of
        the dash power vector is greater or equal to mMaxDashPower */
    float mForceFactor;

    /** sigma for the error distribution to calculate the applied force */
    float mSigma;

    /** The maximum length of the dash power vector. */
    float mMaxPower;
};

DECLARE_CLASS(DashEffector);

#endif // DASHEFFECTOR_H
