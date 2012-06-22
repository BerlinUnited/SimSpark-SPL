/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Mon May 9 2005
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: pantilteffector.cpp 181 2010-02-28 10:55:34Z marianbuchta $

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
#include "pantiltaction.h"
#include "pantilteffector.h"
#include <salt/gmath.h>
#include <zeitgeist/logserver/logserver.h>
#include <soccerbase/soccerbase.h>
#include <restrictedvisionperceptor/restrictedvisionperceptor.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

PanTiltEffector::PanTiltEffector() : oxygen::Effector(),
                                     mMaxPanAngleDelta(90),
                                     mMaxTiltAngleDelta(10)
{
    SetSigma(0.25);
}

PanTiltEffector::~PanTiltEffector()
{
}

void
PanTiltEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if (mAction.get() == 0 || mBody.get() == 0)
    {
        return;
    }

    boost::shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(GetParent().lock());

    if (parent.get() == 0)
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) "
                          << "parent node is not derived from BaseNode\n";
        return;
    }

    boost::shared_ptr<PanTiltAction> panTiltAction =
        shared_dynamic_cast<PanTiltAction>(mAction);
    mAction.reset();

    if (panTiltAction.get() == 0)
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) "
                          << "cannot realize an unknown ActionObject\n";
        return;
    }

    float pan = panTiltAction->GetPanAngle();

    // check for NAN
    if (gIsNan(pan))
    {
        return;
    }

    // cut down the pan angle if necessary
    if (gAbs(pan) > mMaxPanAngleDelta)
    {
        pan = gSign(pan) * mMaxPanAngleDelta;
    }

    float tilt = panTiltAction->GetTiltAngle();

    // check for NAN
    if (gIsNan(tilt))
    {
        return;
    }

    // cut down the tilt angle if necessary
    if (gAbs(tilt) > mMaxTiltAngleDelta)
    {
        tilt = gSign(tilt) * mMaxTiltAngleDelta;
    }

    // apply random error if there is a RNG
    if (mActuatorErrorRNG.get() != 0)
    {
        pan += (*(mActuatorErrorRNG.get()))();
        tilt += (*(mActuatorErrorRNG.get()))();
    }

    // look for vision perceptor and apply change
    boost::shared_ptr<RestrictedVisionPerceptor> rvp =
        parent->FindChildSupportingClass<RestrictedVisionPerceptor>(false);
    if (rvp.get() == 0)
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) "
                          << "cannot find RestrictedVisionPerceptor instance\n";
        return;
    }

    rvp->ChangePanTilt(pan,tilt);

}

boost::shared_ptr<ActionObject>
PanTiltEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) invalid predicate"
                          << predicate.name << "\n";
        return boost::shared_ptr<ActionObject>();
    }

    Predicate::Iterator iter = predicate.begin();

    float pan;
    if (! predicate.AdvanceValue(iter, pan))
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) 2 float parameters expected\n";
        return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
    }
    float tilt;
    if (! predicate.AdvanceValue(iter, tilt))
    {
        GetLog()->Error() << "ERROR: (PanTiltEffector) float parameter expected\n";
        return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
    }
    return boost::shared_ptr<ActionObject>(new PanTiltAction(GetPredicate(),pan,tilt));
}

void
PanTiltEffector::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetAgentState(*this,mAgentState);
}

void
PanTiltEffector::OnUnlink()
{
    mActuatorErrorRNG.reset();
    mTransformParent.reset();
    mBody.reset();
}

void
PanTiltEffector::SetMaxPanAngleDelta(unsigned char max_pan_angle)
{
    mMaxPanAngleDelta = max_pan_angle;
}

void
PanTiltEffector::SetMaxTiltAngleDelta(unsigned char max_tilt_angle)
{
    mMaxTiltAngleDelta = max_tilt_angle;
}

void
PanTiltEffector::SetSigma(float sigma)
{
    NormalRngPtr rng(new salt::NormalRNG<>(0.0,sigma));
    mActuatorErrorRNG = rng;
}


