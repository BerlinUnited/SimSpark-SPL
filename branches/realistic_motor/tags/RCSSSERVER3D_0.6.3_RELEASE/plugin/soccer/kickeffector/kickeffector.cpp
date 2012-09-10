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
#include "kickaction.h"
#include "kickeffector.h"
#include <salt/random.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <soccerbase/soccerbase.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

KickEffector::KickEffector()
    : oxygen::Effector(),
      mKickMargin(0.04),mPlayerRadius(0.0),mBallRadius(0.0),
      mForceFactor(4.0),mTorqueFactor(0.1),
      mMaxPower(100.0),
      mMinAngle(0.0),mMaxAngle(50.0), mSteps(10),
      mSigmaPhiEnd(0.9), mSigmaPhiMid(4.5)
{
}

KickEffector::~KickEffector()
{
}

void
KickEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // this should also include the case when there is no ball
    // (because then there will be no body, neither).
    if (mAction.get() == 0 || mBallBody.get() == 0)
    {
        return;
    }

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) parent node is not derived from BaseNode\n";
        return;
    }

    shared_ptr<KickAction> kickAction =
        shared_dynamic_cast<KickAction>(mAction);
    mAction.reset();

    if (kickAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) cannot realize an unknown ActionObject\n";
        return;
    }

    // if the agent doesn't have a body, we're done (this should never happen)
    if (mBall.get() == 0) return;

    Vector3f force =
        mBallBody->GetWorldTransform().Pos() -
        mAgent->GetWorldTransform().Pos();

    // the ball can be kicked if the distance is
    // less then Ball-Radius + Player-Radius + KickMargin AND
    // the player is close to the ground
    if (mAgent->GetWorldTransform().Pos().z() > mPlayerRadius + 0.01 ||
        force.Length() > mPlayerRadius + mBallRadius + mKickMargin)
    {
        // ball is out of reach, or player is in the air:
        // kick has no effect
        return;
    }

    // get the kick angle in the horizontal plane
    double theta = salt::gArcTan2(force[1], force[0]);
    if (mThetaErrorRNG.get() != 0)
    {
        theta += (*(mThetaErrorRNG.get()))();
    }

    float phi = salt::gMin(salt::gMax(kickAction->GetAngle(), mMinAngle), mMaxAngle);
    if (mSigmaPhiEnd > 0.0 || mSigmaPhiMid > 0.0)
    {
        // f will be close to 0.0 if the angle is near the minimum or the maximum.
        // f will be close to 1.0 if the angle is somewhere in the middle of the range.
        float f = 1.0 - 2.0 * salt::gAbs((phi - mMinAngle) / (mMaxAngle - mMinAngle) - 0.5);
        // f is set to a number between mSigmaPhiEnd and mSigmaPhiMid
        f = salt::gMax(mSigmaPhiEnd + f * (mSigmaPhiMid-mSigmaPhiEnd), 0.0);
        phi = salt::NormalRNG<>(phi,f)();
    }
    phi = salt::gDegToRad(90.0-phi);

    // x = r * cos(theta) * sin(90 - phi), with r = 1.0
    force[0] = salt::gCos(theta) * salt::gSin(phi);
    // y = r * sin(theta) * sin(90 - phi), with r = 1.0
    force[1] = salt::gSin(theta) * salt::gSin(phi);
    // z = r * cos(90 - phi),              with r = 1.0
    force[2] = salt::gCos(phi);

    float kick_power = salt::gMin(salt::gMax(kickAction->GetPower(), 1.0f), mMaxPower);
    if (mForceErrorRNG.get() != 0)
    {
        kick_power += (*(mForceErrorRNG.get()))();
    }

    force *= (mForceFactor * kick_power);

    const Vector3f torque(-mTorqueFactor*force[1]/salt::g2PI,
                          mTorqueFactor*force[0]/salt::g2PI,
                          0.0);

    mBall->SetAcceleration(mSteps,force,torque,mAgent);

    mBallStateAspect->UpdateLastKickingAgent(mAgent);

}

shared_ptr<ActionObject>
KickEffector::GetActionObject(const Predicate& predicate)
{
  do
  {
      if (predicate.name != GetPredicate())
          {
              GetLog()->Error() << "ERROR: (KickEffector) invalid predicate"
                                << predicate.name << "\n";
              break;
          }

      Predicate::Iterator iter = predicate.begin();

      float angle;
      if (! predicate.AdvanceValue(iter, angle))
      {
          GetLog()->Error()
              << "ERROR: (KickEffector) kick angle parameter expected\n";
          break;
      }

      float power;
      if (! predicate.AdvanceValue(iter, power))
          {
              GetLog()->Error()
                  << "ERROR: (KickEffector) kick power expected\n";
              break;
          }

      // construct the KickAction object
      return shared_ptr<KickAction>(new KickAction(GetPredicate(),angle,power));

  } while (0);

  // some error happened
  return shared_ptr<ActionObject>();
}

void
KickEffector::OnLink()
{
    SoccerBase::GetBall(*this,mBall);
    SoccerBase::GetBallBody(*this,mBallBody);

    mAgent = shared_dynamic_cast<AgentAspect>(GetParent().lock());

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) parent node is not derived from AgentAspect\n";
        return;
    }

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(mAgent->GetChild("geometry"));
    if (geom.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) parent node has no SphereCollider child\n";
    } else
        {
            mPlayerRadius = geom->GetRadius();
        }

    if (! SoccerBase::GetBallCollider(*this,geom))
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) ball node has no SphereCollider child\n";
    } else
    {
        mBallRadius = geom->GetRadius();
    }

    if (mBallStateAspect.get() == 0)
    {
        mBallStateAspect = shared_dynamic_cast<BallStateAspect>
            (GetCore()->Get("/sys/server/gamecontrol/BallStateAspect"));
        if (mBallStateAspect.get() == 0) return;
    }
}

void
KickEffector::OnUnlink()
{
    mForceErrorRNG.reset();
    mThetaErrorRNG.reset();
    mBallBody.reset();
    mAgent.reset();
}

void
KickEffector::SetKickMargin(float margin)
{
    mKickMargin = margin;
}

void
KickEffector::SetNoiseParams(double sigma_force, double sigma_theta,
                             double sigma_phi_end, double sigma_phi_mid)
{
    NormalRngPtr rng(new salt::NormalRNG<>(0.0,sigma_force));
    mForceErrorRNG = rng;
    NormalRngPtr rng2(new salt::NormalRNG<>(0.0,sigma_theta));
    mThetaErrorRNG = rng2;
    mSigmaPhiEnd = sigma_phi_end;
    mSigmaPhiMid = sigma_phi_mid;
}

void
KickEffector::SetForceFactor(float force_factor)
{
    mForceFactor = force_factor;
}

void
KickEffector::SetTorqueFactor(float torque_factor)
{
    mTorqueFactor = torque_factor;
}

void
KickEffector::SetSteps(int steps)
{
    mSteps = steps;
}

void
KickEffector::SetMaxPower(float max_power)
{
    mMaxPower = max_power;
}

void
KickEffector::SetAngleRange(float min, float max)
{
    if (max <= min)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) min. kick angle should be < max kick angle\n";
        return;
    }
    mMinAngle = min;
    mMaxAngle = max;
}
