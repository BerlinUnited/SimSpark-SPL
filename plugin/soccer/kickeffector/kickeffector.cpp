/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kickeffector.cpp,v 1.1.2.7 2004/02/06 15:11:02 rollmark Exp $

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
#include <soccer/soccerbase/soccerbase.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

KickEffector::KickEffector()
    : oxygen::Effector(),
      mKickMargin(0.04),mForceFactor(4.0),mMaxPower(100.0),
      mMinAngle(0.0),mMaxAngle(50.0),
      mMinSteps(3),mMaxSteps(75),
      mSigmaForce(0.4), mSigmaTheta(0.02), mSigmaPhiEnd(0.9), mSigmaPhiMid(4.5)
{
}

KickEffector::~KickEffector()
{
}

bool
KickEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    // this should also include the case when there is no ball
    // (because then there will be no body, neither).
    if (mBallBody.get() == 0)
    {
        return false;
    }

    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) parent node is not derived from BaseNode\n";
        return false;
    }

    shared_ptr<KickAction> kickAction =
        shared_dynamic_cast<KickAction>(action);

    if (kickAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    Vector3f force =
        mBallBody->GetWorldTransform().Pos() -
        parent->GetWorldTransform().Pos();

    // the ball can be kicked if the distance is
    // less then Ball-Radius + Player-Radius + KickMargin AND
    // the player is close to the ground
    if (parent->GetWorldTransform().Pos().y() > mPlayerRadius + 0.01 ||
        force.Length() > mPlayerRadius + mBallRadius + mKickMargin)
    {
        // ball is out of reach, or player is in the air:
        // kick has no effect
        return true;
    }

    // get the kick angle in the horizontal plane
    double theta = salt::gArcTan2(force[2], force[0]);
    if (mSigmaTheta > 0.0)
    {
        theta += salt::NormalRNG<>(0.0,mSigmaTheta)();
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
    force[2] = salt::gSin(theta) * salt::gSin(phi);
    // z = r * cos(90 - phi),              with r = 1.0
    force[1] = salt::gCos(phi);

    float kick_power = salt::gMin(salt::gMax(kickAction->GetPower(), 1.0f), mMaxPower);
    if (mSigmaForce > 0.0)
    {
        kick_power += salt::NormalRNG<>(0.0,mSigmaForce)();
    }

    int steps = mMinSteps + static_cast<int>((mMaxSteps-mMinSteps) * (kick_power / mMaxPower));

    force *= (mForceFactor * kick_power);
    Vector3f torque(force[2]/(salt::g2PI * mBallRadius),
                    0.0,
                    -force[0]/(salt::g2PI * mBallRadius));

    GetLog()->Debug() << "DEBUG: (KickEffector): " << kick_power << ": "
                      << force[0] << " " << force[1] << " " << force[2] << " / "
                      << torque[0] << " " << torque[1] << " " << torque[2] << " / "
                      << steps << std::endl;

    // if the agent doesn't have a body, we're done (this should never happen)
    if (mBall.get() == 0) return true;

    mBall->SetAcceleration(steps,force,torque);
    return true;
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
      if (!predicate.GetValue(iter, angle))
      {
          GetLog()->Error()
              << "ERROR: (KickEffector) kick angle parameter expected\n";
          break;
      }

      float power;
      if (! predicate.GetValue(iter, power))
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

    shared_ptr<BaseNode> parent =
        shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (KickEffector) parent node is not derived from BaseNode\n";
        return;
    }

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(parent->GetChild("geometry"));
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
}

void
KickEffector::OnUnlink()
{
    mBallBody.reset();
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
    mSigmaForce = sigma_force;
    mSigmaTheta = sigma_theta;
    mSigmaPhiEnd = sigma_phi_end;
    mSigmaPhiMid = sigma_phi_mid;
}

void
KickEffector::SetForceFactor(float force_factor)
{
    mForceFactor = force_factor;
}

void
KickEffector::SetSteps(int min, int max)
{
    mMinSteps = min;
    mMaxSteps = max;
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
