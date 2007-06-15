/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerinput.cpp,v 1.3 2007/06/15 09:47:29 jboedeck Exp $

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
#include "soccerinput.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

SoccerInput::SoccerInput()
{
}

SoccerInput::~SoccerInput()
{
}

void SoccerInput::OnLink()
{
    shared_ptr<ScriptServer> scriptServer = GetCore()->GetScriptServer();
    scriptServer->CreateVariable("Command.KickOff", CmdKickOff);
    scriptServer->CreateVariable("Command.MoveAgent", CmdMoveAgent);
    scriptServer->CreateVariable("Command.DropBall", CmdDropBall);

    mMonitorClient = shared_dynamic_cast<NetClient>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient"));

    if (mMonitorClient.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
        }
}

void SoccerInput::OnUnlink()
{
    mMonitorClient.reset();
}

void SoccerInput::SendCommand(const std::string& cmd)
{
    if (mMonitorClient.get() == 0)
        {
            return;
        }

    mMonitorClient->SendMessage(cmd);
}

void SoccerInput::ProcessInput(const InputServer::Input& input)
{
    switch (input.id)
        {
        default:
            return;

        case CmdKickOff:
            if (input.KeyPress())
                {
                    SendCommand("(kickOff Left)");
                }
            break;
        case CmdMoveAgent:
            if (input.KeyPress())
                {
                    SendCommand("(agent (team Left)(unum 1)(pos -4.0 1.0 3.5))");
                }
            break;    
        case CmdDropBall:
            if (input.KeyPress())
                {
                    SendCommand("(dropBall)");
                }
            break;
        };
}
