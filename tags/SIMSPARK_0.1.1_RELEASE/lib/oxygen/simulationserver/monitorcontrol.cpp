/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "monitorcontrol.h"
#include "simulationserver.h"
#include "netmessage.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

MonitorControl::MonitorControl() : NetControl(), mFullStateLogged(0)
{
    mLocalAddr.setPort(3200);
}

MonitorControl::~MonitorControl()
{
}

void MonitorControl::OnLink()
{
    NetControl::OnLink();
    RegisterCachedPath(mMonitorServer, "/sys/server/monitor");

    if (mMonitorServer.expired())
        {
            GetLog()->Error()
                << "(MonitorControl) ERROR: MonitorServer not found\n";
            return;
        }
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
    SendClientMessage(client->addr,header);
    
    shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() != 0)
    {
        mFullStateLogged = scene->GetModifiedNum();
    }
}

void MonitorControl::EndCycle()
{
    NetControl::EndCycle();

    if (
        (mMonitorServer.get() == 0) ||
        (mNetMessage.get() == 0)
        )
    {
        return;
    }
    
    // send updates to all connected monitors
    if ( !mClients.empty() )
    {
        string info;
        shared_ptr<Scene> scene = GetActiveScene();
        if (scene.get() != 0
            && scene->GetModifiedNum() > mFullStateLogged )
        {
            mFullStateLogged = scene->GetModifiedNum();
            info = mMonitorServer->GetMonitorHeaderInfo();
        }
        else
        {
            info = mMonitorServer->GetMonitorData();
        }
        
        mNetMessage->PrepareToSend(info);

        for (
            TAddrMap::iterator iter = mClients.begin();
            iter != mClients.end();
            ++iter
            )
        {
            SendClientMessage((*iter).second,info);
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


