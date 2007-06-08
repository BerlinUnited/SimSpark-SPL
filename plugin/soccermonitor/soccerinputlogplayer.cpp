/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerinputlogplayer.cpp,v 1.1.2.2 2007/06/08 00:07:35 hedayat Exp $

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
#include "soccerinputlogplayer.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

SoccerInputLogPlayer::SoccerInputLogPlayer()
{
}

SoccerInputLogPlayer::~SoccerInputLogPlayer()
{
}

void SoccerInputLogPlayer::OnLink()
{
    mScriptServer = GetCore()->GetScriptServer();
    mScriptServer->CreateVariable("Command.Pause", CmdPause);
    mScriptServer->CreateVariable("Command.StepForward", CmdStepForward);
    mScriptServer->CreateVariable("Command.StepBackward", CmdStepBackward);
    mScriptServer->CreateVariable("Command.BPlayback", CmdBPlayback);

    mMonitorClient = shared_dynamic_cast<SimControlNode>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorLogFileServer"));

    if (mMonitorClient.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (SoccerInput) Unable to get SparkMonitorClient\n";
        }

}

void SoccerInputLogPlayer::OnUnlink()
{
}

void SoccerInputLogPlayer::SendCommand(const std::string& cmd)
{
/*    if (mMonitorClient.get() == 0)
        {
            return;
        }
*/}

void SoccerInputLogPlayer::ProcessInput(const InputServer::Input& input)
{

    switch (input.id)
        {
        default:
            return;

        case CmdPause:
            if (input.KeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.pauseMode()");
                }
            break;

        case CmdStepForward:
            if (input.KeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.stepForward()");
                }
            break;

        case CmdStepBackward:
            if (input.KeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.stepBackward()");
                }
            break;

        case CmdBPlayback:
            if (input.KeyPress())
                {
                    mScriptServer->Eval("monitorLogServer = get($serverPath+ \
                                    'simulation/SparkMonitorLogFileServer')");
                    mScriptServer->Eval("monitorLogServer.playBackward()");
                }
            break;
        };
}
