/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simulationserver.cpp,v 1.9 2008/02/22 16:48:18 hedayat Exp $

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
#include <zeitgeist/logserver/logserver.h>
#include <signal.h>
#include <algorithm>
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

SimulationServer::SimulationServer() :
	Node(), mAdjustSpeed(false), mMaxStepsPerCycle(3), mThreadBarrier(0)
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
    RegisterCachedPath(mMonitorServer, "/sys/server/monitor");

    if (mMonitorServer.expired())
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: MonitorServer not found.\n";
    }

    RegisterCachedPath(mGameControlServer, "/sys/server/gamecontrol");

    if (mGameControlServer.expired())
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: GameControlServer not found.\n";
    }

    RegisterCachedPath(mSceneServer, "/sys/server/scene");
    if (mSceneServer.expired())
    {
        GetLog()->Error()
            << "(SimulationServer) ERROR: SceneServer not found.\n";
    }
}


void SimulationServer::Quit()
{
    mExit = true;
}

bool SimulationServer::WantsToQuit()
{
    return mExit;
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

void SimulationServer::ResetTime()
{
    mSimTime = 0.0f;
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
    shared_ptr<SimControlNode> control
        = shared_dynamic_cast<SimControlNode>(GetCore()->New(className));

    if (control.get() == 0)
        {
            GetLog()->Error() << "(SimulationServer) ERROR: "
                              << "Unable to create '" << className << "'\n";
            return false;
        }

    control->SetName(name);
    AddChildReference(control);

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
        mSceneServer.expired() ||
        mGameControlServer.expired()
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
                    mSimTime += mSimStep;
                    if (mAdjustSpeed && mSumDeltaTime > mMaxStepsPerCycle * mSimStep)
                    {
                        GetLog()->Error() << "Skipping remaining time: "
                                << mSumDeltaTime << '\n';
                        mSumDeltaTime = 0;
                    }
                    else
                        mSumDeltaTime -= mSimStep;
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

void SimulationServer::Init(int argc, char** argv)
{
    GetLog()->Normal() << "(SimulationServer) init\n";
    mExit = false;

    // cache argc and argv, to make it accessible for registerd
    // SimControlNodes
    mArgC = argc;
    mArgV = argv;

    ControlEvent(CE_Init);
}

void SimulationServer::Run(int argc, char** argv)
{
    Init(argc, argv);
    GetLog()->Normal() << "(SimulationServer) entering runloop\n";

    if ( mMultiThreads )
    {
        GetLog()->Normal()<< "(SimulationServer) running in multi-threads\n";
        RunMultiThreaded();
    }
    else
    {
        GetLog()->Normal()<< "(SimulationServer) running in single thread\n";
        shared_ptr<SimControlNode> inputCtr = GetControlNode("InputControl");
        if ( !mAutoTime && inputCtr.get() == 0 )
        {
            GetLog()->Error()<< "(SimulationServer) ERROR: can not get InputControl\n";
        }
        else
        {
            while (! mExit)
            {
                Cycle(inputCtr);
            }
        }
    }

    Done();
}

void SimulationServer::Cycle(shared_ptr<SimControlNode> &inputCtr)
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

void SimulationServer::Done()
{
    ControlEvent(CE_Done);

    mArgC = 0;
    mArgV = 0;

    GetLog()->Normal()
        << "(SimulationServer) leaving runloop at t="
        << mSimTime << "\n";
}

shared_ptr<GameControlServer> SimulationServer::GetGameControlServer()
{
    return mGameControlServer.get();
}

shared_ptr<MonitorServer> SimulationServer::GetMonitorServer()
{
    return mMonitorServer.get();
}

shared_ptr<SceneServer> SimulationServer::GetSceneServer()
{
    return mSceneServer.get();
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

void SimulationServer::RunMultiThreaded()
{
    boost::thread_group ctrThrdGroup;

    // count valid SimControlNodes.
    int count = 1;
    for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
    {
        if (shared_dynamic_cast<SimControlNode>(*iter))
            count++;
    }
    mThreadBarrier = new barrier(count);

    // create new threads for each SimControlNodes
    for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
    {
        shared_ptr<SimControlNode> ctrNode =  shared_dynamic_cast<SimControlNode>(*iter);
        if (ctrNode.get() == 0) continue;
//        ctrThrdGroup.create_thread(boost::bind(&SimControlNode::Run, ctrNode.get()));
        ctrThrdGroup.create_thread(boost::bind(&SimulationServer::SimControlThread,
                this, ctrNode));
    }
    // Loops();

    while (true)
    {
        ++mCycle;
        mSceneServer->PrePhysicsUpdate(mSimStep); // NOTICE: mSimStep should be non zero
        mSceneServer->PhysicsUpdate(mSimStep);
        if (mAutoTime)
            AdvanceTime(mSimStep);

        mThreadBarrier->wait();

        if (mExit) // this check should be here so that all threads will quit
            break;
		mSceneServer->PostPhysicsUpdate();
        mGameControlServer->Update(mSimStep);
        mSimTime += mSimStep;
        mSumDeltaTime -= mSimStep;
        Step();
        ControlEvent(CE_EndCycle);
        mThreadBarrier->wait();
    }

    // wait for threads
    ctrThrdGroup.join_all();
}

void SimulationServer::SimControlThread(shared_ptr<SimControlNode> controlNode)
{
    if (!mThreadBarrier)
    {
        GetLog()->Error()
                << "(SimulationServer) mThreadBarrier is not initialized.\n";
        return;
    }

    bool isInputControl = (controlNode->GetName() == "InputControl");

    while ( true )
    {
        if (int(controlNode->GetTime()*100) < int(mSimTime*100))
        {
            controlNode->StartCycle();
            controlNode->SenseAgent();
            controlNode->ActAgent();
            controlNode->SetSimTime(mSimTime);
        }
    	if (isInputControl)
    	{
            while (int(mSumDeltaTime*100) < int(mSimStep*100))
                controlNode->StartCycle(); // advance the time
    	}
        mThreadBarrier->wait();
        if (mExit)
            break;
        // wait for physics update
        mThreadBarrier->wait();
//        EndCycle();
    }
}

void SimulationServer::SetMultiThreads(bool isMThreas)
{
    mMultiThreads = isMThreas;
}

void SimulationServer::SetAdjustSpeed(bool adjustSpeed)
{
    mAdjustSpeed = adjustSpeed;
}

void SimulationServer::SetMaxStepsPerCycle(int max)
{
    mMaxStepsPerCycle = max;
}
