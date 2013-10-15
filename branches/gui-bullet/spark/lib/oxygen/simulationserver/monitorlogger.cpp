/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorlogger.cpp 212 2010-06-06 22:44:51Z hedayat $

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

MonitorLogger::MonitorLogger() : SimControlNode(), mFullStateLogged(0),
        mFullStateLoggedTime(0)
{
}

MonitorLogger::~MonitorLogger()
{
}

void MonitorLogger::OnLink()
{
    SimControlNode::OnLink();
    boost::shared_ptr<SimulationServer> sim = GetSimulationServer();
    if (sim.get() == 0)
        {
            GetLog()->Error()
                << "(MonitorControl) ERROR: SimulationServer not found\n";
            return;
        }

    mMonitorServer = sim->GetMonitorServer();
    if ( 0 == mMonitorServer.get() ){
        GetLog()->Error()
                << "(MonitorControl) ERROR: MonitorServer not found\n";
        return;
    }

    mLogFile.open("sparkmonitor.log");
}

void MonitorLogger::OnUnlink()
{
    SimControlNode::OnUnlink();
    mMonitorServer.reset();
}

void MonitorLogger::EndCycle()
{
    SimControlNode::EndCycle();

    string info;
    boost::shared_ptr<Scene> scene = GetActiveScene();
    // The logger might miss some information as it runs at a lower rate
    if (mTime - mFullStateLoggedTime > 3.0 ||
        (scene.get() != 0 && scene->GetModifiedNum() > mFullStateLogged)
        )
    {
        mFullStateLoggedTime = mTime;
        mFullStateLogged = scene->GetModifiedNum();
        info = mMonitorServer->GetMonitorHeaderInfo();
    }
    else
    {
        info = mMonitorServer->GetMonitorData();
    }

    // log updates
    mLogFile << info << std::endl;
}
