#include "simulationframe_taskwidget.h"
#include "simulationframe.h"

#include <simulationtask.h>
#include <simulationmanager.h>
#include <pluginmanager.h>
#include <simulation.h>
#include <taskdefinition.h>
#include <numberedactionreceiver.h>
#include <namedactionreceiver.h>

#include <QPalette>
#include <QMouseEvent>
#include <QListWidget>
#include <QPoint>

namespace SimulationFrameUtil
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

bool TaskWidget::mInitializedStatic = false;
QPalette TaskWidget::mDefaultPalette;
QPalette TaskWidget::mSelectedPalette;

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

TaskWidget::TaskWidget(SimulationFrame* parent, boost::shared_ptr<SimulationTask> task) :
    mTask(task),
    mParent(parent)
{
    //--------------------
    //UI
    ui.setupUi(this);
    showInfo(false);
    showLoadButton(false);

    mSimulationManager = parent->getCarbon()->getSimulationManager();
    mPluginManager = parent->getCarbon()->getPluginManager();

    if (!mInitializedStatic)
    {
        mInitializedStatic = true;
        mDefaultPalette = this->palette();
        mSelectedPalette = this->palette();
        mSelectedPalette.setColor(QPalette::Light, Qt::darkRed);
        mSelectedPalette.setColor(QPalette::Dark, Qt::darkRed);
    }

    //--------------------
    //Init
    mInitialized = false;
    for (int i = 0; i < TaskDefinition::mMaxPriorityTypes; i++)
    {
        ui.priorityComboBox->addItem(TaskDefinition::mPriorityTypeNames[i]);
    }
    mInitialized = true;
    mFirstListChosenEntry = -1;
    mChosen = false;
    mReactToChangeListEntry = true;
    mLongInfo = false;
    mPluginMatchCount = 0;

    //--------------------
    //Display
    updateDisplay();

    //--------------------
    //Connections

    connect(&task->getTaskDefinition(), SIGNAL(definitionChanged(TaskDefinition::ETaskData)), this, SLOT(updateDisplay()));
    //connection to task state change comes from simulation frame
}

TaskWidget::~TaskWidget()
{
    
}

void TaskWidget::mousePressEvent(QMouseEvent * e)
{
    mParent->chooseWidget(this);
}

void TaskWidget::choose()
{
    mChosen = true;
    setPalette(mSelectedPalette);

    updateListSize();
}

void TaskWidget::unchoose()
{
    mChosen = false;
    setPalette(mDefaultPalette);

    updateListSize();
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void TaskWidget::updateDetailsButtonIcon()
{
    if (ui.detailsButton->isChecked())
    {
        ui.detailsButton->setArrowType(Qt::DownArrow);        
    }
    else
    {
        ui.detailsButton->setArrowType(Qt::UpArrow);
    }
}

TaskDefinition& TaskWidget::getTaskDefinition(boost::shared_ptr<SimulationTask> task)
{
    return const_cast<TaskDefinition&>(task->getTaskDefinition());
}

void TaskWidget::updatePluginMatchList()
{
    std::vector<AbstractPlugin*> matchList = mPluginManager->findPlugins(mFirstValue, mSecondValue, mThirdValueInt);
    
    mPluginMatchList = "";

    QString description;
    for (auto it = matchList.begin(); it != matchList.end(); it++)
    {
        description = QString("Class: %1, Caption: %2, Type: %3, Id: %4\n").
            arg((*it)->getClassName()).
            arg((*it)->getCaption()).
            arg((*it)->getPluginTypeString()).
            arg((*it)->getPluginId());
        mPluginMatchList.append(description);
    }

    mPluginMatchCount = matchList.size();
    showInfo(true);
    ui.infoLabel->setToolTip(mPluginMatchList);

    QString infoText;
    if (mLongInfo && mPluginMatchCount > 0)
        infoText = QString("Plugin Matches: %1\n%2").arg(mPluginMatchCount).arg(mPluginMatchList);
    else if(mLongInfo)
        infoText = QString("Plugin Matches: %1\n%2(No match details)").arg(mPluginMatchCount).arg(mPluginMatchList);
    else
        infoText = QString("Plugin Matches: %1").arg(mPluginMatchCount);

    ui.infoLabel->setText(infoText);
}

void TaskWidget::colorText(QLineEdit& edit, const QColor& color)
{
    QPalette pal = edit.palette();
    pal.setColor(QPalette::Text, color);
    edit.setPalette(pal);
}

void TaskWidget::addFirstListEntry(QString value)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        mFirstList.append(value);
        QListWidgetItem* newItem = new QListWidgetItem(value);
        newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            //Plugin threads do not have scripts in the list
        }
        else
        {
            //Processes and Server have scripts
            checkScript(newItem);
        }

        ui.firstListWidget->addItem(newItem);
    }
    else
        updateDisplay();
}

void TaskWidget::addSecondListEntry(QString value)
{
    mSecondList.append(value);
    QListWidgetItem* newItem = new QListWidgetItem(value);
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui.secondListWidget->addItem(newItem);
}

void TaskWidget::removeFirstListEntry(int index)
{
    mFirstList.removeAt(index);
    delete ui.firstListWidget->takeItem(index);
}

void TaskWidget::removeSecondListEntry(int index)
{
    mSecondList.removeAt(index);
    delete ui.firstListWidget->takeItem(index);
}

void TaskWidget::checkScript(QListWidgetItem* item)
{
    if (mSimulationManager->findScript(item->text()).compare("") == 0)
    {
        item->setTextColor(Qt::red);
    }
    else
    {
        item->setTextColor(Qt::black);
    }
}

void TaskWidget::updateListSize()
{
    if (mChosen)
    {
        if (mFirstList.size() > 1)
        {
            ui.firstListWidget->setMinimumHeight(80);
            ui.firstListWidget->setMaximumHeight(80);
        }
        if (mSecondList.size() > 1)
        {
            ui.secondListWidget->setMinimumHeight(80);
            ui.secondListWidget->setMaximumHeight(80);
        }
    }
    else
    {
        ui.firstListWidget->setMinimumHeight(40);
        ui.firstListWidget->setMaximumHeight(40);
        ui.secondListWidget->setMinimumHeight(40);
        ui.secondListWidget->setMaximumHeight(40);
    }
}

void TaskWidget::changeListEntry(QListWidgetItem* item, QListWidget* listWidget, QStringList& list, int& chosenEntry, bool first)
{
    if (!mInitialized)
        return;

    if (!mReactToChangeListEntry)
        return;

    boost::shared_ptr<SimulationTask> task = mTask.lock();
    if (task.get() == 0)
    {
        updateDisplay();
        return;
    }

    mParent->chooseWidget(this);

    mReactToChangeListEntry = false;

    if (listWidget->currentItem() != item)
        listWidget->setCurrentItem(item);

    if (chosenEntry < 0 || chosenEntry > list.size())
    {
        LOG_ERROR() << "Illegal row index."; 
        return;
    }   

    //Replace in list. May result in text replacement, deletion or creation of a new row
    list.replace(chosenEntry, item->text());

    if (first && task->getTaskDefinition().getType() != TaskDefinition::TT_PLUGINTHREAD)
        checkScript(ui.firstListWidget->item(mFirstListChosenEntry));

    bool added = false, removed = false;
    int removedindex = -1;
    //Make sure there is always an empty entry at the end
    if  (item->text().compare("") != 0)
    {
        if (chosenEntry == list.size() -1)
        {
            if (first)
                addFirstListEntry("");
            else
                addSecondListEntry("");

            added = true;
        }
    }
    else
    {   
        if (chosenEntry == list.size() -1)
        {
            //do nothing, keep last empty entry
        }
        else
        {
            removedindex = chosenEntry;
            removed = true;
            if (first)
                removeFirstListEntry(removedindex);
            else
                removeSecondListEntry(removedindex);

            listWidget->setCurrentRow(removedindex);
            chosenEntry = removedindex;
        }
    }

    //Update definition list
    QStringList& deflist = (first ? getTaskDefinition(task).getFirstListNc() : getTaskDefinition(task).getSecondListNc());
    if (added)
    {
        //Add 
        deflist.push_back(item->text());
    }
    else if (removed)
    {
        //Remove
        if (first)
            getTaskDefinition(task).removeFromFirstList(removedindex);
        else
            getTaskDefinition(task).removeFromSecondList(removedindex);
    }
    else
    {
        //List empty?
        if (chosenEntry >= deflist.size())
        {
            mReactToChangeListEntry = true;
            updateListSize(); 
            return;
        }

        //Change
        deflist.replace(chosenEntry, item->text());
    }

    //Emit update signal
    getTaskDefinition(task).updateDefinitionChanged();
    updateListSize();

    mReactToChangeListEntry = true;
}

int TaskWidget::getItemIndex(QListWidgetItem* item, QListWidget* listWidget)
{
    for (int i = 0; i < listWidget->count(); i++)
    {
        if (listWidget->item(i) == item)
            return i;
    }

    return -1;
}

bool TaskWidget::useListCommand(QListWidgetItem* item, TaskWidget::EListCommand command)
{
    QListWidget* list = item->listWidget();
    int index = getItemIndex(item, ui.firstListWidget); 

    if (command == LC_MOVE_UP)
    {
        if (index == 0)
            return false;

        //Swap item with previous
        QString swap = list->item(index-1)->text();
        list->item(index-1)->setText(item->text());
        item->setText(swap);

        checkScript(list->item(index-1));
        checkScript(item);
    }
    else if (command == LC_MOVE_DOWN)
    {
        if (index >= mFirstList.size()-2)
            return false;

        //Swap item with following
        QString swap = list->item(index+1)->text();
        list->item(index+1)->setText(item->text());
        item->setText(swap);

        checkScript(list->item(index+1));
        checkScript(item);
    }
    else if (command == LC_ADD_ENTRY)
    {
        //Insert item in front of the entry
        for (int i = list->count()-1; i > index; i--)
        {
            QString text = list->item(i-1)->text();
            list->item(i)->setText(text);
            checkScript(list->item(i));
        }
        item->setText("new");
        checkScript(item);
    }
    else if (command == LC_REMOVE_ENTRY)
    {
        //Remove the item (deletion is done by called slots)
        item->setText("");
    }
    else
    {
        LOG_ERROR() << "Unknown list command " << command << ".";
        return false;
    }

    return true;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void TaskWidget::chooseThisWidget()
{
    mParent->chooseWidget(this);
}

void TaskWidget::updateDisplay(bool updateAll)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        //Detail
        const TaskDefinition& def = task->getTaskDefinition();
        ui.nameEdit->setText(def.getName());
        mName = def.getName();
        ui.typeTextLabel->setText(def.getTypeString());
        ui.priorityComboBox->setCurrentIndex((int)def.getPriority());
        ui.secondEdit->setText(def.getSecond());
        ui.thirdEdit->setText(def.getThird());
        ui.firstEdit->setText(def.getFirst());
        
        checkSecond(def.getSecond());
        checkThird(def.getThird());
        checkFirst(def.getFirst());

        if (mReactToChangeListEntry) // dont update script list if the call originated from changing the script list
        {
            ui.firstListWidget->clear();
            mFirstList.clear();
            for (auto it = def.getFirstList().begin(); it != def.getFirstList().end(); it++)
            {
                addFirstListEntry(*it);
            }
            addFirstListEntry(""); //Empty script at the end to change
            if (mFirstListChosenEntry < 0)
                mFirstListChosenEntry = 0;
            if (mFirstListChosenEntry > def.getFirstList().size() +1)
                mFirstListChosenEntry = def.getFirstList().size();

            ui.secondListWidget->clear();
            mSecondList.clear();
            for (auto it = def.getSecondList().begin(); it != def.getSecondList().end(); it++)
            {
                addSecondListEntry(*it);
            }
            addSecondListEntry(""); //Empty script at the end to change
            if (mSecondListChosenEntry < 0)
                mSecondListChosenEntry = 0;
            if (mSecondListChosenEntry > def.getSecondList().size() +1)
                mSecondListChosenEntry = def.getSecondList().size();
        }

        //Label text & access
        if (updateAll || (mLastType != (int)def.getType())) //initialize only if type has changed
        {
            QString explanation = tr("\nRed: File was not found in any of the include directories. Black: File was found.");
            if (def.getType() == TaskDefinition::TT_SERVERTHREAD)
            {
                ui.firstLabel->setText(tr("Exec:","Executable (short)"));
                ui.firstEdit->setEnabled(false);
                ui.firstEdit->setToolTip(tr("An internal server thread has no executable.") + explanation);
                ui.firstEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            
                ui.secondLabel->setText(tr("MainScript:"));
                ui.secondEdit->setEnabled(true);
                ui.secondEdit->setToolTip(tr("Spark initialization script.") + explanation);
                ui.secondEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            
                ui.thirdLabel->setText(tr("Sec.Script:", "Secondary script (short)"));
                ui.thirdEdit->setEnabled(true);
                ui.thirdEdit->setToolTip(tr("Simulation initialization script.") + explanation);
                ui.thirdEdit->setContextMenuPolicy(Qt::CustomContextMenu);
            
                ui.firstListLabel->setText(tr("Scripts:"));
                ui.firstListWidget->setEnabled(true);
                ui.firstListWidget->setToolTip(tr("Additional scripts to execute.") + explanation);

                ui.secondListLabel->setText(tr("Parameters:"));
                ui.secondListWidget->setEnabled(true);
                ui.secondListWidget->setToolTip(tr("Commandline parameters for initialization."));

                showInfo(false);
                showLoadButton(false);
            }
            else if (def.getType() == TaskDefinition::TT_PLUGINTHREAD)
            {
                explanation = tr("\nRed: no plugin matches this single criterium. Black: at least one plugin matches.");
                ui.firstLabel->setText(tr("Class:"));
                ui.firstEdit->setEnabled(true);
                ui.firstEdit->setToolTip(tr("Class name of plugin to start.") + explanation);
                ui.firstEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
            
                ui.secondLabel->setText(tr("Caption:"));
                ui.secondEdit->setEnabled(true);
                ui.secondEdit->setToolTip(tr("Caption of plugin to start.") + explanation);
                ui.secondEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
            
                ui.thirdLabel->setText(tr("Type:"));
                ui.thirdEdit->setEnabled(true);
                ui.thirdEdit->setToolTip(tr("Type of plugin to start. -1: Any, 0: normal, 1: signal, 2: frame") + explanation + tr(" Blue: illegal value."));
                ui.thirdEdit->setContextMenuPolicy(Qt::DefaultContextMenu);

                ui.firstListLabel->setText(tr("Scripts:"));
                ui.firstListWidget->setEnabled(false);
                ui.firstListWidget->setToolTip(tr("Plugins do not use scripts."));

                ui.secondListLabel->setText(tr("Parameters:"));
                ui.secondListWidget->setEnabled(true);
                ui.secondListWidget->setToolTip(tr("Plugin execution parameters."));

                showInfo(true);
                showLoadButton(true);

                ui.infoButton->setToolTip(tr("Show or hide detailed info on plugins that match the specified class, name and type."));
                ui.loadButton->setToolTip(tr("Load a predefined plugin definition."));
            }
            else if (def.getType() == TaskDefinition::TT_SPARKPROCESS)
            {
                ui.firstLabel->setText(tr("Exec:","Executable (short)"));
                ui.firstEdit->setEnabled(true);
                ui.firstEdit->setToolTip(tr("Executable to start.") + explanation);
                ui.firstEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.secondLabel->setText(tr("MainScript:"));
                ui.secondEdit->setEnabled(true);
                ui.secondEdit->setToolTip(tr("Simulation initialization script.") + explanation);
                ui.secondEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.thirdLabel->setText(tr("Sec.Script:", "Secondary script (short)"));
                ui.thirdEdit->setEnabled(true);
                ui.thirdEdit->setToolTip(tr("Spark initialization script.") + explanation);
                ui.thirdEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.firstListLabel->setText(tr("Scripts:"));
                ui.firstListWidget->setEnabled(true);
                ui.firstListWidget->setToolTip(tr("Additional scripts to execute.") + explanation);

                ui.secondListLabel->setText(tr("Parameters:"));
                ui.secondListWidget->setEnabled(true);
                ui.secondListWidget->setToolTip(tr("Commandline parameters for initialization."));

                showInfo(false);
                showLoadButton(false);
            }
            else if (def.getType() == TaskDefinition::TT_PROCESS)
            {
                ui.firstLabel->setText(tr("Exec:","Executable (short)"));
                ui.firstEdit->setEnabled(true);
                ui.firstEdit->setToolTip(tr("Executable to start.") + explanation);
                ui.firstEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.secondLabel->setText(tr("MainScript:"));
                ui.secondEdit->setEnabled(false);
                ui.secondEdit->setToolTip(tr("Default processes do not use scripts.") + explanation);
                ui.secondEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.thirdLabel->setText(tr("Sec.Script:", "Secondary script (short)"));
                ui.thirdEdit->setEnabled(false);
                ui.thirdEdit->setToolTip(tr("Default processes do not use scripts.") + explanation);
                ui.thirdEdit->setContextMenuPolicy(Qt::CustomContextMenu);

                ui.firstListLabel->setText(tr("Scripts:"));
                ui.firstListWidget->setEnabled(false);
                ui.firstListWidget->setToolTip(tr("Processes do not use scripts."));

                ui.secondListLabel->setText(tr("Parameters:"));
                ui.secondListWidget->setEnabled(true);
                ui.secondListWidget->setToolTip(tr("Commandline parameters for process execution."));

                showInfo(false);
                showLoadButton(false);
            }
        }
        mLastType = (int)def.getType();
        
        //Summary
        QString text = QString("%1 (%2)").arg(def.getName()).arg(def.getTypeString());
        ui.headerLabel->setText(text);
        updateState(mSimulationManager->getSimulation()->getTaskExecutionState(&*task));
    }
    else
    {
        //Task has been removed
        QString text = QString("%1 *deleted*").arg(ui.nameEdit->text());
        ui.headerLabel->setText(text);
        updateState(SimulationTask::TES_UNDEFINED);
    }

    updateListSize();
}

void TaskWidget::updateState(SimulationTask::ETaskExecutionState state)
{
    bool undefined = false;
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (state == SimulationTask::TES_INITIALIZED)
        {
            ui.headerDeleteButton->setEnabled(true);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(false);
            ui.headerPlayButton->setIcon(QIcon(":play_green"));
            ui.headerStatusLabel->setText(tr("(initialized)"));
        }
        else if (state == SimulationTask::TES_RUNNING)
        {
            ui.headerDeleteButton->setEnabled(false);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(true);
            ui.headerPlayButton->setIcon(QIcon(":pause_green"));
            ui.headerStopButton->setIcon(QIcon(":stop_green"));
            ui.headerStatusLabel->setText(tr("(running)"));
        }
        else if (state == SimulationTask::TES_PAUSED)
        {
            ui.headerDeleteButton->setEnabled(false);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(true);
            ui.headerPlayButton->setIcon(QIcon(":play_green"));
            ui.headerStopButton->setIcon(QIcon(":stop_green"));
            ui.headerStatusLabel->setText(tr("(paused)"));
        }
        else if (state == SimulationTask::TES_STOPPED)
        {
            ui.headerDeleteButton->setEnabled(true);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(false);
            ui.headerPlayButton->setIcon(QIcon(":play_green"));
            ui.headerStopButton->setIcon(QIcon(":stop_green"));
            ui.headerStatusLabel->setText(tr("(stopped)"));
        }
        else if (state == SimulationTask::TES_FINISHED)
        {
            ui.headerDeleteButton->setEnabled(true);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(false);
            ui.headerPlayButton->setIcon(QIcon(":play_green"));
            ui.headerStopButton->setIcon(QIcon(":stop_green"));
            ui.headerStatusLabel->setText(tr("(finished)"));
        }
        else if (state == SimulationTask::TES_TERMINATED)
        {
            ui.headerDeleteButton->setEnabled(true);
            ui.headerPlayButton->setEnabled(true);
            ui.headerStopButton->setEnabled(false);
            ui.headerPlayButton->setIcon(QIcon(":play_green"));
            ui.headerStopButton->setIcon(QIcon(":stop_green"));
            ui.headerStatusLabel->setText(tr("(terminated)"));
        }
        else
            undefined = true;
    }
    else undefined = true;

    if (undefined)
    {
        //Task not existant anymore, or not initialized yet (but this will soon be replaced)
        ui.headerDeleteButton->setEnabled(true);
        ui.headerPlayButton->setEnabled(false);
        ui.headerPlayButton->setIcon(QIcon(":cross"));
        ui.headerStatusLabel->setText(tr("(removed)"));
    }

    updateListSize();
}

void TaskWidget::changeTaskPriority(int index)
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        getTaskDefinition(task).setPriority(TaskDefinition::mPriorityTypes[index]);
    }
    else
        updateDisplay();
}

void TaskWidget::checkName(QString value)
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        mName = value;
    }
    else
        updateDisplay();    
}

void TaskWidget::changeName()            
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        getTaskDefinition(task).setName(mName);
    }
    else
        updateDisplay();    
}


void TaskWidget::checkFirst(QString value)
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            //Apply value, update color and update plugin match list
            mFirstValue = value;
            
            std::vector<AbstractPlugin*> matches = mPluginManager->findPlugins(mFirstValue, "", -1);
            if (matches.size() == 0)
                colorText(*ui.firstEdit, QColor(Qt::red));
            if (matches.size() >= 1)
                colorText(*ui.firstEdit, QColor(Qt::black));

            updatePluginMatchList();
        }
        else
        {
            //Check script and update color
            mFirstValue = value;
            QString path = mSimulationManager->findExecutable(mFirstValue);
            if (path.compare("") == 0)
                colorText(*ui.firstEdit, QColor(Qt::red));
            else
                colorText(*ui.firstEdit, QColor(Qt::black));
        }
    }
    else
        updateDisplay(); 
}

void TaskWidget::changeFirst()
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        getTaskDefinition(task).setFirst(mFirstValue);
    }
    else
        updateDisplay(); 
}

void TaskWidget::checkSecond(QString value)
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            //Apply value, update color and update plugin match list
            mSecondValue = value;

            std::vector<AbstractPlugin*> matches = mPluginManager->findPlugins(mFirstValue, "", -1);
            if (matches.size() == 0)
                colorText(*ui.secondEdit, QColor(Qt::red));
            if (matches.size() >= 1)
                colorText(*ui.secondEdit, QColor(Qt::black));

            updatePluginMatchList();
        }
        else
        {
            //Check script and update color
            mSecondValue = value;
            QString path = mSimulationManager->findScript(mSecondValue);
            if (path.compare("") == 0)
                colorText(*ui.secondEdit, QColor(Qt::red));
            else
                colorText(*ui.secondEdit, QColor(Qt::black));
        }
    }
    else
        updateDisplay();    
}

void TaskWidget::changeSecond()
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        getTaskDefinition(task).setSecond(mSecondValue);
    }
    else
        updateDisplay(); 
}

void TaskWidget::checkThird(QString value)
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            mThirdValue = value;

            //Apply value, update color and update plugin match list
            int type = -2;
            if      (value.compare("any"   ) == 0 || value.compare("-1") == 0) type = -1;
            else if (value.compare("normal") == 0 || value.compare( "0") == 0) type = 0;
            else if (value.compare("signal") == 0 || value.compare( "1") == 0) type = 1;
            else if (value.compare("frame" ) == 0 || value.compare( "2") == 0) type = 2;

            if (type == -2)
            {
                //something nonexisting entered, dont apply to checks but change color
                colorText(*ui.thirdEdit, QColor(Qt::blue));
                return;
            }

            mThirdValueInt = type;

            std::vector<AbstractPlugin*> matches = mPluginManager->findPlugins("", "", mThirdValueInt);
            if (matches.size() == 0)
                colorText(*ui.thirdEdit, QColor(Qt::red));
            if (matches.size() >= 1)
                colorText(*ui.thirdEdit, QColor(Qt::black));

            updatePluginMatchList();
        }
        else
        {
            //Check script and update color
            mThirdValue = value;
            QString path = mSimulationManager->findScript(mThirdValue);
            if (path.compare("") == 0)
                colorText(*ui.thirdEdit, QColor(Qt::red));
            else
                colorText(*ui.thirdEdit, QColor(Qt::black));
        }
    }
    else
        updateDisplay();  
}

void TaskWidget::changeThird()
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            //Apply value only if legal
            if (mThirdValue.compare("any"   ) == 0 || mThirdValue.compare("-1") == 0 ||
                mThirdValue.compare("normal") == 0 || mThirdValue.compare( "0") == 0 ||
                mThirdValue.compare("signal") == 0 || mThirdValue.compare( "1") == 0 ||
                mThirdValue.compare("frame" ) == 0 || mThirdValue.compare( "2") == 0)
            {
                getTaskDefinition(task).setThird(mThirdValue);
            }
            else
            {
                //Revert to 'any' entry
                ui.thirdEdit->setText("any");
            }
        }
        else
        {
            getTaskDefinition(task).setThird(mThirdValue);
        }
    }
    else
        updateDisplay(); 
}

void TaskWidget::chooseFirstListEntry(int row)
{
    if (row >= 0 && row <= mFirstList.size())
        mFirstListChosenEntry = row;
}

void TaskWidget::changeFirstListEntry(QListWidgetItem* item)
{
    changeListEntry(item, ui.firstListWidget, mFirstList, mFirstListChosenEntry, true);
}

void TaskWidget::chooseSecondListEntry(int row)
{
    if (row >= 0 && row <= mSecondList.size())
        mSecondListChosenEntry = row;
}

void TaskWidget::changeSecondListEntry(QListWidgetItem* item)
{
    changeListEntry(item, ui.secondListWidget, mSecondList, mSecondListChosenEntry, false);
}

void TaskWidget::toggleRunning()         
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        SimulationTask::ETaskExecutionState state = mSimulationManager->getSimulation()->getTaskExecutionState(&*task);
        if (state == SimulationTask::TES_RUNNING)
        {
            mSimulationManager->getSimulation()->pauseTask(&*task);
        }
        //if (state == TaskState::TES_PAUSED || state == TaskState::TES_INITIALIZED)
        else
        {
            mSimulationManager->getSimulation()->runTask(&*task);
        }
    }
    else
        updateDisplay();    
}

void TaskWidget::stopTask()         
{
    if (!mInitialized)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        SimulationTask::ETaskExecutionState state = mSimulationManager->getSimulation()->getTaskExecutionState(&*task);
        if (state == SimulationTask::TES_RUNNING || state == SimulationTask::TES_PAUSED)
        {
            mSimulationManager->getSimulation()->stopTask(&*task);
        }
    }
    else
        updateDisplay();    
}

void TaskWidget::deleteTask()          
{
    if (mSimulationManager->simulationRunning())
    {
        LOG_INFO() << "Cant delete tasks of a running simulation.";
        return;
    }

    SimulationTask* task = 0;
    if(boost::shared_ptr<SimulationTask> sptr = mTask.lock())
    {
        task = &*sptr;
    }
    else
        updateDisplay();    
    
    if (task != 0)
    {
        mSimulationManager->getSimulation()->removeTask(&*task);
    }
}

void TaskWidget::copyTask(bool unique_name)          
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD)
        {
            LOG_INFO() << "Cant create more than one server task.";
            return;
        }

        //Get Task definition to create new task with
        TaskDefinition definition(task->getTaskDefinition());

        if (unique_name)
        {
            //Assert unique name by adding a number
            const SimulationSetup::TTaskDefinitions& deflist = mSimulationManager->getSimulation()->getSetup()->getTaskDefinitions();
            int run = 1;
            int separator = definition.getName().lastIndexOf("_");
            QString baseName = separator == -1 ? definition.getName() : definition.getName().left(separator);
            QString newName;
            bool foundEqualName;
            do
            {
                foundEqualName = false;
                newName = baseName + QString("_") + QString::number(run);
                for (auto it = deflist.begin(); it != deflist.end(); it++)
                {
                    if (it->get()->getName().compare(newName) == 0)
                    {
                        foundEqualName = true;
                        break;
                    }
                }
                run++;
            } while (foundEqualName);

            //Change name to unique name
            definition.setName(newName);
        }

        //Create new task
        mSimulationManager->getSimulation()->addNewTask(definition);
    }
    else
        updateDisplay();    
}

void TaskWidget::clearActionReceivers()
{
    qDeleteAll(mLoadActionReceivers);
    qDeleteAll(mListNumberedActionReceivers);
    qDeleteAll(mListNamedActionReceivers);

    mLoadActionReceivers.clear();
    mListNumberedActionReceivers.clear();
    mListNamedActionReceivers.clear();
}

void TaskWidget::loadTask()
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
             QMenu menu(this);
             clearActionReceivers();

             //Create menu with plugin identifications
             mParent->getCarbon()->getPluginManager()->createPluginContextMenu(&menu, mLoadActionReceivers, this, SLOT(onLoadTask(int)));
        
             //Get popup position 
             QPoint pos = ui.loadButton->mapToGlobal(QPoint(ui.infoButton->x(), ui.infoButton->y()));

             //Open menu
             menu.exec(pos);
        }
        else
        {

        }
    }
    else
        updateDisplay();    
}

void TaskWidget::onLoadTask(int id)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            AbstractPlugin* plugin = mPluginManager->getPlugin(id);

            if (plugin == 0)
            {
                LOG_ERROR() << "Plugin with id " << id << " not found.";
                return;
            }

            //Class edit
            ui.firstEdit->setText(plugin->getClassName());
            checkFirst(plugin->getClassName());
            changeFirst();

            //Caption edit
            ui.secondEdit->setText(plugin->getCaption());
            checkSecond(plugin->getCaption());
            changeSecond();

            //type edit
            switch (plugin->getPluginType())
            {
            case PT_NORMAL: ui.thirdEdit->setText("0"); checkThird("0"); break;
            case PT_SIGNAL: ui.thirdEdit->setText("1"); checkThird("1"); break;
            case PT_FRAME : ui.thirdEdit->setText("2"); checkThird("2"); break;
            default:
                LOG_ERROR() << "Illegal plugin type " << (int) plugin->getPluginType();
                break;
            }
            changeThird();

            //name
            if (ui.nameEdit->text().compare("") == 0)
            {
                QString name = plugin->getCaption() + "thread";
                ui.nameEdit->setText(name);
                checkName(name);
                changeName();
            }
        }
        else
        {

        }
    }
    else
        updateDisplay();      
}

void TaskWidget::toggleDetail(bool state)          
{
    if (!mInitialized)
        return;

    if (ui.detailsButton->isChecked() != state)
    {
        ui.detailsButton->setChecked(state);
        return;
    }

    updateDetailsButtonIcon();
}

void TaskWidget::toggleLongInfo()
{
    mLongInfo = !mLongInfo;

    QString infoText;
    if (mLongInfo && mPluginMatchCount > 0)
        infoText = QString("Plugin Matches: %1\n%2").arg(mPluginMatchCount).arg(mPluginMatchList);
    else if(mLongInfo)
        infoText = QString("Plugin Matches: %1\n%2(No match details)").arg(mPluginMatchCount).arg(mPluginMatchList);
    else
        infoText = QString("Plugin Matches: %1").arg(mPluginMatchCount);

    ui.infoLabel->setText(infoText);
}

void TaskWidget::showInfo(bool state)
{
    ui.infoWidget->setVisible(state);
}

void TaskWidget::showLoadButton(bool state)
{
    ui.loadButton->setVisible(state);
}

void TaskWidget::showFirstContextMenu(QPoint pos)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        QMenu menu(this);
        clearActionReceivers();
        SimulationManager* manager = mParent->getCarbon()->getSimulationManager();

        if (task->getTaskDefinition().getType() != TaskDefinition::TT_PLUGINTHREAD)
        {
            //Create script list context menu entries
            manager->createExecutableContextMenu(&menu, mListNamedActionReceivers, this, SLOT(onFirstContextMenuClick(const QString&)), false); //non-absolute paths
            menu.addSeparator();
        }

        //Add default context menu actions
        menu.addActions(ui.firstEdit->actions());
        
        //Open menu
        menu.exec(ui.firstEdit->mapToGlobal(pos));
    }
}

void TaskWidget::showSecondContextMenu(QPoint pos)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        QMenu menu(this);
        clearActionReceivers();
        SimulationManager* manager = mParent->getCarbon()->getSimulationManager();

        if (task->getTaskDefinition().getType() != TaskDefinition::TT_PLUGINTHREAD)
        {
            //Create script list context menu entries
            manager->createScriptContextMenu(&menu, mListNamedActionReceivers, this, SLOT(onSecondContextMenuClick(const QString&)), false); //non-absolute paths
            menu.addSeparator();
        }

        //Add default context menu actions
        menu.addActions(ui.secondEdit->actions());
        
        //Open menu
        menu.exec(ui.secondEdit->mapToGlobal(pos));
    }
}
void TaskWidget::showThirdContextMenu(QPoint pos)
{
    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        QMenu menu(this);
        clearActionReceivers();
        SimulationManager* manager = mParent->getCarbon()->getSimulationManager();

        if (task->getTaskDefinition().getType() != TaskDefinition::TT_PLUGINTHREAD)
        {
            //Create script list context menu entries
            manager->createScriptContextMenu(&menu, mListNamedActionReceivers, this, SLOT(onThirdContextMenuClick(const QString&)), false); //non-absolute paths
            menu.addSeparator();
        }

        //Add default context menu actions
        menu.addActions(ui.thirdEdit->actions());
        
        //Open menu
        menu.exec(ui.thirdEdit->mapToGlobal(pos));
    }
}

void TaskWidget::onFirstContextMenuClick(const QString& entry)
{
    ui.firstEdit->setText(entry);
    checkFirst(entry);
}

void TaskWidget::onFirstContextMenuClick(int id)
{

}

void TaskWidget::onSecondContextMenuClick(const QString& entry)
{
    ui.secondEdit->setText(entry);
    checkSecond(entry);
}

void TaskWidget::onSecondContextMenuClick(int id)
{

}

void TaskWidget::onThirdContextMenuClick(const QString& entry)
{
    ui.thirdEdit->setText(entry);
    checkThird(entry);
}

void TaskWidget::onThirdContextMenuClick(int id)
{

}

void TaskWidget::showFirstListContextMenu(QPoint pos)
{
    QListWidgetItem* item = ui.firstListWidget->itemAt(pos);
    if (item == 0)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        if (task->getTaskDefinition().getType() == TaskDefinition::TT_PLUGINTHREAD)
        {
            //todo: Show plugin references
        }
        else
        {
            SimulationManager* manager = mParent->getCarbon()->getSimulationManager();

            //Show menu with available scripts
            QMenu menu(this);
            QAction* action;
            NumberedActionReceiver* numberedReceiver;
            clearActionReceivers();
        
            //Add context commands
            int index = getItemIndex(item, ui.firstListWidget);

            //move script up
            numberedReceiver = new NumberedActionReceiver(LC_MOVE_UP);
            mListNumberedActionReceivers.push_back(numberedReceiver);
            action = menu.addAction(tr("move entry up"), numberedReceiver, SLOT(receiveAction()));
            connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
            if (index <= 0) action->setEnabled(false);

            //move script down
            numberedReceiver = new NumberedActionReceiver(LC_MOVE_DOWN);
            mListNumberedActionReceivers.push_back(numberedReceiver);
            action = menu.addAction(tr("move entry down"), numberedReceiver, SLOT(receiveAction()));
            connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
            if (index >= mFirstList.size()-2) action->setEnabled(false);

            //insert new script
            numberedReceiver = new NumberedActionReceiver(LC_ADD_ENTRY);
            mListNumberedActionReceivers.push_back(numberedReceiver);
            action = menu.addAction(tr("add entry"), numberedReceiver, SLOT(receiveAction()));
            connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));

            //remove script entry
            numberedReceiver = new NumberedActionReceiver(LC_REMOVE_ENTRY);
            mListNumberedActionReceivers.push_back(numberedReceiver);
            action = menu.addAction(tr("remove entry"), numberedReceiver, SLOT(receiveAction()));
            connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
            if (index == mFirstList.size()-1) action->setEnabled(false);

            menu.addSeparator();

            //Add scripts

            //Create script list context menu entries
            manager->createScriptContextMenu(&menu, mListNamedActionReceivers, this, SLOT(onFirstListContextMenuClick(const QString&)), false); //non-absolute paths

            //Open menu
            mFirstListClickedAt = pos;
            menu.exec(ui.firstListWidget->mapToGlobal(pos));
        }
    }
}

void TaskWidget::onFirstListContextMenuClick(const QString& entry)
{
    QListWidgetItem* item = ui.firstListWidget->itemAt(mFirstListClickedAt);
    if (item == 0)
    {
        LOG_ERROR() << "Clicked list item not found.";
        return;
    }

    item->setText(entry);
    checkScript(item);
}

void TaskWidget::onFirstListContextMenuClick(int id)
{
    QListWidgetItem* item = ui.firstListWidget->itemAt(mFirstListClickedAt);
    if (item == 0)
    {
        LOG_ERROR() << "Clicked item not found.";
        return;
    }

    //Apply the command to the list item
    useListCommand(item, (EListCommand) id);
}

void TaskWidget::showSecondListContextMenu(QPoint pos)
{
    QListWidgetItem* item = ui.secondListWidget->itemAt(pos);
    if (item == 0)
        return;

    if(boost::shared_ptr<SimulationTask> task = mTask.lock())
    {
        //Show menu with available scripts
        QMenu menu(this);
        QAction* action;
        NumberedActionReceiver* numberedReceiver;
        clearActionReceivers();
        
        //Add context commands
        int index = getItemIndex(item, ui.secondListWidget);

        //move entry up
        numberedReceiver = new NumberedActionReceiver(LC_MOVE_UP);
        mListNumberedActionReceivers.push_back(numberedReceiver);
        action = menu.addAction(tr("move entry up"), numberedReceiver, SLOT(receiveAction()));
        connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
        if (index <= 0) action->setEnabled(false);

        //move entry down
        numberedReceiver = new NumberedActionReceiver(LC_MOVE_DOWN);
        mListNumberedActionReceivers.push_back(numberedReceiver);
        action = menu.addAction(tr("move entry down"), numberedReceiver, SLOT(receiveAction()));
        connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
        if (index >= mSecondList.size()-2) action->setEnabled(false);

        //insert entry
        numberedReceiver = new NumberedActionReceiver(LC_ADD_ENTRY);
        mListNumberedActionReceivers.push_back(numberedReceiver);
        action = menu.addAction(tr("add entry"), numberedReceiver, SLOT(receiveAction()));
        connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));

        //remove entry
        numberedReceiver = new NumberedActionReceiver(LC_REMOVE_ENTRY);
        mListNumberedActionReceivers.push_back(numberedReceiver);
        action = menu.addAction(tr("remove entry"), numberedReceiver, SLOT(receiveAction()));
        connect(numberedReceiver, SIGNAL(actionReceived(int)), this, SLOT(onFirstListContextMenuClick(int)));
        if (index == mSecondList.size()-1) action->setEnabled(false);

        //Open menu
        mSecondListClickedAt = pos;
        menu.exec(ui.secondListWidget->mapToGlobal(pos));
    }
}

void TaskWidget::onSecondListContextMenuClick(const QString& entry)
{
}

void TaskWidget::onSecondListContextMenuClick(int id)
{
    QListWidgetItem* item = ui.secondListWidget->itemAt(mSecondListClickedAt);
    if (item == 0)
    {
        LOG_ERROR() << "Clicked item not found.";
        return;
    }

    //Apply the command to the list item
    useListCommand(item, (EListCommand) id);
}

void TaskWidget::showWidgetContextMenu(QPoint pos)
{
    //Show menu with commands
    QMenu menu(this);
    QAction* action;
    bool changingEnabled = mSimulationManager->simulationInitialized();
    bool deletingEnabled = mSimulationManager->simulationInitialized() && 
                          !mSimulationManager->simulationRunning() && 
                          !mSimulationManager->simulationPaused();
    
    //Add commands
    action = menu.addAction(tr("Copy Task"), this, SLOT(copyTask()));
    action->setEnabled(changingEnabled);
    action->setToolTip(tr("Create a copy of the task with the same parameters and a new name."));
    action = menu.addAction(tr("Delete Task"), this, SLOT(deleteTask()));
    action->setEnabled(deletingEnabled);
    action->setToolTip(tr("Delete the task from the simulation manager. Cannot be done in a running simulation."));

    //Open menu
    menu.exec(mapToGlobal(pos));
}

} //SimulationFrameUtil
