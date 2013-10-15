/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesactevent.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "spadesactevent.h"
#include "spadesserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/agentaspect/agentaspect.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

void SpadesActEvent::Print (std::ostream & /*o*/) const
{
}

bool SpadesActEvent::realizeEventWorldModel(spades::WorldModel* pWM)
{
  // we can't create a shared_ptr to the SpadesServer here, as this
  // shared_ptr wouldn't know of other shared_ptrs to the
  // SpadesServer
  SpadesServer* spadesServer = dynamic_cast<SpadesServer*>(pWM);

  if (spadesServer == NULL)
        {
          // we can't use the LogServer here :(
          return false;
        }

  shared_ptr<GameControlServer> gcs = spadesServer->GetGameControlServer();
  if (gcs.get() == 0)
      {
          spadesServer->GetLog()->Error()
              << "(SpadesActEvent) GameControlServer not found." << endl;
          return false;
      }

  // lookup the AgentAspect
  int id = getAgent();
  shared_ptr<AgentAspect> agent = gcs->GetAgentAspect(id);

  if (agent.get() == 0)
      {
          spadesServer->GetLog()->Warning()
              << "ERROR: (SpadesActEvent) no AgentAspect for id " << id
              << "found." << endl;
          return false;
      }

  // realize the stored actions
  agent->RealizeActions(mActionList);

  // indicate that we 'did something' with the event
  return true;
}
