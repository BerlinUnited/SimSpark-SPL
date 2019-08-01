/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef Q_MOC_RUN
#include <boost/thread/locks.hpp>
#endif
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/simulationserver/simulationserver.h>
#include "monitorserver.h"
#include "monitoritem.h"

using namespace oxygen;
using namespace boost;
using namespace std;

MonitorServer::MonitorServer() : Node(), mDataCycle(0)
{
}

MonitorServer::~MonitorServer()
{
}

void MonitorServer::OnLink()
{
    RegisterCachedPath(mSimulationServer, "/sys/server/simulation");

    if (mSimulationServer.expired())
    {
        GetLog()->Warning()
            << "(MonitorServer) WARNING: SimulationServer not found.\n";
    }
}

bool
MonitorServer::RegisterMonitorSystem(const std::string& monitorSysName)
{
    // check if a monitor system of the requested type was already created
    boost::shared_ptr<MonitorSystem> monitorSys =
        dynamic_pointer_cast<MonitorSystem>(GetChildOfClass(monitorSysName));

    if (monitorSys.get() != 0)
    {
        return true;
    }

    // create the monitor system
    monitorSys = dynamic_pointer_cast<MonitorSystem>(GetCore()->New(monitorSysName));

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

bool
MonitorServer::RegisterMonitorItem(const std::string& monitorItemName)
{
    // check if a monitor item of the requested type was already created
    boost::shared_ptr<MonitorItem> monitorItem =
        dynamic_pointer_cast<MonitorItem>(GetChildOfClass(monitorItemName));

    if (monitorItem.get() != 0)
    {
        return true;
    }

    // create the monitor item
    monitorItem = dynamic_pointer_cast<MonitorItem>(GetCore()->New(monitorItemName));

    if (monitorItem.get() == 0)
    {
        GetLog()->Error() << "ERROR: (MonitorServer) Cannot create monitor item '"
                          << monitorItemName << "'" << std::endl;
        return false;
    }

    // link the monitor item in the hierarchy
    monitorItem->SetName(monitorItemName);

    if (! AddChildReference(monitorItem))
        {
            GetLog()->Error() << "ERROR: (MonitorServer) Cannot link the monitor item '"
                              << monitorItemName << "' to the hierarchy\n";
            return false;
        }

    GetLog()->Debug() << "(MonitorServer) Registered monitor item '"
                      << monitorItemName << "'\n";

    return true;
}

boost::shared_ptr<MonitorSystem> MonitorServer::GetMonitorSystem()
{
    return static_pointer_cast<MonitorSystem>
        (
         FindChildSupportingClass<MonitorSystem>()
         );
}

void
MonitorServer::CollectItemPredicates(bool initial, PredicateList& pList)
{
    Leaf::TLeafList itemList;
    ListChildrenSupportingClass<MonitorItem>(itemList);

    for (
        Leaf::TLeafList::const_iterator iter = itemList.begin();
        iter != itemList.end();
        ++iter
        )
    {
        boost::shared_ptr<MonitorItem> item =
            static_pointer_cast<MonitorItem>(*iter);

        if (initial)
        {
            item->GetInitialPredicates(pList);
        } else
        {
            item->GetPredicates(pList);
        }
    }
}

string MonitorServer::GetMonitorHeaderInfo()
{
    boost::shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() == 0)
    {
        GetLog()->Warning()
            << "WARNING: (MonitorServer) Monitor System missing.\n";
        return string();
    }

    PredicateList pList;
    boost::mutex::scoped_lock dataLock(mMonitorMutex);
    CollectItemPredicates(true,pList);
    return monitorSystem->GetMonitorHeaderInfo(pList);
}

string MonitorServer::GetMonitorData()
{
    int cycle = mSimulationServer->GetCycle();
    boost::mutex::scoped_lock dataLock(mMonitorMutex);

    if ( cycle == mDataCycle ){
        return mData;
    }

    boost::shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() == 0)
        {
            return string();
        }

    PredicateList pList;
    CollectItemPredicates(false,pList);
    mData = monitorSystem->GetMonitorInformation(pList);
    mDataCycle = cycle;
    return mData;
}

void MonitorServer::ParseMonitorMessage(const string& data)
{
    boost::shared_ptr<MonitorSystem> monitorSystem = GetMonitorSystem();

    if (monitorSystem.get() != 0)
        {
            boost::mutex::scoped_lock dataLock(mMonitorMutex);
            monitorSystem->ParseMonitorMessage(data);
        }
}
