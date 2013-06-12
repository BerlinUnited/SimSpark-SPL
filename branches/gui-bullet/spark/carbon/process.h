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

#ifndef PROCESS_H
#define PROCESS_H
/**
 * \file process.h
 * \brief Contains Process class, which contains tools to start and observe a process.
 */

#include "simulationprocess.h"

/*!
 \brief Simple class utilizing process control and communication tools of the abstract SimulationProcess class.

 Can start, stop and restart the process.
*/
class Process : public SimulationProcess
{
    Q_OBJECT

public:
    // public functions

    /*!
    \brief Constructor.
    The TaskDefinition::ETaskType of the definition has to be TaskDefinition::TT_PROCESS or a SimulationTask::IllegalTaskTypeException is thrown.

    \param definition definition for the SimulationTask
    \throw SimulationTask::IllegalTaskTypeException thrown if the TaskDefinition has a wrong task type
    */
    Process(boost::shared_ptr<TaskDefinition> definition);
    /*!
     \brief Destructor.
    */
    virtual ~Process();

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

#endif //PROCESS_H
