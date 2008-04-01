/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorlogger.cpp,v 1.3 2008/04/01 14:25:03 yxu Exp $

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
#include "monitorlogger.h"
#include "simulationserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/monitorserver/monitorserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

MonitorLogger::MonitorLogger() : SimControlNode()
{
    mMonitorLoggerInterval = 30;
}

MonitorLogger::~MonitorLogger()
{
}

void MonitorLogger::OnLink()
{
    SimControlNode::OnLink();
    shared_ptr<SimulationServer> sim = GetSimulationServer();
    if (sim.get() == 0)
        {
            GetLog()->Error()
                << "(MonitorControl) ERROR: SimulationServer not found\n";
            return;
        }

    mMonitorServer = sim->GetMonitorServer();

    logFile.open("sparkmonitor.log");
}

void MonitorLogger::OnUnlink()
{
    SimControlNode::OnUnlink();
    mMonitorServer.reset();
}

void MonitorLogger::EndCycle()
{
    SimControlNode::EndCycle();

    const int cycle = GetSimulationServer()->GetCycle();
    if ( cycle == 1 ){
        string header = mMonitorServer->GetMonitorHeaderInfo();
        logFile << header;
        return;
    }
    
    bool fullState = false;
    shared_ptr<SceneServer> sceneServer =
        GetSimulationServer()->GetSceneServer();
    
    if (sceneServer.get() ==0)
        {
            GetLog()->Error()
                << "(MonitorLogger) ERROR: SceneServer not found\n";
            return;
        }

    shared_ptr<Scene> scene = sceneServer->GetActiveScene();
    if (scene.get() != 0)
    {
        if ( scene->GetModified() )
        {
            fullState = true;
        }
    }

    if ( !fullState && (cycle % mMonitorLoggerInterval) )
    {
        return;
    }

    if ( mMonitorServer.get() == 0 ) 
    {
        return;
    }

    // log updates
    string info = mMonitorServer->GetMonitorData();

    logFile << info;
}

int MonitorLogger::GetMonitorLoggerInterval()
{
    return mMonitorLoggerInterval;
}

void MonitorLogger::SetMonitorLoggerInterval(int i)
{
    mMonitorLoggerInterval = i;
}
