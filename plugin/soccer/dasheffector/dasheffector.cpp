/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: dasheffector.cpp,v 1.1.2.1 2004/01/25 12:09:24 rollmark Exp $

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
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

DashEffector::DashEffector() : oxygen::Effector()
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

  shared_ptr<DashAction> dashAction =
    shared_dynamic_cast<DashAction>(action);

  if (dashAction.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (DashEffector) cannot realize an unknown ActionObject\n";
      return false;
    }

  const Vector3f& force = dashAction->GetForce();
  mBody->AddForce(force);

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
  shared_ptr<BaseNode> parent =
    shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

  if (parent.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (DashEffector) parent node is not derived from BaseNode\n";
      return;
    }

  // parent should be a transform, or some other node, which has a
  // Body-child
  mBody = shared_dynamic_cast<Body>(parent->GetChildOfClass("Body"));

  if (mBody.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (DashEffector) parent node has no Body child;"
          "cannot apply force\n";
      return;
    }
}

void DashEffector::OnUnlink()
{
    mBody.reset();
}


