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
#include "internalsoccerinput.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <gamestateaspect/gamestateaspect.h>
#include <soccerruleaspect/soccerruleaspect.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/sceneserver/fpscontroller.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <agentstate/agentstate.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

InternalSoccerInput::InternalSoccerInput()
    : InputItem(), mLastKickOff(TI_NONE)
{
}

InternalSoccerInput::~InternalSoccerInput()
{
}

void InternalSoccerInput::OnLink()
{
    boost::shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    scriptServer->CreateVariable("Command.KickOff", CmdKickOff);
    scriptServer->CreateVariable("Command.KillAgentLeft", CmdKillAgentLeft);
    scriptServer->CreateVariable("Command.KillAgentRight", CmdKillAgentRight);
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

    scriptServer->CreateVariable("Command.NextCamera", CmdNextCamera);
    scriptServer->CreateVariable("Command.PreviousCamera", CmdPreviousCamera);
    
    // get the GameStateAspect
    mGameState = dynamic_pointer_cast<GameStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));

    if (mGameState.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerInput) Unable to get GameStateAspect\n";
        }

    // get the SoccerRuleAspect
    mSoccerRule = dynamic_pointer_cast<SoccerRuleAspect>
        (GetCore()->Get("/sys/server/gamecontrol/SoccerRuleAspect"));

    if (mSoccerRule.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerInput) Unable to get SoccerRuleAspect\n";
        }

    // get the GameControlServer
    mGameControl = dynamic_pointer_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));

    if (mGameControl.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerInput) Unable to get GameControlServer\n";
        }

    // get camera body
    mCameraBody = dynamic_pointer_cast<RigidBody>
        (GetCore()->Get("/usr/scene/camera/physics"));

    if (mCameraBody.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get camera body\n";
    }

    // get fps controller
    mFPS = dynamic_pointer_cast<FPSController>
        (GetCore()->Get("/usr/scene/camera/physics/controller"));

    if (mFPS.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get FPS controller\n";
    }
    
    // get render server
    mRenderServer = dynamic_pointer_cast<RenderServer>
        (GetCore()->Get("/sys/server/render"));

    if (mRenderServer.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (InternalSoccerInput) Unable to get RenderServer\n";
    }
}

void InternalSoccerInput::OnUnlink()
{
    mGameState.reset();
    mSoccerRule.reset();
    mGameControl.reset();
    mCameraBody.reset();
    mFPS.reset();
}

void InternalSoccerInput::ProcessInput(const Input& input)
{
    switch (input.mId)
        {
        default:
            return;

        case CmdKickOff:
            if (input.GetKeyPress())
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
        case CmdDropBall:
            if (input.GetKeyPress())
                {
                    mSoccerRule->DropBall();
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
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(-fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(90);
                    mFPS->SetVAngleDeg(35);
                }
            break;
        case CmdCameraLeftCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(-fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraMiddleLeft:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(-0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddleRight:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(salt::gRadToDeg(0.625));
                    mFPS->SetVAngleDeg(40);
                }
            break;
        case CmdCameraMiddle:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(0, -fieldSize.y()*1.1, fieldSize.x()*0.6);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(0);
                    mFPS->SetVAngleDeg(45);
                }
            break;
        case CmdCameraRightCorner:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(fieldSize.x()*0.8, -fieldSize.y(), fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-50);
                    mFPS->SetVAngleDeg(30);
                }
            break;
        case CmdCameraRightGoal:
            if (input.GetKeyPress())
                {
                    salt::Vector2f fieldSize = mSoccerRule->GetFieldSize();
                    salt::Vector3f pos(fieldSize.x()*0.8, 0.0, fieldSize.x()*0.4);
                    mCameraBody->SetPosition(pos);
                    mFPS->SetHAngleDeg(-90);
                    mFPS->SetVAngleDeg(35);
                }
            break;
        case CmdFreeKickLeft:
            if (input.GetKeyPress())
                {
                    mGameState->SetPlayMode(PM_FREE_KICK_LEFT);
                }
            break;
        case CmdFreeKickRight:
            if (input.GetKeyPress())
                {
                    mGameState->SetPlayMode(PM_FREE_KICK_RIGHT);

                }
            break;
        case CmdKillAgentLeft:
            if (input.GetKeyPress())
            {
                    // get list of agent aspects
                    GameControlServer::TAgentAspectList agentAspects;
                    mGameControl->GetAgentAspectList(agentAspects);

                    GameControlServer::TAgentAspectList::iterator iter;
                    for (
                          iter = agentAspects.begin();
                          iter != agentAspects.end();
                          ++iter
                        )
                    {
                        // search for the first agent of the left/right side
                        boost::shared_ptr<AgentState> agentState =
                            dynamic_pointer_cast<AgentState>((*iter)->GetChild("AgentState", true));

                        if (agentState.get() == 0)
                        {
                            GetLog()->Error() << "ERROR: (InternalSoccerInput) can't get AgentState\n";
                            continue;
                        }

                        if (agentState->GetTeamIndex() == TI_LEFT)
                        {
                            mGameControl->pushDisappearedAgent((*iter)->ID());

                            GetLog()->Error() << "(InternalSoccerInput) killed agent of team left.\n";

                            break;
                        }
                    }
            }
            break;
            case CmdKillAgentRight:
            if (input.GetKeyPress())
            {
                    // get list of agent aspects
                    GameControlServer::TAgentAspectList agentAspects;
                    mGameControl->GetAgentAspectList(agentAspects);

                    GameControlServer::TAgentAspectList::iterator iter;
                    for (
                          iter = agentAspects.begin();
                          iter != agentAspects.end();
                          ++iter
                        )
                    {
                        // search for the first agent of the left/right side
                        boost::shared_ptr<AgentState> agentState =
                            dynamic_pointer_cast<AgentState>((*iter)->GetChild("AgentState", true));

                        if (agentState.get() == 0)
                        {
                            GetLog()->Error() << "ERROR: (InternalSoccerInput) can't get AgentState\n";
                            continue;
                        }

                        if (agentState->GetTeamIndex() == TI_RIGHT)
                        {
                            mGameControl->pushDisappearedAgent((*iter)->ID());

                            GetLog()->Error() << "(InternalSoccerInput) killed agent of team left.\n";

                            break;
                        }
                    }
            }
            break;
            
            case CmdNextCamera:
            if (input.GetKeyPress())
                mRenderServer->NextCamera();
            break;
            
            case CmdPreviousCamera:
            if (input.GetKeyPress())
                mRenderServer->PreviousCamera();
            break;
            
      };
}
