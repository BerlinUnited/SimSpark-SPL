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

#ifndef SPARKPROCESS_H
#define SPARKPROCESS_H
/**
 * \file sparkprocess.h
 * \brief Contains SparkProcess class, which contains tools to communicate with an external spark process.
 */

#include "simulationprocess.h"

/*!
 \brief Simple class utilizing process control and communication tools of the abstract SimulationProcess class to communicate with a spark process.

 Can start, stop and restart the process.

 \todo At the moment the SparkProcess behaves exactly as a normal Process.
 Utilizing the communication Protocol of Spark it would possible to communicate with an external spark process. With an additional
 communication plugin for Spark servers it would even be possible to extend the control functions. A combined control interface to control
 Spark (as given with the SparkController) could be utilized by both gui-internal SparkSimulationThreads and the SparkSimulationProcess class to
 interact with internal and external spark processes in exactly the same way. This way Simulations could be develeped and debugged in one process and
 executed an a different PC, or they could be debugged and observed from another computer (or many computers).
*/
class SparkProcess : public SimulationProcess
{

public:
    // public functions

    /*!
    \brief Constructor.
    The TaskDefinition::ETaskType of the definition has to be TaskDefinition::TT_SPARKPROCESS or a SimulationTask::IllegalTaskTypeException is thrown.

    \param definition definition for the SimulationTask
    \throw SimulationTask::IllegalTaskTypeException thrown if the TaskDefinition has a wrong task type
    */
    SparkProcess(boost::shared_ptr<TaskDefinition> definition);
    /*!
     \brief Destructor.
    */
    virtual ~SparkProcess();

public:
   // virtual public functions

   /*!
    \brief Initialize and start the process.

    Implementation of SimulationTask interface.
   */
   virtual void run();
   /*!
    \brief Stop the process.

    Implementation of SimulationTask interface.
   */
   virtual void stop();
   /*!
    \brief Pausing the process.

    \todo Pausing the process not possible by default. See SimulationProcess::pauseProcess().

    Implementation of SimulationTask interface.

    \param pause wished pause state
   */
   virtual void pause(bool pause = true);
   /*!
     \brief Stops and restarts the process.

     Implementation of SimulationTask interface.

     Reuses the same executable path and the same argument list used for the last call, even if the TaskDefinition changed, but emits a warning in this case.
   */
   virtual void restart();

};

#endif //SPARKPROCESS_H
