/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorclientcontrol.h,v 1.1 2004/04/25 17:11:43 rollmark Exp $

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

#include <oxygen/simulationserver/simcontrolnode.h>
#include <monitorparser.h>
#include <commserver.h>
#include <monitorparser.h>

/** \class MonitorClientControl is a SimControlNode that handles the
    communication with the soccer server using the MonitorLib and
    updates the scene graph at the begin of each simulation cycle
 */
class MonitorClientControl : public oxygen::SimControlNode
{
public:
    /** called when a new simulation cycle starts, before the
        simulation is stepped */
    virtual void StartCycle();

protected:
    /** mangles the given expression and construcs the corresponding
        sphere or returns an already existing instance
    */
    boost::shared_ptr<oxygen::Transform>
    GetSphere(MonitorParser::Expression& expr);

    /** constructs a unique string used as the node name for the give
        expression
    */
    bool MangleExpr(MonitorParser::Expression& expr, std::string& name);

    virtual void OnLink();

protected:
    /** game state data */
    GameState mGameState;

    /** game parameter data */
    GameParam mGameParam;

    /** the cached CommServer reference */
    boost::shared_ptr<CommServer> mCommServer;

    /** the cached MonitorParser reference */
    boost::shared_ptr<MonitorParser> mMonitorParser;
};

DECLARE_CLASS(MonitorClientControl);
