/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.cpp,v 1.1.2.3 2004/02/06 10:14:50 rollmark Exp $

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
#include "soccerruleaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/sceneserver/scene.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/ballstateaspect/ballstateaspect.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

SoccerRuleAspect::SoccerRuleAspect() : SoccerControlAspect()
{
}

SoccerRuleAspect::~SoccerRuleAspect()
{
}

void SoccerRuleAspect::UpdateBeforeKickOff()
{
    // before the game starts the ball should stay in the middle of
    // the playing field
    salt::Vector3f pos(0,0,0);
    GetSoccerVar("BallRadius",pos[1]);
    mBallBody->SetPosition(pos);
}

bool SoccerRuleAspect::CheckBallLeftField()
{
    if (mBallState->GetBallOnField())
        {
            return false;
        }

    // move the ball back on the ground where it left the playing field
    Vector3f lastValid = mBallState->GetLastValidBallPosition();
    GetSoccerVar("BallRadius",lastValid[1]);
    mBallBody->SetPosition(lastValid);

    return true;
}

bool SoccerRuleAspect::CheckGoal()
{
    TTeamIndex idx = mBallState->GetGoalState();
    if (idx == TI_NONE)
        {
            return false;
        }

    // score the lucky team
    if (idx == TI_LEFT)
        {
            // ball is in the left goal, score the right team
            mGameState->ScoreTeam(TI_RIGHT);
        } else
            {
                // ball is in the right goal, score the left team
                mGameState->ScoreTeam(TI_LEFT);
            }

    // put the ball back in the middle of the playing field
    salt::Vector3f pos(0,0,0);
    GetSoccerVar("BallRadius",pos[1]);
    mBallBody->SetPosition(pos);

    return true;
}

void SoccerRuleAspect::UpdatePlayOn()
{
    // check if the ball is in one of the goals
    if (CheckGoal())
        {
            return;
        }

    // check if the ball is otherwise not on the playing field
    if (CheckBallLeftField())
        {
            return;
        }

    // other checks go here...
}

void SoccerRuleAspect::Update(float deltaTime)
{
    if (
        (mGameState.get() == 0) ||
        (mBallState.get() == 0) ||
        (mBallBody.get() == 0)
        )
        {
            return;
        }

    TPlayMode playMode = mGameState->GetPlayMode();

    switch (playMode)
        {
        case PM_BeforeKickOff:
            UpdateBeforeKickOff();
            break;

        case PM_PlayOn:
            UpdatePlayOn();
            break;

        default:
            GetLog()->Error()
                << "ERROR: (SoccerRuleAspect) unknown play mode "
                << playMode << "\n";
            break;
        }
}

void SoccerRuleAspect::OnLink()
{
    SoccerControlAspect::OnLink();

    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetControlAspect("GameStateAspect"));

    mBallState = shared_dynamic_cast<BallStateAspect>
        (GetControlAspect("BallStateAspect"));

    SoccerBase::GetBallBody(*this,mBallBody);
}

void SoccerRuleAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mGameState.reset();
    mBallState.reset();
    mBallBody.reset();
}


