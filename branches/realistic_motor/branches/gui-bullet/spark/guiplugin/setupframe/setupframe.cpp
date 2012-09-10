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

#include "setupframe.h"
#include "export.h"

#include <simulationmanager.h>
#include <simulationsetup.h>
#include <pluginmanager.h>
#include <simulation.h>
#include <setupdialog.h>
#include <numberedactionreceiver.h>

#include <QMenu>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QStackedWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QModelIndex>
#include <QCursor>
#include <QFileSystemModel>

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(SetupFrame, "setupframe", "Setup", 1)

//Static class information
void SetupFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION, 
        tr("The setup frame can be used to quickly change a simulation setup, including the one currently initialized.\n" \
           "The header list contains all simulation setups found in the setup include directories, but it can load and save any simulation setup file."));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":book");

    //Tags
    QStringList tags;
    tags << "setup" << "plugin";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SetupFrame::SetupFrame() :
    mSettings(getCarbon()->getSettings())
{
    //UI Setup
    ui.setupUi(this);
    loadFrameDesign();

    //Default values
    Carbon* carbon = getCarbon();
    mSimulationManager = carbon->getSimulationManager();
    mSetupChanged = false;
    mReactToChangeSetup = true;
    mReactToEditListEntry = true;
    mReactToEditTaskListEntry = true;
    mSaving = false;
    mChosenLoadPlugin = -1;
    mInitializedSetupIndex = -1;
    mChosenUnloadPlugin = -1;
    mChosenTask = -1;
    mLastSetupIndex = -1;
    mClosed = false;
    mChosenListEntry = -1;
    mChosenListEntryIllegal = false;
    mChosenTaskListEntry = -1;
    mChosenTaskListEntryIllegal = false;
    mPluginListItemIndex = QModelIndex();

    //Settings
    loadDefaultSettings();
    loadSettings();

    //Load Setup (display)
    updateSetupListDisplay();
    loadDefaultSetup();

    //File System Model for Server & Tasks file system explorer view
    mFileSystemModel = new QFileSystemModel();
    mFileSystemModel->setRootPath("");
    mFileSystemModel->setNameFilters(mSimulationManager->getFileExtensions());
    ui.tasksExplorerTreeView->setModel(mFileSystemModel);
    ui.tasksExplorerTreeView->hideColumn(1); //hide size
    ui.tasksExplorerTreeView->hideColumn(2); //hide type
    ui.tasksExplorerTreeView->hideColumn(3); //hide change date
    ui.tasksExplorerTreeView->setRootIndex(mFileSystemModel->index(QDir::rootPath()));
    ui.tasksExplorerTreeView->sortByColumn(0, Qt::AscendingOrder);
    //fetch data of include paths by expanding the view
    if (mPreFetchExplorerData)
    {
        for (int i = 0; i < mSimulationManager->getExecutableIncludeDirectories().size(); i++)
        {
            QModelIndex index = mFileSystemModel->index(mSimulationManager->getExecutableIncludeDirectories().at(i));
            ui.tasksExplorerTreeView->expand(index);
            while (index.parent().isValid())
            { 
                ui.tasksExplorerTreeView->expand(index); 
                index = index.parent();
            }
        }
        for (int i = 0; i < mSimulationManager->getScriptIncludeDirectories().size(); i++)
        {
            QModelIndex index = mFileSystemModel->index(mSimulationManager->getScriptIncludeDirectories().at(i));
            ui.tasksExplorerTreeView->expand(index);
            while (index.parent().isValid())
            { 
                ui.tasksExplorerTreeView->expand(index); 
                index = index.parent();
            }
        }
    }

    //SimulationManager connections
    connect(mSimulationManager, SIGNAL(setupSaved(QString, QString)), this, SLOT(updateSetupSaved(QString, QString)));

    connect(mSimulationManager, SIGNAL(setupListReloaded()), this, SLOT(updateSetupListDisplay()));
    connect(mSimulationManager, SIGNAL(setupListSingleEntryAdded(int)), this, SLOT(updateSetupListDisplay()));
    connect(mSimulationManager, SIGNAL(setupListEntryChanged(int)), this, SLOT(updateSetupListDisplaySingle(int)));

    connect(mSimulationManager, SIGNAL(aboutToInitializeSetup(QString, QString)), this, SLOT(askSaveChanges()));
    connect(mSimulationManager, SIGNAL(setupInitialized(QString, QString)), this, SLOT(updateSetupListDisplay()));

    //Carbon connections. Important to connect to the controllers shutdown signal. Otherwise the managers might be destroyed during this frames destructor 
    connect(carbon, SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));
}

SetupFrame::~SetupFrame()
{
    cleanup();

    clearActionReceivers();
    delete mFileSystemModel;
}

void SetupFrame::loadDefaultSettings()
{
    mLastDirectory = mSimulationManager->getDefaultSetupPath();
    mLastSetup = "";

    mLastServerFirst            = "rcssserver3d";
    mLastSparkProcessFirst      = "rcssserver3d";
    mLastProcessFirst           = "rcssagent3d";
    mLastServerSecond           = "rcssserver3d";
    mLastSparkProcessListEntry  = "rcssserver3d";
    mLastServerThird            = "spark";
    mLastSparkProcessThird      = "spark";
    
    mPreFetchExplorerData       = false;
}

void SetupFrame::loadSettings()
{
    if (mSettings->beginGroup(this, true)) //lock
    {
        mLastDirectory = mSettings->value("mLastDirectory", mLastDirectory).toString();

        mLastServerFirst            = mSettings->value("mLastServerFirst"           , mLastServerFirst       ).toString();
        mLastSparkProcessFirst      = mSettings->value("mLastSparkProcessFirst"     , mLastSparkProcessFirst ).toString();
        mLastProcessFirst           = mSettings->value("mLastProcessFirst"          , mLastProcessFirst      ).toString();
        mLastServerSecond           = mSettings->value("mLastServerSecond"          , mLastServerSecond      ).toString();
        mLastSparkProcessListEntry  = mSettings->value("mLastSparkProcessListEntry" , mLastSparkProcessListEntry).toString();
        mLastServerThird            = mSettings->value("mLastServerThird"           , mLastServerThird       ).toString();
        mLastSparkProcessThird      = mSettings->value("mLastSparkProcessThird"     , mLastSparkProcessThird ).toString();

        mPreFetchExplorerData       = mSettings->value("mPreFetchExplorerData"      , mPreFetchExplorerData ).toBool();
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to load Settings for SetupFrame " << getCaption();
}

void SetupFrame::saveSettings()
{
    if (mSettings->beginGroup(this, true)) //lock
    {
        mSettings->setValue("mLastDirectory", mLastDirectory);

        mSettings->setValue("mLastServerFirst"           , mLastServerFirst       );
        mSettings->setValue("mLastSparkProcessFirst"     , mLastSparkProcessFirst );
        mSettings->setValue("mLastProcessFirst"          , mLastProcessFirst      );
        mSettings->setValue("mLastServerSecond"          , mLastServerSecond      );
        mSettings->setValue("mLastSparkProcessListEntry" , mLastSparkProcessListEntry);
        mSettings->setValue("mLastServerThird"           , mLastServerThird       );
        mSettings->setValue("mLastSparkProcessThird"     , mLastSparkProcessThird );

        mSettings->setValue("mPreFetchExplorerData"      , mPreFetchExplorerData  );
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save Settings for SetupFrame " << getCaption();
}

void SetupFrame::init(const QStringList& parameters)
{
    updateSetupListDisplay();
    if (mSetupList.size() > 0)
        changeSetup(0);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool SetupFrame::checkSaved()
{
    if (!mSetupChanged)
        return true;

    if (mSaving)
        return false;

    QMessageBox messageBox;
    messageBox.setWindowTitle(QString(tr("Save Simulation Setup")));
    messageBox.setText(QString(tr("Do you want to save the changes to the current Simulation Setup?")));
    messageBox.setInformativeText(QString(tr("Setup: '%1', File: '%2'")).arg(mCurrentSetup->getName()).
                                  arg(mCurrentSetup->getFileName()));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
    messageBox.setDefaultButton(QMessageBox::No);
    messageBox.setIcon(QMessageBox::Question);
    int ret = messageBox.exec();

    //Save it
    if (ret == QMessageBox::Yes)
    {
        mReactToChangeSetup = false;
        saveSetup();
        mReactToChangeSetup = true;
        if (!mSetupChanged)
            return true;
        else
            //ask again
            return checkSaved();
    }

    //Aborting returns false
    if (ret == QMessageBox::Abort)
    {
        return false;
    }

    //Declining returns true without saving
    updateSetupChanged(false);
    return true;
}

void SetupFrame::loadDefaultSetup()
{
    if (ui.setupComboBox->count() == 0)
        return;

    //Initialize non-changing entries
    updatePluginListDisplay();

    //Plugin combo box types
    ui.pluginTypeComboBox->clear();
    for (int i = 0; i < PluginType::mMaxTypes; i++)
    {
        ui.pluginTypeComboBox->addItem(PluginType::getTypeString(PluginType::mTypes[i], false));
    }

    //Task combo box types
    ui.tasksTypeComboBox->clear();
    for (int i = 0; i < TaskDefinition::mMaxTypes; i++)
    {
        ui.tasksTypeComboBox->addItem(TaskDefinition::getTypeString(TaskDefinition::mTypes[i]));
    }

    //Task priority combo box types
    ui.tasksPriorityComboBox->clear();
    for (int i = 0; i < TaskDefinition::mMaxPriorityTypes; i++)
    {
        ui.tasksPriorityComboBox->addItem(QString("%1: %2").
                                        arg(i).
                                        arg(TaskDefinition::getPriorityTypeString(TaskDefinition::mPriorityTypes[i])));
    }

    //Change display to last initialized (current) Setup
    QString path = mSimulationManager->getLastInitializedSetupPath();
    int index = 0;
    bool found = false;
    for (auto it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if (it->get()->getFileName().compare(path) == 0)
        {
            changeSetup(index);
            found = true;
            break;
        }
        index++;
    }

    if (!found) 
    {
        if (mSetupList.size() == 0)
        {
            LOG_WARNING() << "No setups found!";
            mCurrentSetup = shared_ptr<SimulationSetup>(new SimulationSetup());
        }
        else
            changeSetup(0);
    }

    //update active/running icons
    updateSetupListIcons();
}

PluginDefinition* SetupFrame::getCurrentPlugin() const
{
    if (mChosenLoadPlugin != -1)
    {
        return const_cast<PluginDefinition*>(&*mCurrentSetup->getAddPlugins().at(mChosenLoadPlugin));
    }

    if (mChosenUnloadPlugin != -1)
    {
        return const_cast<PluginDefinition*>(&*mCurrentSetup->getRemovePlugins().at(mChosenUnloadPlugin));
    }

    return 0;
}

TaskDefinition* SetupFrame::getCurrentTask() const
{
    if (mChosenTask == -1)
        return 0;

    return const_cast<TaskDefinition*>(&*mCurrentSetup->getTaskDefinitions().at(mChosenTask));
}

void SetupFrame::unchoosePlugin()
{
    mReactToChangeSetup = false;
    
    //Plugin Type
    ui.pluginTypeComboBox->setCurrentIndex(0);
    ui.pluginTypeComboBox->setEnabled(false);

    //Caption and Name
    ui.pluginNameEdit->setText("");
    ui.pluginNameEdit->setEnabled(false);
    ui.pluginCaptionEdit->setText("");
    ui.pluginCaptionEdit->setEnabled(false);

    //Attachments
    ui.pluginAttachmentsTextEdit->clear();
    ui.pluginAttachmentsTextEdit->setEnabled(false);

    //Parameters
    ui.pluginParametersTextEdit->clear();
    ui.pluginParametersTextEdit->setEnabled(false);
    
    //Reload definition
    ui.pluginReloadComboBox->setCurrentIndex(0);
    ui.pluginReloadComboBox->setEnabled(false);

    //Name labels
    ui.pluginParametersLabel->setText(tr("Parameters"));
    ui.pluginAttachmentsLabel->setText(tr("Attachments"));
    
    mReactToChangeSetup = true;
}

void SetupFrame::choosePlugin(const PluginDefinition& ref)
{
    mReactToChangeSetup = false;

    //Plugin Type
    int found = ui.pluginTypeComboBox->findText(PluginType::getTypeString(ref.getPluginType(), false));
    if (found != -1) ui.pluginTypeComboBox->setCurrentIndex(found);
    ui.pluginTypeComboBox->setEnabled(true);

    //Caption and Name
    ui.pluginNameEdit->setText(ref.getName());
    ui.pluginNameEdit->setEnabled(true);
    ui.pluginCaptionEdit->setText(ref.getCaption());
    ui.pluginCaptionEdit->setEnabled(true);

    //Attachments
    ui.pluginAttachmentsTextEdit->clear();
    for (auto it = ref.getAttachments().begin(); it != ref.getAttachments().end(); it++)
    {
        ui.pluginAttachmentsTextEdit->appendPlainText(*it);
    }
    ui.pluginAttachmentsTextEdit->setEnabled(true);

    //Parameters
    ui.pluginParametersTextEdit->clear();
    for (auto it = ref.getParameters().begin(); it != ref.getParameters().end(); it++)
    {
        ui.pluginParametersTextEdit->appendPlainText(*it);
    }
    ui.pluginParametersTextEdit->setEnabled(true);

    //Reload definition
    switch (ref.getReloadDefinition())
    {
    case PluginDefinition::RD_INHERIT:
        ui.pluginReloadComboBox->setCurrentIndex(0);
        break;
    case PluginDefinition::RD_RELOAD:
        ui.pluginReloadComboBox->setCurrentIndex(1);
        break;
    case PluginDefinition::RD_NO_RELOAD:
        ui.pluginReloadComboBox->setCurrentIndex(2);
        break;
    default:
        LOG_ERROR() << "Illegal reload type.";
        break;
    }
    ui.pluginReloadComboBox->setEnabled(true);

    mReactToChangeSetup = true;
}

void SetupFrame::unchooseTask()
{
    mReactToChangeSetup = false;

    //Task Type
    ui.tasksTypeComboBox->setCurrentIndex(0);
    ui.tasksTypeComboBox->setEnabled(false);

    //Task Priority
    ui.tasksPriorityComboBox->setCurrentIndex((int)QThread::InheritPriority);
    ui.tasksPriorityComboBox->setEnabled(false);

    //Caption
    ui.tasksCaptionEdit->setText("");
    ui.tasksCaptionEdit->setEnabled(false);

    //Parameters
    ui.tasksParametersTextEdit->clear();
    ui.tasksParametersTextEdit->setEnabled(false);

    //Data
    ui.tasksFirstEdit->clear();
    ui.tasksFirstEdit->setEnabled(false);
    ui.tasksSecondEdit->clear();
    ui.tasksSecondEdit->setEnabled(false);
    ui.tasksThirdEdit->clear();
    ui.tasksThirdEdit->setEnabled(false);

    //Additional Data
    ui.tasksAdditionalListList->clear();
    ui.tasksAdditionalListAddButton->setEnabled(false);
    ui.tasksAdditionalListRemoveButton->setEnabled(false);
    ui.tasksAdditionalListUpButton->setEnabled(false);
    ui.tasksAdditionalListDownButton->setEnabled(false);
    ui.tasksFirstLoadButton->setEnabled(false);
    ui.tasksSecondLoadButton->setEnabled(false);
    ui.tasksThirdLoadButton->setEnabled(false);

    mChosenTask = -1;
    mReactToChangeSetup = true;

    taskFirstCheckText(ui.tasksFirstEdit->text());
    taskSecondCheckText(ui.tasksSecondEdit->text());
    taskThirdCheckText(ui.tasksThirdEdit->text());
}

void SetupFrame::chooseTask(const TaskDefinition& ref)
{
    mReactToChangeSetup = false;

    bool usesList = false;

    //Labels
    TaskDefinition::ETaskType type = ref.getType();
    if (type == TaskDefinition::TT_PLUGINTHREAD)
    {
        ui.tasksFirstLabel ->setText(tr("Class:"));
        ui.tasksSecondLabel->setText(tr("Caption:"));
        ui.tasksThirdLabel ->setText(tr("Type:"));
        ui.tasksAdditionalListLabel->setText(tr("References"));
        ui.tasksFirstEdit->setEnabled(true);
        ui.tasksSecondEdit->setEnabled(true);
        ui.tasksThirdEdit->setEnabled(true);

        ui.tasksFirstLoadButton->setEnabled(true);
        ui.tasksSecondLoadButton->setEnabled(false);
        ui.tasksThirdLoadButton->setEnabled(false);
        ui.tasksFirstLoadButton->setVisible(true);
        ui.tasksSecondLoadButton->setVisible(false);
        ui.tasksThirdLoadButton->setVisible(false);
        ui.tasksFirstLoadButton->setIcon(QIcon(":book"));

        usesList = false;
    }
    else if (type == TaskDefinition::TT_PROCESS)
    {
        ui.tasksFirstLabel ->setText(tr("First:"));
        ui.tasksSecondLabel->setText(tr("Main Script:"));
        ui.tasksThirdLabel ->setText(tr("Spark Script:"));
        ui.tasksAdditionalListLabel->setText(tr("Additional Scripts:"));

        ui.tasksFirstEdit->setEnabled(true);
        ui.tasksSecondEdit->setEnabled(false);
        ui.tasksThirdEdit->setEnabled(false);

        ui.tasksFirstLoadButton->setEnabled(true);
        ui.tasksSecondLoadButton->setEnabled(true);
        ui.tasksThirdLoadButton->setEnabled(true);
        ui.tasksFirstLoadButton->setVisible(true);
        ui.tasksSecondLoadButton->setVisible(true);
        ui.tasksThirdLoadButton->setVisible(true);
        ui.tasksFirstLoadButton->setIcon(QIcon(":folder"));

        usesList = false;
    }
    else
    {
        ui.tasksFirstLabel->setText(tr("First:"));
        ui.tasksSecondLabel->setText(tr("Main Script:"));
        ui.tasksThirdLabel->setText(tr("Spark Script:"));
        ui.tasksAdditionalListLabel->setText(tr("Additional Scripts:"));

        ui.tasksFirstEdit->setEnabled(true);
        ui.tasksSecondEdit->setEnabled(true);
        ui.tasksThirdEdit->setEnabled(true);

        ui.tasksFirstLoadButton->setEnabled(true);
        ui.tasksSecondLoadButton->setEnabled(true);
        ui.tasksThirdLoadButton->setEnabled(true);
        ui.tasksFirstLoadButton->setVisible(true);
        ui.tasksSecondLoadButton->setVisible(true);
        ui.tasksThirdLoadButton->setVisible(true);
        ui.tasksFirstLoadButton->setIcon(QIcon(":folder"));

        usesList = true;
    }

    //Task Type
    int found = ui.tasksTypeComboBox->findText(TaskDefinition::getTypeString(ref.getType()));
    if (found != -1) ui.tasksTypeComboBox->setCurrentIndex(found);
    ui.tasksTypeComboBox->setEnabled(true);

    //Task Priority
    ui.tasksPriorityComboBox->setCurrentIndex(ref.getPriority());
    ui.tasksPriorityComboBox->setEnabled(true);

    //Caption
    ui.tasksCaptionEdit->setText(ref.getName());
    ui.tasksCaptionEdit->setEnabled(true);

    //Parameters
    ui.tasksParametersTextEdit->clear();
    for (auto it = ref.getSecondList().begin(); it != ref.getSecondList().end(); it++)
    {
        ui.tasksParametersTextEdit->appendPlainText(*it);
    }
    ui.tasksParametersTextEdit->setEnabled(true);

    //ListEntrys
    ui.tasksFirstEdit->setText(ref.getFirst());
    ui.tasksSecondEdit->setText(ref.getSecond());
    ui.tasksThirdEdit->setText(ref.getThird());

    //Additional ListEntries
    ui.tasksAdditionalListList->clear();
    if (usesList)
    {
        for (QStringList::const_iterator it = ref.getFirstList().begin(); it != ref.getFirstList().end(); it++)
        {
            //Add to list
            ui.tasksAdditionalListList->addItem(*it);
            //Update ListEntry item color
            QListWidgetItem* item = ui.tasksAdditionalListList->item(ui.tasksAdditionalListList->count()-1);
            if (QFileInfo(*it).exists() || mSimulationManager->hasScript(*it))
                //ListEntry found, black text
                item->setTextColor(QColor(Qt::black));
            else
                //ListEntry not found, red text
                item->setTextColor(QColor(Qt::red));
        }
        ui.tasksAdditionalListAddButton->setEnabled(true);
        ui.tasksAdditionalListRemoveButton->setEnabled(true);
        ui.tasksAdditionalListUpButton->setEnabled(true);
        ui.tasksAdditionalListDownButton->setEnabled(true);
    }
    else
    {
        ui.tasksAdditionalListAddButton->setEnabled(false);
        ui.tasksAdditionalListRemoveButton->setEnabled(false);
        ui.tasksAdditionalListUpButton->setEnabled(false);
        ui.tasksAdditionalListDownButton->setEnabled(false);
    }

    mReactToChangeSetup = true;
}

void SetupFrame::setUniqueTaskName(TaskDefinition& task)
{
    QString compareName = task.getName();

    int run = 1;
    bool search = true;
    while (search)
    {
        bool foundequal = false;
        for (auto it = mCurrentSetup->getTaskDefinitions().begin(); it != mCurrentSetup->getTaskDefinitions().end(); it++)
        {
            //Skip self
            if (&**it == &task)
                continue;
        
            if (it->get()->getName().compare(compareName) == 0)
            {
                compareName = task.getName();
                compareName.append(QString("%1").arg(run));
                run++;
                foundequal = true;
                break;
            }
        }
        if (!foundequal)
            search = false;
    }

    if (run != 1)
    {
        //name was changed, store
        task.setName(compareName);
    }
}

void SetupFrame::updateSetupChanged(bool state)
{
    bool addEditText = false;
    bool removeEditText = false;
    if (mSetupChanged == false && state == true)
        addEditText = true;
    if (mSetupChanged == true && state == false)
        removeEditText = true;

    //update value
    mSetupChanged = state;

    //update view
    int index = (mLastSetupIndex == -1 ? ui.setupComboBox->currentIndex() : mLastSetupIndex);
    if (addEditText)
    {
        QString val = QString("%1 (unsaved)").arg(mSetupList.at(index)->getName());
        ui.setupComboBox->setItemText(index, val);
    }
    if (removeEditText)
    {
        QString val = QString("%1").arg(mSetupList.at(index)->getName());
        ui.setupComboBox->setItemText(index, val);
    }
}

void SetupFrame::showExplorerTreeViewContextMenu(QPoint pos, QTreeView* view, QModelIndex& modelIndex)
{
     QMenu menu(this);
     QAction* action;
     clearActionReceivers();

     //Update model index
     modelIndex = view->indexAt(pos);
     bool isValid = modelIndex.isValid();
     bool addEnabled = isValid && (mChosenTask != -1); //If there is no task chosen, actions are disabled 
        
     //Add chosen item, connect with the set text functions
     action = menu.addAction(tr("Load as executable"), this, SLOT(setTaskFirstTextFromExplorer()));
     action->setEnabled(addEnabled);
     action = menu.addAction(tr("Load as main script"), this,SLOT(setTaskSecondTextFromExplorer()));
     action->setEnabled(addEnabled);
     action = menu.addAction(tr("Load as spark script"), this, SLOT(setTaskThirdTextFromExplorer()));
     action->setEnabled(addEnabled);
     menu.addSeparator();

     //Executable Paths
     int run = 0;
     for (QStringList::ConstIterator it = mSimulationManager->getExecutableIncludeDirectories().begin(); it != mSimulationManager->getExecutableIncludeDirectories().end(); it++)
     {
         NumberedActionReceiver* newReceiver = new NumberedActionReceiver(run);
         mExecutablePathActionReceivers.push_back(newReceiver);
         action = menu.addAction(QString(tr("Open: ")) + *it, newReceiver, SLOT(receiveAction()));
         connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(onTasksExecutablePathClick(int)));
         run++;
     }
     menu.addSeparator();

     //Script Paths
     run = 0;
     for (QStringList::ConstIterator it = mSimulationManager->getScriptIncludeDirectories().begin(); 
         it != mSimulationManager->getScriptIncludeDirectories().end(); it++)
     {
         NumberedActionReceiver* newReceiver = new NumberedActionReceiver(run);
         mScriptPathActionReceivers.push_back(newReceiver);
         action = menu.addAction(QString(tr("Open: ")) + *it, newReceiver, SLOT(receiveAction()));
         connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(onTasksScriptPathClick(int)));
         run++;
     }

     menu.exec(view->mapToGlobal(pos));
}

void SetupFrame::scrollToRegisteredExplorerFile(int number, bool executable) const
{
    QTreeView* view = ui.tasksExplorerTreeView; // One can easily add additional views
    int size = executable ? mSimulationManager->getExecutableIncludeDirectories().size() : mSimulationManager->getScriptIncludeDirectories().size();
    if (size > number && number >= 0)
    {
        //Scroll to and expand target
        QDir dir(executable ? mSimulationManager->getExecutableIncludeDirectories().at(number) : mSimulationManager->getScriptIncludeDirectories().at(number));
        view->setCurrentIndex(mFileSystemModel->index(dir.absolutePath().append('/')));
        view->expand(view->currentIndex());
        view->scrollTo(view->currentIndex());

        //Scroll as far down as needed to see folder content, and up as far as needed to see root again
        QModelIndex currentIndex = view->currentIndex();
        int children = mFileSystemModel->rowCount(currentIndex);
        if (children > 0)
        {
            QModelIndex childIndex = mFileSystemModel->index(children-1, 0, currentIndex);
            view->scrollTo(childIndex);
            view->scrollTo(view->currentIndex());
        }
    }
}

void SetupFrame::setLineEditTextFromExplorer(const QTreeView* view, const QModelIndex& modelIndex, QLineEdit* target,bool executable, bool absolute)
{
    if (!modelIndex.isValid())
        return;

    QVariant var = mFileSystemModel->data(modelIndex, Qt::DisplayRole);
    QString fullpath = mFileSystemModel->filePath(modelIndex);
    QString abspath;

    if (!absolute)
    {
        if (executable)
            abspath = mSimulationManager->findExecutable(var.toString());
        else
            abspath = mSimulationManager->findScript(var.toString());

        if (fullpath.compare(abspath) == 0)
        {
            //Item is contained and identical, add relative
        }
        else
        {
            //Item is either not contained or not identical, add absolute
            absolute = true;
        }
    }
    
    //Apply changes, text is checked automatically, but change has to be called manually
    target->setText(absolute ? fullpath : var.toString());

    updateSetupChanged();
}

void SetupFrame::clearActionReceivers()
{
    while (mScriptPathActionReceivers.size() > 0)
    {
        mScriptPathActionReceivers.back()->deleteLater();
        mScriptPathActionReceivers.pop_back();
    }

    while (mExecutablePathActionReceivers.size() > 0)
    {
        mExecutablePathActionReceivers.back()->deleteLater();
        mExecutablePathActionReceivers.pop_back();
    }

    while (mLoadTaskActionReceivers.size() > 0)
    {
        mLoadTaskActionReceivers.back()->deleteLater();
        mLoadTaskActionReceivers.pop_back();
    }
}

void SetupFrame::loadTaskEntry(int type, QLineEdit* target)
{
    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    QString empty("");
    QString& lastRef = empty;
    QString& defValueRef = empty;
    QString header;
    QString tooltip;

    if (type == 0) //First
    {
        defValueRef = def->getFirst();
        header = QString(tr("Define %1 executable.")).arg(TaskDefinition::getTypeString(def->getType()));
        tooltip = tr("Choose an executable file.");

        if (def->getType() == TaskDefinition::TT_SPARKPROCESS)
        {
            lastRef = mLastSparkProcessFirst;
        }
        else if (def->getType() == TaskDefinition::TT_SERVERTHREAD)
        {
            lastRef = mLastServerFirst;
        }
        else
        {
            lastRef = mLastProcessFirst;
        }
    }
    else if (type == 1) //Primary ListEntry
    {
        defValueRef = def->getSecond();
        header = QString(tr("Define %1 main script.")).arg(TaskDefinition::getTypeString(def->getType()));
        tooltip = tr("Choose a script file.");

        if (def->getType() == TaskDefinition::TT_SERVERTHREAD)
        {
            lastRef = mLastServerSecond;
        }
        else
        {
            lastRef = mLastServerSecond;
        }
    }
    else if (type == 2) //Secondary ListEntry
    {
        defValueRef = def->getThird();
        header = QString(tr("Define %1 secondary script.")).arg(TaskDefinition::getTypeString(def->getType()));
        tooltip = tr("Choose a script file.");

        if (def->getType() == TaskDefinition::TT_SERVERTHREAD)
        {
            lastRef = mLastServerThird;
        }
        else
        {
            lastRef = mLastServerThird;
        }
    }
    else
    {
        LOG_ERROR() << "Illegal task entry type: " << type;
        return;
    }

    QString path;
    if (defValueRef.compare("") == 0)
        path = lastRef;
    else
        path = (type == 0 ? mSimulationManager->findExecutable(defValueRef) : mSimulationManager->findScript(defValueRef));

    bool hasPath = true;
    if (path.compare("") == 0)
        hasPath = false;

    QFileInfo info;
    if (hasPath)
    {
        info = QFileInfo(path);
        if (!info.exists())
        {
            path = lastRef;
            if (path.compare("") == 0)
                hasPath = false;
            else
                info = QFileInfo(path);
        }
    }

    QFileDialog dialog(this, header, path, mSimulationManager->getExecutableExtensionText());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setLabelText(QFileDialog::Accept, tr("Ok"));
    dialog.setToolTip(tooltip);
    dialog.setViewMode(QFileDialog::Detail);

    if (hasPath)
        dialog.selectFile(info.fileName());

    int returning = dialog.exec();
    if (returning != 0)
    {
        //Add relative or absolute
        QFileInfo foundInfo(dialog.selectedFiles().at(0));
        QString saveName;
        if ((type == 0 && mSimulationManager->hasExecutable(foundInfo.fileName())) || 
             type != 0 && mSimulationManager->hasScript(foundInfo.fileName()))
            saveName = foundInfo.fileName();
        else
            saveName = foundInfo.absoluteFilePath();
        
        //Set value
        if (type == 0) def->setFirst(saveName);
        else if (type == 1) def->setSecond(saveName);
        else if (type == 2) def->setThird(saveName);

        lastRef = foundInfo.absoluteFilePath();
        target->setText(saveName);

        //Notify that setup changed
        updateSetupChanged();
    }
}

void SetupFrame::showLoadTaskContextMenu(QPoint pos)
{
    QMenu menu(this);
    QAction* action;
    clearActionReceivers();
    PluginManager* manager = getCarbon()->getPluginManager();
   
    //Add plugins identifications to list
    std::vector<AbstractPlugin*> allplugins = manager->findPlugins("","");
    for (auto it = allplugins.begin(); it != allplugins.end(); it++)
    {
        NumberedActionReceiver* newReceiver = new NumberedActionReceiver((*it)->getPluginId());
        mLoadTaskActionReceivers.push_back(newReceiver);
        QString plugin = (*it)->getCaption() + " (" + (*it)->getClassName() + ", " + (*it)->getPluginTypeString() + ")";
        action = menu.addAction(plugin, newReceiver, SLOT(receiveAction()));
        connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(onLoadTaskContextMenuClick(int)));
    }
   
    //Open menu
    menu.exec(pos);
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SetupFrame::askSaveChanges()
{
    checkSaved();
}

void SetupFrame::addSetup()
{
    if (checkSaved())
    {
        DefineSetupDialog dialog(this);
        dialog.setWindowTitle(tr("Create new Simulation Setup"));
        dialog.setToolTip(tr("Create a new Simulation Setup definition. File may remain empty and will be asked to be specified later."));
        dialog.setName("NewSetup");
        dialog.setFile("");

        int returning = dialog.exec();
        if (returning == QDialog::Rejected)
            return; //Abort
        
        //Save the new setup with Simulation manager, in case an included setup was replaced
        shared_ptr<SimulationSetup> newSetup(new SimulationSetup(dialog.getName(), dialog.getFilePath()));
        shared_ptr<SimulationSetup> savedSetup = mSimulationManager->saveSimulationSetup(newSetup);
        
        if (!savedSetup.get())
        {
            LOG_ERROR() << "Could not add new setup.";
            return;
        }
        
        //Add setup to list or replace old one
        int pos = insertSetup(savedSetup);
        
        //Choose the added or updated setup
        if (pos >= 0 && pos < (int)mSetupList.size())
             changeSetup(pos);
        
        updateSetupChanged(true);
    }
}

int SetupFrame::insertSetup(boost::shared_ptr<SimulationSetup> setup, bool copy)
{
    //Replace in setup list if existant
    bool found = false;
    int index = 0;
    for (auto it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if (it->get()->getFileName().compare(setup.get()->getFileName()) == 0)
        {
            if (copy)
                *it = shared_ptr<SimulationSetup>(new SimulationSetup(*setup));
            else
                *it = setup;

            found = true;
            LOG_INFO() << "Replaced SimulationSetup " << setup->getName() << " at " << setup->getFileName() << ".";
            break;
        }
        index++;
    }

    //If not existant, add at the end
    if (!found)
    {
        if (copy)
            mSetupList.push_back(shared_ptr<SimulationSetup>(new SimulationSetup(*setup)));
        else
            mSetupList.push_back(setup);

        LOG_INFO() << "Added new SimulationSetup " << setup->getName() << " at " << setup->getFileName() << ".";
        index = mSetupList.size() - 1;
    }
    
    //Update the list (reload setups from the simulation manager and update the combo box)
    updateSetupListDisplay();
    
    return index;
}

void SetupFrame::removeSetup()
{
    if (checkSaved())
    {
        //Determine next index
        int currentIndex = ui.setupComboBox->currentIndex();
        int nextIndex = currentIndex;                
        if (currentIndex == (mSetupList.size() - 1))  //Removing at last index -> new index is reduced by 1
            nextIndex = currentIndex -1;

        //Delete the setup
        if (mSimulationManager->getSetupIndexByFilePath(mCurrentSetup->getFileName()) != -1)
        {
            //Remove from simulation manager (if contained). Also asks to delete the file.
            bool success = mSimulationManager->removeSimulationSetup(mCurrentSetup->getFileName());
            if (!success)
            {
                LOG_ERROR() << "Failed to remove setup from simulation manager.";
                return;
            }
        }
        else
        {
            //Ask to delete the setup file
            QMessageBox messageBox;
            messageBox.setWindowTitle(QString(tr("Delete Setup")));
            messageBox.setText(QString(tr("Do you want to delete the Simulation Setup file '%1'?")).arg(mCurrentSetup->getFileName()));
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
            messageBox.setDefaultButton(QMessageBox::No);
            messageBox.setIcon(QMessageBox::Question);
            int ret = messageBox.exec();

            bool deleteThis = false;
            if (ret == QMessageBox::Yes)
            {
                deleteThis = true;
            }
            if (ret == QMessageBox::Abort)
            {
                return;
            }

            if (deleteThis)
            {
                QFileInfo info(mCurrentSetup->getFileName());
                QDir dir = info.absoluteDir();
                QString file = info.fileName();
            
                if (!info.exists())
                {
                    //Setup has no file definition, skip
                    LOG_INFO() << "Setup has no file to delete.";
                }
                else
                {
                    bool removed = info.absoluteDir().remove(info.fileName());
                    if (removed)
                        LOG_INFO() << "Setup file " << info.absoluteFilePath() << " deleted.";
                    else
                        LOG_INFO() << "Could not delete file " << info.absoluteFilePath() << ".";
                }
            }
        }

        //Remove from setup list
        mSetupList.erase(mSetupList.begin() + currentIndex);

        //Choose the old chosen setup (or the first if the selected one was deleted)
        ui.setupComboBox->setCurrentIndex(nextIndex);
    }
}

void SetupFrame::renameSetup()
{
    if (checkSaved())
    {
       DefineSetupDialog dialog(this);
       dialog.setWindowTitle(tr("Rename Simulation Setup"));
       dialog.setToolTip(tr("Rename a new Simulation Setup definition. File may remain empty and will be asked to be specified later."));
       dialog.setName(mCurrentSetup->getName());
       dialog.setFile(mCurrentSetup->getFileName());
       QString oldfilename = mCurrentSetup->getFileName();
       int returning = dialog.exec();

       if (returning == QDialog::Rejected)
       {
           //Abort
           return;
       }

       mCurrentSetup->setName(dialog.getName());
       mCurrentSetup->setFileName(dialog.getFilePath());

       int index = ui.setupComboBox->currentIndex();
       bool updated = mSimulationManager->updateSimulationSetup(mCurrentSetup->getFileName(), *mCurrentSetup, true);

       if (updated)
       {
           updateSetupListDisplay();

           //Choose the old setup index
           ui.setupComboBox->setCurrentIndex(index);
       }
       else
       {
           LOG_INFO() << "Changing data failed.";
       }

       updateSetupChanged();
    }
}

void SetupFrame::changeSetup(int index)
{
    if (!mReactToChangeSetup)
        return;

    //Setup List up-to-date?
    if (ui.setupComboBox->count() != mSimulationManager->getSetupList().size())
    {
        updateSetupListDisplay();
    }

    //Check if current setup was changed and save before changing
    bool change = checkSaved();
    if (!change)
    {
        //revert back to old selection (unsaved)
        mReactToChangeSetup = false;
        ui.setupComboBox->setCurrentIndex(mLastSetupIndex);
        mReactToChangeSetup = true;
        return;
    }

    //Assert correct combo box selection
    if (ui.setupComboBox->currentIndex() != index)
    {
        mReactToChangeSetup = false;
        ui.setupComboBox->setCurrentIndex(index);
        mReactToChangeSetup = true;
    }

    //Change current setup by copying it locally
    mCurrentSetup = shared_ptr<SimulationSetup>(mSetupList.at(index));
    mLastSetupIndex = index;

    //Reset complete view
    updateSetupComboBoxItems();
    updateSetupDisplay();
}

void SetupFrame::initSetup()
{
    //Initialize setup (may close this)

    if (ui.setupComboBox->count() > 0)
        mSimulationManager->initializeSimulation(mCurrentSetup->getName());

    updateSetupListIcons();
}

void SetupFrame::updateSetupDisplay()
{
    //Plugins
    updatePluginDisplay();

    //Tasks
    updateTaskDisplay();
}

void SetupFrame::updateTaskDisplay()
{
    //Reset view
    unchooseTask();

    //Update list
    updateTaskListDisplay();
}

void SetupFrame::updatePluginDisplay()
{
    //Plugin Values
    mReactToChangeSetup = false;
    ui.loadPluginsReloadAllCheckBox->setChecked(mCurrentSetup->getReloadAll());
    ui.unloadPluginsRemoveAllCheckBox->setChecked(mCurrentSetup->getRemoveAll());
    mReactToChangeSetup = true;

    //Plugins lists
    unchoosePlugin();
    updateLoadPluginsDisplay();
    updateUnloadPluginsDisplay();
}

void SetupFrame::updateLoadPluginsDisplay()
{
    mReactToChangeSetup = false;

    int currentIndex = mChosenLoadPlugin;

    //Refresh
    ui.loadPluginsListWidget->clear();
    for (SimulationSetup::TPluginDefinitions::const_iterator it = mCurrentSetup->getAddPlugins().begin(); it != mCurrentSetup->getAddPlugins().end(); it++)
    {
        //Plugin Type to string
        QString typeStr = PluginType::getTypeString(it->get()->getPluginType(), false);

        //Reload definition to string
        QString reloadStr("");
        if (it->get()->getReloadDefinition() != PluginDefinition::RD_INHERIT)
        {
            reloadStr.append(" *");
            if (it->get()->getReloadDefinition() == PluginDefinition::RD_RELOAD)
                reloadStr.append("reload");
            if (it->get()->getReloadDefinition() == PluginDefinition::RD_NO_RELOAD)
                reloadStr.append("noreload");
            reloadStr.append("*");
        }

        //Current amount to string
        int currentAmount = 0;
        for (auto in_it = mCurrentSetup->getAddPlugins().begin(); in_it != mCurrentSetup->getAddPlugins().end(); in_it++)
        {
            if (in_it->get()->getName().compare(it->get()->getName()) == 0)
                currentAmount++;
        }

        //Maximum amount to string
        int maxAmount = 0;
        QString maxAmountString;
        bool unknownClass = false;
        bool illegalType = false;
        EPluginType expectedType = PT_NONE;
        int classId = PluginFactory::getFactory().getClassIdByName(it->get()->getName());
        if (classId <= 0)
        {
            maxAmountString = "?";
            maxAmount = INT_MAX;
            unknownClass = true;
        }
        else
        {
            maxAmount = PluginFactory::getFactory().getMaxInstanceCount(classId);
            maxAmountString = QString("%1").arg(maxAmount);

            expectedType = PluginFactory::getFactory().getPluginType(classId);
            if (expectedType != it->get()->getPluginType())
            {
                illegalType = true;
            }
        }

        //Add item with combined string
        ui.loadPluginsListWidget->addItem(QString("%1 (%3: '%2')%4 %5/%6").
                                         arg(it->get()->getCaption()).
                                         arg(it->get()->getName()).
                                         arg(typeStr).
                                         arg(reloadStr).
                                         arg(currentAmount).
                                         arg(maxAmountString)
                                         );
        
        //Assemble tooltip error string
        QListWidgetItem* item = ui.loadPluginsListWidget->item(ui.loadPluginsListWidget->count()-1);
        bool illegalAmount = currentAmount > maxAmount;
        if (illegalAmount || unknownClass || illegalType)
        {
            //Some Error -> red text and error tooltip
            QString errorString = (tr("Error: ")) +
                                  (illegalAmount ? tr("Too many instances.") : "") + 
                                  (illegalAmount && (unknownClass || illegalType) ? " " : "") + 
                                  (unknownClass ? tr("Unknown class.") : "") +
                                  (unknownClass && illegalType ? " " : "") +
                                  (illegalType ? tr("Illegal plugin type for this plugin. Expected ") + PluginType::getTypeString(expectedType, false) : "");
            
            item->setTextColor(Qt::red);
            item->setToolTip(errorString);
        }
        else
        {
            //No Error -> black text and information tooltip
            item->setTextColor(Qt::black);
            item->setToolTip(item->listWidget()->toolTip());
        }
    }

    mReactToChangeSetup = true;

    //Choose previous one
    if (currentIndex != -1)
        ui.loadPluginsListWidget->setCurrentRow(currentIndex);
}

void SetupFrame::updateUnloadPluginsDisplay()
{
    mReactToChangeSetup = false;

    //Remember
    int currentIndex = ui.unloadPluginsListWidget->currentIndex().row();
    QString chosenPlugin = (currentIndex == -1 ? "" : ui.unloadPluginsListWidget->item(currentIndex)->text());

    //Refresh
    ui.unloadPluginsListWidget->clear();
    for (SimulationSetup::TPluginDefinitions::const_iterator it = mCurrentSetup->getRemovePlugins().begin(); 
        it != mCurrentSetup->getRemovePlugins().end(); it++)
    {
        QString complete;
        if (it->get()->getCaption().compare("") != 0)
        {
            complete.append(it->get()->getCaption());
            complete.append("(Frame");
        }
        if (it->get()->getName().compare("") != 0)
        {
            complete.append(" '");
            complete.append(it->get()->getName());
            complete.append("'");
        }
        complete.append(")");

        ui.unloadPluginsListWidget->addItem(complete);
    }

    mReactToChangeSetup = true;

    //Choose previous one
    if (currentIndex != -1)
    for (int i = 0; i < ui.unloadPluginsListWidget->count(); i++)
    {
        if (ui.unloadPluginsListWidget->item(i)->text() == chosenPlugin)
        {
            ui.unloadPluginsListWidget->setCurrentRow(i);
            break;
        }
    }
}

void SetupFrame::updateSetupSaved(QString setup, QString path)
{
    //Did save call originate from setup frame? then return
    if (mSaving)
        return;

    //TODO: update list
}

void SetupFrame::updatePluginListDisplay()
{
    mReactToChangeSetup = false;
    ui.pluginListListWidget->clear();

    for (auto it = PluginFactory::getFactory().getInstantiators().begin(); it != PluginFactory::getFactory().getInstantiators().end(); it++)
    {
        AbstractPluginInstantiator* instantiator = it->second;

        ui.pluginListListWidget->addItem(QString(tr("%1: %2 (%3, max. %4)", "max. for maximum")).
                                         arg(it->first).
                                         arg(instantiator->getName()).
                                         arg(PluginType::getTypeString(instantiator->getPluginType(), false)).
                                         arg(instantiator->getMaxInstanceCount())
                                         );
    }

    mReactToChangeSetup = true;
}

void SetupFrame::startSetup()
{
    mSimulationManager->runSimulation();
}

void SetupFrame::stopSetup()
{
    mSimulationManager->stopSimulation();
}

void SetupFrame::reloadSetups()                     
{
    if (checkSaved())
    {
        LOG_DEBUG() << "Reloading setup list.";
        mSimulationManager->loadSimulationSetups();
    }
}

void SetupFrame::loadSetup()                        
{
    if (checkSaved())
    {
        int lastSetupIndex = -1;

        QFileDialog dialog(this, tr("Load setups from files."), mLastDirectory, mSimulationManager->getDefaultSetupExtensionText());
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setLabelText(QFileDialog::Accept, tr("Load"));
        dialog.setToolTip(tr("Add single files to the setup list."));
        dialog.setViewMode(QFileDialog::Detail);

        int returning = dialog.exec();
        if (returning != 0)
        {
            QStringList files = dialog.selectedFiles();

            int fileCount = 0;
            int dirCount = 0;
            for (int i = 0; i < files.size(); i++)
            {
                QFileInfo info(files.at(i));

                if (!info.isReadable())
                {
                    LOG_WARNING() << (info.isDir() ? "Directory " : "File ") << files.at(i) << " is not readable.";
                    continue;
                }

                if (info.isDir())
                    dirCount++;
                else if (info.isFile())
                    fileCount++;
            }

            QMessageBox messageBox;
            messageBox.setText(QString(tr("Do you want to add the selected files to the setup include paths?")));
            QString list;
            bool first = true;
            for (QStringList::Iterator it = files.begin(); it != files.end(); it++)
            {
                if (!first)
                    list.append(", ");
                list.append(*it);
            }
            messageBox.setInformativeText(QString(tr("Chosen files to load: %1")).arg(list));
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            messageBox.setDefaultButton(QMessageBox::No);
            messageBox.setIcon(QMessageBox::Question);
            messageBox.setWindowTitle("Load Simulation Setups");

            int ret = messageBox.exec();
            bool loadAlways = ret == QMessageBox::Yes;

            for (int i = 0; i < files.size(); i++)
            {
                shared_ptr<SimulationSetup> loadedSetup = mSimulationManager->loadSimulationSetup(files.at(i), loadAlways);
                
                //Insert into setup list
                lastSetupIndex = insertSetup(loadedSetup);
            }

            if (lastSetupIndex != -1)
            {
                changeSetup(lastSetupIndex);
            }
        }
    }
}

void SetupFrame::loadSetup(QString path)
{
    if (!mReactToChangeSetup)
        return;

    //Setup List up-to-date?
    if (ui.setupComboBox->count() != mSimulationManager->getSetupList().size())
    {
        updateSetupListDisplay();
    }

    //Check if current setup was changed and save before changing
    bool change = checkSaved();

    if (!change)
    {
        //revert back to old selection (unsaved)
        mReactToChangeSetup = false;
        ui.setupComboBox->setCurrentIndex(mLastSetupIndex);
        mReactToChangeSetup = true;
        return;
    }

    //Load setup
    shared_ptr<SimulationSetup> loadedSetup = mSimulationManager->loadSimulationSetup(path, false);
    int pos = insertSetup(loadedSetup);
    
    if (pos != -1)
        changeSetup(pos);
}

void SetupFrame::saveSetup()                        
{
    mSaving = true;

    bool saved = mSimulationManager->saveSimulationSetup(mCurrentSetup);
    mSaving = false;
    if (!saved)
    {
        LOG_ERROR() << "Failed to save Simulation Setup data.";
        return;
    }
    updateSetupChanged(false);

    //If the initialized setups copy was changed, ask to reinitialize the simulation
    bool reinitialize = saved && 
        (mInitializedSetupIndex == mLastSetupIndex) && 
        (mLastSetupIndex != -1);
    if (reinitialize)
    {
        QMessageBox messageBox(QMessageBox::Question,
            QString(tr("Reinitialize simulation.")), 
            QString(tr("The current simulation setup was changed. Do you want to reinitialize the simulation?")), 
            QMessageBox::Yes | QMessageBox::No, this);

        //Execute
        int ok = messageBox.exec();

        //Reinit
        if (ok == QMessageBox::Yes)
            mSimulationManager->initializeSimulationFromFile(mCurrentSetup->getFileName(), true);
    }
}

void SetupFrame::updateSetupListIcons()
{
    QString active("");

    for (int i = 0; i < ui.setupComboBox->count(); i++)
    {
        if (mSimulationManager->getInitializedSetupIndex() == i)
        {
            //Initialized setup gets icon
            ui.setupComboBox->setItemIcon(i, QIcon(":accept"));
        }
        else
        {
            ui.setupComboBox->setItemIcon(i, QIcon(""));
        }
    }
}

void SetupFrame::updateSetupListDisplay()
{
    if (mSaving) //Saving may create a setup and therefore change the setup list
        return;

    mReactToChangeSetup = false;

    //Store chosen setup
    if (mSetupList.size() != 0)
        (mSetupList.at(ui.setupComboBox->currentIndex()));

    //Load / add all setups from manager include directories
    for (SimulationManager::TSetupList::const_iterator it = mSimulationManager->getSetupList().begin(); it != mSimulationManager->getSetupList().end(); it++)
    {        
        //Already in list?
        bool found = false;
        for (auto list_it = mSetupList.begin(); list_it != mSetupList.end(); list_it++)
        {
            if (list_it->get()->getFileName().compare(it->get()->getFileName()) == 0)
            {
                //identical, replace this one
                *list_it = *it;
                found = true;
                break;
            }
        }
        if (!found)
        {
            //new setup, add shared_ptr
            mSetupList.push_back(shared_ptr<SimulationSetup>(*it));
        }
    }

    //Re-fill combo box
    ui.setupComboBox->clear();
    int index = 0;
    mInitializedSetupIndex = -1;
    for (auto it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        bool isCurrent = false;
        if (mSimulationManager->simulationInitialized())
        {
            const shared_ptr<SimulationSetup>& setup = mSimulationManager->getSimulation()->getSetup();
            if (setup->equals(**it))
            {
                isCurrent = true;
            }
        }
        if (isCurrent) 
            mInitializedSetupIndex = index;

        //currently initialized setup has icon
        if (isCurrent)
            ui.setupComboBox->addItem(QIcon(":accept"), (*it)->getName());
        else
            ui.setupComboBox->addItem((*it)->getName());

        index++;
    }

    mReactToChangeSetup = true;

    //Choose old setup again and add "unsaved" info
    int i = 0;
    int pos = -1;
    for (auto it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        pos++;

        if (mCurrentSetup.get()) //has setup
        if (it->get()->getFileName().compare(mCurrentSetup->getFileName()) == 0) //and file is identical
        {
            mReactToChangeSetup = false;
            ui.setupComboBox->setCurrentIndex(pos);

            //Unsaved?
            if (mSetupChanged)
            {
                QString val = QString("%1 (unsaved)").arg(mSetupList.at(pos)->getName());
                ui.setupComboBox->setItemText(pos, val);
           }
            mReactToChangeSetup = true;
        }
    }
}

void SetupFrame::updateSetupComboBoxItems()
{
    if (mSetupList.size() != ui.setupComboBox->count())
    {
        LOG_ERROR() << "Setup list size and setup combo box item count not identical!";
        return;
    }

    int index = 0;
    for (auto it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        bool isCurrent = false;
        if (mSimulationManager->simulationInitialized())
        {
            const shared_ptr<SimulationSetup>& setup = mSimulationManager->getSimulation()->getSetup();
            if (setup->equals(**it))
            {
                isCurrent = true;
            }
        }
        
        //Currently initialized setup has icon
        if (isCurrent)
            ui.setupComboBox->setItemIcon(index, QIcon(":accept"));
        else
            ui.setupComboBox->setItemIcon(index, QIcon(""));

        ui.setupComboBox->setItemText(index, (*it)->getName());

        index++;
    }
}

void SetupFrame::updateSetupListDisplaySingle(int index)
{
    updateSetupListDisplay();
}


void SetupFrame::increaseStackedWidgetPage()
{
    int next = (ui.stackedWidget->currentIndex()+1)% ui.stackedWidget->count();
    ui.stackedWidget->setCurrentIndex(next);

    if (next == 1)
        updateTaskDisplay();
    else if (next == 0)
        updatePluginDisplay();
}

void SetupFrame::decreaseStackedWidgetPage()
{
    int last = (ui.stackedWidget->currentIndex()-1+ui.stackedWidget->count())% ui.stackedWidget->count();
    ui.stackedWidget->setCurrentIndex(last);

    if (last == 1)
        updateTaskDisplay();
    else if (last == 0)
        updatePluginDisplay();
}

void SetupFrame::addLoadPlugin()
{
    mCurrentSetup->insertAddPlugin(PluginDefinition());

    updateSetupChanged();
    updateLoadPluginsDisplay();
}

void SetupFrame::addLoadPluginFromPluginList()
{
    if (!mPluginListItemIndex.isValid())
        return;

    if (PluginFactory::getFactory().getInstantiators().size() == 0)
    {
        LOG_INFO() << "No plugins found.";
        return;
    }

    //Search plugin with the definition of the source instantiator
    auto instantiator_it = PluginFactory::getFactory().getInstantiators().begin();
    for (int i = 0; i < mPluginListItemIndex.row(); i++)
    {
        instantiator_it++;
        if (instantiator_it == PluginFactory::getFactory().getInstantiators().end())
        {
            LOG_ERROR() << "Plugin List end reached. Did the instantiator list change without being updated in SetupFrame?";
            return;
        }
    }

    //Add plugin
    AbstractPluginInstantiator* instantiator = instantiator_it->second;
    mCurrentSetup->insertAddPlugin(PluginDefinition(instantiator->getName(), instantiator->getName(), instantiator->getPluginType(),
        PluginDefinition::RD_INHERIT));

    //Update view
    updateSetupChanged();
    updateLoadPluginsDisplay();

    //Choose new plugin
    chooseLoadPlugin(mCurrentSetup->getAddPlugins().size() - 1);
}

void SetupFrame::overwriteLoadPluginFromPluginList()
{
    if (!mPluginListItemIndex.isValid())
        return;

    if (mChosenLoadPlugin == -1)
        return;

    if (PluginFactory::getFactory().getInstantiators().size() == 0)
    {
        LOG_INFO() << "No plugins found.";
        return;
    }

    //Search plugin with the definition of the source instantiator
    auto instantiator_it = PluginFactory::getFactory().getInstantiators().begin();
    for (int i = 0; i < mPluginListItemIndex.row(); i++)
    {
        instantiator_it++;
        if (instantiator_it == PluginFactory::getFactory().getInstantiators().end())
        {
            LOG_ERROR() << "Plugin List end reached. Did the instantiator list change without being updated in SetupFrame?";
            return;
        }
    }

    //Change current plugin
    AbstractPluginInstantiator* instantiator = instantiator_it->second;
    PluginDefinition* plugin = getCurrentPlugin();
    int currentPlugin = mChosenLoadPlugin;
    plugin->setName(instantiator->getName());
    plugin->setCaption(instantiator->getName());
    plugin->setPluginType(instantiator->getPluginType());
    plugin->setReloadDefinition(PluginDefinition::RD_INHERIT);

    //Update view
    updateSetupChanged();
    updateLoadPluginsDisplay();
    
    //Re-choose plugin to update plugin view
    chooseLoadPlugin(currentPlugin);
}

void SetupFrame::removeLoadPlugin()
{
    if (mChosenLoadPlugin == -1)
        return;

    mCurrentSetup->eraseAddPlugin(mChosenLoadPlugin);

    updateSetupChanged();
    updateLoadPluginsDisplay();
}

void SetupFrame::addUnloadPlugin()
{
    mCurrentSetup->insertRemovePlugin(PluginDefinition());

    updateSetupChanged();
    updateUnloadPluginsDisplay();
}

void SetupFrame::removeUnloadPlugin()
{
    if (mChosenUnloadPlugin == -1)
        return;

    mCurrentSetup->eraseRemovePlugin(mChosenUnloadPlugin);

    updateSetupChanged();
    updateLoadPluginsDisplay();
}


void SetupFrame::changePluginType(int index)
{
    if (!mReactToChangeSetup)
        return;

    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    bool changed = plugin->setPluginType(PluginType::mTypes[index]);
    if (changed)
    {
        //reload current display
        if (mChosenLoadPlugin != -1)
            chooseLoadPlugin(mChosenLoadPlugin);
        else if (mChosenUnloadPlugin != -1)
            chooseUnloadPlugin(mChosenUnloadPlugin);
        
        updateSetupChanged();

        if (mChosenLoadPlugin != -1) updateLoadPluginsDisplay();
        if (mChosenUnloadPlugin != -1) updateUnloadPluginsDisplay();
    }
}

void SetupFrame::editPluginParameters()     
{
    if (!mReactToChangeSetup)
        return;

    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    plugin->clearParameters();

    QString text = ui.pluginParametersTextEdit->toPlainText();

    int start = 0;
    int end = text.indexOf("\n", start);
    if (end == -1)
        end = text.size();
    while (start < text.size())
    {
        QString line = text.mid(start, end-start);
        plugin->addParameter(line);

        start = end+1;
        end = text.indexOf("\n", start);
        if (end == -1)
            end = text.size();
    }

    updateSetupChanged();
}

void SetupFrame::editPluginAttachments()     
{
    if (!mReactToChangeSetup)
        return;

    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    plugin->clearAttachments();

    QString text = ui.pluginAttachmentsTextEdit->toPlainText();

    int start = 0;
    int end = text.indexOf("\n", start);
    if (end == -1)
        end = text.size();
    while (start < text.size())
    {
        QString line = text.mid(start, end-start);
        plugin->addAttachment(line);

        start = end+1;
        end = text.indexOf("\n", start);
        if (end == -1)
            end = text.size();
    }

    updateSetupChanged();
}

void SetupFrame::editPluginCaption(QString value)     
{
    if (!mReactToChangeSetup)
        return;

    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    if (plugin->getCaption().compare(value) == 0)
        return;

    plugin->setCaption(value);

    updateSetupChanged();
    if (mChosenLoadPlugin != -1) updateLoadPluginsDisplay();
    if (mChosenUnloadPlugin != -1) updateUnloadPluginsDisplay();
}

void SetupFrame::editPluginCaption()     
{
    if (!mReactToChangeSetup)
        return;

    editPluginCaption(ui.pluginCaptionEdit->text());
}

void SetupFrame::editPluginName(QString value)     
{
    if (!mReactToChangeSetup)
        return;

    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    if (plugin->getName().compare(value) == 0)
        return;

    //Set value
    plugin->setName(value);

    updateSetupChanged();
    if (mChosenLoadPlugin != -1) updateLoadPluginsDisplay();
    if (mChosenUnloadPlugin != -1) updateUnloadPluginsDisplay();
}

void SetupFrame::editPluginName()     
{
    if (!mReactToChangeSetup)
        return;

    editPluginName(ui.pluginNameEdit->text());
}

void SetupFrame::changePluginReload(int index)
{
    if (!mReactToChangeSetup)
        return;
    
    PluginDefinition* plugin = getCurrentPlugin();
    if (plugin == 0)
        return;

    if (index == 0)
        plugin->setReloadDefinition(PluginDefinition::RD_INHERIT);
    else if (index == 1)
        plugin->setReloadDefinition(PluginDefinition::RD_RELOAD);
    else if (index == 2)
        plugin->setReloadDefinition(PluginDefinition::RD_NO_RELOAD);
    else
        LOG_ERROR() << "Index out of bounds.";

    updateSetupChanged();
    if (mChosenLoadPlugin != -1) updateLoadPluginsDisplay();
}

void SetupFrame::changeRemoveAll(bool state)
{
    if (!mReactToChangeSetup)
        return;

    mCurrentSetup->setRemoveAll(state);

    updateSetupChanged();
}

void SetupFrame::changeReloadAll(bool state)
{
    if (!mReactToChangeSetup)
        return;

    mCurrentSetup->setReloadAll(state);

    updateSetupChanged();
}

void SetupFrame::chooseLoadPlugin(int index)
{
    if (!mReactToChangeSetup)
        return;

    if ((int)mCurrentSetup->getAddPlugins().size() <= index)
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    const PluginDefinition& ref = *mCurrentSetup->getAddPlugins().at(index);

    choosePlugin(ref);

    mChosenLoadPlugin = index;
    mChosenUnloadPlugin = -1;

    ui.loadPluginsRemoveButton->setEnabled(true);
    ui.unloadPluginsRemoveButton->setEnabled(false);
}

void SetupFrame::chooseUnloadPlugin(int index)
{
    if (!mReactToChangeSetup)
        return;

    if ((int)mCurrentSetup->getRemovePlugins().size() <= index)
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    const PluginDefinition& ref = *mCurrentSetup->getRemovePlugins().at(index);

    choosePlugin(ref);

    mChosenLoadPlugin = -1;
    mChosenUnloadPlugin = index;

    ui.loadPluginsRemoveButton->setEnabled(false);
    ui.unloadPluginsRemoveButton->setEnabled(true);
}

void SetupFrame::showPluginListContextMenu(QPoint pos)
{
     QMenu menu(this);
     QAction* action;

     //Update model index
     mPluginListItemIndex = ui.pluginListListWidget->indexAt(pos);

     //Add chosen item, connect with the set plugin functions
     action = menu.addAction(tr("Add as new plugin"), this, SLOT(addLoadPluginFromPluginList()));
     action = menu.addAction(tr("Overwrite current plugin"), this, SLOT(overwriteLoadPluginFromPluginList()));
     action->setEnabled(mChosenLoadPlugin != -1);

     menu.exec(ui.pluginListListWidget->mapToGlobal(pos));
}
    
void SetupFrame::chooseLoadPlugin(QModelIndex index)
{
    chooseLoadPlugin(index.row());
}

void SetupFrame::chooseUnloadPlugin(QModelIndex index)
{
    chooseUnloadPlugin(index.row());
}

void SetupFrame::chooseTask(QModelIndex index)
{
    chooseTask(index.row());
}

void SetupFrame::chooseTask(int index)
{
    if (!mReactToChangeSetup)
        return;

    if ((int)mCurrentSetup->getTaskDefinitions().size() <= index)
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    const TaskDefinition& ref = *mCurrentSetup->getTaskDefinitions().at(index);
    chooseTask(ref);

    mChosenTask = index;

    //Update coloration
    taskFirstCheckText(ref.getFirst());
    taskSecondCheckText(ref.getSecond());
    taskThirdCheckText(ref.getThird());
}

void SetupFrame::updateTaskListDisplay()
{
    mReactToChangeSetup = false;

    //Remember
    int currentIndex = ui.tasksList->currentIndex().row();
    QString chosenTask = (currentIndex == -1 ? "" : ui.tasksList->item(currentIndex)->text());

    //Refresh
    ui.tasksList->clear();
    for (SimulationSetup::TTaskDefinitions::const_iterator it = mCurrentSetup->getTaskDefinitions().begin();
         it != mCurrentSetup->getTaskDefinitions().end(); it++)
    {
        ui.tasksList->addItem(QString("%1 (%2)").arg(it->get()->getName()).arg(TaskDefinition::getTypeString(it->get()->getType())));
    }

    mReactToChangeSetup = true;

    //Choose previous one
    if (currentIndex != -1)
    for (int i = 0; i < ui.tasksList->count(); i++)
    {
        if (ui.tasksList->item(i)->text() == chosenTask)
        {
            ui.tasksList->setCurrentRow(i);
            break;
        }
    }

    if (ui.tasksList->count() == 0)
    {
        unchooseTask();
    }
}

void SetupFrame::addTask()
{
    mCurrentSetup->insertTask(TaskDefinition());
    setUniqueTaskName(*mCurrentSetup->getTaskDefinitions().at(mCurrentSetup->getTaskDefinitions().size()-1));

    updateSetupChanged();
    updateTaskListDisplay();
}

void SetupFrame::removeTask()
{
    if (mChosenTask == -1)
        return;

    mCurrentSetup->eraseTask(mChosenTask);

    updateSetupChanged();
    updateTaskListDisplay();
}

void SetupFrame::changeTaskType(int index)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* task = getCurrentTask();
    if (task == 0)
        return;

    TaskDefinition::ETaskType oldType = task->getType();
    TaskDefinition::ETaskType newType = TaskDefinition::mTypes[index];

    if (oldType == newType)
        return;

    if (newType == TaskDefinition::TT_SERVERTHREAD)
    {
        if (mCurrentSetup->hasTaskDefinition(TaskDefinition::TT_SERVERTHREAD))
        {
            //Can only have one server
            LOG_INFO() << "There must only be one Server in a Simulation Setup!";
            ui.tasksTypeComboBox->setCurrentIndex((int)oldType);
            return;
        }
    }

    //Set type
    task->setType(newType);

    //Set to default values for new type
    task->setFirst   (                   mSimulationManager->getDefaultTaskData(newType, TaskDefinition::TD_FIRST   ).toString());
    task->setSecond  (                   mSimulationManager->getDefaultTaskData(newType, TaskDefinition::TD_SECOND  ).toString());
    task->setThird   (                   mSimulationManager->getDefaultTaskData(newType, TaskDefinition::TD_THIRD   ).toString());
    task->setName    (                   mSimulationManager->getDefaultTaskData(newType, TaskDefinition::TD_NAME    ).toString());
    task->setPriority((QThread::Priority)mSimulationManager->getDefaultTaskData(newType, TaskDefinition::TD_PRIORITY).toInt());
    setUniqueTaskName(*task);

    updateSetupChanged();
    chooseTask(mChosenTask);
    updateTaskListDisplay();
}

void SetupFrame::changeTaskPriority(int index)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* task = getCurrentTask();
    if (task == 0)
        return;

    if ((int)task->getPriority() == index)
        return;

    //Set priority
    task->setPriority((QThread::Priority)index);

    updateSetupChanged();
}

void SetupFrame::editTaskCaption(QString value)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* task = getCurrentTask();
    if (task == 0)
        return;

    if (task->getName().compare(value) == 0)
        return;

    task->setName(value);

    updateSetupChanged();
    if (mChosenTask != -1) updateTaskListDisplay();
}

void SetupFrame::editTaskCaption()
{
    if (!mReactToChangeSetup)
        return;

    editTaskCaption(ui.tasksCaptionEdit->text());
}

void SetupFrame::editTaskParameters()
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* task = getCurrentTask();
    if (task == 0)
        return;

    task->clearSecondList();

    QString text = ui.tasksParametersTextEdit->toPlainText();

    int start = 0;
    int end = text.indexOf("\n", start);
    if (end == -1)
        end = text.size();
    while (start < text.size())
    {
        QString line = text.mid(start, end-start);
        task->addToSecondList(line);

        start = end+1;
        end = text.indexOf("\n", start);
        if (end == -1)
            end = text.size();
    }

    updateSetupChanged();
}

void SetupFrame::loadTaskFirst()
{
    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    if (def->isPlugin())
    {
        //Load predefined plugin
        showLoadTaskContextMenu(ui.tasksFirstLoadButton->mapToGlobal(QPoint(0, 0)));
    }
    else
    {
         //Load executable from file dialog and fill in line edit
        loadTaskEntry(0, ui.tasksFirstEdit);
    }
}

void SetupFrame::loadTaskSecond()
{
    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    //Load main script from file dialog and fill in line edit
    loadTaskEntry(1, ui.tasksSecondEdit);
}

void SetupFrame::loadTaskThird()
{
    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    //Load secondary script from file dialog and fill in line edit
    loadTaskEntry(2, ui.tasksThirdEdit);
}

void SetupFrame::taskFirstCheckText(QString text)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def != 0 && def->isPlugin())
    {
        //black text
        QPalette pal = ui.tasksFirstEdit->palette();
        pal.setColor(QPalette::Text, Qt::black);
        ui.tasksFirstEdit->setPalette(pal);
    }
    else
    {
        if (QFileInfo(text).exists() ||
            mSimulationManager->hasExecutable(text))
        {
            //First found, black text
            QPalette pal = ui.tasksFirstEdit->palette();
            pal.setColor(QPalette::Text, Qt::black);
            ui.tasksFirstEdit->setPalette(pal);
        }
        else
        {
            //First not found, red text
            QPalette pal = ui.tasksFirstEdit->palette();
            pal.setColor(QPalette::Text, Qt::red);
            ui.tasksFirstEdit->setPalette(pal);
        }
    }
}

void SetupFrame::taskFirstChangeText()
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    QString text = ui.tasksFirstEdit->text();
    if (text.compare(def->getFirst()) == 0)
        return;

    if (def->isPlugin())
    {
        //Todo: Update plugin availability
    }
    else
    {
        if (!QFileInfo(text).exists() &&
            !mSimulationManager->hasExecutable(text) &&
            !text.compare("") == 0)
        {
            LOG_WARNING() << "First " << text << " not found. Add the file or an include path to the SimulationManager executable include path before executing the setup.";
        }
    }

    def->setFirst(text);
    updateSetupChanged(true);
}

void SetupFrame::taskSecondCheckText(QString text)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def != 0 && def->isPlugin())
    {
        //black text
        QPalette pal = ui.tasksSecondEdit->palette();
        pal.setColor(QPalette::Text, Qt::black);
        ui.tasksSecondEdit->setPalette(pal);
    }
    else
    {
        if (QFileInfo(text).exists() ||
            mSimulationManager->hasScript(text))
        {
            //ListEntry found, black text
            QPalette pal = ui.tasksSecondEdit->palette();
            pal.setColor(QPalette::Text, Qt::black);
            ui.tasksSecondEdit->setPalette(pal);
        }
        else
        {
            //ListEntry not found, red text
            QPalette pal = ui.tasksSecondEdit->palette();
            pal.setColor(QPalette::Text, Qt::red);
            ui.tasksSecondEdit->setPalette(pal);
        }
    }   
}

void SetupFrame::taskSecondChangeText()
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    QString text = ui.tasksSecondEdit->text();
    if (text.compare(def->getSecond()) == 0)
        return;

    if (def->isPlugin())
    {
        //Todo: Update plugin availability
    }
    else
    {
        if (!QFileInfo(text).exists() &&
            !mSimulationManager->hasScript(text) &&
            !text.compare("") == 0)
        {
            LOG_WARNING() << "ListEntry " << text << " not found. Add the file or an include path to the SimulationManager script include path before executing the setup.";
        }
    }

    def->setSecond(text);
    updateSetupChanged(true);
}

void SetupFrame::taskThirdCheckText(QString text)
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def != 0 && def->isPlugin())
    {
        //black text
        QPalette pal = ui.tasksThirdEdit->palette();
        pal.setColor(QPalette::Text, Qt::black);
        ui.tasksThirdEdit->setPalette(pal);
    }
    else
    {
        if (QFileInfo(text).exists() ||
            mSimulationManager->hasScript(text))
        {
            //ListEntry found, black text
            QPalette pal = ui.tasksThirdEdit->palette();
            pal.setColor(QPalette::Text, Qt::black);
            ui.tasksThirdEdit->setPalette(pal);
        }
        else
        {
            //ListEntry not found, red text
            QPalette pal = ui.tasksThirdEdit->palette();
            pal.setColor(QPalette::Text, Qt::red);
            ui.tasksThirdEdit->setPalette(pal);
        }
    }
}

void SetupFrame::taskThirdChangeText()
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    QString text = ui.tasksThirdEdit->text();
    if (text.compare(def->getThird()) == 0)
        return;

    if (def->isPlugin())
    {
        //Todo: Update plugin availability
    }
    else
    {
        if (!QFileInfo(text).exists() &&
            !mSimulationManager->hasScript(text) &&
            !text.compare("") == 0)
        {
            LOG_WARNING() << "ListEntry " << text << " not found. Add the file or an include path to the SimulationManager script include path before executing the setup.";
        }
    }

    def->setThird(text);
    updateSetupChanged(true);
}

void SetupFrame::addTaskListEntry()
{
    if (!mReactToChangeSetup)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    def->addToFirstList("script");

    ui.tasksAdditionalListList->addItem("script");
    ui.tasksAdditionalListList->item(ui.tasksAdditionalListList->count()-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui.tasksAdditionalListList->setCurrentRow(ui.tasksAdditionalListList->count()-1);

    checkCurrentTaskListEntry();
    updateSetupChanged();
}

void SetupFrame::removeTaskListEntry()
{
    if (!mReactToChangeSetup)
        return;

    if (ui.tasksAdditionalListList->count() == 0)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    if (mChosenTask < 0 || mChosenTask >= ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    if (def->getFirstList().size() != ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "ListEntry count out of sync.";
        return;
    }

    //Erase
    mChosenTaskListEntryIllegal = false;
    int thisListEntry = mChosenTaskListEntry;
    def->removeFromFirstList(thisListEntry);
    ui.tasksAdditionalListList->takeItem(thisListEntry);

    //Select next
    if (ui.tasksAdditionalListList->count() > 0)
    {
        int nextListEntry = thisListEntry < ui.tasksAdditionalListList->count() ? thisListEntry : thisListEntry-1;
        ui.tasksAdditionalListList->setCurrentRow(nextListEntry);
        mChosenTaskListEntry = nextListEntry;
    }
    else
        mChosenTaskListEntry = -1;

    //Notify that setup has changed
    updateSetupChanged();
}

void SetupFrame::moveTaskListEntryUp()
{
    if (!mReactToChangeSetup)
        return;

    if (ui.tasksAdditionalListList->count() == 0)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    if (mChosenTaskListEntry == 0)
        return;

    if (mChosenTaskListEntry < 0 || mChosenTaskListEntry >= ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    if (def->getFirstList().size() != ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "ListEntry count out of sync.";
        return;
    }

    //Switch
    bool currentIllegal = mChosenTaskListEntryIllegal;
    mChosenTaskListEntryIllegal = false;
    int thisListEntry = mChosenTaskListEntry;
    QString thisListEntryText = ui.tasksAdditionalListList->item(thisListEntry)->text();
    def->swapInFirstList(thisListEntry, thisListEntry-1);
    ui.tasksAdditionalListList->takeItem(thisListEntry);
    ui.tasksAdditionalListList->insertItem(thisListEntry-1, thisListEntryText);
    ui.tasksAdditionalListList->item(thisListEntry-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    mChosenTaskListEntryIllegal = currentIllegal;

    //Select moved one
    ui.tasksAdditionalListList->setCurrentRow(thisListEntry-1);
    mChosenTaskListEntry = thisListEntry-1;
    checkCurrentTaskListEntry();

    //Notify that setup has changed
    updateSetupChanged();
}

void SetupFrame::moveTaskListEntryDown()
{
    if (!mReactToChangeSetup)
        return;

    if (ui.tasksAdditionalListList->count() == 0)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    if (mChosenTaskListEntry == ui.tasksAdditionalListList->count() - 1)
        return;

    if (mChosenTaskListEntry < 0 || mChosenTaskListEntry >= ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "Index out of bounds.";
        return;
    }

    if (def->getFirstList().size() != ui.tasksAdditionalListList->count())
    {
        LOG_ERROR() << "ListEntry count out of sync.";
        return;
    }

    //Switch
    bool currentIllegal = mChosenTaskListEntryIllegal;
    mChosenTaskListEntryIllegal = false;
    int thisListEntry = mChosenTaskListEntry;
    QString thisListEntryText = ui.tasksAdditionalListList->item(thisListEntry)->text();
    def->swapInFirstList(thisListEntry, thisListEntry+1);
    ui.tasksAdditionalListList->takeItem(thisListEntry);
    ui.tasksAdditionalListList->insertItem(thisListEntry+1, thisListEntryText);
    ui.tasksAdditionalListList->item(thisListEntry+1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    mChosenTaskListEntryIllegal = currentIllegal;

    //Select moved one
    ui.tasksAdditionalListList->setCurrentRow(thisListEntry+1);
    mChosenTaskListEntry = thisListEntry+1;
    checkCurrentTaskListEntry();

    //Notify that setup has changed
    updateSetupChanged();
}

void SetupFrame::chooseTaskListEntry(int row)
{
    if (mChosenTaskListEntryIllegal)
    {
        if (ui.tasksAdditionalListList->count() > mChosenTaskListEntry && mChosenTaskListEntry >= 0)
            LOG_INFO() << "ListEntry " << ui.tasksAdditionalListList->item(mChosenTaskListEntry)->text() << " not found. Add its path or the single file to the SimulationManagers script include paths.";
    }

    mChosenTaskListEntry = row;
    mChosenTaskListEntryIllegal = false;
}

void SetupFrame::editTaskListEntry(QListWidgetItem* item)
{
    if (!mReactToChangeSetup || !mReactToEditTaskListEntry)
        return;

    if (item == 0)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    for (int i = 0; i < item->listWidget()->count(); i++)
    {
        if (item->listWidget()->item(i) == item)
        {
            if (def->getFirstListNc().at(i).compare(item->text()) == 0)
                return;
            break;
        }
    }

    //Set color
    mReactToEditTaskListEntry = false;
    checkTaskListEntry(item);

    if (ui.tasksAdditionalListList->currentItem() == item)
    if ( item->textColor() == QColor(Qt::red))
    {
        //Save it, but send a warning before changing
        mChosenTaskListEntryIllegal = true;
    }
    else
    {
        mChosenTaskListEntryIllegal = false;
    }

    mReactToEditTaskListEntry = true;

    //Change
    for (int i = 0; i < item->listWidget()->count(); i++)
    {
        if (item->listWidget()->item(i) == item)
        {
            def->getFirstListNc().replace(i, item->text());
            def->updateDefinitionChanged();
            updateSetupChanged();
            break;
        }
    }
}

void SetupFrame::editCurrentTaskListEntry(QString text)
{
    if (!mReactToChangeSetup)
        return;

    editTaskListEntry(ui.tasksAdditionalListList->currentItem());
}

void SetupFrame::checkTaskListEntry(QListWidgetItem* item)
{
    if (!mReactToChangeSetup)
        return;

    if (item == 0)
        return;

    TaskDefinition* def = getCurrentTask();
    if (def == 0)
        return;

    QString text = item->text();

    if (QFileInfo(text).exists() ||
        mSimulationManager->hasScript(text))
    {
        //ListEntry found, black text
        item->setTextColor(QColor(Qt::black));
    }
    else
    {
        //ListEntry not found, red text
        item->setTextColor(QColor(Qt::red));
    }
}

void SetupFrame::checkCurrentTaskListEntry()
{
    if (!mReactToChangeSetup)
        return;

    checkTaskListEntry(ui.tasksAdditionalListList->currentItem());
}

void SetupFrame::showTasksExplorerTreeViewContextMenu(QPoint pos)
{
    showExplorerTreeViewContextMenu(pos, ui.tasksExplorerTreeView, mTasksExplorerItemIndex);
}

void SetupFrame::onTasksScriptPathClick(int number)
{
    scrollToRegisteredExplorerFile(number, false);
}

void SetupFrame::onTasksExecutablePathClick(int number)
{
    scrollToRegisteredExplorerFile(number, true);
}

void SetupFrame::onLoadTaskContextMenuClick(int id)
{
    TaskDefinition* task = getCurrentTask();
    if (task == 0)
        return;

    PluginManager* manager = getCarbon()->getPluginManager();

    if (task->isPlugin())
    {
        AbstractPlugin* plugin = manager->getPlugin(id);

        if (plugin == 0)
        {
            LOG_ERROR() << "Plugin with id " << id << " not found.";
            return;
        }

        //Class edit
        ui.tasksFirstEdit->setText(plugin->getClassName());
        taskFirstCheckText(plugin->getClassName());
        taskFirstChangeText();

        //Caption edit
        ui.tasksSecondEdit->setText(plugin->getCaption());
        taskSecondCheckText(plugin->getCaption());
        taskSecondChangeText();

        //type edit
        switch (plugin->getPluginType())
        {
        case PT_NORMAL: ui.tasksThirdEdit->setText("0"); taskThirdCheckText("0"); break;
        case PT_SIGNAL: ui.tasksThirdEdit->setText("1"); taskThirdCheckText("1"); break;
        case PT_FRAME : ui.tasksThirdEdit->setText("2"); taskThirdCheckText("2"); break;
        default:
            LOG_ERROR() << "Illegal plugin type " << (int) plugin->getPluginType();
            break;
        }
        taskThirdChangeText();
    }
    else
    {
        LOG_INFO() << "Illegal task type.";
    }
}

void SetupFrame::setTaskFirstTextFromExplorer(bool absolute)
{
    setLineEditTextFromExplorer(ui.tasksExplorerTreeView, mTasksExplorerItemIndex, ui.tasksFirstEdit, true, absolute);
    taskFirstChangeText();
}

void SetupFrame::setTaskSecondTextFromExplorer(bool absolute)
{
    setLineEditTextFromExplorer(ui.tasksExplorerTreeView, mTasksExplorerItemIndex, ui.tasksSecondEdit, false, absolute);
    taskSecondChangeText();
}

void SetupFrame::setTaskThirdTextFromExplorer(bool absolute)
{
    setLineEditTextFromExplorer(ui.tasksExplorerTreeView, mTasksExplorerItemIndex, ui.tasksThirdEdit, false, absolute);
    taskThirdChangeText();
}

void SetupFrame::cleanup()
{
    if (mClosed)
        return;

    mClosed = true;

    //Save current setup
    checkSaved();

    //only ask to save this setup once
    int current = mSimulationManager->getSetupIndex(mCurrentSetup->getName());
    if (current != -1)
        mSimulationManager->getSetupList().at(current)->setDontSave(true);

    //Save settings
    saveSettings();
}
