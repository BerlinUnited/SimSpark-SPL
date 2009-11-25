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
#ifndef SOCCERINPUTLOGPLAYER_H
#define SOCCERINPUTLOGPLAYER_H

#include <kerosin/inputserver/inputitem.h>
#include <kerosin/inputserver/inputcontrol.h>
#include <oxygen/simulationserver/simcontrolnode.h>


class SoccerInputLogPlayer : public kerosin::InputItem
{
public:
    enum ECmds
        {
            CmdUser    = kerosin::InputControl::CmdUser,
            CmdPause   = CmdUser + 1,
            CmdStepForward   = CmdUser + 2,
            CmdStepBackward  = CmdUser + 3,
            CmdBPlayback     = CmdUser + 4,
            CmdCameraLeftGoal    = CmdBPlayback + 1,
            CmdCameraLeftCorner  = CmdCameraLeftGoal + 1,
            CmdCameraMiddleLeft  = CmdCameraLeftCorner + 1,
            CmdCameraMiddle      = CmdCameraMiddleLeft + 1,
            CmdCameraMiddleRight = CmdCameraMiddle + 1,
            CmdCameraRightCorner = CmdCameraMiddleRight + 1,
            CmdCameraRightGoal   = CmdCameraRightCorner + 1
        };

public:
    SoccerInputLogPlayer();
    virtual ~SoccerInputLogPlayer();

    /** this method is called every time an input unknown to the
        parent InputControl node is received
    */
    virtual void ProcessInput(const kerosin::Input& input);

    /** registers the soccermonitor command constants and cache
        SparkMonitorClient reference
    */
    virtual void OnLink();

    /** reset SparkMonitorClient reference */
    virtual void OnUnlink();

protected:
    void SendCommand(const std::string& cmd);

protected:
    /** cached reference to the monitor client */
    boost::shared_ptr<oxygen::SimControlNode> mMonitorClient;

    /** cached reference to the script server */
    boost::shared_ptr<zeitgeist::ScriptServer> mScriptServer;

    /** cached reference to the camera body */
    boost::shared_ptr<oxygen::RigidBody> mCameraBody;

    /** cached reference to the camera body */
    boost::shared_ptr<oxygen::FPSController> mFPS;
};

DECLARE_CLASS(SoccerInputLogPlayer);

#endif // SOCCERINPUTLOGPLAYER_H
