/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simulationserver.cpp,v 1.1 2004/04/25 16:39:59 rollmark Exp $

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
#include "simulationserver.h"
#include "simcontrolnode.h"
#include <oxygen/monitorserver/monitorserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <signal.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

bool SimulationServer::mExit = false;

void SimulationServer::CatchSignal(int sig_num)
{
    if (sig_num == SIGINT)
        {
            signal(SIGINT, CatchSignal);
            SimulationServer::mExit = true;
            std::cout << "(SimulationServer) caught SIGINT. exiting.\n";
        }
}

SimulationServer::SimulationServer() : Node()
{
    mSimTime      = 0.0f;
    mSimStep      = 0.2f;
    mAutoTime     = true;
    mSumDeltaTime = 0;
    mArgC = 0;
    mArgV = 0;

    signal(SIGINT, CatchSignal);
}

SimulationServer::~SimulationServer()
{
}

void SimulationServer::OnLink()
{
    mMonitorServer = shared_dynamic_cast<MonitorServer>
        (GetCore()->Get("/sys/server/monitor"));

    if (mMonitorServer.get() == 0)
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: MonitorServer not found.\n";
    }

    mGameControlServer = shared_dynamic_cast<GameControlServer>
        (GetCore()->Get("/sys/server/gamecontrol"));

    if (mGameControlServer.get() == 0)
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: GameControlServer not found.\n";
    }

    mSceneServer = shared_dynamic_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: SceneServer not found.\n";
    }
}

void SimulationServer::OnUnlink()
{
    mMonitorServer.reset();
    mGameControlServer.reset();
    mSceneServer.reset();
}

void SimulationServer::Quit()
{
    mExit = true;
}

int SimulationServer::GetArgC()
{
    return mArgC;
}

char** SimulationServer::GetArgV()
{
    return mArgV;
}

float SimulationServer::GetTime()
{
    return mSimTime;
}

float SimulationServer::GetSimStep()
{
    return mSimStep;
}

void SimulationServer::SetSimStep(float deltaTime)
{
    mSimStep = deltaTime;
}

float SimulationServer::GetSumDeltaTime()
{
    return mSumDeltaTime;
}

void SimulationServer::SetAutoTimeMode(bool set)
{
    mAutoTime = set;
}

bool SimulationServer::GetAutoTimeMode()
{
    return mAutoTime;
}

bool SimulationServer::InitControlNode(const string& controlName)
{
    shared_ptr<SimControlNode> importer
        = shared_dynamic_cast<SimControlNode>(GetCore()->New(controlName));

    if (importer.get() == 0)
        {
            GetLog()->Error() << "(SimulationServer) ERROR: "
                              << "Unable to create '" << controlName << "'\n";
            return false;
        }

    importer->SetName(controlName);
    AddChildReference(importer);

    GetLog()->Normal()
        << "(SimulationServer) SimControlNode '"
        << controlName << "' registered\n";

    return true;
}

shared_ptr<SimControlNode>
SimulationServer::GetControlNode(const string& controlName)
{
    shared_ptr<SimControlNode> ctrNode =
        shared_dynamic_cast<SimControlNode>(GetChild(controlName));

    if (ctrNode.get() == 0)
        {
            GetLog()->Normal()
                << "(SimulationServer) SimControlNode '"
                << controlName << "' not found\n";
        }

    return ctrNode;
}

void SimulationServer::AdvanceTime(float deltaTime)
{
    mSumDeltaTime += deltaTime;
}

void SimulationServer::Step()
{
    if (
        (mSceneServer.get() == 0) ||
        (mGameControlServer.get() == 0)
        )
        {
            return;
        }

    if (mSimStep > 0)
        {
            // world is stepped in discrete steps
            while (mSumDeltaTime >= mSimStep)
                {
                    mSceneServer->Update(mSimStep);
                    mGameControlServer->Update(mSimStep);
                    mSumDeltaTime -= mSimStep;
                    mSimTime += mSimStep;
                }
        } else
            {
                // simulate passed time in one single step
                mSceneServer->Update(mSumDeltaTime);
                mGameControlServer->Update(mSimStep);
                mSimTime += mSumDeltaTime;
                mSumDeltaTime = 0;
            }
}

void SimulationServer::ControlEvent(EControlEvent event)
{
    for (
         TLeafList::iterator iter=begin();
         iter != end();
         ++iter
         )
        {
            shared_ptr<SimControlNode> ctrNode =
                shared_static_cast<SimControlNode>(*iter);

            switch (event)
                {
                case CE_Init :
                    ctrNode->InitSimulation();
                    break;

                case CE_Done :
                    ctrNode->DoneSimulation();
                    break;

                case CE_StartCycle :
                    ctrNode->StartCycle();
                    break;

                case CE_SenseAgent :
                    ctrNode->SenseAgent();
                    break;

                case CE_ActAgent :
                    ctrNode->ActAgent();
                    break;

                case CE_EndCycle :
                    ctrNode->EndCycle();
                    break;

                default:
                    GetLog()->Error()
                        << "(SimulationServer) ERROR: unknown control event "
                        << event << "\n";
                    return;
                }
        }
}

void SimulationServer::Run(int argc, char** argv)
{
    GetLog()->Normal() << "(SimulationServer) entering runloop\n";

    // cache argc and argv, to make it accessible for registerd
    // SimControlNodes
    mArgC = argc;
    mArgV = argv;

    ControlEvent(CE_Init);

    while (! mExit)
        {
            ControlEvent(CE_StartCycle);
            ControlEvent(CE_SenseAgent);
            ControlEvent(CE_ActAgent);

            if (mAutoTime)
                {
                    AdvanceTime(mSimStep);
                }
            Step();

            ControlEvent(CE_EndCycle);
        }

    ControlEvent(CE_Done);

    mArgC = 0;
    mArgV = 0;

    GetLog()->Normal()
        << "(SimulationServer) leaving runloop at t="
        << mSimTime << "\n";
}

shared_ptr<GameControlServer> SimulationServer::GetGameControlServer()
{
    return mGameControlServer;
}

shared_ptr<MonitorServer> SimulationServer::GetMonitorServer()
{
    return mMonitorServer;
}

shared_ptr<SceneServer> SimulationServer::GetSceneServer()
{
    return mSceneServer;
}


