#include "simulationframe.h"
#include "simulationframe_taskwidget.h"
#include "export.h"

#include <carbon.h>
#include <pluginmanager.h>
#include <simulationmanager.h>
#include <simulation.h>
#include <taskdefinition.h>
#include <sparksimulationthread.h>
#include <simulationtask.h>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(SimulationFrame, "simulationframe", "Simulation", 1)

//Static class information
void SimulationFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION, 
        tr("The simulation frame lists all tasks of the current simulation setup.\n" \
           "New tasks can be created, their parameters can be changed, and tasks can be started, stopped or paused individually or all together."));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":clock");

    //Tags
    QStringList tags;
    tags << "simulation" << "task" << "thread" << "process";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SimulationFrame::SimulationFrame() :
    mSettings(getCarbon()->getSettings())
{
    ui.setupUi(this);
    loadFrameDesign();

    //-------------------
    //Initialization
    mClosed = false;
    mSimulationManager = getCarbon()->getSimulationManager();
    init();

    //-------------------
    //Settings
    loadDefaultSettings();
    loadSettings();

    //-------------------
    //Connections
    Simulation* simulation = &*mSimulationManager->getSimulation();

    connect(simulation, SIGNAL(taskAdded(int, int)), this, SLOT(updateTaskAdded(int, int)));
    connect(simulation, SIGNAL(taskRemoved(int, int)), this, SLOT(updateTaskRemoved(int, int)));
    connect(simulation, SIGNAL(taskStateChanged(int, int, SimulationTask::ETaskExecutionState)), this, SLOT(updateTaskDisplay(int, int, SimulationTask::ETaskExecutionState)));

    connect(simulation, SIGNAL(initialized()), this, SLOT(updateHeader()));
    connect(simulation, SIGNAL(paused()), this, SLOT(updateHeader()));
    connect(simulation, SIGNAL(running()), this, SLOT(updateHeader()));
    connect(simulation, SIGNAL(stopped()), this, SLOT(updateHeader()));

    //Update Display
    updateTaskList();
    updateHeader();
}

SimulationFrame::~SimulationFrame()
{
    cleanup();
}

void SimulationFrame::loadSettings()
{

}

void SimulationFrame::loadDefaultSettings()
{

}

void SimulationFrame::saveSettings()       
{

}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void SimulationFrame::init()
{
    //Task Type list
    ui.tasksComboBox->clear();
    for (int i = 0; i < TaskDefinition::mMaxTypes; i++)
    {
        ui.tasksComboBox->addItem(TaskDefinition::mTypeNames[i]);
    }

    mChosenTask = 0;
    ui.tasksComboBox->setCurrentIndex(0);
}

void SimulationFrame::clearTaskWidgets()
{
    while (mTaskWidgets.size() > 0)
    {
        mTaskWidgets.back()->deleteLater();
        mTaskWidgets.pop_back();
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationFrame::updateHeader()  
{
    if (!mSimulationManager->getSimulation()->isInitialized())
    {
        ui.statusLabel->setText(tr("not initialized"));
        return;
    }
    
    if (mSimulationManager->getSimulation()->isPaused())
    {
        ui.statusLabel->setText(tr("paused"));
    }
    else if (mSimulationManager->getSimulation()->isRunning())
    {
        ui.statusLabel->setText(tr("running"));
    }
    else
    {
        ui.statusLabel->setText(tr("initialized"));
    }
}

void SimulationFrame::chooseTaskType(int index)  
{
    mChosenTask = index;
}

void SimulationFrame::addTask()                  
{
    if (mChosenTask < 0)
        return;

    bool add = false;
    switch (TaskDefinition::mTypes[mChosenTask])
    {
    case TaskDefinition::TT_SERVERTHREAD: 
    case TaskDefinition::TT_PLUGINTHREAD:
    case TaskDefinition::TT_SPARKPROCESS:
    case TaskDefinition::TT_PROCESS:
        LOG_INFO() << "Adding task of type " << TaskDefinition::mTypeNames[mChosenTask] << "...";
        add = true;
        break;
    default:
        LOG_INFO() << "Cannot create Task of type " << TaskDefinition::mTypeNames[mChosenTask] << ". Handling of this type has not been implemented yet.";
        break;
    }

    if (add)
    {
        boost::shared_ptr<TaskDefinition> newDefinition(mSimulationManager->newTaskDefinition(TaskDefinition::mTypes[mChosenTask]));
        
        bool success = static_cast<bool>(mSimulationManager->getSimulation()->addNewTask(*newDefinition));
        if (!success)
            LOG_INFO() << "Could not initialize task " << newDefinition->getName() << " (" << TaskDefinition::getTypeString(newDefinition->getType()) << ")";
        else
            LOG_INFO() << "Added task " << newDefinition->getName();
    }
}

void SimulationFrame::runTasks()       
{
    mSimulationManager->runSimulation();
}

void SimulationFrame::pauseTasks()     
{
    mSimulationManager->pauseSimulation();
}

void SimulationFrame::stopTasks()      
{
    mSimulationManager->stopSimulation();
}

void SimulationFrame::restartTasks()   
{
    mSimulationManager->stopSimulation();
    mSimulationManager->runSimulation();
}

void SimulationFrame::collapseTasks()  
{
    for (auto it = mTaskWidgets.begin(); it != mTaskWidgets.end(); it++)
    {
        (*it)->toggleDetail(true);
    }

    ui.tasksExpandButton->setChecked(false);
}

void SimulationFrame::expandTasks()    
{
    for (auto it = mTaskWidgets.begin(); it != mTaskWidgets.end(); it++)
    {
        (*it)->toggleDetail(false);
    }

    ui.tasksCollapseButton->setChecked(false);
}

void SimulationFrame::updateTaskList()
{
    clearTaskWidgets();
    
    for (auto it = mSimulationManager->getSimulation()->getTaskList().begin(); it != mSimulationManager->getSimulation()->getTaskList().end(); it++)
    {
        //Create Widget
        SimulationFrameUtil::TaskWidget* newWidget = new SimulationFrameUtil::TaskWidget(this, *it);
        mTaskWidgets.push_back(newWidget);

        //Store
        ui.tasksWidget->layout()->addWidget(newWidget);
        newWidget->updateDisplay();
    }
}

void SimulationFrame::updateTaskDisplay(int index, int id) 
{
    if (index < 0 || index >= (int)mTaskWidgets.size())
        return;

    mTaskWidgets.at(index)->updateDisplay();
}

void SimulationFrame::updateTaskDisplay(int index, int id, SimulationTask::ETaskExecutionState state) 
{
    if (index < 0 || index >= (int)mTaskWidgets.size())
        return;

    mTaskWidgets.at(index)->updateState(state);
}

void SimulationFrame::updateTaskAdded(int index, int id)   
{
    //Create Widget
    SimulationFrameUtil::TaskWidget* newWidget = new SimulationFrameUtil::TaskWidget(this, mSimulationManager->getSimulation()->getTaskList().at(index));
    mTaskWidgets.insert(mTaskWidgets.begin() + index, newWidget);

    //Insert into layout
    ((QVBoxLayout*)ui.tasksWidget->layout())->insertWidget(index, newWidget);

    //Init details
    if (ui.tasksCollapseButton->isChecked())
        newWidget->toggleDetail(true);
    if (ui.tasksExpandButton->isChecked())
        newWidget->toggleDetail(false);

    //Complete update
    newWidget->updateDisplay(true);
}

void SimulationFrame::updateTaskRemoved(int index, int id) 
{
    mTaskWidgets.at(index)->deleteLater();
    mTaskWidgets.erase(mTaskWidgets.begin() + index);
}

void SimulationFrame::chooseWidget(SimulationFrameUtil::TaskWidget* widget)
{
    bool found = false;
    for (auto it = mTaskWidgets.begin(); it != mTaskWidgets.end(); it++)
    {
        if (*it == widget)
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        for (auto it = mTaskWidgets.begin(); it != mTaskWidgets.end(); it++)
        {
            if (*it != widget)
            {
                (*it)->unchoose();
            }
        }
        widget->choose();
    }
}

void SimulationFrame::cleanup()  
{
    if (mClosed)
        return;

    mClosed = true;

    clearTaskWidgets();
}

