/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestate.cpp,v 1.8 2004/06/11 14:30:33 fruit Exp $

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

   Main of the rcssmonitor3D application

   This is a very simple 3D monitor example which will hopefully be
   useful for the first steps of 3D SoccerServer
*/
#include "gamestate.h"
#include "types.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

using namespace oxygen;

GameState::GameState() :
    mTeamLeft("team_left"),
    mTeamRight("team_right"),
    mHalf(-1),
    mTime(0.0),
    mScoreLeft(0),
    mScoreRight(0),
    mPlayMode(-1)
{
    ResetBall();
    ResetPlayers();
    ResetFlags();
    SetupProcessMap();
    SetupFlagMap();
    SetupPlaymodeMap();

    mFieldWidth = DEFAULT_FIELD_WIDTH;
    mFieldLength = DEFAULT_FIELD_LENGTH;
    mFieldHeight = DEFAULT_FIELD_HEIGHT;
    mGoalWidth = DEFAULT_GOAL_WIDTH;
    mGoalDepth = DEFAULT_GOAL_DEPTH;
    mGoalHeight = DEFAULT_GOAL_HEIGHT;
    mBorderSize = DEFAULT_BORDER_SIZE;
    mLineWidth = DEFAULT_LINE_WIDTH;
    mFinished = false;
}

GameState::~GameState()
{
}

std::string
GameState::PlayMode2Str(int mode) const
{
    if (mode < 0 || mode >= mPlaymodes.size())
    {
        return "playmode_unknown";
    }
    return mPlaymodes[mode];
}

void
GameState::ProcessInput(boost::shared_ptr<PredicateList> plist)
{
    for (PredicateList::TList::const_iterator iter = plist->begin();
         iter != plist->end();
         ++iter)
    {
        const Predicate& predicate = (*iter);

        TProcessMap::const_iterator i = mProcessMap.find(predicate.name);
        if (i != mProcessMap.end())
        {
            CALL_MEMBER_FN(*this,i->second)(predicate);
        }
    }
}

std::string
GameState::GetTeamnameLeft() const
{
    return mTeamLeft;
}

std::string
GameState::GetTeamnameRight() const
{
    return mTeamRight;
}

int
GameState::GetHalf() const
{
    return mHalf;
}

float
GameState::GetTime() const
{
    return mTime;
}

int
GameState::GetScoreLeft() const
{
    return mScoreLeft;
}

int
GameState::GetScoreRight() const
{
    return mScoreRight;
}

int
GameState::GetPlayMode() const
{
    return mPlayMode;
}

bool
GameState::GetPlayer(int index,
                     salt::Vector3f& pos, TTeamIndex& side, int& unum, float& size) const
{
    if (index < 0 || index >= mSeenPlayers.size() || !mSeenPlayers[index])
        return false;

    pos = mPlayers[index].mPos;
    side = mPlayers[index].mSide;
    unum = mPlayers[index].mUnum;
    size = DEFAULT_AGENT_RADIUS;
    return true;
}

bool
GameState::GetAck(std::string &ack)
{
    if(mAck.empty()){return false;}
    ack = mAck;
    return true;
}

bool
GameState::GetPlayerMessage(int index, std::string& message) const
{
    if (index < 0 || index >= mSeenPlayers.size() || !mSeenPlayers[index])
        return false;

    if (mPlayers[index].mMessage.empty())
        return false;

    message = mPlayers[index].mMessage;
    return true;
}

bool
GameState::GetBall(salt::Vector3f& pos) const
{
    if (!mSeenBall) return false;
    pos = mBall;
    return true;
}

bool
GameState::GetBall(salt::Vector3f& pos, float& size) const
{
    if (!mSeenBall) return false;
    pos = mBall;
    size = DEFAULT_BALL_RADIUS;
    return true;
}

bool
GameState::GetFlag(EFlagType id, salt::Vector3f& pos) const
{
    std::map<EFlagType, bool>::const_iterator i = mSeenFlags.find(id);
    if (i == mSeenFlags.end() || i->second == false) return false;

    std::map<EFlagType, salt::Vector3f>::const_iterator j = mFlags.find(id);
    if (j == mFlags.end()) return false;
    pos = j->second;
    return true;
}

std::string
GameState::GetFlagName(EFlagType id) const
{
    TFlagNameMap::const_iterator i = mFlagNameMap.find(id);
    if (i == mFlagNameMap.end()) return std::string();
    return i->second;
}

void
GameState::ResetBall()
{
    mSeenBall = false;
}

void
GameState::ResetPlayers()
{
    for (std::vector<bool>::iterator i = mSeenPlayers.begin();
         i != mSeenPlayers.end(); ++i)
    {
        *i = false;
    }
}

void
GameState::ResetFlags()
{
    for (int i=0; i<eILLEGAL; ++i)
    {
        mSeenFlags[static_cast<EFlagType>(i)] = false;
    }
}

void
GameState::SetupProcessMap()
{
    mProcessMap.clear();

    // predicate name -> member function
    mProcessMap["Init"] = &GameState::ProcessInit;
    mProcessMap["Info"] = &GameState::ProcessInfo;
    mProcessMap["Die"] = &GameState::ProcessShutdown;
}

void
GameState::SetupFlagMap()
{
    mFlagTypeMap.clear();
    mFlagNameMap.clear();
    mFlagTypeMap["F_1_l"] = eFLAG_1_L;  mFlagNameMap[eFLAG_1_L] = "F_1_l";
    mFlagTypeMap["F_2_l"] = eFLAG_2_L;  mFlagNameMap[eFLAG_2_L] = "F_2_l";
    mFlagTypeMap["F_1_r"] = eFLAG_1_R;  mFlagNameMap[eFLAG_1_R] = "F_1_r";
    mFlagTypeMap["F_2_r"] = eFLAG_2_R;  mFlagNameMap[eFLAG_2_R] = "F_2_r";
    mFlagTypeMap["G_1_l"] = eGOAL_1_L;  mFlagNameMap[eGOAL_1_L] = "G_1_l";
    mFlagTypeMap["G_2_l"] = eGOAL_2_L;  mFlagNameMap[eGOAL_2_L] = "G_2_l";
    mFlagTypeMap["G_1_r"] = eGOAL_1_R;  mFlagNameMap[eGOAL_1_R] = "G_1_r";
    mFlagTypeMap["G_2_r"] = eGOAL_2_R;  mFlagNameMap[eGOAL_2_R] = "G_2_r";
}

void
GameState::SetupPlaymodeMap()
{
    // This is the default mapping for play modes.
    // It was used for the first version of the logfile format, when the
    // simulator did not send the play modes mapping in the init message.
    // In order to be able to replay these logfiles, this mapping here should
    // not be changed.
    // In this logfiles, modes 3,11,12,16, and 17 should not occur.
    mPlaymodes.resize(18);
    mPlaymodes[0] = STR_PM_BeforeKickOff;       //  0: before kick off
    mPlaymodes[1] = STR_PM_KickOff_Left;        //  1: kick off left
    mPlaymodes[2] = STR_PM_KickOff_Right;       //  2: kick off right
    mPlaymodes[3] = "kick_off";                 //  3: pseudo kick off state
    mPlaymodes[4] = STR_PM_PlayOn;              //  4: play on
    mPlaymodes[5] = STR_PM_KickIn_Left;         //  5: kick in left
    mPlaymodes[6] = STR_PM_KickIn_Right;        //  6: kick in right
    mPlaymodes[7] = STR_PM_CORNER_KICK_LEFT;    //  7: corner kick left
    mPlaymodes[8] = STR_PM_CORNER_KICK_RIGHT;   //  8: corner kick right
    mPlaymodes[9] = STR_PM_GOAL_KICK_LEFT;      //  9: goal kick left
    mPlaymodes[10] = STR_PM_GOAL_KICK_RIGHT;    // 10: goal kick right
    mPlaymodes[11] = STR_PM_OFFSIDE_LEFT;       // 11: offside left
    mPlaymodes[12] = STR_PM_OFFSIDE_RIGHT;      // 12: offside left
    mPlaymodes[13] = STR_PM_GameOver;           // 13: game over
    mPlaymodes[14] = STR_PM_Goal_Left;          // 14: goal left
    mPlaymodes[15] = STR_PM_Goal_Right;         // 15: goal right
    mPlaymodes[16] = STR_PM_FREE_KICK_LEFT;     // 16: free kick left
    mPlaymodes[17] = STR_PM_FREE_KICK_RIGHT;    // 17: free kick right
}

void
GameState::ProcessInit(const Predicate& predicate)
{
    Predicate::Iterator iter(predicate);

    predicate.GetValue(iter, "FieldLength", mFieldLength);
    predicate.GetValue(iter, "FieldWidth", mFieldWidth);
    predicate.GetValue(iter, "FieldHeight", mFieldHeight);
    predicate.GetValue(iter, "GoalWidth", mGoalWidth);
    predicate.GetValue(iter, "GoalDepth", mGoalDepth);
    predicate.GetValue(iter, "GoalHeight", mGoalHeight);
    predicate.GetValue(iter, "BorderSize", mBorderSize);
    predicate.GetValue(iter, "LineWidth", mLineWidth);
    predicate.GetValue(iter, "FreeKickDistance", mFreeKickDistance);
    predicate.GetValue(iter, "AgentMass", mAgentMass);
    predicate.GetValue(iter, "AgentRadius", mAgentRadius);
    predicate.GetValue(iter, "AgentMaxSpeed", mAgentMaxSpeed);
    predicate.GetValue(iter, "BallRadius", mBallRadius);
    predicate.GetValue(iter, "BallMass", mBallMass);

    if (predicate.FindParameter(iter, "play_modes"))
    {
        for (int i=0; i<mPlaymodes.size(); ++i)
        {
            mPlaymodes[i] = STR_PM_Unknown;
        }

        int n = 0;
        for (Predicate::Iterator i = iter; i != iter.end(); ++i)
        {
            // read the play mode name
            std::string name;
            if (!predicate.GetValue(i,name) || name.length() == 0)
            {
                continue;
            }
            if (mPlaymodes.size() <= n)
            {
                mPlaymodes.resize(n+1);
            }
            mPlaymodes[n] = name;
            ++n;
        }
    }
}

void
GameState::ProcessInfo(const Predicate& predicate)
{
    ResetBall();
    ResetPlayers();
    ResetFlags();

    Predicate::Iterator iter(predicate);
    mAck.clear();
    predicate.GetValue(iter, "ack", mAck);
    predicate.GetValue(iter, "time", mTime);
    predicate.GetValue(iter, "half", mHalf);
    predicate.GetValue(iter, "score_left", mScoreLeft);
    predicate.GetValue(iter, "score_right", mScoreRight);
    predicate.GetValue(iter, "team_left", mTeamLeft);
    predicate.GetValue(iter, "team_right", mTeamRight);
    predicate.GetValue(iter, "play_mode", mPlayMode);

    int players_seen = 0;
    PlayerInfo pi;
    int t1;
    std::string t2;

    for (Predicate::Iterator i = iter; i != iter.end(); ++i)
    {
        Predicate::Iterator j = i;
        if (!predicate.DescentList(j))
        {
            continue;
        }

        // read the object name
        std::string name;
        if (!predicate.GetValue(j,name) || name.length() == 0)
        {
            continue;
        }

        salt::Vector3f pos;
        if (!predicate.GetValue(j, "pos", pos))
        {
            continue;
        }

        switch (name[0])
        {
        case 'B':
            mSeenBall = true;
            mBall = pos;
            break;
        case 'P':
            pi.mPos = pos;
            if (predicate.GetValue(j, "s", t1))
            {
                pi.mSide = static_cast<TTeamIndex>(t1);
            }
            if (!predicate.GetValue(j, "id", pi.mUnum))
            {
                pi.mUnum = 0;
            }
            if (!predicate.GetValue(j, "say", pi.mMessage))
            {
                pi.mMessage.clear();
            }
            if (mSeenPlayers.size() <= players_seen)
                mSeenPlayers.resize(players_seen + 1, false);
            if (mPlayers.size() <= players_seen)
                mPlayers.resize(players_seen + 1);
            mSeenPlayers[players_seen] = true;
            mPlayers[players_seen] = pi;
            ++players_seen;
            break;
        case 'F':
        case 'G':
            t2 = "";
            if (!predicate.GetValue(j, "id", t2))
            {
                break;
            }
            t2 = name + "_" + t2;

            TFlagTypeMap::const_iterator f = mFlagTypeMap.find(t2);
            if (f == mFlagTypeMap.end()) break;
            mSeenFlags[f->second] = true;
            mFlags[f->second] = pos;
            break;
        }
    }
}

void
GameState::ProcessShutdown(const Predicate& /* predicate */ )
{
    mFinished = true;
}

float
GameState::GetFieldLength() const
{
    return mFieldLength;
}

float
GameState::GetFieldWidth() const
{
    return mFieldWidth;
}

float
GameState::GetFieldHeight() const
{
    return mFieldHeight;
}

float
GameState::GetLineWidth() const
{
    return mLineWidth;
}

float
GameState::GetBorderSize() const
{
    return mBorderSize;
}

float
GameState::GetGoalWidth() const
{
    return mGoalWidth;
}

float
GameState::GetGoalHeight() const
{
    return mGoalHeight;
}

float
GameState::GetGoalDepth() const
{
    return mGoalDepth;
}

salt::Vector3f
GameState::GetGoalSize(bool invert_x) const
{
    return salt::Vector3f(invert_x ? mGoalDepth : -mGoalDepth,
                          mGoalWidth,
                          mGoalHeight);
}

float
GameState::GetAgentMass() const
{
    return mAgentMass;
}

float
GameState::GetAgentRadius() const
{
    return mAgentRadius;
}

float
GameState::GetAgentMaxSpeed() const
{
    return mAgentMaxSpeed;
}

float
GameState::GetBallRadius() const
{
    return mBallRadius;
}

float
GameState::GetBallMass() const
{
    return mBallMass;
}

