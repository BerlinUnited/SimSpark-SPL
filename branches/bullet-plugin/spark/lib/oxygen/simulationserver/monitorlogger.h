
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
#ifndef OXYGEN_MONITORLOGGER_H
#define OXYGEN_MONITORLOGGER_H

#include "simcontrolnode.h"
#include <oxygen/oxygen_defines.h>
#include <fstream>

namespace oxygen
{
class MonitorServer;

/** \class MonitorLogger is a SimControlNode node that logs
    the sent messages to monitors in cooperation with the
    MonitorServer.
*/
class OXYGEN_API MonitorLogger : public SimControlNode
{
public:
    MonitorLogger();
    virtual ~MonitorLogger();

    /** logs the scene at the end of each simulation cycle */
    virtual void EndCycle();

    /** returns the monitor logger update interval in cycles */
    int GetMonitorLoggerInterval();

    /** sets the monitor logger update interval in cycles */
    void SetMonitorLoggerInterval(int i);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

protected:
    /** cached reference to the MonitorServer */
    boost::shared_ptr<MonitorServer> mMonitorServer;

    /** ofstream object to log monitor messages */
    std::ofstream mLogFile;

    /** number of full state logged */
    int mFullStateLogged;

    /** the time of the last full state logging */
    float mFullStateLoggedTime;
};

DECLARE_CLASS(MonitorLogger);

} // namespace oxygen

#endif // OXYGEN_MONITORLOGGER_H
