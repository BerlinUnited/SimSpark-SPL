/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccermonitor.cpp,v 1.1 2004/12/22 16:15:19 rollmark Exp $

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
#include "soccermonitor.h"

using namespace oxygen;
using namespace std;

SoccerMonitor::SoccerMonitor() : CustomMonitor()
{
    mTime     = 0.0;
    mPlayMode = -1;
    SetupPredicateMap();
}

SoccerMonitor::~SoccerMonitor()
{
}


void SoccerMonitor::ParseCustomPredicates(const PredicateList& pList)
{
    ParsePredicates(pList);
}

void SoccerMonitor::ParsePredicates(const PredicateList& pList)
{
    for (
         PredicateList::TList::const_iterator iter = pList.begin();
         iter != pList.end();
         ++iter
         )
        {
            const Predicate& pred = (*iter);
            TPredicateMap::const_iterator pIter = mPredMap.find(pred.name);
            if (pIter == mPredMap.end())
                {
                    continue;
                }

            const EPredicate& type = (*pIter).second;
            switch (type)
                {
                case P_TIME:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mTime);
                        }
                    break;

                case P_PLAYMODE:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mPlayMode);
                        }
                    break;

                default:
                    break;
                }
        }
}

void SoccerMonitor::SetupPredicateMap()
{
    mPredMap.clear();
    mPredMap["time"]      = P_TIME;
    mPredMap["play_mode"] = P_PLAYMODE;
}

