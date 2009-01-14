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
#include "gamestateitem.h"
#include "gamestateaspect.h"
#include <soccerbase/soccerbase.h>

using namespace oxygen;
using namespace std;

GameStateItem::GameStateItem() : MonitorItem()
{
    ResetSentFlags();
}

GameStateItem::~GameStateItem()
{
}

void GameStateItem::ResetSentFlags()
{
    mSentLeftTeamname = false;
    mSentRightTeamname = false;
    mLastHalf = GH_NONE;
    mLastLeftScore = -1;
    mLastRightScore = -1;
    mLastPlayMode = PM_NONE;
    mSentFlags = false;
}

void GameStateItem::PutFloatParam(const string& name, PredicateList& pList)
{
    float value;
    if (! SoccerBase::GetSoccerVar(*this,name,value))
    {
        return;
    }

    Predicate& pred = pList.AddPredicate();
    pred.name = name;
    pred.parameter.AddValue(value);
}

void GameStateItem::GetInitialPredicates(PredicateList& pList)
{
    ResetSentFlags();

    // field geometry parameter
    PutFloatParam("FieldLength",pList);
    PutFloatParam("FieldWidth",pList);
    PutFloatParam("FieldHeight",pList);
    PutFloatParam("GoalWidth",pList);
    PutFloatParam("GoalDepth",pList);
    PutFloatParam("GoalHeight",pList);
    PutFloatParam("BorderSize",pList);
    PutFloatParam("FreeKickDistance",pList);
    PutFloatParam("WaitBeforeKickOff",pList);

    // agent parameter
    PutFloatParam("AgentMass",pList);
    PutFloatParam("AgentRadius",pList);
    PutFloatParam("AgentMaxSpeed",pList);

    // ball parameter
    PutFloatParam("BallRadius",pList);
    PutFloatParam("BallMass",pList);

    // soccer rule parameters
    PutFloatParam("RuleGoalPauseTime",pList);
    PutFloatParam("RuleKickInPauseTime",pList);
    PutFloatParam("RuleHalfTime",pList);

    // play modes
    Predicate& pred = pList.AddPredicate();
    pred.name = "play_modes";

    for (int i=0; i<PM_NONE; ++i)
    {
        pred.parameter.AddValue
            (SoccerBase::PlayMode2Str(static_cast<TPlayMode>(i)));
    }
}

void GameStateItem::GetPredicates(PredicateList& pList)
{
    if (mGameState.get() == 0)
        {
            return;
        }

    Predicate& timePred = pList.AddPredicate();
    timePred.name = "time";
    timePred.parameter.AddValue(mGameState->GetTime());

    if (! mSentLeftTeamname)
        {
            // team names
            string name = mGameState->GetTeamName(TI_LEFT);
            if (! name.empty())
                {
                    Predicate& teamPredLeft = pList.AddPredicate();
                    teamPredLeft.name = "team_left";
                    teamPredLeft.parameter.AddValue(name);
                    mSentLeftTeamname = true;
                }
        }

    if (! mSentRightTeamname)
        {
            // team names
            string name = mGameState->GetTeamName(TI_RIGHT);
            if (! name.empty())
                {
                    Predicate& teamPredRight = pList.AddPredicate();
                    teamPredRight.name = "team_right";
                    teamPredRight.parameter.AddValue(name);
                    mSentRightTeamname = true;
                }
        }

    // game half
    TGameHalf half = mGameState->GetGameHalf();
    if (half != mLastHalf)
        {
            mLastHalf = half;
            Predicate& halfPred = pList.AddPredicate();
            halfPred.name = "half";
            halfPred.parameter.AddValue(static_cast<int>(half));
        }

    // scores
    int left_score = mGameState->GetScore(TI_LEFT);
    if (left_score != mLastLeftScore)
        {
            mLastLeftScore = left_score;
            Predicate& scoreLeftPred = pList.AddPredicate();
            scoreLeftPred.name = "score_left";
            scoreLeftPred.parameter.AddValue(left_score);
        }

    int right_score = mGameState->GetScore(TI_RIGHT);
    if (right_score != mLastRightScore)
        {
            mLastRightScore = right_score;
            Predicate& scoreRightPred = pList.AddPredicate();
            scoreRightPred.name = "score_right";
            scoreRightPred.parameter.AddValue(right_score);
        }

    // gamestate
    TPlayMode play_mode = mGameState->GetPlayMode();
    if (play_mode != mLastPlayMode)
        {
            mLastPlayMode = play_mode;
            Predicate& modePred = pList.AddPredicate();
            modePred.name = "play_mode";
            modePred.parameter.AddValue(static_cast<int>(play_mode));
        }
}

void GameStateItem::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);
}

void GameStateItem::OnUnlink()
{
    mGameState.reset();
}
