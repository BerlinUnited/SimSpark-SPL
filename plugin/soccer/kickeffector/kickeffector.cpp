/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kickeffector.cpp,v 1.1.2.2 2004/01/26 15:19:52 fruit Exp $

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
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

KickEffector::KickEffector()
    : oxygen::Effector(),
      mKickMargin(0.04),
      mSigmaForce(-1.0), mSigmaTheta(-1.0), mSigmaPhi(-1.0)
{
}

KickEffector::~KickEffector()
{
}

bool
KickEffector::Realize(boost::shared_ptr<ActionObject> action)
{
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

  Vector3f vec =
      mBallBody->GetWorldTransform().Pos() -
      parent->GetWorldTransform().Pos();

  if (salt::gAbs(vec[1]) >= 0.3 || vec.Length() >= 0.3 + 0.111 + 0.04)
  {
      // ball is out of reach, kick
      // has no effect
      return true;
  }

  // kick horizontally, i.e. zero up component
  vec[1] = 0;
  vec.Normalize();
  if (kickAction->GetType() == KickAction::KT_STEEP)
  {
      vec[1] = salt::gCos(salt::gDegToRad(50.0));
  }

  vec *= kickAction->GetPower();

  mBallBody->AddForce(vec);

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

      string strType;
      if (! predicate.GetValue(iter, strType))
          {
              GetLog()->Error()
                  << "ERROR: (KickEffector) kick type parameter expected\n";
              break;
          }

      KickAction::EKickType type;
      if (strType == "up")
          {
              type = KickAction::KT_STEEP;
          }
      else if (strType == "down")
          {
              type = KickAction::KT_HORIZONTAL;
          }
      else
          {
              GetLog()->Error()
                  << "ERROR: (KickEffector) kick type parameter expected\n";
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
      return shared_ptr<KickAction>(new KickAction(GetPredicate(),type,power));

  } while (0);

  // some error happened
  return shared_ptr<ActionObject>();
}

void KickEffector::OnLink()
{
    mBallBody = shared_dynamic_cast<Body>
        (GetCore()->Get("/usr/scene/Ball/physics"));

  if (mBallBody.get() == 0)
      {
          GetLog()->Error()
              << "ERROR: (KickEffector) Ball body node not found\n";
          return;
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
KickEffector::SetNoiseParams(double sigma_force, double sigma_theta, double sigma_phi)
{
    mSigmaForce = sigma_force;
    mSigmaTheta = sigma_theta;
    mSigmaPhi = sigma_phi;
}
