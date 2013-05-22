/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateitem.cpp 259 2011-03-24 20:25:24Z hedayat $

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
#include "soccerruleitem.h"
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace std;

SoccerRuleItem::SoccerRuleItem()
    : MonitorItem(),
      mLastFoul(0)
{
}

SoccerRuleItem::~SoccerRuleItem()
{
}

void SoccerRuleItem::GetInitialPredicates(PredicateList& pList)
{
    GetPredicates(pList);
}

void SoccerRuleItem::AddFoul(SoccerRuleAspect::Foul const& foul, PredicateList& pList)
{
    Predicate& predicate = pList.AddPredicate();
    predicate.name = "foul";
    predicate.parameter.AddValue(foul.index);
    predicate.parameter.AddValue((int)foul.type);
    predicate.parameter.AddValue((int)foul.agent->GetTeamIndex());
    predicate.parameter.AddValue((int)foul.agent->GetUniformNumber());
}

void SoccerRuleItem::GetPredicates(PredicateList& pList)
{
    if (mSoccerRule.get() == 0)
    {
        return;
    }

    vector<SoccerRuleAspect::Foul> newFouls = mSoccerRule->GetFoulsSince(mLastFoul);
    for (unsigned i = 0; i < newFouls.size(); ++i)
    {
        AddFoul(newFouls[i], pList);
        mLastFoul = newFouls[i].index;
    }
}

void SoccerRuleItem::OnLink()
{
    SoccerBase::GetSoccerRuleAspect(*this, mSoccerRule);
}

void SoccerRuleItem::OnUnlink()
{
    mSoccerRule.reset();
}
