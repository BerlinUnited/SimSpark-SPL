/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "sparksimulationthread.h"
#include "simspark.h"
#include "sparkcontroller.h"
#include "simulationmanager.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SparkSimulationThread::SparkSimulationThread(boost::shared_ptr<TaskDefinition> definition) :
    SimulationThread(definition),
    mController(new SparkController())
{
    //Connect state update signals. running and paused are sent by the spark controller.
    connect(this, SIGNAL(sparkRunning(SparkController*)), getMessenger(), SLOT(updateRunning()));    //running(id) is emitted by the spark controller
    connect(this, SIGNAL(sparkPaused(SparkController*)), getMessenger(), SLOT(updatePaused()));      //paused(id) is emitted by the spark controller
    //finished(id) is emitted via updateFinished() by the SimulationThread, when the QThread emits finished()
    //terminated is never emitted

    //Init spark controller connections. SparkSimulationThreads use SimSpark signals to specify running and paused. finished is sent by the thread 
    connect(&*mController, SIGNAL(sparkRunning(SparkController*)), this, SIGNAL(sparkRunning(SparkController*)));
    connect(&*mController, SIGNAL(sparkPaused(SparkController*)), this, SIGNAL(sparkPaused(SparkController*)));
    //connect(mController, SIGNAL(sparkFinished(SparkController*)), this, SIGNAL(finished(SparkController*)));

}

SparkSimulationThread::~SparkSimulationThread()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------
/*
boost::shared_ptr<SparkSimulationThread> SparkSimulationThread::getHostSparkThread()
{
    if (mSpark.get() != 0)
    {
        //This thread is the host, return copy of its shared_ptr from the simulation manager

        SimulationManager* manager = Carbon::get()->getSimulationManager();
        int index = -1;
        int run = 0;
        for (auto it = manager->getThreadList().begin(); it != manager->getThreadList().end(); it++)
        {
            if (it->get() == this)
            {
                index = run;
                break;
            }
        }

        if (index == -1)
            LOG_ERROR() << "Current thread not found in SimulationManagers thread list.";
        else
            return boost::shared_static_cast<SparkSimulationThread>(manager->getThreadList().at(index));
    }
    else
    {
        //This thread is not the host, or spark has not been initialized yet
        LOG_INFO() << "TODO: getHostSparkThread()";
    }

    return boost::shared_ptr<SparkSimulationThread>();
}*/

boost::shared_ptr<SparkController> SparkSimulationThread::getSparkController() const
{
    return mController;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

int SparkSimulationThread::initSpark(bool restart)
{
    if (!restart && mController->getSpark().get() != 0)
        return 0;

    //Init with values from the task definition
    return mController->initSpark(getTaskDefinition().getSecond(), getTaskDefinition().getThird(), getTaskDefinition().getFirstList(), 
        getTaskDefinition().getSecondList(), SparkController::RL_DEFAULT, restart);
}


int SparkSimulationThread::runSpark()
{
    if (!assertSpark())
        return 1;

    return mController->runSpark();
}

void SparkSimulationThread::stopSpark()
{
    if (!assertSpark())
        return;

    mController->stopSpark();
}

void SparkSimulationThread::pauseSpark(bool pause)
{
    if (!assertSpark())
        return;

    mController->pauseSpark(pause);
}

void SparkSimulationThread::resetSpark()
{
    if (!assertSpark())
        return;

    mController->resetSpark();
}

void SparkSimulationThread::initSparkLogging()
{
    if (!assertSpark())
        return;

    int logSource = Logger::getLogSource(std::string("Spark"));

    mController->initLogging(logSource);
}

//--------------------------------------------------------------
// Public virtual
//--------------------------------------------------------------

void SparkSimulationThread::reset()
{
    resetSpark();
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool SparkSimulationThread::assertSpark()
{
    if (mController->getSpark().get() == 0)
    {
        LOG_ERROR() << "No spark object initialized in the SparkController.";
        return false;
    }

    return true;
}