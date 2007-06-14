/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simulationserver.cpp,v 1.5.8.1 2007/06/14 23:20:59 jboedeck Exp $

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
#include <boost/bind.hpp>

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
    mCycle        = 0;
    mSumDeltaTime = 0;
    mArgC = 0;
    mArgV = 0;
    mMultiThreads = true;

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

int SimulationServer::GetCycle()
{
    return mCycle;
}

bool SimulationServer::InitControlNode(const std::string& className, const std::string& name)
{
    shared_ptr<SimControlNode> importer
        = shared_dynamic_cast<SimControlNode>(GetCore()->New(className));

    if (importer.get() == 0)
        {
            GetLog()->Error() << "(SimulationServer) ERROR: "
                              << "Unable to create '" << className << "'\n";
            return false;
        }

    importer->SetName(name);
    AddChildReference(importer);

    GetLog()->Normal()
        << "(SimulationServer) SimControlNode '"
        << name << "' registered\n";

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
            while (int(mSumDeltaTime*100) >= int(mSimStep*100))
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
                mGameControlServer->Update(mSumDeltaTime);
                mSimTime += mSumDeltaTime;
                mSumDeltaTime = 0;
            }
}

void SimulationServer::ControlEvent(EControlEvent event)
{
    int time = int(mSimTime*100);
    for (
         TLeafList::iterator iter=begin();
         iter != end();
         ++iter
         )
        {
            shared_ptr<SimControlNode> ctrNode =
                shared_dynamic_cast<SimControlNode>(*iter);

            if (ctrNode.get() == 0)
                {
                    continue;
                }

            if ( int(ctrNode->GetTime()*100) > time  ) continue;

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
                    ctrNode->SetSimTime(mSimTime);
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

    if ( mMultiThreads )
    {
        GetLog()->Normal()<< "(SimulationServer) running in multi-threads\n";
        boost::thread_group ctrThrdGroup;

        // create new threads for each SimControlNodes
        for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
        {
            shared_ptr<SimControlNode> ctrNode =  shared_dynamic_cast<SimControlNode>(*iter);
            if (ctrNode.get() == 0) continue;
            ctrThrdGroup.create_thread(boost::bind(&SimControlNode::Run, ctrNode.get()));
        }

        // run the physics and game control loop
        Loops();

        // wait for threads
        ctrThrdGroup.join_all();
    }
    else
    {
        GetLog()->Normal()<< "(SimulationServer) running in single thread\n";
        ControlEvent(CE_Init);
        shared_ptr<SimControlNode> inputCtr = GetControlNode("InputControl");
        if ( !mAutoTime && inputCtr.get() == 0 )
        {
            GetLog()->Error()<< "(SimulationServer) ERROR: can not get InputControl\n";
        }
        else
        {
            while (! mExit)
            {
                ++mCycle;

                ControlEvent(CE_StartCycle);
                ControlEvent(CE_SenseAgent);
                ControlEvent(CE_ActAgent);

                if (mAutoTime)
                {
                    AdvanceTime(mSimStep);
                }
                else
                {
                    while (int(mSumDeltaTime*100) < int(mSimStep*100))
                    {
                        inputCtr->StartCycle();// advance the time
                    }
                }
                Step();

                ControlEvent(CE_EndCycle);
            }
        }

        ControlEvent(CE_Done);
    }

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

void SimulationServer::Loops()
{  
    if (
        (mSceneServer.get() == 0) ||
        (mGameControlServer.get() == 0)
        )
        {
            return;
        }

    bool isStep = false;
    while (! mExit)
    {
          if( isStep)
            {
              mSceneServer->PhysicsUpdate(mSimStep);
            }
        
	// lock all SimControlNode threads
        vector< boost::shared_ptr<boost::mutex::scoped_lock> > locks;
        for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
        {
            shared_ptr<SimControlNode> ctrNode =  shared_dynamic_cast<SimControlNode>(*iter);
            if (ctrNode.get() == 0) continue;
            boost::shared_ptr<boost::mutex::scoped_lock> lp(new boost::mutex::scoped_lock(ctrNode->mMutex));
            locks.push_back(lp);
            ctrNode->Wait(*lp);
        }

       ++mCycle;

       if ( isStep )
       {
       	mSceneServer->PostPhysicsUpdate();
		mGameControlServer->Update(mSimStep);
		mSumDeltaTime -= mSimStep;
        mSimTime += mSimStep;
             	isStep = false;
       }

	if( int(mSumDeltaTime*100) >= int(mSimStep*100) )
	{
		mSceneServer->PrePhysicsUpdate(mSimStep);
		isStep = true;
	}

	// notify all SimControlNode threads
        for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
        {
            shared_ptr<SimControlNode> ctrNode =  shared_dynamic_cast<SimControlNode>(*iter);
            if (ctrNode.get() == 0) continue;
            ctrNode->NotifyOne();
        }
    }
}

void SimulationServer::SetMultiThreads(bool isMThreas)
{
    mMultiThreads = isMThreas;
}
