/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorserver.h,v 1.1.2.2 2003/11/19 18:37:25 rollmark Exp $

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

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>

namespace oxygen
{

class MonitorServer : public zeitgeist::Node
{
public:
    MonitorServer();
    virtual ~MonitorServer();

    /** This function creates an instance of class 'monitorSysName'
     *  and adds it as a child node below this server
     */
    bool RegisterMonitorSystem(const std::string& monitorSysName);

#if 0
    /** This function is called once for every monitor. It should return any
     *  header/setup information that is needed.
     */
    spades::DataArray getMonitorHeaderInfo();
    /* This function will be called periodically to get information about the
     * current state of the world. The format is completely determined by what
     * the monitors will expect; no processing is done by the simulation engine
     */
    spades::DataArray getMonitorInfo(spades::SimTime time);
    /* If a monitor sends information to the world model, this function is
     * called to process it. Note that only the data section of the message
     * (not the ID part which indicates that it is a message for the world
     * model and not the simulation engine) is included here. If you need to
     * keep the data, you must copy it */
    void parseMonitorMessage(const char* data, unsigned datalen);
#endif

private:
};

DECLARE_CLASS(MonitorServer);

} // namespace oxygen

#endif // OXYGEN_MONITORSERVER_H
