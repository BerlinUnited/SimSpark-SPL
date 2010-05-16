/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "soccerinput.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
#include "soccermonitor.h"
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/fpscontroller.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;
using namespace std;

SoccerInput::SoccerInput()
    : InputItem(),
      mCmdMode(CmdModeDefault)
{
}

SoccerInput::~SoccerInput()
{
}

void SoccerInput::OnLink()
{
    boost::shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    scriptServer->CreateVariable("Command.KickOff", CmdKickOff);
    scriptServer->CreateVariable("Command.KickOffRight", CmdKickOffRight);
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
    
    scriptServer->CreateVariable("Command.NextMode", CmdNextMode);

    
    mMonitorClient = shared_dynamic_cast<NetClient>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient"));

    if (mMonitorClient.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
    }

    // get camera body
    mCameraBody = shared_dynamic_cast<RigidBody>
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
    // get list of registered SoccerMonitor objects
    TLeafList soccerMonitorList;
    mMonitorClient->ListChildrenSupportingClass<SoccerMonitor>(soccerMonitorList);

    if (soccerMonitorList.empty())
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get SoccerMonitor\n";
        return;
    }

    boost::shared_ptr<SoccerMonitor> soccerMonitor =
        shared_static_cast<SoccerMonitor>(soccerMonitorList.front());
    
    switch (input.mId)
        {
        default:
            return;

        case CmdNextMode:
            if (input.GetKeyPress())
            {
                mCmdMode = (ECmdMode)(mCmdMode + 1);
                if (mCmdMode == CmdModeNone)
                  mCmdMode = CmdModeDefault;
            }
            break;
            
        case CmdKickOff:
            if (input.GetKeyPress())
                {
                    SendCommand("(kickOff Left)");
                }
            break;
        case CmdKickOffRight:
            if (input.GetKeyPress())
                {
                    SendCommand("(kickOff Right)");
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
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(-fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(90);
                    mFPS->SetVAngleDeg(35);
                }
            break;
        case CmdCameraLeftCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(-fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraMiddleLeft:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddleRight:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddle:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y()*1.1, fieldSize.x()*0.6);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(0);
                    mFPS->SetVAngleDeg(45);
                }
            break;
        case CmdCameraRightCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraRightGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();
                    salt::Vector3f pos(fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-90);
                    mFPS->SetVAngleDeg(35);
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
