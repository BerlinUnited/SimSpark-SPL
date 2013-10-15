/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Author: Patrick Geib
   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
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

#include "soccercontrolframe_soccercommands.h"

#include <sparkcommandqueue.h>
#include <simspark.h>
#include <cutelogger\logger.h>

#include <gamestateaspect\gamestateaspect.h>
#include <ballstateaspect\ballstateaspect.h>
#include <soccerruleaspect\soccerruleaspect.h>

using namespace boost;
using namespace zeitgeist;
using namespace SparkCommand;

namespace SoccerControlFrameUtil
{
//--------------------------------------------------------------
// SoccerCommand base class
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SoccerCommand::~SoccerCommand()
{
};

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

SoccerCommand::SoccerCommand(int type) :
    Command(type)
{
}

//--------------------------------------------------------------
// Derived classes
//--------------------------------------------------------------
// ChangePlayMode
//--------------------------------------------------------------

bool ChangePlayMode::execute()
{
    if (mGameState.get() == 0)
        return false;

    mGameState->SetPlayMode(mMode);
    return true;
}

//--------------------------------------------------------------
// SetTime
//--------------------------------------------------------------

bool SetTime::execute()
{
    if (mGameState.get() == 0)
        return false;

    mGameState->SetTime(mTime);
    return true;
}

//--------------------------------------------------------------
// KickOff
//--------------------------------------------------------------

bool KickOff::execute()
{
    if (mGameState.get() == 0)
        return false;

    mGameState->KickOff();
    return true;
}

//--------------------------------------------------------------
// SetTeamGoals
//--------------------------------------------------------------

bool SetTeamGoals::execute()
{
    if (mGameState.get() == 0)
        return false;

    if (mTeam == TI_LEFT)
        mGameState->SetScores(mGoals, mGameState->GetScore(TI_RIGHT));
    else if (mTeam == TI_RIGHT)
        mGameState->SetScores(mGameState->GetScore(TI_LEFT), mGoals);
    else 
    {
        LOG_ERROR() << "Illegal team index " << mTeam;
        return false;
    }

    return true;
}

//--------------------------------------------------------------
// SetTeamName
//--------------------------------------------------------------

bool SetTeamName::execute()
{
    if (mGameState.get() == 0)
        return false;

    if (mTeam == TI_LEFT || mTeam == TI_RIGHT)
        mGameState->SetTeamName(mTeam, mName.toStdString());
    else 
    {
        LOG_ERROR() << "Illegal team index " << mTeam;
        return false;
    }
    return true;
}

//--------------------------------------------------------------
// SetAgentBattery
//--------------------------------------------------------------

bool SetAgentBattery::execute()
{
    return true;
}

//--------------------------------------------------------------
// SetAgentTemperature
//--------------------------------------------------------------

bool SetAgentTemperature::execute()
{
    return true;
}

} //namespace SparkCommands
