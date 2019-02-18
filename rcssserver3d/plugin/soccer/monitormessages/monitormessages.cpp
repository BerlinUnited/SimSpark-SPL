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
#include "monitormessages.h"
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace std;

MonitorMessages::MonitorMessages()
    : MonitorItem()
{
}

MonitorMessages::~MonitorMessages()
{
}

void MonitorMessages::GetInitialPredicates(PredicateList& /*pList*/)
{
}

void MonitorMessages::GetPredicates(PredicateList& pList)
{
//    std::cout << "available messages: " << msg_list.size() << std::endl;
    Predicate& pred = pList.AddPredicate();
    pred.name = "messages";
    for(std::list<message_info>::iterator it = msg_list.begin(); it != msg_list.end(); ++it)
    {
      std::stringstream ss;
      ss << "("
          << it->message << " "
          << "(side " << it->ti_side << ") "
          << "(ip " << it->ip << ") "
        << ")";
      pred.parameter.AddValue(ss.str());
    }
    msg_list.clear();
}

void MonitorMessages::OnLink()
{
  /* INFO: don't know which is the "recommended" way to get a control node!?
   * -> "direct": as implemented
   * or
   * -> "indirect": get SimulationServer first and then 'GetControlNode("AgentControl")'
   */
  // set the agent control, needed for retrieving agent/client address
  mAgentControl = boost::dynamic_pointer_cast<AgentControl>(GetCore()->Get("/sys/server/simulation/AgentControl"));
  if (mAgentControl.get() == 0)
  {
    GetLog()->Error() << "ERROR: (MonitorMessages) Unable to get AgentControl\n";
  }
}

void MonitorMessages::OnUnlink()
{
    msg_list.clear();
    mAgentControl.reset();
}

void MonitorMessages::addMessage(const std::string& msg, const TTeamIndex idx, const int num)
{
  std::string ip = "127.0.0.1";
  if (mAgentControl.get() != 0)
  {
    mAgentControl->GetClientAddr(num, ip);
  }
  msg_list.push_back({idx, msg, ip});
}
