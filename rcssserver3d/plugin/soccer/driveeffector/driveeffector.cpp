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
#include "driveaction.h"
#include "driveeffector.h"
#include <salt/gmath.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <soccerbase/soccerbase.h>
#include <oxygen/sceneserver/transform.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

DriveEffector::DriveEffector() : oxygen::Effector(),
                                 mForceFactor(60.0),
                                 mMaxPower(100.0), mConsumption(1.0/18000.0)
{
}

DriveEffector::~DriveEffector()
{
}

bool
DriveEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mBody.get() == 0)
    {
        return false;
    }

    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(GetParent().lock());

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (DriveEffector) parent node is not derived from BaseNode\n";
        return false;
    }

    shared_ptr<DriveAction> driveAction = shared_dynamic_cast<DriveAction>(action);

    if (driveAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (DriveEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    mForce = driveAction->GetForce();

    // cut down the drive power vector to maximum length
    if (mForce.SquareLength() > mMaxPower * mMaxPower)
    {
        mForce.Normalize();
        mForce *= mMaxPower;
    }

    if (mForceErrorRNG.get() != 0)
    {
        mForce[0] = mForce[0] * (*(mForceErrorRNG.get()))() * mForceFactor;
        mForce[1] = mForce[1] * (*(mForceErrorRNG.get()))() * mForceFactor;
        mForce[2] = mForce[2] * (*(mForceErrorRNG.get()))() * mForceFactor;
    } else {
        mForce = mForce * mForceFactor;
    }
    return true;
}

shared_ptr<ActionObject>
DriveEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (DriveEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  Vector3f force;
  if (! predicate.GetValue(predicate.begin(), force))
  {
      GetLog()->Error()
          << "ERROR: (DriveEffector) Vector3f parameter expected\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
  }

  return shared_ptr<ActionObject>(new DriveAction(GetPredicate(),force));
}

void
DriveEffector::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
    SoccerBase::GetBody(*this,mBody);
    SoccerBase::GetAgentState(*this,mAgentState);

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(mTransformParent->GetChild("geometry"));

    mMaxDistance = 0.001;
    if (geom.get() == 0)
    {
        GetLog()->Error() << "ERROR: (DriveEffector) parent node has "
                          << "no 'geometry' sphere child\n";
    } else {
            mMaxDistance += geom->GetRadius();
    }
}

void
DriveEffector::OnUnlink()
{
    mForceErrorRNG.reset();
    mTransformParent.reset();
    mBody.reset();
}

void
DriveEffector::SetForceFactor(float force_factor)
{
    mForceFactor = salt::gAbs(force_factor);
}

void
DriveEffector::SetSigma(float sigma)
{
    NormalRngPtr rng(new salt::NormalRNG<>(1.0,sigma));
    mForceErrorRNG = rng;
}

void
DriveEffector::SetMaxPower(float max_power)
{
    mMaxPower = max_power;
}

void
DriveEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    Effector::PrePhysicsUpdateInternal(deltaTime);
    if (mBody.get() == 0 ||
        mForce.Length() <= std::numeric_limits<float>::epsilon())
    {
        return;
    }

    Vector3f vec = mTransformParent->GetWorldTransform().Pos();
    if (vec.z() > mMaxDistance) return;

    if (mAgentState->ReduceBattery(mForce.Length() * mConsumption))
    {
        mBody->AddForce(SoccerBase::FlipView(mForce,mAgentState->GetTeamIndex()));
    }
}

void
DriveEffector::SetConsumption(float consume_time)
{
    mConsumption = 1.0 / consume_time;
}


