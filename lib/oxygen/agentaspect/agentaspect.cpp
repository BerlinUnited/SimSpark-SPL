/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentaspect.cpp,v 1.3.4.3 2003/12/10 10:26:35 rollmark Exp $

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
#include "agentaspect.h"
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

bool
AgentAspect::RealizeActions(boost::shared_ptr<ActionObject::TList> actions)
{
  UpdateEffectorMap();

  for (
       ActionObject::TList::iterator iter = actions->begin();
       iter != actions->end();
       ++iter
       )
    {
      shared_ptr<ActionObject> action = (*iter);
      std::string predicate = action->GetPredicate();

      TEffectorMap::iterator iter = mEffectorMap.find(predicate);
      if (iter == mEffectorMap.end())
        {
          GetLog()->Warning()
            <<  "(AgentAspect) No effector found for predicate "
            << predicate << "\n";
          continue;
        }

      shared_ptr<Effector> effector = (*iter).second;
      bool realized = effector->Realize(action);

      if (! realized)
        {
          GetLog()->Warning()
            << "(AgentAspect) Failed to realize predicate "
            << predicate << "\n";
        }
    }

  return true;
}

BaseParser::TPredicateList
AgentAspect::QueryPerceptors()
{
  // traverse all Perceptors and build a list of the collected
  // Predicates
  return BaseParser::TPredicateList();
}

void AgentAspect::UpdateEffectorMap()
{
  // build list of effectors, searching recursively
  TLeafList effectors;
  GetChildrenSupportingClass("oxygen/Effector",effectors,true);

  // build the effector map
  mEffectorMap.clear();

  for (
       TLeafList::iterator iter = effectors.begin();
       iter != effectors.end();
       ++iter
       )
    {
      shared_ptr<Effector> effector
        = shared_dynamic_cast<Effector>(*iter);

      if (effector.get() == 0)
        {
          continue;
        }

      mEffectorMap[effector->GetPredicate()] = effector;
    }
}
