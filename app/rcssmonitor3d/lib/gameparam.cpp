/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: gameparam.cpp,v 1.3 2004/04/09 19:20:28 fruit Exp $

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
#include "gameparam.h"
#include "types.h"

GameParam::GameParam()
{
    mFieldLength  = DEFAULT_FIELD_LENGTH;
    mFieldWidth   = DEFAULT_FIELD_WIDTH;
    mFieldHeight  = DEFAULT_FIELD_HEIGHT;

    mBorderSize   = DEFAULT_BORDER_SIZE;
    mLineWidth    = DEFAULT_LINE_WIDTH;

    mGoalWidth    = DEFAULT_GOAL_WIDTH;
    mGoalDepth    = DEFAULT_GOAL_DEPTH;
    mGoalHeight   = DEFAULT_GOAL_HEIGHT;

    mAgentMass      = DEFAULT_AGENT_MASS;
    mAgentRadius    = DEFAULT_AGENT_RADIUS;
    mAgentMaxSpeed  = DEFAULT_AGENT_MAXSPEED;

    mBallRadius = DEFAULT_BALL_RADIUS;
    mBallMass   = DEFAULT_BALL_MASS;
}

GameParam::~GameParam()
{
}

const float&
GameParam::GetFieldLength() const
{
    return mFieldLength;
}

void
GameParam::SetFieldLength(float f)
{
    mFieldLength = f;
}

const float&
GameParam::GetFieldWidth() const
{
    return mFieldWidth;
}

void
GameParam::SetFieldWidth(float f)
{
    mFieldWidth = f;
}

const float&
GameParam::GetFieldHeight() const
{
    return mFieldHeight;
}

void GameParam::SetFieldHeight(float f)
{
    mFieldHeight = f;
}

const float&
GameParam::GetBorderSize() const
{
    return mBorderSize;
}

void
GameParam::SetBorderSize(float f)
{
    mBorderSize = f;
}

const float&
GameParam::GetLineWidth() const
{
    return mLineWidth;
}

void
GameParam::SetLineWidth(float f)
{
    mLineWidth = f;
}

const float&
GameParam::GetGoalWidth() const
{
    return mGoalWidth;
}

void
GameParam::SetGoalWidth(float f)
{
    mGoalWidth = f;
}

const float&
GameParam::GetGoalDepth() const
{
    return mGoalDepth;
}

void
GameParam::SetGoalDepth(float f)
{
    mGoalDepth = f;
}

const float&
GameParam::GetGoalHeight() const
{
    return mGoalHeight;
}

void
GameParam::SetGoalHeight(float f)
{
    mGoalHeight = f;
}

const float&
GameParam::GetAgentMass() const
{
    return mAgentMass;
}

void
GameParam::SetAgentMass(float f)
{
    mAgentMass = f;
}

const float&
GameParam::GetAgentRadius() const
{
    return mAgentRadius;
}

void
GameParam::SetAgentRadius(float f)
{
    mAgentRadius = f;
}

const float&
GameParam::GetAgentMaxSpeed() const
{
    return mAgentMaxSpeed;
}

void
GameParam::SetAgentMaxSpeed(float f)
{
    mAgentMaxSpeed = f;
}

const float&
GameParam::GetBallRadius() const
{
    return mBallRadius;
}

void
GameParam::SetBallRadius(float f)
{
    mBallRadius = f;
}

const float&
GameParam::GetBallMass() const
{
    return mBallMass;
}

void
GameParam::SetBallMass(float f)
{
    mBallMass = f;
}
