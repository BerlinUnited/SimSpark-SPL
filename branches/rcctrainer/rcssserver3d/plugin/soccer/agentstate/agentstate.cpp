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
#include "agentstate.h"
#include <soccertypes.h>
#include <soccerbase/soccerbase.h>
#include <gamestateaspect/gamestateaspect.h>
#include <kerosin/renderserver/rendernode.h>
#include <sstream>

using namespace oxygen;
using namespace kerosin;
using namespace std;

AgentState::AgentState() : ObjectState(), mTeamIndex(TI_NONE),
                           mTemperature(20.0), mBattery(100.0),
                           mHearMax(2), mHearInc(1),
                           mHearDecay(2), mHearMateCap(2),
                           mHearOppCap(2), mIfSelfMsg(false),
                           mIfMateMsg(false), mIfOppMsg(false),
                           mSelected(false)
{
    // set mID and mUniformNumber into a joint state
    SetUniformNumber(0);
}

AgentState::~AgentState()
{
}

void
AgentState::SetTeamIndex(TTeamIndex idx)
{
    mTeamIndex = idx;
}

TTeamIndex
AgentState::GetTeamIndex() const
{
    return mTeamIndex;
}


void
AgentState::SetUniformNumber(int number)
{
    mUniformNumber = number;
    std::ostringstream ss;
    ss << number;
    ObjectState::SetID(ss.str());
}

int
AgentState::GetUniformNumber() const
{
    return mUniformNumber;
}

void
AgentState::SetID(const std::string& id, TPerceptType pt)
{
    std::istringstream iss(id);
    iss >> mUniformNumber;
    if (!iss)
    {
        // conversion failed. mUniformNumber is not changed.
        return;
    }
    ObjectState::SetID(id,pt);
}

float
AgentState::GetBattery() const
{
    return mBattery;
}

void
AgentState::SetBattery(float battery)
{
    mBattery = battery;
}

float
AgentState::GetTemperature() const
{
    return 23.0;
}

void
AgentState::SetTemperature(float temperature)
{
    mTemperature = temperature;
}

bool
AgentState::ReduceBattery(float consumption)
{
    if (mBattery - consumption >= 0.0)
    {
        mBattery -= consumption;
        return true;
    }
    return false;
}

void
AgentState::AddMessage(const string& msg, float direction, bool teamMate)
{
    if (teamMate)
    {
        if (mHearMateCap < mHearDecay)
        {
            return;
        }

        mHearMateCap -= mHearDecay;

        mMateMsg = msg;
        mMateMsgDir = direction;
        mIfMateMsg = true;
    }
    else
    {
        if (mHearOppCap < mHearDecay)
        {
            return;
        }

        mHearOppCap -= mHearDecay;

        mOppMsg = msg;
        mOppMsgDir = direction;
        mIfOppMsg = true;
    }
}

void
AgentState::AddSelfMessage(const string& msg, float timeStamp)
{
    mSelfMsg = msg;
    mSelfMsgTimeStamp = timeStamp;
    mIfSelfMsg = true;
}

bool
AgentState::GetMessage(string& msg, float& direction, bool teamMate)
{
    if (teamMate)
    {
        if (mHearMateCap < mHearMax)
        {
            mHearMateCap += mHearInc;
        }

        if (! mIfMateMsg)
        {
            return false;
        }

        msg = mMateMsg;
        direction = mMateMsgDir;
        mIfMateMsg = false;
        return true;
    }
    else
    {
        if (mHearOppCap < mHearMax)
        {
            mHearOppCap += mHearInc;
        }

        if (! mIfOppMsg)
        {
            return false;
        }

        msg = mOppMsg;
        direction = mOppMsgDir;
        mIfOppMsg = false;
        return true;
    }
}

bool
AgentState::GetSelfMessage(string& msg, bool check)
{
    if (check && ! mIfSelfMsg)
    {
        return false;
    }

    msg = mSelfMsg;
    mIfSelfMsg = false;

    return true;
}

float
AgentState::GetSelfMessageTimeStamp()
{
    return mSelfMsgTimeStamp;
}

void
AgentState::UpdateHierarchyInternal()
{
    boost::shared_ptr<RenderNode> node = boost::shared_dynamic_cast<RenderNode>(GetChild("SelectionMarker", true));
    if (!node)
    {
      GetLog()->Error() << "ERROR: (AgentState::UpdateHierarchyInternal) could not find selection marker\n";
      return;
    }
    node->SetVisible(mSelected);
}

void
AgentState::OnUnlink()
{
    ObjectState::OnUnlink();

    boost::shared_ptr<GameStateAspect> game_state;
    if (!SoccerBase::GetGameState(*this,
                 game_state))
    {
      GetLog()->Error() << "ERROR: (AgentState::OnUnlink) could not get game state\n";
      return;
    }
    
    game_state->ReturnUniform(GetTeamIndex(), GetUniformNumber());
}

bool
AgentState::IsSelected() const
{
    return mSelected;
}

void
AgentState::Select(bool s)
{
    mSelected = s;
}

void
AgentState::UnSelect()
{
    mSelected = false;
}
