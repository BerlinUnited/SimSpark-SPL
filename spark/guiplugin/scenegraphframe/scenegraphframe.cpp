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

#include "scenegraphframe.h"
#include "scenegraphframe_scenegraphwidget.h"
#include "export.h"

#include <carbon.h>
#include <pluginmanager.h>
#include <simulationmanager.h>
#include <communicationmanager.h>
#include <simulationtask.h>
#include <sparksimulationthread.h>
#include <taskdefinition.h>
#include <settings.h>
#include <simulation.h>
#include <sparkcontroller.h>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(SceneGraphFrame, "scenegraphframe", "SceneGraph", 1)

//Static class information
void SceneGraphFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION, 
        tr("The scene graph frame displays and grants access to the scene graph of a simulation task.\n" \
           "It contains a header area, where you can select the task to show the scene graph from, a scene graph area, " \
           "where you can explore the single nodes of the graph, and a property area, where you can inspect and change properties " \
           "of selected nodes in the scene graph."));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":find");

    //Tags
    QStringList tags;
    tags << "scenegraph" << "properties" << "task";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SceneGraphFrame::SceneGraphFrame() :
    mSettings(getCarbon()->getSettings())
{
    ui.setupUi(this);
    loadFrameDesign();
    mClosed = false;
    mSimulationManager = getCarbon()->getSimulationManager();
    mCurrentTask = -1;
    mPropertyFrame = ui.propertyFrame;
    mPropertyFrame->init(this);

    //-------------------------
    //Load Settings
    
    //Default Settings
    loadDefaultSettings();

    //Saved Settings
    loadSettings();

    //-------------------------
    //Init list of threads
    initTaskList();
    if (ui.taskComboBox->count() > 0)
    {
        chooseTask(0);
    }

    //-------------------------
    //Connect signals
    Simulation* simulation = &*mSimulationManager->getSimulation();

    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(timerTick())); 

    connect(simulation, SIGNAL(taskAdded(int, int)), this, SLOT(updateTaskAdded(int, int)));
    connect(simulation, SIGNAL(taskRemoved(int, int)), this, SLOT(updateTaskRemoved(int, int)));
    connect(simulation, SIGNAL(taskStateChanged(int, int, SimulationTask::ETaskExecutionState)), this, SLOT(updateTask(int, int, SimulationTask::ETaskExecutionState)));

    connect(getCarbon(), SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));

    //-------------------------
    //Communication
    CommunicationManager* commManager = getCarbon()->getCommunicationManager();
    commManager->registerReceiver(this, "Server_LeafPicks", SLOT(pickLeaf(QString)));
}

SceneGraphFrame::~SceneGraphFrame()
{
    cleanup();

    mSceneGraphWidgets.clear();
}

SparkProperty::PropertyWidget* SceneGraphFrame::getPropertyWidget()
{
    return mPropertyFrame->getPropertyWidget();
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

SceneGraphFrameUtil::PropertyFrame* SceneGraphFrame::getPropertyFrame()
{
    return mPropertyFrame;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void SceneGraphFrame::loadSettings()
{
    if (mSettings->beginGroup(this, true)) //lock
    {
        mAutomaticUpdate         = mSettings->value("mAutomaticUpdate",         mAutomaticUpdate).toBool();
        mAutomaticUpdateInterval = mSettings->value("mAutomaticUpdateInterval", mAutomaticUpdateInterval).toInt();
        mLastSceneFileDirectory  = mSettings->value("mLastSceneFileDirectory",  mLastSceneFileDirectory).toString();
        mSceneFileNameFilters    = mSettings->value("mSceneFileNameFilters",    mSceneFileNameFilters).toStringList();
    
        mSettings->endGroup(true); //unlock

        ui.updateCheckBox->setChecked(mAutomaticUpdate);
        toggleTimer(mAutomaticUpdate);
    }
    else
        LOG_ERROR() << "Could not access Settings object to load Settings for SceneGraphFrame " << getCaption();
}

void SceneGraphFrame::loadDefaultSettings()
{
    mAutomaticUpdate = false;
    mAutomaticUpdateInterval = 1000;
    mLastSceneFileDirectory  = "";
    mSceneFileNameFilters.clear();
    mSceneFileNameFilters << "Rsg-Files (*.rsg)";

    ui.updateCheckBox->setChecked(mAutomaticUpdate);
    toggleTimer(mAutomaticUpdate);
}

void SceneGraphFrame::saveSettings()
{
    if (mSettings->beginGroup(this, true)) //lock
    {
        mSettings->setValue("mAutomaticUpdate",         mAutomaticUpdate);
        mSettings->setValue("mAutomaticUpdateInterval", mAutomaticUpdateInterval);
        mSettings->setValue("mLastSceneFileDirectory",  mLastSceneFileDirectory);
        mSettings->setValue("mSceneFileNameFilters",    mSceneFileNameFilters);
    
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save Settings for SceneGraphFrame " << getCaption();
}

void SceneGraphFrame::initTaskList()
{
    ui.taskComboBox->clear();
    mCurrentTask = -1;

    int index = 0; //position in simualtions task list
    int localIndex = 0; //position in displayed list
    for (auto it = mSimulationManager->getSimulation()->getTaskList().begin(); it != mSimulationManager->getSimulation()->getTaskList().end(); it++)
    {
        boost::shared_ptr<SimulationTask> task(*it);

        bool inserted = checkAndInsertTask(task, localIndex);

        if (inserted)
            localIndex++;

        index++;
    }

    //Delete other stackedwidget pages
    while (localIndex < ui.stackedWidget->count())
    {
        ui.stackedWidget->removeWidget(ui.stackedWidget->widget(ui.stackedWidget->count()-1));
    }
}

void SceneGraphFrame::observeSparkSimulationThread(boost::shared_ptr<SparkSimulationThread> thread)
{
    if (thread->getTaskDefinition().isSpark())
    {
        connect(boost::static_pointer_cast<SparkSimulationThread>(thread).get(), SIGNAL(sparkRunning(SparkController*)), this, SLOT(updateSparkRunning(SparkController*)));
        connect(boost::static_pointer_cast<SparkSimulationThread>(thread).get(), SIGNAL(sparkFinished(SparkController*)), this, SLOT(updateSparkFinished(SparkController*)));
    }
}

boost::shared_ptr<SparkController> SceneGraphFrame::getSparkController(boost::shared_ptr<SimulationTask> task)
{
    //Stacked Widget content
    if (task->getTaskDefinition().isSpark() && task->getTaskDefinition().isThread())
    {
        //Has scene graph
        return boost::static_pointer_cast<SparkSimulationThread>(task)->getSparkController();
    }
    else
    {
        //Has no scene graph
        return boost::shared_ptr<SparkController>();
    }
}

boost::shared_ptr<SimulationTask> SceneGraphFrame::getCurrentTask()
{
    if (mCurrentTask < 0 || mCurrentTask >= (int)mTasks.size())
        return boost::shared_ptr<SimulationTask>();

    int currentId = mTasks.at(mCurrentTask).first;
    int globalIndex = mSimulationManager->getSimulation()->getTaskIndex(currentId);

    if (globalIndex < 0 || (int)mSimulationManager->getSimulation()->getTaskList().size() <= globalIndex)
    {
        LOG_ERROR() << "Task with id " << currentId << " could not be found in Simulations task list";
    }

    return mSimulationManager->getSimulation()->getTaskList().at(globalIndex);
}

void SceneGraphFrame::addSceneGraphWidget(int localindex)
{
    if (localindex < 0 || localindex >= (int)mTasks.size())
    {
        LOG_ERROR() << "Index out of bounds " << localindex;
        return;
    }

    boost::shared_ptr<SimulationTask> task(mTasks.at(localindex).second);

    //We can only create scene graph widgets for tasks with scene graph
    if (task->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD)
    {
        boost::shared_ptr<SparkSimulationThread> thread(boost::static_pointer_cast<SparkSimulationThread>(task));

        //Create widget
        SceneGraphFrameUtil::SceneGraphWidget* newWidget = new SceneGraphFrameUtil::SceneGraphWidget(thread, this);

        //Insert widget
        ui.stackedWidget->insertWidget(localindex, newWidget);
        observeSparkSimulationThread(thread);
        mSceneGraphWidgets.insert(mSceneGraphWidgets.begin() + localindex, newWidget);
    }
    else
    {
        LOG_ERROR() << "Cant create SceneGraphWidget with a task to observe that is not a server thread.";
    }
}

bool SceneGraphFrame::checkAndInsertTask(boost::shared_ptr<SimulationTask> task, int localIndex)
{
    //Which tasks to display in list? Only include those with scene graph
    if (task->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD)
    {
        //Servers
        mTasks.push_back(std::make_pair(task->getId(), task));

        //ComboBox Entry
        ui.taskComboBox->addItem(task->getTaskDefinition().getName());
        updateTask(localIndex, task->getId(), task->getExecutionState());

        //Stacked widget page
        addSceneGraphWidget(localIndex);

        return true;
    }

    return false;
}

int SceneGraphFrame::getLocalIndex(int id)
{
    int pos = 0;
    for (auto it = mTasks.begin(); it != mTasks.end(); it++)
    {
        if (it->first == id)
            return pos;

        pos++;
    }

    return -1;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SceneGraphFrame::chooseTask(int index)
{
    if (index < 0 || index > ui.taskComboBox->count())
    {
        mCurrentTask= -1;
        mCurrentTaskName = "";
    }
    else
    {
        mCurrentTask = index;
        mCurrentTaskName = ui.taskComboBox->itemText(index);
        ui.stackedWidget->setCurrentIndex(index);
    }
}

void SceneGraphFrame::updateTaskList()
{
    //Remember choice
    int chosen = mCurrentTask;

    //Refill list
    initTaskList();

    //Rechoose
    int index = ui.taskComboBox->findText(mCurrentTaskName);

    chooseTask(index);
}

void SceneGraphFrame::updateTaskAdded(int index, int id)
{
    //Remember choice
    int chosen = mCurrentTask;

    //Add entry to list
    boost::shared_ptr<SimulationTask> task(mSimulationManager->getSimulation()->getTaskList().at(index));

    //Get index of previous task in the local list
    int targetindex = 0;
    bool search = true;
    while (search)
    {
        if (targetindex >= (int)mTasks.size())
        {
            //Reached end of list, insert here
            search = false;
            break;
        }

        //index of the element still smaller than global index?
        int searchedindex = mSimulationManager->getSimulation()->getTaskIndex(mTasks.at(targetindex).first); //get index if task with the searched id
        if (searchedindex < index)
        {
            //Yes, search next
            targetindex++;
        }
        else
        {
            //No, we found position to insert to (next position with index bigger than what we searched for)
            targetindex++;
            search = false;
        }
    }

    //Insert one position after index of the previous found task
    bool inserted = checkAndInsertTask(task, targetindex);

    if (inserted)
    {
        //Choose
        if (chosen == -1)
            chooseTask(targetindex);
    }
}

void SceneGraphFrame::updateTaskRemoved(int index, int id)
{   
    //Remember choice
    int chosen = mCurrentTask;

    //Remove entries from list
    int localindex = -1;
    bool removed = false;
    for (int i = 0; i < (int)mTasks.size(); i++)
    {
        localindex++;
        if (mTasks.at(i).first == id)
        {
            //found entry for this global id, remove
            mTasks.erase(mTasks.begin() + i);
            removed = true;
            break;
        }
    }

    if (!removed)
        return;

    //ComboBox Entry
    ui.taskComboBox->removeItem(localindex);

    //Stacked Widget content
    ui.stackedWidget->removeWidget(ui.stackedWidget->widget(localindex));
    if (localindex < 0 || localindex >= (int)mSceneGraphWidgets.size())
    {
        LOG_ERROR() << "SceneGraphWidget index out of range.";
        return;
    }
    mSceneGraphWidgets.erase(mSceneGraphWidgets.begin() + localindex);

    //Choose
    if (chosen >= localindex)
    {
        //need to lower chosen index
        if (chosen != 0 || ui.stackedWidget->count() == 0)
            //only lower from 0 to -1 if the list is empty
            chooseTask(localindex-1);
        else
            chooseTask(localindex);
    }
}

void SceneGraphFrame::updateTask(int index, int id, SimulationTask::ETaskExecutionState state)
{
    int localindex = getLocalIndex(id);

    if (localindex == -1)
    {
        //Task not in the list
        return;
    }

    //Update Icon in the task list
    switch(state)
    {
    case SimulationTask::TES_INITIALIZED:  ui.taskComboBox->setItemIcon(localindex, QIcon(":accept")); break;
    case SimulationTask::TES_RUNNING:      ui.taskComboBox->setItemIcon(localindex, QIcon(":play_green")); break;
    case SimulationTask::TES_PAUSED:       ui.taskComboBox->setItemIcon(localindex, QIcon(":pause_green")); break;
    case SimulationTask::TES_STOPPED:      ui.taskComboBox->setItemIcon(localindex, QIcon(":stop_green")); break;
    case SimulationTask::TES_FINISHED:     ui.taskComboBox->setItemIcon(localindex, QIcon(":accept")); break;
    case SimulationTask::TES_TERMINATED:   ui.taskComboBox->setItemIcon(localindex, QIcon(":cross")); break;
    case SimulationTask::TES_UNDEFINED:    ui.taskComboBox->setItemIcon(localindex, QIcon(":help")); break;
    default:  
        ui.taskComboBox->setItemIcon(localindex, QIcon("")); break;
    }
}

void SceneGraphFrame::updateSparkRunning(SparkController* controller)
{
    if (mCurrentTask == -1)
        return;

    if (controller->getSpark().get() == 0)
        return;
    
    //Was the currently selected thread started?   
    boost::shared_ptr<SimulationTask> currentTask(getCurrentTask());
    if (getSparkController(currentTask).get() == controller)
    {
        //Update current view
        mSceneGraphWidgets.at(mCurrentTask)->updateDisplay(boost::static_pointer_cast<SparkSimulationThread>(currentTask));
    }
    
    //Init/update property frame
    mPropertyFrame->init(this);
}

void SceneGraphFrame::updateSparkFinished(SparkController* spark)
{
}

void SceneGraphFrame::updateCurrent()
{
    if (mCurrentTask == -1)
        return;    

    //Update current view
    boost::shared_ptr<SimulationTask> updateTask(getCurrentTask());
    if (updateTask->getTaskDefinition().getType() != TaskDefinition::TT_SERVERTHREAD)
    {
        LOG_ERROR() << "Simulation task cant be used to update scene graph widget because it is no spark server thread.";
        return;
    }

    //Update scene graph
    mSceneGraphWidgets.at(mCurrentTask)->updateDisplay(boost::static_pointer_cast<SparkSimulationThread>(updateTask));
}

void SceneGraphFrame::updatePropertyFrame()
{
    if (mCurrentTask == -1)
        mPropertyFrame->releaseLeaf();
    else
        mPropertyFrame->updateDisplay();
}

void SceneGraphFrame::updateData()
{
    updateCurrent();
    updatePropertyFrame();
}

void SceneGraphFrame::collapseAll()
{
    if (mCurrentTask == -1)
        return;   

    mSceneGraphWidgets.at(mCurrentTask)->collapseAll();
}

void SceneGraphFrame::expandAll()
{
    if (mCurrentTask == -1)
        return;   

    mSceneGraphWidgets.at(mCurrentTask)->expandAll();
}

void SceneGraphFrame::toggleTimer(bool state)
{
    if (!state)
        mUpdateTimer.stop();
    else
        mUpdateTimer.start(mAutomaticUpdateInterval);

    mAutomaticUpdate = state;
}

void SceneGraphFrame::timerTick()
{
    updateData();
}

void SceneGraphFrame::pickLeaf(QString path)
{
    if (mClosed)
        return;

    if (mCurrentTask == -1)
        return;

    mSceneGraphWidgets.at(mCurrentTask)->pickLeaf(path);
}

void SceneGraphFrame::cleanup()
{
    if (mClosed)
        return;

    mClosed = true;

    //Save settings
    saveSettings();

    //Stop updating
    toggleTimer(false);

    //Clear widgets
    mSceneGraphWidgets.clear();
    //mPropertyFrame->deleteLater(); //Releases SparkController shared ptr
}

