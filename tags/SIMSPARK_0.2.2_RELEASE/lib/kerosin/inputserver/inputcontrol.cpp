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
#include "inputcontrol.h"
#include "inputitem.h"
#include <oxygen/simulationserver/simulationserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

InputControl::InputControl()
    : SimControlNode()
{
    mDeltaTime = 0;
    mHorSens = 0.3f;
    mVertSens = 0.3f;
    mAdvanceTime = false;
    mMouseLook = false;
}

InputControl::~InputControl()
{
}

bool InputControl::SetFPSController(const std::string& path)
{
    RegisterCachedPath(mFPSController, path);
    if (mFPSController.get() == 0)
        {
            GetLog()->Error()
                << "(InputControl) ERROR: no FPSController found at '"
                << path << "'\n";
            return false;
        }

    return true;
}

boost::shared_ptr<FPSController> InputControl::GetFPSController()
{
    return mFPSController.get();
}

void InputControl::OnLink()
{
    boost::shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();

    // publish common command constants to the scripts
    scriptServer->CreateVariable("Command.Quit",     CmdQuit);
    scriptServer->CreateVariable("Command.Timer",    CmdTimer);
    scriptServer->CreateVariable("Command.MouseX",   CmdMouseX);
    scriptServer->CreateVariable("Command.MouseY",   CmdMouseY);
    scriptServer->CreateVariable("Command.Left",     CmdLeft);
    scriptServer->CreateVariable("Command.Right",    CmdRight);
    scriptServer->CreateVariable("Command.Forward",  CmdForward);
    scriptServer->CreateVariable("Command.Backward", CmdBackward);
    scriptServer->CreateVariable("Command.Up",       CmdUp);
    scriptServer->CreateVariable("Command.Down",     CmdDown);
    scriptServer->CreateVariable("Command.Mouselook", CmdMouseLook);

    RegisterCachedPath(mInputServer, "/sys/server/input");

    if (mInputServer.get() == 0)
        {
            GetLog()->Error()
                << "(InputControl) ERROR: InputServer not found\n";
        }
}

void InputControl::OnUnlink()
{
    // we have to make sure, the inputServer is shut down before the
    // opengl server, as the opengl server shuts down SDL ... this
    // will conflict with the input server
    mInputServer->Unlink();
    mInputServer.reset();
    mFPSController.reset();
}

void InputControl::SetHorizontalSensitivity(float s)
{
    mHorSens = s;
}

void InputControl::SetVerticalSensitivity(float s)
{
    mVertSens = s;
}

float InputControl::GetHorizontalSensitivity()
{
    return mHorSens;
}

float InputControl::GetVerticalSensitivity()
{
    return mVertSens;
}

void InputControl::InitSimulation()
{
    if (mAdvanceTime)
        {
            // this node will step the simulation time
            GetSimulationServer()->SetAutoTimeMode(false);
        }
}

void InputControl::StartCycle()
{
    // Process incoming input
    mDeltaTime = 0.0f;
    static Input input;

    while (mInputServer->GetInput(input))
        {
            switch (input.mId)
                {
                case CmdQuit:
                    GetSimulationServer()->Quit();
                    break;

                case CmdMouseLook:
                    mMouseLook = (input.mData.l == 1);
                    break;

                case CmdTimer:
                    mDeltaTime = (float) input.mData.l/1000.0f;
                    break;

                case CmdMouseX:
                    if (
                        (mMouseLook) &&
                        (mFPSController.get() != 0)
                        )
                        {
                            mFPSController->AdjustHAngle(mHorSens*(float)input.mData.l);
                        }
                    break;

                case CmdMouseY:
                    if (
                        (mMouseLook) &&
                        (mFPSController.get() != 0)
                        )
                        {
                            mFPSController->AdjustVAngle(mVertSens*(float)input.mData.l);
                        }
                    break;

                case CmdUp:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Up(input.mData.l!=0);
                        }
                    break;

                case CmdDown:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Down(input.mData.l!=0);
                        }
                    break;

                case CmdLeft:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->StrafeLeft(input.mData.l!=0);
                        }
                    break;

                case CmdRight:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->StrafeRight(input.mData.l!=0);
                        }
                    break;

                case CmdForward:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Forward(input.mData.l!=0);
                        }
                    break;

                case CmdBackward:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Backward(input.mData.l!=0);
                        }
                    break;

                default:
                    // pass unknown events on to the registered InputItems
                    TLeafList items;
                    ListChildrenSupportingClass<InputItem>(items);

                    for (
                          TLeafList::iterator iter = items.begin();
                          iter != items.end();
                          ++iter
                          )
                        {
                            shared_static_cast<InputItem>(*iter)
                                ->ProcessInput(input);
                        }
                    break;
                }
        }

    // No longer works
//    if (mAdvanceTime)
//    {
//        // pass the delta time on to the SimulationServer
//        GetSimulationServer()->AdvanceTime(mDeltaTime);
//    }
}

float
InputControl::GetDeltaTime()
{
    return mDeltaTime;
}

void
InputControl::SetAdvanceTime(bool advance)
{
    mAdvanceTime = advance;
}

bool
InputControl::GetAdvanceTime()
{
    return mAdvanceTime;
}

bool
InputControl::RegisterInputItem(const string& inputItemName, const string& name)
{
    // check if a input item of the requested type was already created
    boost::shared_ptr<InputItem> inputItem =
        shared_dynamic_cast<InputItem>(GetChildOfClass(inputItemName));

    if (inputItem.get() != 0)
    {
        return true;
    }

    // create the input item
    inputItem = shared_dynamic_cast<InputItem>(GetCore()->New(inputItemName));

    if (inputItem.get() == 0)
    {
        GetLog()->Error() << "ERROR: (InputControl) Cannot create input item '"
                          << inputItemName << "'" << std::endl;
        return false;
    }

    // link the input item in the hierarchy
    inputItem->SetName(name);

    if (! AddChildReference(inputItem))
    {
        GetLog()->Error() << "ERROR: (InputControl) Cannot link the input item '"
                          << inputItemName << "' to the hierarchy\n";
        return false;
    }

    GetLog()->Debug() << "(InputControl) Registered input item '"
                      << inputItemName << "'\n";

    return true;
}

