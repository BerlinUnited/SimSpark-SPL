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
#ifndef OXYGEN_MONITORSERVER_H
#define OXYGEN_MONITORSERVER_H

#ifndef Q_MOC_RUN
#include <boost/thread/mutex.hpp>
#endif
#include <oxygen/oxygen_defines.h>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "monitorsystem.h"

namespace oxygen
{
class SimulationServer;

class OXYGEN_API MonitorServer : public zeitgeist::Node
{
public:
    MonitorServer();
    virtual ~MonitorServer();

    // one note about std::string. This class is capable of handling
    // binary data (i.e. not NULL terminated strings). Use the
    // std::string(basic_string(const charT* s, size_type n)
    // constructor to construct a string containing binary data and
    // the const charT* data() member to access binary data.

    /** This function creates an instance of class 'monitorSysName'
     *  and adds it as a child node below this server
     */
    bool RegisterMonitorSystem(const std::string& monitorSysName);

    /** This function creates an instance of class 'monitorItem' and
        adds it as a child node below this server
     */
    bool RegisterMonitorItem(const std::string& monitorItemName);

    /** the following set of functions if called by the SpadesServer */

    /** This function is called once for every monitor. It should
     *  return any header/setup information that is needed.
     */
    std::string GetMonitorHeaderInfo();

    /** This function will be called periodically to get information
     * about the current state of the world. The format is completely
     * determined by what the monitors will expect; no processing is
     * done by the simulation engine
     */
    std::string GetMonitorData();

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     */
    void ParseMonitorMessage(const std::string& data);

protected:
    /** returns a boost::shared_ptr to the first registered MonitorSystem */
    boost::shared_ptr<MonitorSystem> GetMonitorSystem();

    /** collects a list of predicates from all registered MonitorItems */
    void CollectItemPredicates(bool initial, PredicateList& pList);

    virtual void OnLink();

private:
    /** a cached reference to the SimulationServer */
    CachedPath<SimulationServer> mSimulationServer;

    /** a cached data about current state */
    std::string mData;

    /** the cycle of cacahed data */
    int mDataCycle;

    /** a mutex to protect monitor related internal data */
    boost::mutex mMonitorMutex;
};

DECLARE_CLASS(MonitorServer);

} // namespace oxygen

#endif // OXYGEN_MONITORSERVER_H
