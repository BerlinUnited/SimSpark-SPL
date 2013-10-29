/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerinputlogplayer.cpp 176 2010-02-25 12:19:37Z a-held $

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
#include "soccerinputlogplayer.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/physicsserver/rigidbody.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

SoccerInputLogPlayer::SoccerInputLogPlayer()
{
}

SoccerInputLogPlayer::~SoccerInputLogPlayer()
{
}

void SoccerInputLogPlayer::OnLink()
{
    mScriptServer = GetCore()->GetScriptServer();
    mScriptServer->CreateVariable("Command.Pause", CmdPause);
    mScriptServer->CreateVariable("Command.StepForward", CmdStepForward);
    mScriptServer->CreateVariable("Command.StepBackward", CmdStepBackward);
    mScriptServer->CreateVariable("Command.BPlayback", CmdBPlayback);

    mScriptServer->CreateVariable("Command.CameraLeftGoal", CmdCameraLeftGoal);
    mScriptServer->CreateVariable("Command.CameraLeftCorner", CmdCameraLeftCorner);
    mScriptServer->CreateVariable("Command.CameraMiddleLeft", CmdCameraMiddleLeft);
    mScriptServer->CreateVariable("Command.CameraMiddleRight", CmdCameraMiddleRight);
    mScriptServer->CreateVariable("Command.CameraMiddle", CmdCameraMiddle);
    mScriptServer->CreateVariable("Command.CameraRightCorner", CmdCameraRightCorner);
    mScriptServer->CreateVariable("Command.CameraRightGoal", CmdCameraRightGoal);

    mMonitorClient = dynamic_pointer_cast<SimControlNode>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorLogFileServer"));

    if (mMonitorClient.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
        }

    // get fps controller
    mFPS = dynamic_pointer_cast<FPSController>
        (GetCore()->Get("/usr/scene/camera/physics/controller"));

    if (mFPS.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get FPS controller\n";
    }

    // get camera body
    mCameraBody = dynamic_pointer_cast<RigidBody>
        (GetCore()->Get("/usr/scene/camera/physics"));

    if (mCameraBody.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get camera body\n";
    }

}

void SoccerInputLogPlayer::OnUnlink()
{
}

void SoccerInputLogPlayer::SendCommand(const std::string& cmd)
{
/*    if (mMonitorClient.get() == 0)
        {
            return;
        }
*/}

void SoccerInputLogPlayer::ProcessInput(const Input& input)
{

    switch (input.mId)
        {
        default:
            return;

        case CmdPause:
            if (input.GetKeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.pauseMode()");
                }
            break;

        case CmdStepForward:
            if (input.GetKeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.stepForward()");
                }
            break;

        case CmdStepBackward:
            if (input.GetKeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.stepBackward()");
                }
            break;

        case CmdBPlayback:
            if (input.GetKeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.playBackward()");
                }
            break;
        case CmdCameraLeftGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(-12*0.8, 0.0, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(90);
                    mFPS->SetVAngleDeg(35);
                }
            break;
        case CmdCameraLeftCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(-12*0.8, -8, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraMiddleLeft:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(0, -8, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddleRight:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(0, -8, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddle:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(0, -8*1.1, 12*0.6);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(0);
                    mFPS->SetVAngleDeg(45);
                }
            break;
        case CmdCameraRightCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(12*0.8, -8, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraRightGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector3f pos(12*0.8, 0.0, 12*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-90);
                    mFPS->SetVAngleDeg(35);
                }
            break;
        };
}
