/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerinput.h,v 1.1 2004/12/31 11:30:30 rollmark Exp $

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
#ifndef SOCCERINPUT_H
#define SOCCERINPUT_H

#include <kerosin/inputserver/inputitem.h>
#include <kerosin/inputserver/inputcontrol.h>
#include <oxygen/simulationserver/netclient.h>

class SoccerInput : public kerosin::InputItem
{
public:
    enum ECmds
        {
            CmdUser    = kerosin::InputControl::CmdUser,
            CmdKickOff = CmdUser + 1
        };

public:
    SoccerInput();
    virtual ~SoccerInput();

    /** this method is called every time an input unknown to the
        parent InputControl node is received
    */
    virtual void ProcessInput(const kerosin::InputServer::Input& input);

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
    boost::shared_ptr<oxygen::NetClient> mMonitorClient;
};

DECLARE_CLASS(SoccerInput);

#endif // SOCCERINPUT_H
