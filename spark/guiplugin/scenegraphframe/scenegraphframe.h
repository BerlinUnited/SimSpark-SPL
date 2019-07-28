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

#ifndef SCENEGRAPHFRAME_H
#define SCENEGRAPHFRAME_H
/**
 * \file scenegraphframe.h
 * \brief Contains SceneGraphFrame class.
 */

#include <attachableframe.h>
#include <simulationtask.h>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include <vector>
#include <QTimer>

#include "ui_scenegraphframe.h"

namespace SceneGraphFrameUtil
{
    class SceneGraphWidget;
    class PropertyFrame;
}

namespace SparkProperty
{
    class PropertyWidget;
}

class SimulationManager;
class SimulationTask;
class SparkSimulationThread;
class SparkController;

/*!
 \brief AttachableFrame plugin that allows displaying scene graphs of SimulationTasks like the spark ServerThread.

 The frame lists all spark servers of the current Simulation. For every server there exists one SceneGraphFrameUtil::SceneGraphWidget in a QStackedWidget.
 When selected, the SceneGraphWidget displays the scene graph of the Task.
 Additionally, there is a PropertyWidget, which displays properties of nodes selected in the scene graph.\n

 \note
 At the moment there can only be one SimSpark object so there will only be one scene graph. However this can easily extended by for example an
 observer for the scene graph of an external SparkProcess. Additional types of SceneGraphFrameUtil::SceneGraphWidget could improve the frame to
 display more than one source of information.

 \warning
 Calling any slots from the outside has to be done with great care. Not all of the functions are safe to be used as standalone and in worst case
 might cause the frame to reach an illegal state.

 Generally, all the update-slots are always safe to use as they only update the view from the current setup. Changing a setup via the SimulationManager
 if the current Simulation automatically updates the SceneGraphFrame.
*/
class SceneGraphFrame : public AttachableFrame
{
    Q_OBJECT
    GUI_FRAME

    friend class SceneGraphFrameUtil::PropertyFrame;
    friend class SceneGraphFrameUtil::SceneGraphWidget;

public:
    // public functions

    /*!
     \brief Constructor.
    */
    SceneGraphFrame();
    /*!
     \brief Destructor.
    */
    virtual ~SceneGraphFrame();

    /*!
     \brief Returns the property widget.
    */
    SparkProperty::PropertyWidget* getPropertyWidget();

protected:
    //protected functions

    /*!
     \brief Returns the property frame.

     Used by the SceneGraphWidget.
    */
    SceneGraphFrameUtil::PropertyFrame* getPropertyFrame();

private:
    //private functions

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

    /*!
     \brief Initializes the list of displayed tasks.

     Called in the constructor.
    */
    void initTaskList();

    /*!
     \brief Returns the SimSpark controller of the given SimulationTask.

     \param task task to check
     \return returns pointer to controller or 0-pointer if task has no SparkController
    */
    boost::shared_ptr<SparkController> getSparkController(boost::shared_ptr<SimulationTask> task);
    /*!
     \brief Returns pointer to the currently selected task or 0-pointer if nothing is selected.
    */
    boost::shared_ptr<SimulationTask> getCurrentTask();
    /*!
     \brief Begins observing a SparkSimulationThread by connecting to its signals.

     Used to be notified when spark started completely, so the SceneGraphFrameUtil::SceneGraphWidget can be updated.

     \param thread thread containing the SparkController to observe
    */
    void observeSparkSimulationThread(boost::shared_ptr<SparkSimulationThread> thread);
    /*!
     \brief Adds a new SceneGraphFrameUtil::SceneGraphWidget at the given index.

     \param localindex index to add the widget to
    */
    void addSceneGraphWidget(int localindex);

    /*!
     \brief Checks is the given task has a scene graph and adds it at the given index.

     If the task has no scene graph, it is not added.
     Updates task combo box, the widget list and the task reference list.

     \param task task to check and add
     \param localIndex index to add it to
     \return true if it was added, false otherwise
    */
    bool checkAndInsertTask(boost::shared_ptr<SimulationTask> task, int localIndex);

    /*!
     \brief Returns the local index of a task with the given id, or 0 if the task is not in the list.
    */
    int getLocalIndex(int id);

public slots:
    //public slots

    /*!
     \brief Pick a leaf in the scene graph specified by its absolute path in the zeitgeist tree.

     \param path path of the leaf
    */
    void pickLeaf(QString path);

    /*!
     \brief Selects the task at the given local index.

     \param localindex index in mTasks
    */
    void chooseTask(int localindex);

    /*!
     \brief Updates the currently selected SceneGraphWidget.
    */
    void updateCurrent();
    /*!
     \brief Updates the ProeprtyFrame.
    */
    void updatePropertyFrame();
    /*!
     \brief Updates the TaskList.
    */
    void updateTaskList();
    /*!
     \brief Update the lists when a new task was added.

     Called by SimulationManager.

     \param index index of new task
     \param id id of new task
    */
    void updateTaskAdded(int index, int id);
    /*!
     \brief Update the lists when a task was removed.

     Called by SimulationManager.

     \param index former index of deleted task
     \param id id of deleted task
    */
    void updateTaskRemoved(int index, int id);
    /*!
     \brief Update the lists when a tasks execution state changed.

     Called by SimulationManager.

     \param index index of new task
     \param id id of new task
     \param state new state of the task
    */
    void updateTask(int index, int id, SimulationTask::ETaskExecutionState state);
    /*!
     \brief Updates the current view if SimSpark of the currently selected Task started running.

     \param spark controller that started SimSpark
    */
    void updateSparkRunning(SparkController* spark);
    /*!
     \brief Updates the current view if SimSpark of the currently selected Task finished.

     \param spark controller whise SimSpark finished
    */
    void updateSparkFinished(SparkController* spark);

    /*!
     \brief Updates the model data of the SceneGraphWidget and the PropertyFrame.
    */
    void updateData();
    /*!
     \brief Collapses all items of the SceneGraphWidgets SparkTreeView.
    */
    void collapseAll();
    /*!
     \brief Expands all items of the SceneGraphWidgets SparkTreeView.
    */
    void expandAll();

    /*!
     \brief Changes the timer state.

     \param state true activates periodical update, false deactivates update
    */
    void toggleTimer(bool state);
    /*!
     \brief Called when the timer ticks. Updates display.
    */
    void timerTick();

    /*!
     \brief Clears lists and prepares the frame for deletion.
    */
    void cleanup();

protected:
    // protected members

    QString mLastSceneFileDirectory;   /*!< Path of the last scene file loaded. */
    QStringList mSceneFileNameFilters; /*!< List of file name filters for scene files. (e.g. "Rsg-Files (*.rsg)")*/

private:
    // private members

    Ui::SceneGraphFrame ui;                /*!< UI definition. */
    SimulationManager* mSimulationManager; /*!< Reference to simulation manager. */
    boost::shared_ptr<Settings> mSettings; /*!< Pointer to global settings object. */
    SceneGraphFrameUtil::PropertyFrame* mPropertyFrame; /*!< Pointer to property frame gui element. */
    
    bool mClosed; /*!< True after cleanup() was called. */

    std::vector<std::pair<int, boost::shared_ptr<SimulationTask> > > mTasks; /*!< List of all tasks shown with their id and a pointer to the task. */
    std::vector<SceneGraphFrameUtil::SceneGraphWidget*> mSceneGraphWidgets;  /*!< List of scene graph widgets showing the tasks scene graphs. */
    int mCurrentTask;             /*!< Index of selected task in mTasks. */
    QString mCurrentTaskName;     /*!< Name of currently selected task. */
    bool mAutomaticUpdate;        /*!< Automatic update of all scene graph frames. */
    int mAutomaticUpdateInterval; /*!< Interval for automatic update. */
    QTimer mUpdateTimer;          /*!< Timer triggering the automatic update. */
};

#endif // SCENEGRAPHFRAME_H
