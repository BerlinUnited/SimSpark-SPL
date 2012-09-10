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
#ifndef OXYGEN_SIMULATIONSERVER_H
#define OXYGEN_SIMULATIONSERVER_H

#include <zeitgeist/node.h>
#include <oxygen/oxygen_defines.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/monitorserver/monitorserver.h>
#include <boost/thread/barrier.hpp>

namespace oxygen
{
class SimControlNode;

class OXYGEN_API SimulationServer : public zeitgeist::Node
{
/** \class SimulationServer manages the runloop of a simulation,
    i.e. it is responsible to manage the simulation time and advance
    the simulation. All simulation specifics, like rendering, network,
    agent and monitor management are delegated to a set
    SimControlNodes that are registered to the SimulationServer
 */
public:
    enum EControlEvent
        {
            CE_Init, // the simulation is initially started
            CE_Done, // the simulation is shut down
            CE_StartCycle, // a new cycle of the simulation loop
                           // started
            CE_SenseAgent, // collect sensor data and send it to the
                           // agents
            CE_ActAgent,   // collect (and possibly wait for) agent
                           // commands and carry them out
            CE_EndCycle // the current cycle ended
        };

public:
    SimulationServer();
    ~SimulationServer();

    /** exits the simulation on the next simulation step */
    static void Quit();

    /** returns true if Quit() was called previously */
    static bool WantsToQuit();

    /** returns the current simulation time */
    virtual float GetTime();

    /** resets the simulation time */
    virtual void ResetTime();

    /** sets the simulation time step */
    virtual void SetSimStep(float deltaTime);

    /** returns the simulation time step */
    virtual float GetSimStep();

    /** increases the accumulated time since the last simulation step,
        but does not step the simulation
     */
    virtual void AdvanceTime(float deltaTime);

    /** returns the accumulated time since the last simulation step
     */
    virtual float GetSumDeltaTime();

    /** creates and registers a new SimControlNode to the
        SimulationServer */
    bool InitControlNode(const std::string& className, const std::string& name);

    /** sets the auto time mode of the SimulationServer. if set to
        true the SimulationServer automatically advances the
        simulation mSimStep time every cycle, this is the default
        mode. Otherwise a registered ControlNode takes the
        responsibility to do so (by calling AdvanceTime regularly)
     */
    void SetAutoTimeMode(bool set);

    /** returns the current auto time setting */
    bool GetAutoTimeMode();

    /** returns the instance of a registerd SimControlNode */
    boost::shared_ptr<oxygen::SimControlNode>
    GetControlNode(const std::string& controlName);

    /** returns the cached argc */
    int GetArgC();

    /** returns the cached argv */
    char** GetArgV();

    /** init the runloop and all registered control nodes */
    virtual void Init(int argc = 0, char** argv = 0);

    /** go through on cycle of the runloop, i.e. sense, act, step */
    virtual void Cycle(boost::shared_ptr<SimControlNode> &inputCtr);

    /** shutdown server and all registered control nodes */
    virtual void Done();

    /** the runloop of the simulation, i.e. init(), cycle() until
        exit, done()
    */
    virtual void Run(int argc = 0, char** argv = 0);

    /** returns the cached MonitorServer reference */
    boost::shared_ptr<MonitorServer> GetMonitorServer();

    /** returns the cached GameControlServer reference */
    boost::shared_ptr<GameControlServer> GetGameControlServer();

    /** returns thr cached SceneServer reference */
    boost::shared_ptr<SceneServer> GetSceneServer();

    /** returns the current simulation cycle */
    int GetCycle();

    float GetSumDeltaTime() const { return mSumDeltaTime; }

    /** set the simulation run in multi-threads or in a signal thread */
    void SetMultiThreads(bool isMThreas);

    /** if set to true, the simulation speed will be adjusted according
     * to the system speed */
    void SetAdjustSpeed(bool adjustSpeed);

    /** set the maximum allowed steps per simulation cycle */
    void SetMaxStepsPerCycle(int max);

protected:
    virtual void OnLink();

    /** advances the simulation mSumDeltaTime seconds. If mSimStep is
        nonzero this is done in discrete steps */
    virtual void Step();

protected:
    /** dispatches a ControlEvent to the set of registered
        SimControlNodes, i.e. calls a method corresponding to the
        given event
     */
    void ControlEvent(EControlEvent event);

    /** SIGINT handler used to catch ctrl-C */
    static void CatchSignal(int sig_num);

    /** the multi-threaded runloop of the simulation */
    void RunMultiThreaded();

    /** the thread function which controls a single SimControlNode in
     *  multi-threaded mode. */
    void SimControlThread(boost::shared_ptr<SimControlNode> controlNode);

    /** updates mSumDeltaTime after a step in descreet simulations */
    void UpdateDeltaTimeAfterStep();

protected:
    /** the argc parameter passed to Run() */
    int mArgC;

    /** the argv parameter passed to Run() */
    char** mArgV;

    /** if true, the simulaion will exit on next simulation step */
    static bool mExit;

    /** the current simulation time */
    float mSimTime;

    /** the discrete simulation time step, or 0 for a non discrete
        simulation */
    float mSimStep;

    /** the accumulated simulation time since the last simulation
        step */
    float mSumDeltaTime;

    /** true if the server automatically advances the simulation time
        mSimStep every cycle (this is the default), otherwise a
        SimControlNode is responsible to advance the ime */
    bool mAutoTime;

    /** the current simulation cycle */
    int mCycle;

    /** a cached reference to the monitor server */
    CachedPath<MonitorServer> mMonitorServer;

    /** a cached reference to the GameControlServer */
    CachedPath<GameControlServer> mGameControlServer;

    /** a cached reference to the SceneServer */
    CachedPath<SceneServer> mSceneServer;

    /** indicates the simulation run in multi-threads or single thread */
    bool mMultiThreads;

    /** skips physical simulation for some time to catch up real time */
    bool mAdjustSpeed;

    /** determines the number of allowed steps per cycle when mAdjustSpeed is true */
    int mMaxStepsPerCycle;

    /** barrier object for synchronizing threads in multi-threaded mode */
    boost::barrier *mThreadBarrier;
};

DECLARE_CLASS(SimulationServer);

} // namespace oxygen

#endif // OXYGEN_SIMULATIONSERVER_H
