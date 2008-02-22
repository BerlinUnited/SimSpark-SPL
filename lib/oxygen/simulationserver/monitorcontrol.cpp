/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorcontrol.cpp,v 1.5 2008/02/22 07:52:15 hedayat Exp $

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
#include "monitorcontrol.h"
#include "simulationserver.h"
#include "netmessage.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/monitorserver/monitorserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

MonitorControl::MonitorControl() : NetControl()
{
    mLocalAddr.setPort(3200);
    mMonitorInterval = 30;
}

MonitorControl::~MonitorControl()
{
}

void MonitorControl::OnLink()
{
    NetControl::OnLink();
    shared_ptr<SimulationServer> sim = GetSimulationServer();
    if (sim.get() == 0)
        {
            GetLog()->Error()
                << "(MonitorControl) ERROR: SimulationServer not found\n";
            return;
        }

    mMonitorServer = sim->GetMonitorServer();
}

void MonitorControl::OnUnlink()
{
    NetControl::OnUnlink();
    mMonitorServer.reset();
}

void MonitorControl::ClientConnect(shared_ptr<Client> client)
{
    if (
        (mMonitorServer.get() == 0) ||
        (mNetMessage.get() == 0)
        )
        {
            return;
        }

    string header = mMonitorServer->GetMonitorHeaderInfo();
    mNetMessage->PrepareToSend(header);
    SendMessage(client->addr,header);
}

void MonitorControl::EndCycle()
{
    NetControl::EndCycle();

    const int cycle = GetSimulationServer()->GetCycle();
    if (cycle % mMonitorInterval)
        {
            return;
        }

    if (
        (mMonitorServer.get() == 0) ||
        (mNetMessage.get() == 0) ||
        (mClients.size() == 0)
        )
        {
            return;
        }

    // send updates to all connected monitors
    string info = mMonitorServer->GetMonitorData();
    mNetMessage->PrepareToSend(info);

    for (
         TAddrMap::iterator iter = mClients.begin();
         iter != mClients.end();
         ++iter
         )
        {
            SendMessage((*iter).second,info);
        }

    // reset the modified flag for the active scene
    shared_ptr<SceneServer> sceneServer =
        GetSimulationServer()->GetSceneServer();

    if (sceneServer.get() !=0)
        {
            shared_ptr<Scene> scene = sceneServer->GetActiveScene();
            if (scene.get() != 0)
                {
                    scene->SetModified(false);
                }
        }
}

void MonitorControl::StartCycle()
{
    NetControl::StartCycle();

    if (
        (mMonitorServer.get() == 0) ||
        (mNetMessage.get() == 0)
        )
        {
            return;
        }

    // pass all received messages to the MonitorServer
    for (
         TBufferMap::iterator iter = mBuffers.begin();
         iter != mBuffers.end();
         ++iter
         )
        {
            shared_ptr<NetBuffer>& netBuff = (*iter).second;
            if (
                (netBuff.get() == 0) ||
                (netBuff->IsEmpty())
                )
                {
                    continue;
                }

            string message;
            while (mNetMessage->Extract(netBuff,message))
                {
                    mMonitorServer->ParseMonitorMessage(message);
                }
        }
}

int MonitorControl::GetMonitorInterval()
{
    return mMonitorInterval;
}

void MonitorControl::SetMonitorInterval(int i)
{
    mMonitorInterval = i;
}


