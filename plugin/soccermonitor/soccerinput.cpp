/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerinput.cpp,v 1.9 2008/02/22 16:48:20 hedayat Exp $

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
#include "soccerinput.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
//#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/sceneserver/fpscontroller.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

SoccerInput::SoccerInput()
{
}

SoccerInput::~SoccerInput()
{
}

void SoccerInput::OnLink()
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

    mMonitorClient = shared_dynamic_cast<NetClient>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient"));

    if (mMonitorClient.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
    }

    // get camera body
    mCameraBody = shared_dynamic_cast<Body>
        (GetCore()->Get("/usr/scene/camera/physics"));

    if (mCameraBody.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get camera body\n";
    }

    // get fps controller
    mFPS = shared_dynamic_cast<FPSController>
        (GetCore()->Get("/usr/scene/camera/physics/controller"));

    if (mFPS.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get FPS controller\n";
    }
}

void SoccerInput::OnUnlink()
{
    mMonitorClient.reset();
    mCameraBody.reset();
    mFPS.reset();
}

void SoccerInput::SendCommand(const std::string& cmd)
{
    if (mMonitorClient.get() == 0)
        {
            return;
        }

    mMonitorClient->SendMessage(cmd);
}

void SoccerInput::ProcessInput(const Input& input)
{
    switch (input.mId)
        {
        default:
            return;

        case CmdKickOff:
            if (input.GetKeyPress())
                {
                    SendCommand("(kickOff Left)");
                }
            break;
        case CmdMoveAgent:
            if (input.GetKeyPress())
                {
                    //SendCommand("(agent (team Left)(unum 1)(pos -2.0 1.0 3.5))");
                }
            break;
        case CmdDropBall:
            if (input.GetKeyPress())
                {
                    SendCommand("(dropBall)");
                }
            break;
        case CmdShootBall:
            if (input.GetKeyPress())
                {
                    //SendCommand("(ball (vel -4.0 0.0 2.0))");
                }
            break;
        case CmdMoveBall:
            if (input.GetKeyPress())
                {
                    //SendCommand("(ball (pos -42.0 0.0 0.3))");
                }
            break;
        case CmdCameraLeftGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(-40.0, 0.0, 21.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(1.55));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(1.05));
                }
            break;
        case CmdCameraLeftCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(-40.0, -30.5, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.855));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(0.88));
                }
            break;
        case CmdCameraMiddleLeft:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(6.0, -29.0, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-0.625));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(0.965));
                }
            break;
        case CmdCameraMiddleRight:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(-6.0, -29.0, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.625));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(0.965));
                }
            break;
        case CmdCameraMiddle:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(0.0, -43.5, 39.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.002));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(1.16));
                }
            break;
        case CmdCameraRightCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(40.0, -30.5, 20.0);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-0.855));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(0.88));
                }
            break;
        case CmdCameraRightGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(40.0, 0.0, 21.5);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-1.55));
                    mFPS->SetVAngleDeg(salt::gRadToDeg(1.05));
                }
            break;
        case CmdFreeKickLeft:
            if (input.GetKeyPress())
                {
                    SendCommand("(playMode free_kick_left)");
                }
            break;
        case CmdFreeKickRight:
            if (input.GetKeyPress())
                {
                    SendCommand("(playMode free_kick_right)");
                }
            break;
        };
}
