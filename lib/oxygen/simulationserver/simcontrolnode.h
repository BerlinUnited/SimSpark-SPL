/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simcontrolnode.h,v 1.2 2004/12/22 15:54:42 rollmark Exp $

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
#ifndef OXYGEN_SIMCONTROLNODE_H
#define OXYGEN_SIMCONTROLNODE_H

#include <zeitgeist/node.h>

namespace oxygen
{
class SimulationServer;

/** \class SimControlNode defines an interface for classes that are
    registered to the SimulationServer. The interface defines
    callbacks for the initial startup of the simulation, it's final
    shutdown and for different phases of each simulation
    cycle. SimControlNodes are used extend and customize the
    simulation runloop and to delegate tasks, like network management,
    reading of mouse and keyboard input, writing a simulation log,
    rendering etc.
 */
class SimControlNode : public zeitgeist::Node
{
public:
    SimControlNode();
    virtual ~SimControlNode();

    /** called once when the simulation is started */
    virtual void InitSimulation() {};

    /** called once before the simulation is shut down */
    virtual void DoneSimulation() {};

    /** called when a new simulation cycle starts, before the
        simulation is stepped */
    virtual void StartCycle() {};

    /** called each simulation cycle to send agent sensor
        information */
    virtual void SenseAgent() {};

    /** called each simulation cycle to carry out agent actions */
    virtual void ActAgent() {};

    /** called when the current simulation cycle ends */
    virtual void EndCycle() {};

protected:
    /** returns a reference to the SimulationServer */
    boost::shared_ptr<SimulationServer> GetSimulationServer();
};

DECLARE_CLASS(SimControlNode);

} // namespace oxygen

#endif // OXYGEN_SIMCONTROLNODE_H





