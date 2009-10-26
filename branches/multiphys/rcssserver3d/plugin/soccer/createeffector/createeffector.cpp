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
#include "createeffector.h"
#include "createaction.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace std;

CreateEffector::CreateEffector() : Effector()
{
}

void CreateEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{

    if ( mAction.get() == 0 )
        return;

  shared_ptr<CreateAction> createAction =
    shared_dynamic_cast<CreateAction>(mAction);
    mAction.reset();
  if (createAction.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (CreateEffector) cannot realize an unknown ActionObject\n";
      return;
    }

  shared_ptr<AgentAspect> aspect = GetAgentAspect();
  if (aspect.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (CreateEffector) cannot find the AgentAspect\n";
      return;
    }

  // call the ruby addAgent function that has to be defined in the
  // simulator init script with the AgentAspect path as the
  // argument. This function is then responsible to construct the
  // remaining agent nodes
  string cmd = "addAgent('" + aspect->GetFullPath() + "')";
  GetCore()->GetScriptServer()->Eval(cmd);

}

shared_ptr<ActionObject>
CreateEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (CreateEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  //
  // don't care for the supplied parameters for now; the desired agent
  // type should be passed later on and stored in the CreateAction object
  //

  return shared_ptr<CreateAction>(new CreateAction(GetPredicate()));
}

