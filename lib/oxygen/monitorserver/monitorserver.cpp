/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorserver.cpp,v 1.4 2004/04/25 16:36:35 rollmark Exp $

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

#include <zeitgeist/logserver/logserver.h>
#include "monitorserver.h"

using namespace oxygen;
using namespace boost;
using namespace std;

MonitorServer::MonitorServer() : Node()
{
}

MonitorServer::~MonitorServer()
{
}

bool
MonitorServer::RegisterMonitorSystem(const std::string& monitorSysName)
{
    // check if a monitor system of the requested type was already created
    shared_ptr<MonitorSystem> monitorSys =
        shared_dynamic_cast<MonitorSystem>(GetChildOfClass(monitorSysName));

    if (monitorSys.get() != 0)
    {
        return true;
    }

    // create the monitor system
    monitorSys = shared_dynamic_cast<MonitorSystem>(GetCore()->New(monitorSysName));

    if (monitorSys.get() == 0)
    {
        GetLog()->Error() << "ERROR: (MonitorServer) Cannot create monitor system '"
                          << monitorSysName << "'" << std::endl;
        return false;
    }

    // link the monitor system in the hierarchy
    monitorSys->SetName(monitorSysName);

    if (! AddChildReference(monitorSys))
        {
            GetLog()->Error() << "ERROR: (MonitorServer) Cannot link the monitor system '"
                              << monitorSysName << "' to the hierarchy\n";
            return false;
        }

    GetLog()->Debug() << "(MonitorServer) Registered monitor system '"
                      << monitorSysName << "'\n";

    return true;
}

boost::shared_ptr<MonitorSystem> MonitorServer::GetMonitorSystem()
{
    return shared_static_cast<MonitorSystem>
        (
         FindChildSupportingClass<MonitorSystem>()
         );
}

string MonitorServer::GetMonitorHeaderInfo()
{
    shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() != 0)
        {
            return monitorSystem->GetMonitorHeaderInfo();
        } else
            {
                return string();
            }
}

string MonitorServer::GetMonitorInfo()
{
    shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() != 0)
        {
            return monitorSystem->GetMonitorInfo();
        } else
            {
                return string();
            }
}

void MonitorServer::ParseMonitorMessage(const string& data)
{
    shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() != 0)
        {
            monitorSystem->ParseMonitorMessage(data);
        }
}



