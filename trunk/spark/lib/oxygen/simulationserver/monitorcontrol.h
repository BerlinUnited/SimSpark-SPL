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
#ifndef OXYGEN_MONITORCONTROL_H
#define OXYGEN_MONITORCONTROL_H

#include "netcontrol.h"
#include <oxygen/oxygen_defines.h>
#include <oxygen/monitorserver/monitorserver.h>

namespace oxygen
{

/** \class MonitorConrol is a NetControl node that manages the
    communication with monitors in cooperation with the
    MonitorServer.
*/
class OXYGEN_API MonitorControl : public NetControl
{
public:
    MonitorControl();
    virtual ~MonitorControl();

    /** forwards all pending messages from all connected monitors to
        the MonitorServer */
    virtual void StartCycle();

    /** updates all connected monitors at the end of each simulation
        cycle */
    virtual void EndCycle();

    /** called when a new client connects */
    virtual void ClientConnect(boost::shared_ptr<Client> client);

    /** returns the monitor update interval in cycles */
    int GetMonitorInterval();

    /** sets the monitor update interval in cycles */
    void SetMonitorInterval(int i);

    /** Request that next update gives the full state */
    void RequestFullState();
    
protected:
    virtual void OnLink();

protected:
    /** cached reference to the MonitorServer */
    CachedPath<MonitorServer> mMonitorServer;

    /** number of full state logged */
    int mFullStateLogged;
};

DECLARE_CLASS(MonitorControl);

} // namespace oxygen

#endif // OXYGEN_MONITORCONTROL_H
