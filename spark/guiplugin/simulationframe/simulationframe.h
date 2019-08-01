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

#ifndef SIMULATIONFRAME_H
#define SIMULATIONFRAME_H
/**
 * \file simulationframe.h
 * \brief Contains SimulationFrame class.
 */

#include <attachableframe.h>
#include <simulationtask.h>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <vector>

#include "ui_simulationframe.h"

class SimulationManager;
class Settings;
class SimSpark;
class SparkSimulationTask;

namespace SimulationFrameUtil
{
    class TaskWidget;
}

/*!
 \brief The SimulationFrame is a container for SimulationFrameUtil::TaskWidget objects, representing a SimulationTask of the current Simulation.

 The SimulationFrame is notified about creation, deletion and other changes in the the task definition list of the current SimulationSetup in the
 currently initialized Simulation and creates, removes or updates task widgets automatically.
 The SimulationFrame shows the state of the current Simulation and can create new SimulationTasks of a any type.
*/
class SimulationFrame : public AttachableFrame
{
    Q_OBJECT
    GUI_FRAME

public:
    // public functions
    /*!
     \brief Constructor.
    */
    SimulationFrame();
    /*!
     \brief Destructor.
    */
    virtual ~SimulationFrame();

    /*!
     \brief Loads settings from the QSettings object.
    */
    void loadSettings();
    /*!
     \brief Loads default settings from the Ui class.
    */
    void loadDefaultSettings();
    /*!
     \brief Saves current settings to the QSettings object.
    */
    void saveSettings();

private:
    //private functions

    /*!
     \brief Initialize non-changing fields.
    */
    void init();
    /*!
     \brief Removes all task widgets.
    */
    void clearTaskWidgets();

public slots:
    //public slots

    /*!
     \brief Updates the header area.
    */
    void updateHeader();

    /*!
     \brief Chooses a task type by index.

     \param index type to select
    */
    void chooseTaskType(int index);
    /*!
     \brief Adds a new task of the currently selected task type.
    */
    void addTask();
    
    /*!
     \brief Executes the Simulation and all tasks.
    */
    void runTasks();
    /*!
     \brief Pauses all tasks.
    */
    void pauseTasks();
    /*!
     \brief Stops all tasks.
    */
    void stopTasks();
    /*!
     \brief Restarts all tasks.
    */
    void restartTasks();
    /*!
     \brief Hides the details section of all TaskWidgets.
    */
    void collapseTasks();
    /*!
     \brief Shows the details section of all TaskWidgets.
    */
    void expandTasks();

    /*!
     \brief Updates the complete task list.

     Only called after initialization of a Simulation.
    */
    void updateTaskList();
    /*!
     \brief Updates the whole display of a single task widget.

     Called when a task definition of the initialized Simulation changed.

     \param index index in SimulationSetups list
     \param id unique id of the task
    */
    void updateTaskDisplay(int index, int id);
    /*!
     \brief Updates the execution state display of a single task widget.

     \param index index in SimulationSetups list
     \param id unique id of the task
     \param state state of the task
    */
    void updateTaskDisplay(int index, int id, SimulationTask::ETaskExecutionState state);
    /*!
     \brief Adds a new TaskWidget to the list.

     Called by the Simulation when a SimulationTask was added.

     \param index index of the new task
     \param id unique id of the new task
    */
    void updateTaskAdded(int index, int id);
    /*!
     \brief Removes a TaskWidget from the list.

     Called by the Simulation when a task was removed.

     \param index former index in SimulationSetups task list
     \param id unique id of the task
    */
    void updateTaskRemoved(int index, int id);

    /*!
     \brief Chooses a task widget from the list.

     Called by the widget itself if it is clicked.

     \param widget widget to choose
    */
    void chooseWidget(SimulationFrameUtil::TaskWidget* widget);

    /*!
     \brief Prepares the SimulationFrame for deletion.

     Called by the Carbon::aboutToShutdown()
    */
    void cleanup();

private:
    // private members

    Ui::SimulationFrame ui; /*!< UI definition. */
    boost::shared_ptr<Settings> mSettings; /*!< Pointer to global settings. */
    SimulationManager* mSimulationManager; /*!< Poitner to manager for quick access. */
    bool mClosed; /*!< True after shutdown(). */
    
    std::vector<SimulationFrameUtil::TaskWidget*> mTaskWidgets; /*!< List of task widgets. */
    int mChosenTask; /*!< Index of chosen task. */
};

#endif // SIMULATIONFRAME_H
