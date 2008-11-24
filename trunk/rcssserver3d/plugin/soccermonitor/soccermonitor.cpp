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
#include "soccermonitor.h"

using namespace zeitgeist;
using namespace oxygen;
using namespace std;

SoccerMonitor::SoccerMonitor() : CustomMonitor()
{
    mTime     = 0.0;
    mPlayMode = PM_NONE;
    mHalf     = 0;
    mScoreLeft = 0;
    mScoreRight = 0;
    
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
                case P_PLAYMODES:
                    ParsePlayModes(pred);
                    break;

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

                case P_HALF:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mHalf);
                        }
                    break;
                case P_LEFTTEAM:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mTeamNameLeft);
                        }                    
                    break;
                case P_RIGHTTEAM:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mTeamNameRight);
                        }                   
                    break;
                case P_LEFTSCORE:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mScoreLeft);
                        }                    
                    break;
                case P_RIGHTSCORE:
                    if (pred.parameter.GetSize() == 1)
                        {
                            pred.GetValue(pred.begin(),mScoreRight);
                        }                    
                    break;
                case P_FIELD_LENGTH:
                    if (pred.parameter.GetSize() == 1){
                        pred.GetValue(pred.begin(),mFieldLength);
                    }
                    break;
                case P_FIELD_WIDTH:
                    if (pred.parameter.GetSize() == 1){
                        pred.GetValue(pred.begin(),mFieldWidth);
                    }
                    break;
                default:
                    break;
                }
        }
}

void SoccerMonitor::ParsePlayModes(const Predicate& pred)
{
    mPlayModes.clear();

    const ParameterList& paramList = pred.parameter;
    ParameterList::TVector::const_iterator pIter = paramList.begin();

    std::string pMode;
    while (
           (pIter != paramList.end()) &&
           (paramList.AdvanceValue(pIter, pMode))
           )
        {
            mPlayModes.push_back(pMode);
        }
}

void SoccerMonitor::SetupPredicateMap()
{
    mPredMap.clear();
    mPredMap["play_modes"]  = P_PLAYMODES;
    mPredMap["time"]        = P_TIME;
    mPredMap["play_mode"]   = P_PLAYMODE;
    mPredMap["half"]        = P_HALF;
    mPredMap["team_left"]   = P_LEFTTEAM;
    mPredMap["team_right"]  = P_RIGHTTEAM;
    mPredMap["score_left"]  = P_LEFTSCORE;
    mPredMap["score_right"] = P_RIGHTSCORE;
    mPredMap["FieldLength"] = P_FIELD_LENGTH;
    mPredMap["FieldWidth"] = P_FIELD_WIDTH;
}

TTime SoccerMonitor::GetTime() const
{
    return static_cast<TTime>(mTime);
}

TPlayMode SoccerMonitor::GetPlayMode() const
{
    return static_cast<TPlayMode>(mPlayMode);
}

string SoccerMonitor::GetPlayModeString() const
{
    if (
        (mPlayMode < 0) ||
        (mPlayMode >= mPlayModes.size())
        )
        {
            return "playmode_unknown";
        }

    return mPlayModes[mPlayMode];
}

TGameHalf SoccerMonitor::GetGameHalf() const
{
    return static_cast<TGameHalf>(mHalf);
}

string SoccerMonitor::GetGameHalfString() const
{
    switch (mHalf)
        {
        default:
            return "unknown half";

        case 1:
            return "1st half";

        case 2:
            return "2nd half";
        }
}

std::string SoccerMonitor::GetTeamNameLeft() const
{
    return mTeamNameLeft;
}

std::string SoccerMonitor::GetTeamNameRight() const
{
    return mTeamNameRight;
}

int SoccerMonitor::GetScoreLeft() const
{
    return mScoreLeft;
}

int SoccerMonitor::GetScoreRight() const
{
    return mScoreRight;
}

salt::Vector2f SoccerMonitor::GetFieldSize() const
{
    return salt::Vector2f(mFieldLength,mFieldWidth);
}
