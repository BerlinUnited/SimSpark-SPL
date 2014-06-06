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
#include "gamestateaspect.h"
#include <algorithm>
#include <zeitgeist/logserver/logserver.h>
#include <soccerbase/soccerbase.h>
#include <agentstate/agentstate.h>
#include <salt/random.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

GameStateAspect::GameStateAspect() : SoccerControlAspect()
{
    mPlayMode = PM_BeforeKickOff;
    mTime = 0;
    mLeadTime = 0;
    mFupTime = 0;
    mLastModeChange = 0;
    mGameHalf = GH_FIRST;
    mRobotTypeCount[0].push_back(0); // add count for type 0 (default type)
    mRobotTypeCount[1].push_back(0);
    mScore[0] = 0;
    mScore[1] = 0;
    mLastKickOffGameHalf = GH_NONE;
    mNextHalfKickOff = TI_NONE;
    mLeftInit = Vector3f(0,0,0);
    mRightInit = Vector3f(0,0,0);
    mAgentRadius = 3.5;
    mFinished = false;
    mGamePaused = true;
    mMaxRobotTypeCount = 7;
    mMinRobotTypesCount = 3;
    mMaxSumTwoRobotTypes = 9;
    mInternalIndex[TI_NONE] = -1;
    mInternalIndex[TI_LEFT] = 0;
    mInternalIndex[TI_RIGHT] = 1;
}

GameStateAspect::~GameStateAspect()
{
}

void
GameStateAspect::UpdateTime(float deltaTime)
{
    switch (mPlayMode)
    {
    case PM_BeforeKickOff:
        mLeadTime += deltaTime;
        break;
    case PM_GameOver:
        mFupTime += deltaTime;
        break;
    default:
        mTime += deltaTime;
    }
}

void
GameStateAspect::Update(float deltaTime)
{
    UpdateTime(deltaTime);
}

TPlayMode
GameStateAspect::GetPlayMode() const
{
    return mPlayMode;
}

void
GameStateAspect::SetPlayMode(TPlayMode mode)
{
    if (mode == mPlayMode)
    {
        return;
    }

    GetLog()->Normal() << "(GameStateAspect) playmode changed to "
                       << SoccerBase::PlayMode2Str(mode) << " at t="
                       << mTime << "\n";
    mPlayMode = mode;
    mLastModeChange = mTime;
    mLeadTime = 0.0;
    mFupTime = 0.0;
}


void
GameStateAspect::KickOff(TTeamIndex ti)
{
    // throw a coin to determine which team kicks off, except if a new half
    // is started in which the opposite team should kick off
    if (ti == TI_NONE)
    {
        ti = (salt::UniformRNG<>(0,1)() <= 0.5) ? TI_LEFT : TI_RIGHT;

        if (mGameHalf != mLastKickOffGameHalf)
        {
            if (mNextHalfKickOff != TI_NONE)
                ti = mNextHalfKickOff;

            bool changeSides;
            SoccerBase::GetSoccerVar(*this, "ChangeSidesInSecondHalf",
                changeSides);

            if (changeSides)
                mNextHalfKickOff = ti;
            else
                mNextHalfKickOff = SoccerBase::OpponentTeam(ti);
        }
    }

    SetPlayMode((ti == TI_LEFT) ? PM_KickOff_Left : PM_KickOff_Right);
    mLastKickOffGameHalf = mGameHalf;
}

TTime
GameStateAspect::GetTime() const
{
    return mTime;
}

TTime
GameStateAspect::GetModeTime() const
{
    switch (mPlayMode)
    {
    case PM_BeforeKickOff:
        return mLeadTime;
    case PM_GameOver:
        return mFupTime;
    default:
        return mTime - mLastModeChange;
    }
}

TTime
GameStateAspect::GetLastModeChange() const
{
    return mLastModeChange;
}

void
GameStateAspect::SetTeamName(TTeamIndex idx, const std::string& name)
{
    int i = GetInternalIndex(idx);
    if (i > -1)
        mTeamName[i] = name;
    return;
}

std::string
GameStateAspect::GetTeamName(TTeamIndex idx) const
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return "";
    return mTeamName[i];
}

TTeamIndex
GameStateAspect::GetTeamIndex(const std::string& teamName)
{
    for (int i=0; i<=1; ++i)
    {
        if (mTeamName[i].empty())
        {
            mTeamName[i] = teamName;
            return GetInternalIndex(TI_LEFT) == i ? TI_LEFT : TI_RIGHT;
        }

        if (mTeamName[i] == teamName)
        {
            return GetInternalIndex(TI_LEFT) == i ? TI_LEFT : TI_RIGHT;
        }
    }

    return TI_NONE;
}

inline int
GameStateAspect::GetInternalIndex(TTeamIndex idx) const
{
    return mInternalIndex[idx];
}

bool
GameStateAspect::InsertUnum(TTeamIndex idx, int unum)
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return false;

    TUnumSet& set = mUnumSet[i];

    if (
        (set.size() >= 11) ||
        (set.find(unum) != set.end())
        )
    {
        return false;
    }

    set.insert(unum);

    return true;
}

bool
GameStateAspect::EraseUnum(TTeamIndex idx, int unum)
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return false;

    TUnumSet& set = mUnumSet[i];

    if (
        (set.find(unum) == set.end())
        )
    {
        return false;
    }

    set.erase(unum);

    return true;
}

bool
GameStateAspect::InsertRobotType(TTeamIndex idx, int type)
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return false;

    int numRobots = 0;
    int numRobotTypes = 0;
    int maxSumTwoRobotTypes = 0;

    if (mRobotTypeCount[i].size() <= type)
        mRobotTypeCount[i].resize(type+1);

    for (int j = 0; j < mRobotTypeCount[i].size(); j++) 
    {
        if (mRobotTypeCount[i][j] > 0)
        {
            numRobots += mRobotTypeCount[i][j];
            numRobotTypes++;
        }
        int sumTwoRobotTypes = mRobotTypeCount[i][type]+1;
        if (j != type)
            sumTwoRobotTypes += mRobotTypeCount[i][j];
       
        if (sumTwoRobotTypes > maxSumTwoRobotTypes)
            maxSumTwoRobotTypes = sumTwoRobotTypes;
    }

    if (mRobotTypeCount[i][type] == mMaxRobotTypeCount)
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::InsertRobotType) No more robots "
            "of type " << type << " are allowed.\n";
        return false;
    }

    if (maxSumTwoRobotTypes > mMaxSumTwoRobotTypes) 
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::InsertRobotType) Maximum sum of "
            "robots of two robot types limit reached. No more robots of "
            "type " << type << " are allowed.\n";
        return false;
    }
    
    if ((11-numRobots) <= (mMinRobotTypesCount-numRobotTypes)) 
    {
        if (mRobotTypeCount[i][type] != 0) 
        {
            GetLog()->Error()
                << "ERROR: (GameStateAspect::InsertRobotType) Minimum number"
                " of different robot types not reached. Only robots of a type"
                " not yet used can be added.\n";
            return false;
        }
    }

    ++mRobotTypeCount[i][type];

    return true;
}

bool
GameStateAspect::EraseRobotType(TTeamIndex idx, int type)
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return false;

    if (mRobotTypeCount[i].size() <= type || !mRobotTypeCount[i][type])
    {
        return false;
    }

    --mRobotTypeCount[i][type];

    return true;
}

bool
GameStateAspect::RequestUniform(boost::shared_ptr<AgentState> agentState,
                                std::string teamName, unsigned int unum)
{
    if (agentState.get() == 0)
    {
        return false;
    }

    TTeamIndex idx = GetTeamIndex(teamName);

    if (idx == TI_NONE)
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::RequestUniform) invalid teamname "
            << teamName << "\n";
        return false;
    }

    if (unum == 0)
    {
        unum = RequestUniformNumber(idx);
    }

    if (! InsertUnum(idx,unum))
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::RequestUniform) cannot insert uniform"
            " number " << unum << " to team " << teamName << "\n";
        return false;
    }

    if (!InsertRobotType(idx, agentState->GetRobotType()))
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::RequestUniform) cannot insert robot"
            " of type " << agentState->GetRobotType() << " to team "
            << teamName << "\n";
        return false;
    }

    agentState->SetUniformNumber(unum);
    agentState->SetTeamIndex(idx);
    //agentState->SetPerceptName(teamName, ObjectState::PT_Default);
    agentState->SetPerceptName(teamName, ObjectState::PT_Default, ObjectState::PT_Player );

    agentState->SetPerceptName("player", ObjectState::PT_TooFar);

    GetLog()->Normal() << "(GameStateAspect) handed out uniform number "
                       << unum << " for team " << teamName << "\n";

    return true;
}

bool
GameStateAspect::ReturnUniform(TTeamIndex ti, unsigned int unum, int type)
{
    if (! EraseUnum(ti,unum))
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::ReturnUniform) cannot erase uniform"
            " number " << unum << " from team " << ti << "\n";
        return false;
    }

    if (!EraseRobotType(ti, type))
    {
        GetLog()->Error()
            << "ERROR: (GameStateAspect::ReturnUniform) cannot erase robot "
            " type " << type << " from team " << ti << "\n";
        return false;
    }

    return true;
}

void
GameStateAspect::SetGameHalf(TGameHalf half)
{
    if (
        (half != GH_FIRST) &&
        (half != GH_SECOND)
        )
    {
        return;
    }

    mGameHalf = half;
}

TGameHalf
GameStateAspect::GetGameHalf() const
{
    return mGameHalf;
}

void
GameStateAspect::ScoreTeam(TTeamIndex idx)
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return;

    ++mScore[i];
}

int
GameStateAspect::GetScore(TTeamIndex idx) const
{
    int i = GetInternalIndex(idx);
    if (i < 0)
        return 0;

    return mScore[i];
}

Vector3f
GameStateAspect::RequestInitPosition(const TTeamIndex ti)
{
    if (ti == TI_NONE)
    {
        GetLog()->Debug()
            << "(GameStateAspect) RequestInitPosition called with "
            << "ti=TI_NONE\n";
        return Vector3f(0,0,10);
    }

    salt::Vector3f& init = (ti ==TI_LEFT) ? mLeftInit : mRightInit;

    Vector3f pos = init;
    init[1] -= mAgentRadius * 3;

    float fieldWidth;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",fieldWidth);

    if (init[1] < -fieldWidth/2.0)
        {
            init[1] = fieldWidth/2 - mAgentRadius*2;
            init[0] += mAgentRadius * 2;
        }

    return pos;
}

float
GameStateAspect::RequestInitOrientation(const TTeamIndex ti) const
{
    switch ( ti )
    {
        case TI_LEFT: return -90;
        break;
        case TI_RIGHT: return 90;
        break;
    }
    return 0;
}

void
GameStateAspect::OnLink()
{
    // setup the initial starting positions for the agents
    float fieldWidth = 64.0;
    SoccerBase::GetSoccerVar(*this,"FieldWidth",fieldWidth);

    float fieldLength = 100.0;
    SoccerBase::GetSoccerVar(*this,"FieldLength",fieldLength);

    mAgentRadius = 3.5;
    SoccerBase::GetSoccerVar(*this,"AgentRadius",mAgentRadius);

    mLeftInit = Vector3f
        (
            -fieldLength/2.0 + mAgentRadius*2,
            fieldWidth/2 - mAgentRadius*2,
            mAgentRadius
            );

    mRightInit = Vector3f
        (
            +fieldLength/2.0 - mAgentRadius*2,
            fieldWidth/2  - mAgentRadius*2,
            mAgentRadius
            );

    bool coinTossKickOff = true;
    SoccerBase::GetSoccerVar(*this, "CoinTossForKickOff", coinTossKickOff);
    if (!coinTossKickOff)
        mNextHalfKickOff = TI_LEFT;

    SoccerBase::GetSoccerVar(*this, "MaxRobotTypeCount", mMaxRobotTypeCount);
    SoccerBase::GetSoccerVar(*this, "MinRobotTypesCount", mMinRobotTypesCount);
    SoccerBase::GetSoccerVar(*this, "MaxSumTwoRobotTypes", mMaxSumTwoRobotTypes);
}

int
GameStateAspect::RequestUniformNumber(TTeamIndex ti) const
{
    int idx = GetInternalIndex(ti);
    if (idx < 0)
        return 0;

    for (int i = 1; i <=11; ++i)
      if (mUnumSet[idx].find(i) == mUnumSet[idx].end())
        return i;

    return 0;
}

void GameStateAspect::SetTime(float time)
{
    mTime = time;
}

void GameStateAspect::SetScores(int scoreLeft, int scoreRight)
{
    mScore[0] = scoreLeft;
    mScore[1] = scoreRight;
}

bool GameStateAspect::IsPaused() const
{
    return mGamePaused;
}

void GameStateAspect::SetPaused(bool paused)
{
    mGamePaused = paused;
}

void GameStateAspect::SwapTeamIndexes()
{
    swap(mInternalIndex[TI_LEFT], mInternalIndex[TI_RIGHT]);
}
