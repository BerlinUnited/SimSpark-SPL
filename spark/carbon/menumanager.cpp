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

#include "menumanager.h"
#include "simulationmanager.h"
#include "carbon.h"
#include "attachableframe.h"
#include "abstractplugin.h"
#include "menuobject.h"
#include "menuobjectmenu.h"
#include "menuobjecttoolbar.h"
#include "menuobjectfile.h"
#include "windowmanager.h"
#include "settingsdialog.h"
#include "plugindialog.h"
#include "numberedactionreceiver.h"
#include "simulation.h"
#include "simulationmanager.h"
#include "sparkcontroller.h"
#include "aboutdialog.h"

#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QMainWindow>
#include <QLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QWhatsThis>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

QSize MenuManager::mDefaultToolBarIconSize = QSize(20,20);
QSize MenuManager::mBigToolBarIconSize = QSize(40,40);

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

MenuManager::MenuManager(Carbon* parent)
{
    mParent = parent;
    mMenuBar = 0;
    mInitialized = false;
    mClosed = false;
    mMenuObjects.clear();

    connect(Carbon::get(), SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));
}

MenuManager::~MenuManager()
{
    cleanup();

    clearCreatePluginReceivers();
}

bool MenuManager::initMenu()
{
    //Creates the main menu for the application

    QMainWindow* mainWindow = Carbon::get()->getWindowManager()->getMainWindow();
    if (mainWindow == 0)
    {
        LOG_ERROR() << "MainWindow has to initialize WindowManager::mMainWindow before initializing menu manager!";
        return false;
    }

    QAction* action;

    //------------------------------------------------------
    // Init Menu
    //------------------------------------------------------
    mMenuBar = mainWindow->menuBar();
    mMenuBar->clear();
    mMenuBar->show();

    //------------------------------------------------------
    // File Menu
    //------------------------------------------------------
    mFileMenu = new QMenu(tr("&File"));
    changeMenuStyle(mFileMenu, MS_MAIN_MENU);

    action = mFileMenu->addAction(QIcon(":folder"), tr("&Load"));
    action->setToolTip("Load a simulation state.");
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(action, SIGNAL(triggered()), this, SLOT(load()));

    action = mFileMenu->addAction(QIcon(":save"), tr("&Save"));
    action->setToolTip("Save the current simulation state.");
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(action, SIGNAL(triggered()), this, SLOT(save()));

    action = mFileMenu->addAction(QIcon(":save"), tr("Save As"));
    action->setToolTip("Save the current simulation state.");
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));

    action = mFileMenu->addAction(tr("&Reload Setup"));
    action->setToolTip("Reload the currently initialized Setup. All unsaved changes will be lost.");
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(action, SIGNAL(triggered()), this, SLOT(reload()));

    mFileMenuSetups = mFileMenu->addMenu(tr("&Init preset Setup"));
    mFileMenuSetups->setToolTip("Initializes one of the preset Simulation Setups from the include directories. \nSetups marked with * are unsaved. ");
    updateSimulationSetupList();

    mFileMenu->addSeparator();

    action = mFileMenu->addAction(QIcon(":folder"), tr("&Load Clean"));
    action->setToolTip("Load a simulation state without loading data files for the plugins.");
    connect(action, SIGNAL(triggered()), this, SLOT(loadClean()));

    action = mFileMenu->addAction(QIcon(":save"), tr("&Save Clean"));
    action->setToolTip("Save the current simulation setup without saving data files for the plugins.");
    connect(action, SIGNAL(triggered()), this, SLOT(saveClean()));

    action = mFileMenu->addAction(QIcon(":save"), tr("Save Clean As"));
    action->setToolTip("Save the current simulation setup without saving data files for the plugins.");
    connect(action, SIGNAL(triggered()), this, SLOT(saveCleanAs()));

    mFileMenu->addSeparator();

    action = mFileMenu->addAction(tr("&Exit"));
    action->setToolTip("Close the program.");
    connect(action, SIGNAL(triggered()), this, SLOT(exit()));

    addNamedMenu(mFileMenu, "File");
    //------------------------------------------------------

    //------------------------------------------------------
    //Simulation Menu
    //------------------------------------------------------
    mSimulationMenu = new QMenu(tr("&Simulation"));
    changeMenuStyle(mSimulationMenu, MS_MAIN_MENU);

    action = mSimulationMenu->addAction(QIcon(":play"), tr("&Run"));
    action->setShortcut(QKeySequence(Qt::Key_F5));
    connect(action, SIGNAL(triggered()), this, SLOT(runSimulation()));

    action = mSimulationMenu->addAction(QIcon(":pause"), tr("&Pause"));
    action->setShortcut(QKeySequence(Qt::Key_F6));
    connect(action, SIGNAL(triggered()), this, SLOT(pauseSimulation()));

    action = mSimulationMenu->addAction(QIcon(":stop"), tr("&Stop"));
    action->setShortcut(QKeySequence(Qt::Key_F7));
    connect(action, SIGNAL(triggered()), this, SLOT(stopSimulation()));

    action = mSimulationMenu->addAction(QIcon(":previous"), tr("R&estart"));
    action->setShortcut(QKeySequence(Qt::Key_F8));
    connect(action, SIGNAL(triggered()), this, SLOT(restartSimulation()));

    addNamedMenu(mSimulationMenu, "Simulation");

    //------------------------------------------------------

    //------------------------------------------------------
    //Tools Menu
    //------------------------------------------------------
    mToolsMenu = new QMenu(tr("&Tools"));
    changeMenuStyle(mToolsMenu, MS_MAIN_MENU);

    action = mToolsMenu->addAction(tr("&Settings"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(action, SIGNAL(triggered()), this, SLOT(showSettings()));

    action = mToolsMenu->addAction(tr("&Plugins"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    connect(action, SIGNAL(triggered()), this, SLOT(showPlugins()));

    addNamedMenu(mToolsMenu, "Tools");

    //------------------------------------------------------

    //------------------------------------------------------
    //Log Menu
    //------------------------------------------------------
    mLogMenu = new QMenu(tr("&Log"));
    changeMenuStyle(mLogMenu, MS_MAIN_MENU);

    action = mLogMenu->addAction(tr("&Re-send init messages"));
    connect(action, SIGNAL(triggered()), this, SLOT(sendInitMessages()));

    mLogMenu->addSeparator();

    mSparkDebugLevelAction = mLogMenu->addAction(tr("Set SimSpark log level: Debug"));
    connect(mSparkDebugLevelAction, SIGNAL(triggered()), this, SLOT(setSparkLogLevelDebug()));

    mSparkInfoLevelAction = mLogMenu->addAction(tr("Set SimSpark log level: Info"));
    connect(mSparkInfoLevelAction, SIGNAL(triggered()), this, SLOT(setSparkLogLevelInfo()));

    mSparkWarningLevelAction = mLogMenu->addAction(tr("Set SimSpark log level: Warning"));
    connect(mSparkWarningLevelAction, SIGNAL(triggered()), this, SLOT(setSparkLogLevelWarning()));

    mSparkErrorLevelAction = mLogMenu->addAction(tr("Set SimSpark log level: Error"));
    connect(mSparkErrorLevelAction, SIGNAL(triggered()), this, SLOT(setSparkLogLevelError()));

    addNamedMenu(mLogMenu, "Log");
    //------------------------------------------------------

    //------------------------------------------------------
    //Help Menu
    //------------------------------------------------------
    mHelpMenu = new QMenu(tr("&Help"));
    changeMenuStyle(mHelpMenu, MS_MAIN_MENU);

    //TODO: Add Help menu
    //action = mHelpMenu->addAction(tr("&View Help"));
    //connect(action, SIGNAL(triggered()), this, SLOT(help()));

    action = mHelpMenu->addAction(tr("&About"));
    connect(action, SIGNAL(triggered()), this, SLOT(about()));

    action = mHelpMenu->addAction(tr("&Whats this?"));
    action->setShortcut(Qt::CTRL + Qt::Key_W);
    connect(action, SIGNAL(triggered()), this, SLOT(whatsThis()));

    addNamedMenu(mHelpMenu, "Help");
    //------------------------------------------------------

    //------------------------------------------------------
    // Init ToolBars
    //------------------------------------------------------

    //------------------------------------------------------
    // File Toolbar
    //------------------------------------------------------
    mFileToolBar = new QToolBar(tr("&File"));
    changeToolBarStyle(mFileToolBar, TS_SMALL);

    action = mFileToolBar->addAction(QIcon(":folder"), tr("&Load"));
    connect(action, SIGNAL(triggered()), this, SLOT(load()));
    action->setToolTip(tr("Load a simulation setup file."));

    action = mFileToolBar->addAction(QIcon(":save"), tr("&Save"));
    connect(action, SIGNAL(triggered()), this, SLOT(save()));
    action->setToolTip(tr("Saves the current simulation setup."));
    action->setWhatsThis(tr("Saves the current simulation setup to its specified target file."));

    addNamedToolBar(mFileToolBar, "File");

    //------------------------------------------------------
    //Simulation ToolBar
    //------------------------------------------------------
    mSimulationToolBar = new QToolBar(tr("&Simulation"));
    changeMenuStyle(mSimulationMenu, MS_MAIN_MENU);

    action = mSimulationToolBar->addAction(QIcon(":play"), tr("&Run Simulation"));
    connect(action, SIGNAL(triggered()), this, SLOT(runSimulation()));
    action->setToolTip(tr("Execute the current simulation."));
    action->setWhatsThis(tr("Executes the current simulation by starting all initialized tasks immediately.\n" \
        "Tasks can be added using plugins like the 'Simulation' plugin."));

    action = mSimulationToolBar->addAction(QIcon(":pause"), tr("&Pause Simulation"));
    connect(action, SIGNAL(triggered()), this, SLOT(pauseSimulation()));
    action->setToolTip(tr("Pause the current simulation by pausing all running tasks."));
    action->setWhatsThis(tr("Pauses the current simulation by pausing all running tasks.\n" \
        "Tasks can be added using plugins like the 'Simulation' plugin.\n" \
        "Not all tasks can be paused."));

    action = mSimulationToolBar->addAction(QIcon(":stop"), tr("&Stop Simulation"));
    connect(action, SIGNAL(triggered()), this, SLOT(stopSimulation()));
    action->setToolTip(tr("Stops the current simulation by stopping all running or paused tasks."));
    action->setWhatsThis(tr("Stops the current simulation by stopping all running or paused tasks immediately.\n" \
        "Tasks can be added using plugins like the 'Simulation' plugin.\n" \
        "A simulation server task cant be stopped completely but is paused instead."));

    action = mSimulationToolBar->addAction(QIcon(":previous"), tr("R&estart Simulation"));
    connect(action, SIGNAL(triggered()), this, SLOT(restartSimulation()));
    action->setToolTip(tr("Restart the current simulation by restarting all tasks."));
    action->setWhatsThis(tr("Restart the current simulation by restarting all tasks.\n" \
        "Tasks can be added using plugins like the 'Simulation' plugin.\n" \
        "Stops and executes the task again (if possible)."));

    addNamedToolBar(mSimulationToolBar, "Simulation");

    //------------------------------------------------------
    // Window Toolbar
    //------------------------------------------------------

    //------------------------------------------------------
    // Plugin Toolbar
    //------------------------------------------------------
    mPluginToolBar = new QToolBar(tr("&Plugin"));
    changeToolBarStyle(mPluginToolBar, TS_SMALL);

    //initially empty

    addNamedToolBar(mPluginToolBar, "Plugin");
    updatePluginToolBar();
    //--------------------------------------------------

    //Connect update functions for menu
    initConnections();

    mInitialized = true;
    return true;
}

QSize MenuManager::getDefaultToolBarIconSize()
{
    return mDefaultToolBarIconSize;
}

QSize MenuManager::getBigToolBarIconSize()
{
    return mBigToolBarIconSize;
}

MenuManager* MenuManager::getManager()
{
    return Carbon::get()->getMenuManager();
}

QMenuBar* MenuManager::getMenuBar()
{
    return getManager()->mMenuBar;
}

void MenuManager::changeToolBarStyle(QToolBar* bar, EToolBarStyle style)
{
    //:TODO: More
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (style == TS_SMALL)
    {
        bar->setIconSize(mDefaultToolBarIconSize);
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    if (style == TS_BIG)
    {
        bar->setIconSize(mBigToolBarIconSize);
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    if (style == TS_BIG_NAMED)
    {
        bar->setIconSize(mBigToolBarIconSize);
        bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
}

void MenuManager::changeMenuStyle(QMenu* menu, EMenuStyle style)
{
    //:TODO: More
}

MenuSystem::Menu* MenuManager::provideMenu(const AbstractPlugin* caller, const QString& menuName, QMenu* menu, EMenuObjectLocation location)
{
    if (!assertProvidedParameters(caller, menuName, (QObject*)menu, MOT_MENU))
        return 0;

    MenuSystem::Menu* newObj = 0;
    QMenuBar* targetBar = 0;
    QWidget* callerWidget = 0;
    QLayout* target = 0;

    if (caller->hasVisualRepresentation() == false)
    {
        callerWidget = 0;
        target = 0;
        if (location == MOL_LOCAL)
        {
            LOG_WARNING() << "Cant provide 'Local' menu objects in a plugin without visual representation. Ignoring this flag.";
            location = MOL_MAIN;
        }
    }
    else
    {
        callerWidget = caller->getVisualRepresentation();
        target = callerWidget->layout();
    }

    if (location == MOL_LOCAL)
    {
        if (target == 0)
        {
            LOG_ERROR() << "Cannot store menu to target 0-pointer.";
            return 0;
        }

        //Get/Set MenuBar
        QMenuBar* targetBar = (QMenuBar*)(target->menuBar());
        if (targetBar == 0)
        {
            target->setMenuBar(new QMenuBar);
            targetBar = (QMenuBar*)(target->menuBar());
        }
    }
    else if (location == MOL_MAIN)
    {
        targetBar = mMenuBar;
    }

    //Does The Menu object already exist?
    MenuSystem::MenuObject* foundMenuObject = findMenuObject(caller, menuName, location, MOT_MENU);
    if (foundMenuObject == 0)
    {
        //not found, add
        newObj = new MenuSystem::Menu(menuName, menu, location, caller);
        bool success = newObj->attach();
        if (success)
            mMenuObjects.push_back(newObj);
        else
        {
            LOG_ERROR() << "Failed to attach new menu object. Deleting it.";
            delete newObj;
            return 0;
        }
    }
    else
    {
        //already created
        if (foundMenuObject->getCreator() == caller)
        {
            //same creator, replace
            foundMenuObject->update(menu);
            newObj = (MenuSystem::Menu*)foundMenuObject;
        }
        else
        {
            //different creator but same place, dont overwrite
            LOG_ERROR() << "Cannot overwrite other Widget's Menu " << menuName << ". Creator ID differs.";
            return 0;
        }
    }

    return newObj;
}

MenuSystem::MenuAction* MenuManager::provideMenuAction(const AbstractPlugin* caller, const QString& menuName, const QString& actionName, QAction* action, EMenuObjectLocation location)
{
    if (!assertProvidedParameters(caller, actionName, (QObject*)action, MOT_ACTION))
        return 0;

    LOG_INFO() << "TODO";

    return 0;
}

MenuSystem::ToolBar* MenuManager::provideToolBar(const AbstractPlugin* caller, const QString& toolBarName, QToolBar* toolBar, EMenuObjectLocation location)
{
    if (!assertProvidedParameters(caller, toolBarName, (QObject*)toolBar, MOT_TOOLBAR))
        return 0;

    MenuSystem::ToolBar* newObj = 0;
    QWidget* callerWidget = 0;

    if (caller->hasVisualRepresentation() == false)
    {
        callerWidget = 0;
        if (location == MOL_LOCAL)
        {
            LOG_WARNING() << "Cant provide 'Local' menu objects in a plugin without visual representation. Ignoring this flag.";
            location = MOL_MAIN;
        }
    }
    else
    {
        callerWidget = caller->getVisualRepresentation();
    }

    if (location == MOL_LOCAL)
    {
        if (callerWidget->layout() == 0)
        {
            callerWidget->setLayout(new QVBoxLayout());
        }
    }

    //Does the ToolBar object already exist?
    MenuSystem::MenuObject* foundMenuObject = findMenuObject(caller, toolBarName, location, MOT_TOOLBAR);
    if (foundMenuObject == 0)
    {
        //not found, add
        newObj = new MenuSystem::ToolBar(toolBarName, toolBar, location, caller);
        bool success = newObj->attach();
        if (success)
            mMenuObjects.push_back(newObj);
        else
        {
            LOG_ERROR() << "Failed to attach new menu object. Deleting it.";
            delete newObj;
            return 0;
        }
    }
    else
    {
        //already created
        if (foundMenuObject->getCreator() == caller)
        {
            //same creator, replace
            foundMenuObject->update(toolBar);
            newObj = (MenuSystem::ToolBar*)foundMenuObject;
        }
        else
        {
            //different creator but same place, dont overwrite
            LOG_ERROR() << "Cannot overwrite other Widget's ToolBar " << toolBarName << ". Creator ID differs.";
            return 0;
        }
    }

    return newObj;
}

MenuSystem::File* MenuManager::provideSourceFile(const AbstractPlugin* caller, const QString& containerName, const QString& fileName)
{
    if (!assertProvidedParameters(caller, containerName, 0, MOT_FILE))
        return 0;

    MenuSystem::File* newObj;

    //Does the ToolBar object already exist?
    MenuSystem::MenuObject* foundMenuObject = findMenuObject(caller, containerName, MOL_LOCAL, MOT_FILE);
    if (foundMenuObject == 0)
    {
        //not found, add
        newObj = new MenuSystem::File(containerName, fileName, caller);
        bool success = newObj->attach();
        if (success)
            mMenuObjects.push_back(newObj);
        else
        {
            LOG_ERROR() << "Failed to attach new menu object. Deleting it.";
            delete newObj;
            return 0;
        }
    }
    else
    {
        //already created
        if (foundMenuObject->getCreator() == caller)
        {
            //:TODO:
            LOG_INFO() << "TODO: Update File";
        }
        else
        {
            //different creator but same place, dont overwrite
            LOG_ERROR() << "Cannot overwrite other Widget's Container File " << containerName << ". Creator ID differs.";
            return 0;
        }
    }

    return newObj;
}


void MenuManager::cleanEntries(const AbstractPlugin* origin)
{

    TMenuObjects::iterator it = mMenuObjects.begin();
    while (it != mMenuObjects.end())
    {
        if ((*it)->getCreator() == origin)
        {
            (*it)->cleanEntry();
            delete *it;
            *it = 0;
            it = mMenuObjects.erase(it);
        }
        else
        {
            it++;
        }
    }
}

MenuSystem::MenuObject* MenuManager::findMenuObject(const AbstractPlugin* target, const QString& menuName, EMenuObjectLocation location, EMenuObjectType type) const
{
    for (TMenuObjects::const_iterator it = mMenuObjects.begin(); it != mMenuObjects.end(); it++)
    {
        //Search for parent Widget/Owner
        if ((*it)->getCreator() == target)
        {
            //Search for Type
            if ((*it)->getType() == type)
            {
                //Search for location
                if ((*it)->getLocation() == location)
                {
                    //Search for name
                    if ((*it)->getName().compare(menuName) == 0)
                    {
                        return *it;
                    }
                }
            }
        }
    }
    return 0;
}

void MenuManager::initConnections()
{
    //Connect menu update functions
    SimulationManager* simManager = mParent->getSimulationManager();
    PluginManager* pluginManager = mParent->getPluginManager();

    connect(simManager, SIGNAL(setupListReloaded()), this, SLOT(updateSimulationSetupList()));
    connect(simManager, SIGNAL(setupListEntryChanged(int)), this, SLOT(updateSimulationSetupList()));
    connect(simManager, SIGNAL(setupListSingleEntryAdded(int)), this, SLOT(updateSimulationSetupList()));
    connect(simManager, SIGNAL(setupListSingleEntryRemoved(int)), this, SLOT(updateSimulationSetupList()));
    connect(simManager, SIGNAL(setupSaved(QString, QString)), this, SLOT(updateSimulationSetupList()));

    connect(pluginManager, SIGNAL(pluginAdded(int)), this, SLOT(updatePluginToolBar()));
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool MenuManager::assertProvidedParameters(const AbstractPlugin* caller, const QString& objectName, const QObject* object, EMenuObjectType type) const
{
    if (!mInitialized)
    {
        LOG_ERROR() << "MenuManager not initialized yet. (When initializing: " << objectName << ")";
        return false;
    }

    if (caller == 0)
    {
        LOG_ERROR() << "Cannot accept menu object without caller. Caller pointer is 0. (When initializing: " << objectName << ")";
        return false;
    }

    if ((object == 0) && (type != MOT_FILE))
    {
        LOG_ERROR() << "Cannot accept menu object without object pointer. Object pointer is 0. (When initializing: " << objectName << ")";
        return false;
    }

    if ((object != 0) && (type == MOT_FILE))
    {
        LOG_WARNING() << "No object pointer expected when providing a source file. (When initializing: " << objectName << ")";
        return false;
    }

    return true;
}

bool MenuManager::addNamedMenu(QMenu* menu, const QString& name)
{
    //Add Menu entry with parent pointer 0 (main menu)
    MenuSystem::MenuObject* obj = (MenuSystem::MenuObject*)new MenuSystem::Menu(name, menu);
    obj->attach();
    mMenuObjects.push_back(obj);

    return true;
}

bool MenuManager::addNamedToolBar(QToolBar* toolBar, const QString& name)
{
    //Add ToolBox with parent pointer 0 (main menu)
    MenuSystem::MenuObject* obj = new MenuSystem::ToolBar(name, toolBar);
    obj->attach();
    mMenuObjects.push_back(obj);

    return true;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------
// File Menu / Tools

void MenuManager::load()
{
    mParent->getSimulationManager()->loadSimulationSetupByDialog(true, false); //load from last saved location, load data
}

void MenuManager::save()
{
    mParent->getSimulationManager()->saveSimulationSetupByDialog(true, false); //overwrite automatically if possible, save data
}

void MenuManager::saveAs()
{
    mParent->getSimulationManager()->saveSimulationSetupByDialog(false, false); //do not overwriting automatically, save data
}

void MenuManager::loadClean()
{
    mParent->getSimulationManager()->loadSimulationSetupByDialog(true, true); //load from last saved location, do not load data
}

void MenuManager::saveClean()
{
    mParent->getSimulationManager()->saveSimulationSetupByDialog(true, true); //overwrite automatically if possible, do not save data
}

void MenuManager::saveCleanAs()
{
    mParent->getSimulationManager()->saveSimulationSetupByDialog(false, true); //do not overwriting automatically, do not save data
}

void MenuManager::reload()
{
    SimulationManager* manager = mParent->getSimulationManager();
    QString currentSimulation = manager->getSimulation()->getFilename();
    manager->initializeSimulationFromFile(currentSimulation, true);
}

void MenuManager::openSetup(int index)
{
    //Initializes setup from current include file list -> use index
    QString setup = mParent->getSimulationManager()->getSetupList().at(index)->getName();
    mParent->getSimulationManager()->initializeSimulation(setup);
}

void MenuManager::exit()
{
    mParent->exit();
}

void MenuManager::updateSimulationSetupList()
{
    mFileMenuSetups->clear();
    while (mFileMenuSetupsActionReceivers.size() > 0)
    {
        delete mFileMenuSetupsActionReceivers.back();
        mFileMenuSetupsActionReceivers.pop_back();
    }
    SimulationManager* manager = mParent->getSimulationManager();

    QAction* action;
    int index = 0;
    for (SimulationManager::TSetupList::const_iterator it = manager->getSetupList().begin(); it != manager->getSetupList().end(); it++)
    {
        const SimulationSetup& setup = **it;
        NumberedActionReceiver* newReceiver = new NumberedActionReceiver(index);
        mFileMenuSetupsActionReceivers.push_back(newReceiver);

        QString caption = QString("%1%2").
            arg(manager->containsSetupIncludePath((*it)->getFileName()) == false ? "- " : " ").
            arg(setup.getName());

        action = mFileMenuSetups->addAction(caption, newReceiver, SLOT(receiveAction()));
        action->setToolTip(QString(tr("Load Simulation Setup '%12'. %2")).
            arg(setup.getName()).
            arg(setup.isSaved() == false ? "(not saved) " : ""));
        connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(openSetup(int)));
        index++;
    }
}


//------------------------------------------------------
// Simulation Menu / Tools

void MenuManager::runSimulation()
{
    mParent->getSimulationManager()->runSimulation();
}

void MenuManager::stopSimulation()
{
    mParent->getSimulationManager()->stopSimulation();
}

void MenuManager::pauseSimulation()
{
    mParent->getSimulationManager()->pauseSimulation();
}

void MenuManager::restartSimulation()
{
    mParent->getSimulationManager()->restartSimulation();
}

//------------------------------------------------------
// File Menu / Tools
void MenuManager::help()
{
    LOG_INFO() << "TODO";
}

void MenuManager::about()
{
    AboutDialog about(mParent->getWindowManager()->getMainWindow());
    about.exec();
}

void MenuManager::whatsThis()
{
    if (QWhatsThis::inWhatsThisMode())
        QWhatsThis::leaveWhatsThisMode();
    else
        QWhatsThis::enterWhatsThisMode();
}

//------------------------------------------------------
//Log Menu

void MenuManager::sendInitMessages()
{
    mParent->sendInitMessages();
}

void MenuManager::setSparkLogLevelDebug()
{
    MenuManager::setSparkLogLevel((int)Logger::Debug);
}

void MenuManager::setSparkLogLevelInfo()
{
    MenuManager::setSparkLogLevel((int)Logger::Info);
}

void MenuManager::setSparkLogLevelWarning()
{
    MenuManager::setSparkLogLevel((int)Logger::Warning);
}

void MenuManager::setSparkLogLevelError()
{
    MenuManager::setSparkLogLevel((int)Logger::Error);
}

void MenuManager::setSparkLogLevel(int level)
{
    if (level < Logger::Debug || level > Logger::Fatal)
    {
        LOG_ERROR() << "Illegal log level: " << level;
        return;
    }

    //Set log level for all spark controllers
    for (auto it = mSparkController.begin(); it != mSparkController.end(); it++)
    {
        (*it)->setLogProperties((Logger::LogLevel) level);
    }
}

void MenuManager::updateSparkLogLevel(SparkController* controller, Logger::LogLevel logLevel)
{
    //Add the controller to the update list
    if (!mSparkController.contains(controller))
    {
        mSparkController.push_back(controller);
        connect(controller, SIGNAL(destroyed(QObject*)), this, SLOT(removeSparkController(QObject*)));
    }

    //Update the menu entries with the log level
    if (logLevel == Logger::Debug)
        mSparkDebugLevelAction->setIcon(QIcon(":accept"));
    else
        mSparkDebugLevelAction->setIcon(QIcon(""));

    if (logLevel == Logger::Info)
        mSparkInfoLevelAction->setIcon(QIcon(":accept"));
    else
        mSparkInfoLevelAction->setIcon(QIcon(""));

    if (logLevel == Logger::Warning)
        mSparkWarningLevelAction->setIcon(QIcon(":accept"));
    else
        mSparkWarningLevelAction->setIcon(QIcon(""));

    if (logLevel == Logger::Error)
        mSparkErrorLevelAction->setIcon(QIcon(":accept"));
    else
        mSparkErrorLevelAction->setIcon(QIcon(""));
}

void MenuManager::removeSparkController(QObject* controller)
{
    for (auto it = mSparkController.begin(); it != mSparkController.end(); it++)
    {
        if ((*it) == controller)
        {
            mSparkController.erase(it);
            return;
        }
    }
}

//------------------------------------------------------
// Tools Menu / Tools
void MenuManager::showSettings()
{
    Carbon::get()->getSettingsDialog()->show();
}

void MenuManager::showPlugins()
{
    PluginDialog dialog(Carbon::get()->getWindowManager()->getMainWindow());
    dialog.exec();
}

//------------------------------------------------------
// Tools Menu / Tools
void MenuManager::createPlugin(int classId)
{
    PluginManager* pluginManager = mParent->getPluginManager();
    auto pos = PluginFactory::getFactory().getInstantiators().find(classId);

    if (pos == PluginFactory::getFactory().getInstantiators().end())
    {
        LOG_ERROR() << "Plugin class not found.";
        return;
    }

    if (!PluginFactory::getFactory().canCreatePlugin(classId))
    {
        LOG_INFO() << "Cant create another plugin of class " << pos->second->getName();
        return;
    }

    //Choose attachment and parameters
    QString parameter = "";
    QString attachment = "";
    if (pos->second->getPluginType() == PT_FRAME)
    {
        FrameAttachmentPoint* attachmentPoint = mParent->getWindowManager()->findDefaultFrameAttachmentPoint(true);
        if (attachmentPoint)
            attachment = attachmentPoint->getName();
    }

    //Create plugin definition
    boost::shared_ptr<PluginDefinition> definition(new PluginDefinition(
        pos->second->getName(), pos->second->getName(), pos->second->getPluginType(), PluginDefinition::RD_INHERIT, parameter, attachment));

    //Create plugin and store in current simulation
    AbstractPlugin* plugin = pluginManager->createPlugin(*definition, true);
}

void MenuManager::updatePluginToolBar()
{
    QAction* action;
    PluginManager* manager = mParent->getPluginManager();
    if (manager == 0)
        return;

    //Clear
    clearCreatePluginReceivers();
    mPluginToolBar->clear();

    //Create actions
    for (auto it = PluginFactory::getFactory().getInstantiators().begin(); it != PluginFactory::getFactory().getInstantiators().end(); it++)
    {
        int classId = it->first;

        //Create message receiver
        NumberedActionReceiver* receiver = new NumberedActionReceiver(classId);
        mCreatePluginActionReceivers.push_back(receiver);

        //Find icon
        QString iconName = manager->getPluginClassInfo(classId, AbstractPlugin::PI_ICON_NAME).toString();
        if (iconName.compare("") == 0)
            iconName = ":star";

        //Create action
        action = mPluginToolBar->addAction(QIcon(iconName), "", receiver, SLOT(receiveAction()));

        //Only enabled if maximum amount not reached
        if (manager->getInstanceCount(classId) >= manager->getMaxInstanceCount(classId))
            action->setEnabled(false);

        //Find description
        QString descriptionHeader = "\n\nDescription:\n";
        QString description = manager->getPluginClassInfo(classId, AbstractPlugin::PI_DESCRIPTION).toString();
        if (description.size() == 0)
            descriptionHeader = "";

        //Add description and whatsthis
        action->setToolTip("Create Plugin: " + it->second->getName() + descriptionHeader + description);

        QString typeText;
        switch (it->second->getPluginType())
        {
            case PT_NORMAL: typeText = "plugin"; break;
            case PT_SIGNAL: typeText = "(signal) plugin"; break;
            case PT_FRAME: typeText = "(attachable frame) plugin"; break;
            default: break;
        }
        action->setWhatsThis("Create a new " + typeText + " of class '" + it->second->getName() + "'.\n" \
            "The maximum amount for plugins of this class is " + QString::number(it->second->getMaxInstanceCount()) + "."
            + descriptionHeader + description);

        //Connect button
        connect(receiver, SIGNAL(actionReceived(int)), this, SLOT(createPlugin(int)));
    }
}

void MenuManager::clearCreatePluginReceivers()
{
    while (mCreatePluginActionReceivers.size() > 0)
    {
        mCreatePluginActionReceivers.back()->deleteLater();
        mCreatePluginActionReceivers.pop_back();
    }
}

//------------------------------------------------------

void MenuManager::cleanup()
{
    if (mClosed)
        return;
    mClosed = true;

    //clean all entries
    /*while (mMenuObjects.size() > 0)
    {
        delete mMenuObjects.back();
        mMenuObjects.pop_back();
    }*/
}
