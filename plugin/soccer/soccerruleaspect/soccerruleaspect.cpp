/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.cpp,v 1.1.2.1 2004/01/29 19:53:53 rollmark Exp $

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

void SoccerRuleAspect::UpdatePlayOn()
{
    if (mBallBody.get() == 0)
        {
            GetLog()->Debug() << "(SoccerRuleAspect::UpdatePlayOn) ballbody is null " << std::endl;
        }

    // check if the ball is not on the playing field
    if (mBallState->GetBallOnField())
        {
            return;
        }

    // move the ball back on the ground where it left the playing field
    Vector3f lastValid = mBallState->GetLastValidBallPosition();
    GetSoccerVar("BallRadius",lastValid[1]);
    mBallBody->SetPosition(lastValid);
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
                << "ERROR: (SoccerRuleAspect) unknown play mode " << playMode << "\n";
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

    mBallBody = GetBallBody();
}

void SoccerRuleAspect::OnUnlink()
{
    SoccerControlAspect::OnUnlink();

    mGameState.reset();
    mBallState.reset();
    mBallBody.reset();
}


