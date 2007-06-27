/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: internalsoccerinput.cpp,v 1.2 2007/06/27 22:29:52 jamu Exp $

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
#include "internalsoccerinput.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/soccerruleaspect/soccerruleaspect.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/sceneserver/fpscontroller.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

InternalSoccerInput::InternalSoccerInput() : mLastKickOff(TI_NONE)
{
}

InternalSoccerInput::~InternalSoccerInput()
{
}

void InternalSoccerInput::OnLink()
{
    shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    scriptServer->CreateVariable("Command.KickOff", CmdKickOff);
    scriptServer->CreateVariable("Command.MoveAgent", CmdMoveAgent);
    scriptServer->CreateVariable("Command.DropBall", CmdDropBall);
    scriptServer->CreateVariable("Command.ShootBall", CmdShootBall);
    scriptServer->CreateVariable("Command.MoveBall", CmdMoveBall);
    scriptServer->CreateVariable("Command.CameraLeftGoal", CmdCameraLeftGoal);
    scriptServer->CreateVariable("Command.CameraLeftCorner", CmdCameraLeftCorner);
    scriptServer->CreateVariable("Command.CameraMiddleLeft", CmdCameraMiddleLeft);
    scriptServer->CreateVariable("Command.CameraMiddleRight", CmdCameraMiddleRight);
    scriptServer->CreateVariable("Command.CameraMiddle", CmdCameraMiddle);
    scriptServer->CreateVariable("Command.CameraRightCorner", CmdCameraRightCorner);
    scriptServer->CreateVariable("Command.CameraRightGoal", CmdCameraRightGoal);
    //JAN
    scriptServer->CreateVariable("Command.FreeKickLeft", CmdFreeKickLeft);
    scriptServer->CreateVariable("Command.FreeKickRight", CmdFreeKickRight);

    // get the GameStateAspect
    mGameState = shared_dynamic_cast<GameStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));

    if (mGameState.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerInput) Unable to get GameStateAspect\n";
        }

    // get the SoccerRuleAspect
    mSoccerRule = shared_dynamic_cast<SoccerRuleAspect>
        (GetCore()->Get("/sys/server/gamecontrol/SoccerRuleAspect"));

    if (mSoccerRule.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerInput) Unable to get SoccerRuleAspect\n";
        }

    // get camera body
    mCameraBody = shared_dynamic_cast<Body>
        (GetCore()->Get("/usr/scene/camera/physics"));

    if (mCameraBody.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get camera body\n";
    }

    // get fps controller
    mFPS = shared_dynamic_cast<FPSController>
        (GetCore()->Get("/usr/scene/camera/physics/controller"));

    if (mFPS.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get FPS controller\n";
    }
}

void InternalSoccerInput::OnUnlink()
{
    mGameState.reset();
    mSoccerRule.reset();
    mCameraBody.reset();
    mFPS.reset();
}

void InternalSoccerInput::ProcessInput(const InputServer::Input& input)
{
    switch (input.id)
        {
        default:
            return;

        case CmdKickOff:
            if (input.KeyPress())
                {
                    if (mLastKickOff == TI_NONE)
                    {
                        mGameState->KickOff(TI_LEFT);
                        mLastKickOff = TI_LEFT;
                    }
                    else
                    {
                        mGameState->KickOff(TI_RIGHT);
                        mLastKickOff = TI_RIGHT;
                    }
                }
            break;
        case CmdMoveAgent:
            if (input.KeyPress())
                {
                    //SendCommand("(agent (team Left)(unum 1)(pos -2.0 1.0 3.5))");
                }
            break;    
        case CmdDropBall:
            if (input.KeyPress())
                {
                    mSoccerRule->DropBall();
                }
            break;
        case CmdShootBall:
            if (input.KeyPress())
                {
                    //SendCommand("(ball (vel -4.0 0.0 2.0))");
                }
            break;
        case CmdMoveBall:
            if (input.KeyPress())
                {
                    //SendCommand("(ball (pos -42.0 0.0 0.3))");
                }
            break;
        case CmdCameraLeftGoal:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(-40.0, 0.0, 21.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(1.55));
                    mFPS->SetVAngle(salt::gRadToDeg(1.05));
                }
            break;
        case CmdCameraLeftCorner:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(-40.0, -30.5, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(0.855));
                    mFPS->SetVAngle(salt::gRadToDeg(0.88));
                }
            break;
        case CmdCameraMiddleLeft:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(6.0, -29.0, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(-0.625));
                    mFPS->SetVAngle(salt::gRadToDeg(0.965));
                }
            break;
        case CmdCameraMiddleRight:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(-6.0, -29.0, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(0.625));
                    mFPS->SetVAngle(salt::gRadToDeg(0.965));
                }
            break;
        case CmdCameraMiddle:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(0.0, -43.5, 39.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(0.002));
                    mFPS->SetVAngle(salt::gRadToDeg(1.16));
                }
            break;
        case CmdCameraRightCorner:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(40.0, -30.5, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(-0.855));
                    mFPS->SetVAngle(salt::gRadToDeg(0.88));
                }
            break;
        case CmdCameraRightGoal:
            if (input.KeyPress())
                {
                    salt::Vector3f pos(40.0, 0.0, 21.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngle(salt::gRadToDeg(-1.55));
                    mFPS->SetVAngle(salt::gRadToDeg(1.05));
                }
            break;
        case CmdFreeKickLeft:
            if (input.KeyPress())
                {
                    mGameState->SetPlayMode(PM_FREE_KICK_LEFT);
                }
            break;    
        case CmdFreeKickRight:
            if (input.KeyPress())
                {
                    mGameState->SetPlayMode(PM_FREE_KICK_RIGHT);

                }
            break;    
        };
}
