/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "simulationserver.h"
#include "simcontrolnode.h"
#include "timersystem.h"
#include <zeitgeist/logserver/logserver.h>
#include <signal.h>
#include <algorithm>
#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

std::vector<SimulationServer*> SimulationServer::mServers = std::vector<SimulationServer*>();

void SimulationServer::CatchSignal(int sig_num)
{
    static bool exiting = false;

    if (sig_num == SIGINT)
        if (!exiting)
        {
            signal(SIGINT, CatchSignal);
            for (auto it = mServers.begin(); it != mServers.end(); it++) 
                (*it)->mExit = true;
            std::cout << "(SimulationServer) caught SIGINT. exiting.\n";

            exiting = true;
        }
}

SimulationServer::SimulationServer() :
    Node(), mAdjustSpeed(false), mExitThreads(false), mMaxStepsPerCycle(3),
    mThreadBarrier(0)
{
    mSimTime      = 0.0f;
    mSimStep      = 0.2f;
    mAutoTime     = true;
    mCycle        = 0;
    mPausedCycle  = 0;
    mSumDeltaTime = 0;
    mArgC         = 0;
    mArgV         = 0;
    mMultiThreads = true;
    mExit         = false;
    mRunning      = false;
    mCyclePaused  = false;
    mContinueCycle= false;

    mServers.push_back(this);

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
    for (auto it = mServers.begin(); it != mServers.end(); it++) 
        (*it)->mExit = true;
}

bool SimulationServer::WantsToQuit()
{
    bool allQuit = true;
    for (auto it = mServers.begin(); it != mServers.end(); it++) 
        if ((*it)->mExit = false) allQuit = false;

    return allQuit;
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

bool SimulationServer::GetRunning()
{
    return mRunning;
}

bool SimulationServer::GetPaused()
{
    return mCyclePaused && mRunning;
}

bool SimulationServer::InitControlNode(const std::string& className, const std::string& name)
{
    boost::shared_ptr<Leaf> prevCtrNode = GetChild(name);
    if (prevCtrNode.get() != 0)
        {
            RemoveChildReference(prevCtrNode);

            GetLog()->Debug()
                << "(SimulationServer) removing previous SimControlNode instance "
                << name << "'\n";
        }

    boost::shared_ptr<SimControlNode> control
        = dynamic_pointer_cast<SimControlNode>(GetCore()->New(className));

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

/** creates and registers a new TimerSystem to the SimulationServer */
bool SimulationServer::InitTimerSystem(const std::string& className)
{
    if (mTimerSystem)
        {
            GetLog()->Error() << "(SimulationServer) ERROR: "
                              << "Another timer system already in use!\n";
            return false;
        }

    mTimerSystem = dynamic_pointer_cast<TimerSystem>(GetCore()->New(className));
    SetAutoTimeMode(false);

    if (!mTimerSystem)
        {
            GetLog()->Error() << "(SimulationServer) ERROR: "
                              << "Unable to create '" << className << "'\n";
            return false;
        }

    GetLog()->Normal()
        << "(SimulationServer) TimerSystem '"
        << className << "' registered\n";

    return true;
}

boost::shared_ptr<SimControlNode>
SimulationServer::GetControlNode(const string& controlName)
{
    boost::shared_ptr<SimControlNode> ctrNode =
        dynamic_pointer_cast<SimControlNode>(GetChild(controlName));

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

void SimulationServer::WaitStep()
{
    // do not update scene or game control server, do not advance total simulation time
    // simply ignore the passed time since the last step 
    mSumDeltaTime = 0;
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
            float finalStep = 0;
            while (int(mSumDeltaTime*100) >= int(mSimStep*100))
                {
                    mSceneServer->PrePhysicsUpdate(mSimStep);
                    mSceneServer->PhysicsUpdate(mSimStep);
                    UpdateDeltaTimeAfterStep(mSumDeltaTime);
                    finalStep += mSimStep;
                }
            mSceneServer->PostPhysicsUpdate();
            mGameControlServer->Update(finalStep);
            mSimTime += finalStep;

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
    for (
         TLeafList::iterator iter=begin();
         iter != end();
         ++iter
         )
        {
            boost::shared_ptr<SimControlNode> ctrNode =
                dynamic_pointer_cast<SimControlNode>(*iter);

            if (ctrNode.get() == 0)
                {
                    continue;
                }

            if ( ctrNode->GetTime() - mSimTime > 0.005f ) continue;

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

                case CE_WaitCycle :
                    ctrNode->WaitCycle();
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
    mRunning = false;
    mCyclePaused = false;

    // cache argc and argv, to make it accessible for registerd
    // SimControlNodes
    mArgC = argc;
    mArgV = argv;

    ControlEvent(CE_Init);

    if (mTimerSystem)
        {
            mTimerSystem->Initialize();
        }
}

void SimulationServer::Run(int argc, char** argv)
{
    Init(argc, argv);
    GetLog()->Normal() << "(SimulationServer) entering runloop\n";

    if ( !mAutoTime && !mTimerSystem )
        {
            GetLog()->Error()<< "(SimulationServer) ERROR: can not get"
                    " any TimerSystem objects.\n";
        }
    else
        {
            mRunning = true;
            mCyclePaused = false;
            mContinueCycle = false;
            mPausedCycle = 0;

            if ( mMultiThreads )
                {
                    GetLog()->Normal()<< "(SimulationServer) running in "
                            "multi-threaded mode\n";
                    RunMultiThreaded();
                }
            else
                {
                    GetLog()->Normal()<< "(SimulationServer) running in single "
                            "thread mode\n";
                    while (! mExit)
                        {
                            Cycle();
                        }
                }
        }

    mRunning = false;
    Done();
}

void SimulationServer::PauseCycle(bool state)
{
    if (!mRunning)
        return;
    
    if (state == true)
        mCyclePaused = true;
    else
        mContinueCycle = true;
}

void SimulationServer::Cycle()
{
    if (mCyclePaused)
    {
        if (mPausedCycle == 0)
            GetLog()->Normal()<< "(SimulationServer) Server paused." << std::endl;

        ControlEvent(CE_WaitCycle);

        WaitStep();
        SyncTime();

        ++mPausedCycle;

        if (mContinueCycle)
        {
            mCyclePaused = false;
            mContinueCycle = false;
            mPausedCycle = 0;
            GetLog()->Normal()<< "(SimulationServer) Server running." << std::endl;
        }
    }
    else
    {
        ++mCycle;
        ControlEvent(CE_StartCycle);
        ControlEvent(CE_SenseAgent);
        ControlEvent(CE_ActAgent);

        Step();
        SyncTime();

        ControlEvent(CE_EndCycle);
    }
}

void SimulationServer::Done()
{
    if (mTimerSystem)
        {
            mTimerSystem->Finalize();
        }

    ControlEvent(CE_Done);

    mArgC = 0;
    mArgV = 0;

    GetLog()->Normal()
        << "(SimulationServer) leaving runloop at t="
        << mSimTime << "\n";
}

boost::shared_ptr<GameControlServer> SimulationServer::GetGameControlServer()
{
    return mGameControlServer.get();
}

boost::shared_ptr<MonitorServer> SimulationServer::GetMonitorServer()
{
    return mMonitorServer.get();
}

boost::shared_ptr<SceneServer> SimulationServer::GetSceneServer()
{
    return mSceneServer.get();
}

void SimulationServer::RunMultiThreaded()
{
    // count valid SimControlNodes.
    int count = 1;
    for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
        {
            if (dynamic_pointer_cast<SimControlNode>(*iter))
                count++;
        }
    mThreadBarrier = new barrier(count);

    // create new threads for each SimControlNode
    boost::thread_group ctrThrdGroup;
    for ( TLeafList::iterator iter=begin(); iter != end(); ++iter )
        {
            boost::shared_ptr<SimControlNode> ctrNode =  dynamic_pointer_cast<SimControlNode>(*iter);
            if (ctrNode.get() == 0) continue;

            ctrThrdGroup.create_thread(boost::bind(&SimulationServer::SimControlThread,
                                                   this, ctrNode));
        }

    boost::shared_ptr<SimControlNode> renderControl = GetControlNode("RenderControl");

    //float initDelta, finalDelta;      //unused variables
    mExitThreads = false;
    while (!mExitThreads)
        {
            if (mCyclePaused) //wait until simulation is started again
                {
                    if (mPausedCycle == 0)
                        GetLog()->Normal()<< "(SimulationServer) Server paused." << std::endl;
                    
                    ++mPausedCycle;

                    // Sync at start of wait-cycle (signals wait-cycle)
                    mThreadBarrier->wait();
                    if (mExit)
                        mExitThreads = true;

                    // Advance time, but do not change the state of the simulation
                    WaitStep();
                    SyncTime(); 

                    //Continue running in next cycle 
                    if (mContinueCycle)
                    {
                        mCyclePaused = false;
                        mContinueCycle = false;
                        mPausedCycle = 0;
                        GetLog()->Normal()<< "(SimulationServer) Server running." << std::endl;
                    }

                    // End Cycle
                    mThreadBarrier->wait();
                }
            else 
                {
                    ++mCycle;

                    // Signal start of cycle
                    mThreadBarrier->wait();
                    if (mExit)
                        mExitThreads = true;

                    // Wait for SimControlNodes' acts at the beginning of a cycle
                    mThreadBarrier->wait();

                    Step();
                    SyncTime();

                    if (renderControl
                        && renderControl->GetTime() - mSimTime < 0.005f )
                        renderControl->EndCycle();

                    // End Cycle
                    mThreadBarrier->wait();
                }
        }

    // wait for threads
    ctrThrdGroup.join_all();
}

void SimulationServer::SimControlThread(boost::shared_ptr<SimControlNode> controlNode)
{
    if (!mThreadBarrier)
        {
            GetLog()->Error()
                << "(SimulationServer) mThreadBarrier is not initialized.\n";
            return;
        }

    bool isRenderControl = (controlNode->GetName() == "RenderControl");
    bool newCycle = false;

    while (!mExitThreads)
        {
            if (mCyclePaused)
                {
                    mThreadBarrier->wait();
                    controlNode->WaitCycle();
                    mThreadBarrier->wait();
                }
            else
                {
                    mThreadBarrier->wait();

                    newCycle = false;
                    if ( controlNode->GetTime() - mSimTime <= 0.005f)
                        {
                            newCycle = true;
                            controlNode->StartCycle();
                            controlNode->SenseAgent();
                            controlNode->ActAgent();
                            controlNode->SetSimTime(mSimTime);
                        }

                    mThreadBarrier->wait();

                    // wait for physics update
                    mThreadBarrier->wait();
                    if (!isRenderControl && newCycle)
                        controlNode->EndCycle();
                }
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

inline void SimulationServer::UpdateDeltaTimeAfterStep(float &deltaTime)
{
    if (mAdjustSpeed && deltaTime > mMaxStepsPerCycle
            * mSimStep)
    {
        float diff = deltaTime - mSimStep;
        if (diff > 0.0001)
        {
            GetLog()->Debug()
                    << "(SimulationServer) Warning: Skipping remaining time: "
                    << diff << '\n';
        }
        deltaTime = 0;
    }
    else
        deltaTime -= mSimStep;
}

inline void SimulationServer::SyncTime()
{
    if (mAutoTime)
        {
            AdvanceTime(mSimStep);
        }
    else
        {
            mTimerSystem->WaitFromLastQueryUntil(mSimStep - mSumDeltaTime);
            AdvanceTime(mTimerSystem->GetTimeSinceLastQuery());
        }
}
