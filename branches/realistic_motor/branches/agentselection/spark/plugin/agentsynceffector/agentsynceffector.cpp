/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

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

#include "agentsynceffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/agentaspect/agentaspect.h>

using namespace boost;
using namespace oxygen;
using namespace std;

AgentSyncEffector::AgentSyncEffector() : oxygen::Effector()
{
}

AgentSyncEffector::~AgentSyncEffector()
{
}

bool AgentSyncEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    bool res = Effector::Realize(action);

    if (mAgentAspect)
    {
        mAgentAspect->SetSynced(true);
    }
    return res;
}

boost::shared_ptr<ActionObject>
AgentSyncEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (AgentSyncEffector) invalid predicate"
                        << predicate.name << "\n";
      return boost::shared_ptr<ActionObject>();
    }

  return boost::shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
}

void AgentSyncEffector::OnLink()
{
  mAgentAspect = GetAgentAspect();

  if (mAgentAspect.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (AgentSyncEffector) cannot get the AgentAspect object\n";
      return;
    }

  mAgentAspect = mAgentAspect->FindParentSupportingClass<AgentAspect>().lock();
  if (mAgentAspect.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (AgentSyncEffector) cannot get the grand parent AgentAspect"
                " object\n";
      return;
    }
}

void AgentSyncEffector::OnUnlink()
{
    mAgentAspect.reset();
}
