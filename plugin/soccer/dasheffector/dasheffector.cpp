/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: dasheffector.cpp,v 1.1.2.6 2004/02/06 10:08:38 rollmark Exp $

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
                               mForceFactor(500.0),mSigma(-1.0),mMaxPower(100.0)
{
}

DashEffector::~DashEffector()
{
}

bool
DashEffector::Realize(boost::shared_ptr<ActionObject> action)
{
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

    Vector3f vec = parent->GetWorldTransform().Pos();

    shared_ptr<SphereCollider> geom =
        shared_dynamic_cast<SphereCollider>(parent->GetChild("geometry"));

    float max_dist = std::numeric_limits<float>::epsilon();
    if (geom.get() == 0)
    {
        GetLog()->Error() << "ERROR: (DashEffector) parent node has "
                          << "no 'geometry' sphere child\n";
    } else
        {
            max_dist += geom->GetRadius();
        }

    // we can only dash if the sphere is on the ground
    if (vec[1] > max_dist)
    {
        return true;
    }

    Vector3f force = dashAction->GetForce();

    // cut down the dash power vector to maximum length
    if (force.SquareLength() > mMaxPower * mMaxPower)
    {
        force.Normalize();
        force *= mMaxPower;
    }

    if (mSigma > 0.0)
    {
        force[0] = force[0] * mForceFactor / salt::NormalRNG<>(mMaxPower,mSigma)();
        force[1] = force[1] * mForceFactor / salt::NormalRNG<>(mMaxPower,mSigma)();
        force[2] = force[2] * mForceFactor / salt::NormalRNG<>(mMaxPower,mSigma)();
    } else {
        force = force * mForceFactor / mMaxPower;
    }

    if (mAgentState.get() == 0)
    {
        mAgentState =
            shared_static_cast<AgentState>(parent->GetChild("AgentState"));
    }

    TTeamIndex ti = TI_NONE;
    if (mAgentState.get() != 0)
    {
        ti = mAgentState->GetTeamIndex();
    }
    mBody->AddForce(SoccerBase::FlipView(Vector3f(force[0],force[2],force[1]), ti));

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

void DashEffector::OnLink()
{
    SoccerBase::GetBody(*this,mBody);
}

void DashEffector::OnUnlink()
{
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
