/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: inputcontrol.cpp,v 1.4 2004/12/31 11:02:13 rollmark Exp $

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
#include <oxygen/sceneserver/fpscontroller.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

InputControl::InputControl()
{
    mDeltaTime = 0;
    mHorSens = 0.3;
    mVertSens = 0.3;
    mAdvanceTime = true;
    mMouseLook = false;
}

InputControl::~InputControl()
{
}

bool InputControl::SetFPSController(const std::string& path)
{
    if (path.empty())
        {
            mFPSController.reset();
            return true;
        }

    shared_ptr<Leaf> leaf = GetCore()->Get(path);

    if (leaf.get() == 0)
        {
            GetLog()->Error()
                << "(InputControl) ERROR: invalid path "
                << path << "'\n";
            return false;
        }

    mFPSController = shared_dynamic_cast<FPSController>
        (GetCore()->Get(path));

    if (mFPSController.get() == 0)
        {
            // the path is valid but doesn't point to an FPSController;
            // for convenience search below for a controller
            mFPSController  =
                leaf->FindChildSupportingClass<FPSController>(true);
        }

    if (mFPSController.get() == 0)
        {
            GetLog()->Error()
                << "(InputControl) ERROR: no FPSController found at '"
                << path << "'\n";
            return false;
        }

    return true;
}

void InputControl::SetFPSController(shared_ptr<FPSController> controller)
{
    mFPSController = controller;
}

void InputControl::OnLink()
{
    shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();

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

    mInputServer = shared_dynamic_cast<InputServer>
        (GetCore()->Get("/sys/server/input"));

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
    static InputServer::Input input;

    while (mInputServer->GetInput(input))
        {
            switch (input.id)
                {
                case CmdQuit:
                    GetSimulationServer()->Quit();
                    break;

                case CmdMouseLook:
                    mMouseLook = (input.data.l == 1);
                    break;

                case CmdTimer:
                    mDeltaTime = (float) input.data.l/1000.0f;
                    break;

                case CmdMouseX:
                    if (
                        (mMouseLook) &&
                        (mFPSController.get() != 0)
                        )
                        {
                            mFPSController->AdjustHAngle(mHorSens*(float)input.data.l);
                        }
                    break;

                case CmdMouseY:
                    if (
                        (mMouseLook) &&
                        (mFPSController.get() != 0)
                        )
                        {
                            mFPSController->AdjustVAngle(mVertSens*(float)input.data.l);
                        }
                    break;

                case CmdUp:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Up(input.data.l!=0);
                        }
                    break;

                case CmdDown:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Down(input.data.l!=0);
                        }
                    break;

                case CmdLeft:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->StrafeLeft(input.data.l!=0);
                        }
                    break;

                case CmdRight:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->StrafeRight(input.data.l!=0);
                        }
                    break;

                case CmdForward:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Forward(input.data.l!=0);
                        }
                    break;

                case CmdBackward:
                    if (mFPSController.get() != 0)
                        {
                            mFPSController->Backward(input.data.l!=0);
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

    if (mAdvanceTime)
        {
            // pass the delta time on to the SimulationServer
            GetSimulationServer()->AdvanceTime(mDeltaTime);
        }
}

float InputControl::GetDeltaTime()
{
    return mDeltaTime;
}

void InputControl::SetAdvanceTime(bool advance)
{
    mAdvanceTime = advance;
}

bool InputControl::GetAdvanceTime()
{
    return mAdvanceTime;
}

bool InputControl::RegisterInputItem(const string& inputItemName, const string& name)
{
    // check if a input item of the requested type was already created
    shared_ptr<InputItem> inputItem =
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
