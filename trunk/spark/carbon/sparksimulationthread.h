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

#ifndef SPARKSIMULATIONTHREAD_H
#define SPARKSIMULATIONTHREAD_H
/**
 * \file sparksimulationthread.h
 * \brief Contains SparkSimulationThread class, the base class for threads that contain a SimSpark object and thus a zeitgeist::Core.
 */

#include "simulationthread.h"

class SimSpark;
class LogStream;
class SparkController;

/*!
 \brief Abstract base class for threads containing a SparkController.

 The interaction with SimSpark is done via the contained SparkController. SparkSimulationThread merely maps the calls of the base derived
 Thread class to the Controller.

 \todo When it should ever be possible to completely reset and restart SimSpark as well as running multiple SimSpark instances there could
 be different kinds of predefined spark SimulationTypes (as the the general server ServerThread is one).

 \todo When there is a communication concept with other SparkProcesses or even a way to completely serialize and deserialize a zeitgeist tree
 a general communication interface for SimSpark can be found in the SparkController. By adding a controller to SparkSimulationProcess or deriving
 both SparkSimulationProcess and SparkSimulationThread from one controlling class it would be possible to interact with internal and external
 spark instances in the same way. See SimSpark documentation for more information.
*/
class SparkSimulationThread : public SimulationThread
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Destructor. Deletes the SparkController and SimSpark.
    */
    virtual ~SparkSimulationThread();

    /*!
     \brief Returns the controller.
    */
    boost::shared_ptr<SparkController> getSparkController() const;

protected:
    // protected functions

    /*!
     \brief Constructor for derived classes.

     \param definition definition to store
    */
    SparkSimulationThread(boost::shared_ptr<TaskDefinition> definition);

    /*!
     \brief Initializes spark.

     \param restart if true, the server is restartet if neccessary

     \sa SparkController
    */
    int initSpark(bool restart = false);

    /*!
     \brief Initializes spark logging.

     \sa SparkController
    */
    void initSparkLogging();

    /*!
     \brief Executes spark.

     \sa SparkController
    */
    int runSpark();
    /*!
     \brief Stops spark.

     \sa SparkController
    */
    void stopSpark();
    /*!
     \brief Pauses spark.

     \param pause pause or unpause

     \sa SparkController
    */
    void pauseSpark(bool pause = true);
    /*!
     \brief Resets spark.

     \sa SparkController
    */
    void resetSpark();

public:
    //public virtual functions

    /*!
     \brief Calls resetSpark()

     \sa SparkController
    */
    void reset();

private:
    // private functions

    /*!
     \brief Checks if there is a SparkController and a SimSpark object. Logs an error if not.

     \sa SparkController
    */
    bool assertSpark();

public: signals:
    //public signals

    /*!
     \brief Emitted when spark started running.

     Invoked by the SparkController and used to update the SimulationTasks execution state.

     \param controller pointer to spark controller
     \sa SparkController::sparkRunning()
    */
    void sparkRunning(SparkController* controller);
    /*!
     \brief Emitted when spark finished.

     Invoked by the SparkController and used to update the SimulationTasks execution state.

     \param controller pointer to spark controller
     \sa SparkController::sparkFinished()
    */
    void sparkFinished(SparkController* controller);
    /*!
     \brief Emitted when spark was paused.

     Invoked by the SparkController and used to update the SimulationTasks execution state.

     \param controller pointer to spark controller
     \sa SparkController::sparkPaused()
    */
    void sparkPaused(SparkController* controller);

public slots:	
    //public slots
    
protected:
    // protected members

    boost::shared_ptr<SparkController> mController; /*!< Pointer to spark controller. */

private:
    // private members
};

#endif //SPARKSIMULATIONTHREAD_H
