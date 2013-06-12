/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#ifndef ISIMCONTROL_H
#define ISIMCONTROL_H
/**
 * \file isimcontrol.h
 * \brief Contains ISimControl class, an interface class for objects that are called by the spark simulation server when simulation events happen.
 */

/*!
 \brief Interface class for objects that are called by the spark simulation server when simulation events happen.

 ISimControl derived objects can be create by plugins or other Gui elements and registered to a SparkControllers
 GuiSimControl node. Whenever a simulation event happens (e.g. startCycle(), endCycle(), or waitCycle()) the respective
 function of all registered ISimControl objects is called.

 This way you can make sure that you can have code be executed by the Simulationserver-thread. If you only need to queue simple reoccurring
 commands that should be executed by the simulation server you can use a command queue derived from AbstractCommandQueue and commands
 derived from AbstractCommand instead.

 \warning Since the code of the derived classes virtual functions is executed by the simulation server, code with a long runtime will
 slow down the simulation. Make sure to only use this system to call functions with low complexity.
*/
class ISimControl
{
    friend class GuiSimControl;

protected:
    //protected constructor

    /*!
     \brief Protected constructor.
    */
    ISimControl();

public:
    //public functions

    /*!
     \brief Destructor.
    */
    virtual ~ISimControl();

protected:
    //protected virtual functions

    /*!
     \brief Called once when the simulation is started.
    */
    virtual void initSimulation() {}
    /*!
     \brief Called once before the simulation is shut down.
    */
    virtual void doneSimulation() {}
    /*!
     \brief Called when a new simulation cycle starts, before the simulation is stepped.
    */
    virtual void startCycle() {}
    /*!
     \brief Called each simulation cycle when agents send sensor information.
    */
    virtual void senseAgent() {}
    /*!
     \brief Called each simulation cycle when carrying out agent actions.
    */
    virtual void actAgent() {}
    /*!
     \brief Called when the current simulation cycle ends.
    */
    virtual void endCycle() {}
    /*!
     \brief Called during a wait-cycle in a paused simulation.
    */
    virtual void waitCycle() {}
};

#endif // ISIMCONTROL_H
