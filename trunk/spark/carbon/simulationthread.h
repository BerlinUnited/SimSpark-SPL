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

#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H
/**
 * \file simulationthread.h
 * \brief Contains SimulationThread class, the base class for all threads managed by the SimulationManager.
 */

#include "carbon.h"
#include "simulationtask.h"
#include "taskdefinition.h"

#include <QThread>

/*!
 \brief Abstract base class for all threads managed by the SimulationManager.

 Simply combines QThread and SimulationTask to a single class.
*/
class SimulationThread : public QThread, public SimulationTask
{
    Q_OBJECT

public:
    // public functions
    /*!
     \brief Destructor.
    */
    virtual ~SimulationThread();
       
   /*!
    \brief Quits the execution loop (if running) and shuts down the server.

    Running simspark will not be stopped. This function just quits exec() and prevents its execution.
   */
   virtual void shutdown();  

protected:
    // protected functions
    /*!
     \brief Constructor for base classes.

     \param definition definition to store
    */
    SimulationThread(boost::shared_ptr<TaskDefinition> definition);  

protected:
    // protected members

    bool mShutdown; /*!< True if thread is requested to quit. Read this out before starting exec() loop. */
};

#endif //SIMULATIONTHREAD_H
