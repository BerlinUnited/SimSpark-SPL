/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorinputcontrol.cpp,v 1.1.2.1 2004/05/10 11:53:55 fruit Exp $

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
#include "monitorinputcontrol.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace zeitgeist;
using namespace boost;
using namespace std;

void MonitorInputControl::OnLink()
{
    InputControl::OnLink();

    mCommServer = shared_dynamic_cast<CommServer>
        (GetCore()->Get("/sys/server/comm"));

    if (mCommServer.get() == 0)
        {
            GetLog()->Error()
                << "(MonitorInputControl) ERROR: CommServer not found\n";
        }

    // publish user defined command constants
    shared_ptr<ScriptServer> script = GetCore()->GetScriptServer();
    script->CreateVariable("Command.KickOff", CmdKickOff);
}

void MonitorInputControl::ProcessInput(kerosin::InputServer::Input& input)
{
    if (mCommServer.get() == 0)
        {
            return;
        }

    switch (input.id)
        {
        case CmdKickOff:
            // kick off
            GetLog()->Normal() <<"--- Kick Off\n";
            mCommServer->SendKickOffCmd();
            break;
        }
}
