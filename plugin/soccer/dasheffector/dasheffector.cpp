/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: dasheffector.cpp,v 1.1.2.7 2004/02/08 22:45:11 fruit Exp $

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
#include "dashaction.h"
#include "dasheffector.h"
#include <salt/random.h>
#include <salt/gmath.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <soccer/soccerbase/soccerbase.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

DashEffector::DashEffector() : oxygen::Effector(),
                               mForceFactor(60.0),mSigma(-1.0),mMaxPower(100.0)
{
}

DashEffector::~DashEffector()
{
}

bool
DashEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    GetLog()->Error()
        << "ERROR: (DashEffector) Realize\n ";

    if (mBody.get() == 0)
    {
        return false;
    }

    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (DashEffector) parent node is not derived from BaseNode\n";
        return false;
    }

    shared_ptr<DashAction> dashAction = shared_dynamic_cast<DashAction>(action);

    if (dashAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (DashEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    mForce[0] = dashAction->GetForce().x();
    mForce[1] = dashAction->GetForce().z();
    mForce[2] = dashAction->GetForce().y();

    // cut down the dash power vector to maximum length
    if (mForce.SquareLength() > mMaxPower * mMaxPower)
    {
        mForce.Normalize();
        mForce *= mMaxPower;
    }

    if (mSigma > 0.0)
    {
        mForce[0] = mForce[0] * salt::NormalRNG<>(1.0,mSigma)() * mForceFactor;
        mForce[1] = mForce[1] * salt::NormalRNG<>(1.0,mSigma)() * mForceFactor;
        mForce[2] = mForce[2] * salt::NormalRNG<>(1.0,mSigma)() * mForceFactor;
    } else {
        mForce = mForce * mForceFactor;
    }
    return true;
}

shared_ptr<ActionObject>
DashEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (DashEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  Predicate::Iterator iter = predicate.begin();
  Vector3f force;
  if (! predicate.GetValue(iter, force))
  {
      GetLog()->Error()
          << "ERROR: (DashEffector) Vector3f parameter expected\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return shared_ptr<ActionObject>(new DashAction(GetPredicate(),force));
}

void
DashEffector::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetAgentState(*this,mAgentState);

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(mTransformParent->GetChild("geometry"));

    mMaxDistance = 0.001;
    if (geom.get() == 0)
    {
        GetLog()->Error() << "ERROR: (DashEffector) parent node has "
                          << "no 'geometry' sphere child\n";
    } else {
            mMaxDistance += geom->GetRadius();
    }
}

void
DashEffector::OnUnlink()
{
    mTransformParent.reset();
    mBody.reset();
}

void
DashEffector::SetForceFactor(float force_factor)
{
    mForceFactor = salt::gAbs(force_factor);
}

void
DashEffector::SetSigma(float sigma)
{
    mSigma = sigma;
}

void
DashEffector::SetMaxPower(float max_power)
{
    mMaxPower = max_power;
}

void
DashEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    Effector::PrePhysicsUpdateInternal(deltaTime);
    if (mBody.get() == 0 ||
        mForce.Length() <= std::numeric_limits<float>::epsilon())
    {
        return;
    }

    Vector3f vec = mTransformParent->GetWorldTransform().Pos();
    if (vec[1] > mMaxDistance) return;

    mBody->AddForce(mAgentState->ApplyMotorForce(mForce * deltaTime));
}
