/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: agentcontrol.cpp,v 1.1 2004/04/25 16:46:57 rollmark Exp $

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
#include "agentcontrol.h"
#include "simulationserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

AgentControl::AgentControl() : NetControl()
{
    mLocalAddr.setPort(3100);
}

AgentControl::~AgentControl()
{
}

void AgentControl::OnLink()
{
    NetControl::OnLink();
    shared_ptr<SimulationServer> sim = GetSimulationServer();
    if (sim.get() == 0)
        {
            GetLog()->Error()
                << "(AgentControl) ERROR: SimulationServer not found\n";
            return;
        }

    mGameControlServer = sim->GetGameControlServer();
}

void AgentControl::OnUnlink()
{
    NetControl::OnUnlink();
    mGameControlServer.reset();
}

void AgentControl::ClientConnect(shared_ptr<Client> client)
{
    if (mGameControlServer.get() == 0)
        {
            return;
        }

    mGameControlServer->AgentConnect(client->id);
}

void AgentControl::ClientDisconnect(shared_ptr<Client> client)
{
    if (mGameControlServer.get() == 0)
        {
            return;
        }

    mGameControlServer->AgentDisappear(client->id);
}

