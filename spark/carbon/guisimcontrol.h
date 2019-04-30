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

#ifndef GUISIMCONTROL_H
#define GUISIMCONTROL_H
/**
 * \file guisimcontrol.h
 * \brief Contains GuiSimControl class, an oxygen SimulationControlNode for the Gui.
 */

#include <oxygen/simulationserver/simcontrolnode.h>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QList>

class ISimControl;

/*!
 \brief oxygen SimulationControlNode for the Gui. Coordinates execution of Gui functions by the Simulation threads.

 A SparkController creates a single GuiSimControl node and registers it into the node tree.
 So the Gui has a single node entry in the zeitgeist node tree of a simulation.

 Plugins and other sources can create ISimControl objects and add them to the GuiSimControls node list.
 The GuiSimControl node is called whenever a simulation cycle
 or wait cycle is executed (when a Simulationcontrol event happens). Then it calls the respective functions of registered
 ISimControl objects.

 This way the SimulationThread executes the code defined in the virtual functions of the ISimControl derived class.
*/
class GuiSimControl : public oxygen::SimControlNode
{
public:
    //public functions

    /*!
     \brief Default constructor.
    */
    GuiSimControl();
    /*!
     \brief Destructor.
    */
    virtual ~GuiSimControl();

    /*!
     \brief Registers an ISimControl derived object.

     The virtual functions of the object will be called every time a simulation event occurs.

     Every ISimControl node registered with this function has to be unregistered with unregisterSimControl(), or
     it will remain in the GuiSimControl list and never be deleted as long as the SparkController exists.
     \param simControl
    */
    void registerSimControl(boost::shared_ptr<ISimControl> simControl);
    /*!
     \brief Unregisters a registered ISimControl derived object.

     \param simControl pointer to object to remove
    */
    void unregisterSimControl(boost::shared_ptr<ISimControl> simControl);
    /*!
     \brief Unregisters a registered ISimControl derived object.

     \param simControl pointer of object to remove
    */
    void unregisterSimControl(ISimControl* simControl);

protected:
    //protected functions

    /*!
     \brief Called when GuiSimControl is linked into the zeitgeist tree.
    */
    virtual void OnLink();

private:
    //private functions

    /*!
     \brief Called once when the simulation is started.
    */
    virtual void InitSimulation();

    /*!
     \brief Called once before the simulation is shut down.
    */
    virtual void DoneSimulation();

    /*!
     \brief Called when a new simulation cycle starts, before the simulation is stepped.
    */
    virtual void StartCycle();

    /*!
     \brief Called each simulation cycle when agents send sensor information.
    */
    virtual void SenseAgent();

    /*!
     \brief Called each simulation cycle when carrying out agent actions.
    */
    virtual void ActAgent();

    /*!
     \brief Called when the current simulation cycle ends.
    */
    virtual void EndCycle();

    /*!
     \brief Called during a wait-cycle in a paused simulation.
    */
    virtual void WaitCycle();

private:
    //private members

    QList<boost::shared_ptr<ISimControl>> mSimControlNodes; /*!< List of registered ISimControl objects. */

};

DECLARE_CLASS(GuiSimControl); /*!< zeitgeist class declaration */

#endif // GUISIMCONTROL_H
