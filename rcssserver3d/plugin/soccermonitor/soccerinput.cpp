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
    scriptServer->CreateVariable("Command.One", CmdOne);
    scriptServer->CreateVariable("Command.Two", CmdTwo);
    scriptServer->CreateVariable("Command.Three", CmdThree);
    scriptServer->CreateVariable("Command.Four", CmdFour);
    scriptServer->CreateVariable("Command.Five", CmdFive);
    scriptServer->CreateVariable("Command.Six", CmdSix);
    scriptServer->CreateVariable("Command.Seven", CmdSeven);
    scriptServer->CreateVariable("Command.Eight", CmdEight);
    scriptServer->CreateVariable("Command.Nine", CmdNine);
    scriptServer->CreateVariable("Command.Zero", CmdZero);
    scriptServer->CreateVariable("Command.Left", CmdLeft);
    scriptServer->CreateVariable("Command.Right", CmdRight);
    scriptServer->CreateVariable("Command.DirectLeft", CmdDirectLeft);
    scriptServer->CreateVariable("Command.DirectRight", CmdDirectRight);
    
    scriptServer->CreateVariable("Command.PlayerSelectMode", CmdPlayerSelectMode);
    scriptServer->CreateVariable("Command.SelectNextAgent", CmdSelectNextAgent);
    scriptServer->CreateVariable("Command.ResetSelection", CmdResetSelection);
    scriptServer->CreateVariable("Command.KillSelection", CmdKillSelection);
    scriptServer->CreateVariable("Command.ReposSelection", CmdReposSelection);
    
    mMonitorClient = dynamic_pointer_cast<NetClient>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient"));

    if (mMonitorClient.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
    }

    // get camera body
    mCameraBody = dynamic_pointer_cast<RigidBody>
        (GetCore()->Get("/usr/scene/camera/physics"));

    if (mCameraBody.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SoccerInput) Unable to get camera body\n";
    }

    // get fps controller
    mFPS = dynamic_pointer_cast<FPSController>
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

        case CmdPlayerSelectMode:
            if (input.GetKeyPress())
            {
                mCmdMode = CmdModePlayerSelect;
            }
            break;
        
        case CmdSelectNextAgent:
            if (input.GetKeyPress())
                {
                    SendCommand("(select)");
                }
            break;
        
        case CmdResetSelection:
            if (input.GetKeyPress())
                {
                    SendCommand("(select (unum -1))");
                }
            break;
            
        case CmdKillSelection:
            if (input.GetKeyPress())
                {
                    SendCommand("(kill)");
                }
            break;
            
        case CmdReposSelection:
            if (input.GetKeyPress())
                {
                    SendCommand("(repos)");
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
                    SendCommand("(agent)");
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

        case CmdOne:
        case CmdTwo:
        case CmdThree:
        case CmdFour:
        case CmdFive:
        case CmdSix:
        case CmdSeven:
        case CmdEight:
        case CmdNine:
        case CmdZero:
            if (input.GetKeyPress())
                {
                    if (mCmdMode == CmdModeDefault)
                        SelectCamera(input.mId - CmdOne);
                    else
                    {
                        ostringstream u_ss;
                        u_ss << "(select (unum " << (input.mId - CmdOne + 1) << ") ";
                        if (mCmdMode == CmdModeLeftPlayerSelect)
                        {
                            u_ss << "(team Left))";
                            SendCommand(u_ss.str());
                            mCmdMode = CmdModeDefault;
                        }
                        else if (mCmdMode == CmdModeRightPlayerSelect)
                        {
                            u_ss << "(team Right))";
                            SendCommand(u_ss.str());
                            mCmdMode = CmdModeDefault;
                        }
                    }
                }
            break;
        case CmdLeft:
            if (input.GetKeyPress())
                {
                    if (mCmdMode == CmdModeDefault)
                        SendCommand("(playMode free_kick_left)");
                    else if (mCmdMode == CmdModePlayerSelect || mCmdMode == CmdModeRightPlayerSelect)
                        mCmdMode = CmdModeLeftPlayerSelect;
                }
            break;
        case CmdRight:
            if (input.GetKeyPress())
                {
                    if (mCmdMode == CmdModeDefault)
                        SendCommand("(playMode free_kick_right)");
                    else if (mCmdMode == CmdModePlayerSelect || mCmdMode == CmdModeLeftPlayerSelect)
                        mCmdMode = CmdModeRightPlayerSelect;
                }
            break;
        case CmdDirectLeft:
            if (input.GetKeyPress())
                {
                    if (mCmdMode == CmdModeDefault)
                        SendCommand("(playMode direct_free_kick_left)");
                    else if (mCmdMode == CmdModePlayerSelect || mCmdMode == CmdModeRightPlayerSelect)
                        mCmdMode = CmdModeLeftPlayerSelect;
                }
            break;
        case CmdDirectRight:
            if (input.GetKeyPress())
                {
                    if (mCmdMode == CmdModeDefault)
                        SendCommand("(playMode direct_free_kick_right)");
                    else if (mCmdMode == CmdModePlayerSelect || mCmdMode == CmdModeLeftPlayerSelect)
                        mCmdMode = CmdModeRightPlayerSelect;
                }
            break;
        };
}

void SoccerInput::SelectCamera(int idx)
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
        static_pointer_cast<SoccerMonitor>(soccerMonitorList.front());
    
    salt::Vector2f fieldSize = soccerMonitor->GetFieldSize();

    switch (idx)
    {
        case 0:
            {
                salt::Vector3f pos(-fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(90);
                mFPS->SetVAngleDeg(35);
            }
            break;
        case 1:
            {
                salt::Vector3f pos(-fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(50);
                mFPS->SetVAngleDeg(30);
            }
            break;
        case 2:
            {
                salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(salt::gRadToDeg(-0.625));
                mFPS->SetVAngleDeg(40);
            }
            break;
        case 3:
            {
                salt::Vector3f pos(0, -fieldSize.y()*1.1, fieldSize.x()*0.6);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(0);
                mFPS->SetVAngleDeg(45);
            }
            break;
        case 4:
            {
                salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(salt::gRadToDeg(0.625));
                mFPS->SetVAngleDeg(40);
            }
            break;
        case 5:
            {
                salt::Vector3f pos(fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(-50);
                mFPS->SetVAngleDeg(30);
            }
            break;
        case 6:
            {
                salt::Vector3f pos(fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                mCameraBody->SetPosition(pos);
                mFPS->SetHAngleDeg(-90);
                mFPS->SetVAngleDeg(35);
            }
            break;
    }
}
