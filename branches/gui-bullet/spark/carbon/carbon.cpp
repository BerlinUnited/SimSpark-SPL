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

#include "carbon.h"
#include "windowmanager.h"
#include "menumanager.h"
#include "pluginmanager.h"
#include "communicationmanager.h"
#include "simulationmanager.h"
#include "openglmanager.h"
#include "attachableframe.h"
#include "settingsdialog.h"
#include "settings.h"
#include "cutelogger\logmessage.h"
#include "simulationtask.h"
#include "sparkproperty.h"
#include "topic.h"
#include "libraryloader.h"

#include <stdio.h>
#include <iostream>
#include <QtGlobal>
#include <QDir>
#include <QMetaType>
#include <QMainWindow>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

Carbon* Carbon::carbon = 0;

QString Carbon::mUserDataFolder = QDir::homePath() + "/carbon/";

boost::shared_ptr<Carbon::CarbonLogControl> Carbon::mCarbonLogControlStatic = 
    boost::shared_ptr<Carbon::CarbonLogControl>(new Carbon::CarbonLogControl());

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Carbon::Carbon(const QApplication& app) :
    mSettings(new Settings("guisettings")),
    mApplication(app)
{
    //Use the static log control object
    mCarbonLogControl = boost::shared_ptr<CarbonLogControl>(Carbon::mCarbonLogControlStatic);

    //Choosing default Layout. This can be changed before calling initGui()
    mLayoutFile = "";
    mDefaultLayoutFile = "guigridlayout";
    mStartSetup = "";
    mUseJre = false;
    mInitialized = false;
    mClosed = false;
    mWindowManager = 0;
    mMenuManager = 0;
    mPluginManager = 0;
    mSimulationManager = 0;
    mOpenGLManager = 0;
    mCommunicationManager = 0;
    mInitialLayoutDirectories.clear();
    mInitialPluginDirectories.clear();
    mInitialSetupDirectories.clear(); //Will be handled by default setup

    //register this carbon pointer globally
    if (Carbon::carbon == 0)
    {
        Carbon::carbon = this;
    }
    else
    {
        LOG_FATAL() << "Only one Carbon must be instantiated.";
    }

    mWindowManager = new WindowManager(this);
    mMenuManager = new MenuManager(this);
    mPluginManager = new PluginManager(this);
    mSimulationManager = new SimulationManager(this);
    mOpenGLManager = new OpenGLManager(this);
    mCommunicationManager = new CommunicationManager(this);

    mSettingsDialog = new SettingsDialog();
    
    //Close carbon with the application
    //connect(&mApplication, SIGNAL(aboutToQuit()), this, SLOT(close()));
}

Carbon::~Carbon()
{
    close();
}

Carbon* Carbon::get()
{
    return Carbon::carbon;
}

const QString& Carbon::getLayoutFile() const
{
    return mLayoutFile;
}

WindowManager* Carbon::getWindowManager() const
{
    return mWindowManager;
}

MenuManager* Carbon::getMenuManager() const
{
    return mMenuManager;
}

PluginManager* Carbon::getPluginManager() const
{
    return mPluginManager;
}

SimulationManager* Carbon::getSimulationManager() const
{
    return mSimulationManager;
}

OpenGLManager* Carbon::getOpenGLManager() const
{
    return mOpenGLManager;
}

CommunicationManager* Carbon::getCommunicationManager() const
{
    return mCommunicationManager;
}

SettingsDialog* Carbon::getSettingsDialog() const
{
    return mSettingsDialog;
}

boost::shared_ptr<Settings> Carbon::getSettings() const
{
    return mSettings;
}

bool Carbon::isInitialized()
{
    if (Carbon::carbon != 0)
        return Carbon::get()->mInitialized;
    return false;
}

void Carbon::printHelp() const
{
    std::cout << "\nusage: gui [options]\n"
        << "\noptions:\n"
        << " --help\t\t\t print this message.\n"
        << " --layout-path NAME\t Adds an include path for layout library files.\n"
        << " --plugin-path NAME\t Adds an include path for plugin library files.\n"
        << " --setup-path NAME\t Adds an include path for simulation setup files.\n"
        << " --script-path NAME\t Adds a search path for script files.\n"
        << " --executable-path NAME\t Adds a search path for executables.\n"
        << " --layout NAME\t\t Name of gui layout library to use from one of the layout folders (without extension!).\n"
        << " --setup NAME\t\t Absolute path of name of simulation setup from one of the layout folders to initialize.\n"
        // SimSpark options
        << "\noptions (SimSpark):\n"
        << " --agent-port PORTNUM\t default port for agents to connect to.\n"
        << " --server-port PORTNUM\t default port for monitors to connect to.\n"
        << "\n";
}

int Carbon::initApp(int argc, char* argv[])
{
    int returning = processCmdLine(argc, argv);
    if (returning == 0) return 0;

    mInitialized = true;
    return returning;
}

bool Carbon::processCmdLine(int argc, char* argv[])
{
    for( int i = 1; i < argc; i++)
    {
        if(strcmp( argv[i], "--help" ) == 0){
            printHelp();
            return false;
        }
        else if (strcmp( argv[i], "--layout" ) == 0){
            i++;
            bool err = false;
            if (i < argc)
            {
                mLayoutFile = QString(argv[i]);
            }
            else 
                err = true;

            if (err)
            {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--setup") == 0){
            i++;
            if (i < argc)
                mStartSetup = QString(argv[i]);
            else {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--layout-path") == 0){
            i++;
            if (i < argc)
            mInitialLayoutDirectories << argv[i];
            else {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--plugin-path") == 0){
            i++;
            if (i < argc)
            mInitialPluginDirectories << argv[i];
            else {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--setup-path") == 0){
            i++;
            if (i < argc)
            mInitialSetupDirectories << argv[i];
            else {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--script-path") == 0){
            i++;
            if (i < argc)
            mInitialScriptDirectories << argv[i];
            else {
                printHelp();
                return false;
            }
        }
        else if(strcmp(argv[i], "--executable-path") == 0){
            i++;
            if (i < argc)
            mInitialExecutableDirectories << argv[i];
            else {
                printHelp();
                return false;
            }
        }
        else if (strcmp(argv[i], "--agent-port") == 0){
            i++;
            if (i < argc)
            mDefaultAgentPort = argv[i];
            else{
                printHelp();
                return false;
            }
        }
        else if (strcmp(argv[i], "--server-port") == 0){
            i++;
            if (i < argc)
                mDefaultServerPort = argv[i];
            else{
                printHelp();
                return false;
            }
        }
    }

    return true;
}

bool Carbon::initGui()
{
    LOG_DEBUG() << "Init Gui...";
    bool returning = true;

    //Additional Qt meta types
    qRegisterMetaType<boost::shared_ptr<LogMessage>>("LogMessageSPtr");
    qRegisterMetaType<boost::shared_ptr<SparkProperty::Property>>("SparkPropertySPtr");
    qRegisterMetaType<boost::shared_ptr<Topic>>("TopicSPtr");
    qRegisterMetaType<SimulationTask::ETaskExecutionState>("TaskExecutionState");
    qRegisterMetaType<Logger::LogLevel>("LogLevel");
    qRegisterMetaType<OpenGLManager::EGLStateSignal>("GLStateSignal");

    //Check for pre-initialization errors
    for (std::vector<QString>::iterator it = PluginFactory::getFactory().mErrorMessages.begin();
         it != PluginFactory::getFactory().mErrorMessages.end(); it++)
    {
        LOG_ERROR() << "During plugin initialization:" << *it;
    }

    //Create the data folder
    if (!Carbon::getUserDataFolder().compare("") == 0)
    {
        QDir dir(Carbon::getUserDataFolder());
        if (!dir.exists())
        {
            QDir root(QDir::rootPath());
            root.mkpath(dir.absolutePath());
        }
    }

    //Initialize
    LOG_DEBUG() << "Initializing Menu...";
    bool success = initMenu();
    if (!success)
    {
        LOG_ERROR() << "Error initializing menu. You can display initialization errors using the log menu.";
        returning = false;
    }

    if (returning == true)
    {
        LOG_DEBUG() << "Initializing PluginManager & Extension Plugins...";
        success = initPluginManager();
        if (!success)
        {
            LOG_ERROR() << "Error initializing PluginManager. You can display initialization errors using the log menu.";
            returning = false;
        }
    }

    if (returning == true)
    {
        LOG_DEBUG() << "Initializing Simulation Setup & Plugins...";
        success = initSetup();
        if (!success)
        {
            LOG_ERROR() << "Error initializing Simulation Setup. You can display initialization errors using the log menu.";
            returning = false;
        }
    }

    if (returning == true)
    {
        LOG_DEBUG() << "Initializing core SettingWidgets...";
        success = initSettingsWidgets();
        if (!success)
        {
            LOG_ERROR() << "Error initializing Setting Widgets. You can display initialization errors using the log menu.";
            returning = false;
        }
    }

    //Disable init logging
    useInitLogging(false);

    return returning;
}

bool Carbon::loadLayout()
{
    static bool layoutLoaded = false;
    if (layoutLoaded)
        return false;

    //Create loader, search available layouts
    LibraryLoader loader;

    if (mInitialLayoutDirectories.empty())
        mInitialLayoutDirectories.push_back("layout/");

    bool found_dir = false;
    for (auto it = mInitialLayoutDirectories.begin(); it != mInitialLayoutDirectories.end(); it++)
    {
        QDir dir(*it);
        if (!dir.exists())
        {
            LOG_DEBUG() << "Folder 'layouts' with gui window layout libraries was not found.";
            continue;
        }

        loader.addDirectory(dir.absolutePath());
        found_dir = true;
    }

    if (!found_dir)
    {
        LOG_ERROR() << "No include directory for gui layouts found.";
        return false;
    }

    //Search file
    boost::shared_ptr<QLibrary> lib = loader.getLibraryByFileName(mLayoutFile);
    if (!lib.get())
    {
        if (mLayoutFile.size() > 0) //not found
            LOG_ERROR() << "File " << mLayoutFile << " not found.";
        
        //Try default file
        mLayoutFile = mDefaultLayoutFile;
        lib = loader.getLibraryByFileName(mLayoutFile);
        if (!lib.get())
        {
            LOG_ERROR() << "File " << mLayoutFile << " not found.";
            return false;
        }
    }

    //Load and import
    bool loaded = false;
    if (mLayoutFile.size() > 0)
    {
        //Load library
        loaded = loader.load(lib);
        if (!loaded)
        {
            LOG_ERROR() << "Could not load layout library " << mLayoutFile << ".";
            return false;
        }

        //Import main window
        QMainWindow* mainWindow = loader.exportLayout(lib);
        if (mainWindow == 0)
        {
            LOG_ERROR() << "Could not create a main window with layout library " << mLayoutFile << ": " << lib->errorString();
            return false;
        }

        mWindowManager->setMainWindow(mainWindow);
        mainWindow->show();
    }
    else
        return false;

    layoutLoaded = true;
    return true;
}

void Carbon::updateLogSourceAdded(int id)
{
    if (!getLogControl()->mLoggingInitialized)
        return;

    emit logSourceAdded(id);
}

bool Carbon::initLogging()
{
    //Define log message sources
    int source = Logger::defineLogSource("Gui");
    int sparksource = Logger::defineLogSource("Spark");

    //Register 3 global loggers and store log messages before setup initialization
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    useInitLogging(control->mUseInitLogging);
    useFileLogging(control->mUseFileLogging);
    useDebugLogging(control->mUseDebugLogging);
    useConsoleLogging(control->mUseConsoleLogging);

    getLogControl()->mLoggingInitialized = true;

    if (source != 0)
    {
        LOG_WARNING() << "Gui log source id is not 0. Make sure not to define log sources befoe the gui called initLogging().";
    }

    return true;
}

void Carbon::useConsoleLogging(bool use)
{
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    if (control->mLoggingInitialized == false || control->mUseConsoleLogging == false)
    {
        //Register
        control->mConsoleAppender = new ConsoleAppender();
        //[log level] message
        control->mConsoleAppender->setFormat("[%l] %m\n");
        Logger::registerAppender(control->mConsoleAppender);
    }
    else
    {
        //Unregister
        Logger::unregisterAppender(control->mConsoleAppender);
        control->mConsoleAppender = 0;
    }
}

void Carbon::useFileLogging(bool use)
{
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    if (control->mLoggingInitialized == false || control->mUseFileLogging == false)
    {
        //Register
        QFileInfo info(mUserDataFolder + "/log/guilog.txt");
        control->mFileAppender = new FileAppender(info.absoluteFilePath());
        //DateTime [7 characters log level] <Calling Function, SourceId> message
        control->mFileAppender->setFormat("%t{dd-MM-yyyy, HH:mm} [%-7l] <%C, %s> %m\n");
        Logger::registerAppender(control->mFileAppender);
    }
    else
    {
        //Unregister
        Logger::unregisterAppender(control->mFileAppender);
        control->mFileAppender = 0;
    }
}

void Carbon::useDebugLogging(bool use)
{
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    if (control->mLoggingInitialized == false || control->mUseDebugLogging == false)
    {
        //Register
        control->mDebugAppender = new DebugAppender();
        //[7 characters log level] <Calling Function, SourceId> message
        control->mDebugAppender->setFormat("[%-7l] <%C, %s> %m\n");
        Logger::registerAppender(control->mDebugAppender);
    }
    else
    {
        //Unregister
        Logger::unregisterAppender(control->mDebugAppender);
        control->mDebugAppender = 0;
    }
}

void Carbon::useInitLogging(bool use)
{
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    if (control->mLoggingInitialized == false || control->mUseInitLogging == false)
    {
        //Register
        control->mInitMessageAppender = new FunctionAppender(storeInitMessage);
        //[7 characters log level] 'filename', l.line <Calling Function> message
        control->mInitMessageAppender->setFormat("[%-7l] '%f', l.%i <%C> %m");
        Logger::registerAppender(control->mInitMessageAppender);
    }
    else
    {
        //Unregister
        Logger::unregisterAppender(control->mInitMessageAppender);
        control->mDebugAppender = 0;
    }
}

void Carbon::setUserDataFolder(const QString& path)
{
    mUserDataFolder = path;
}

const QString& Carbon::getUserDataFolder()
{
    return mUserDataFolder;
}

void Carbon::sendInitMessages(Logger::LogLevel minLevel, bool message)
{
    boost::shared_ptr<CarbonLogControl> control(getLogControl());
    bool initLogging = control->mUseInitLogging;
    useInitLogging(false);

    if (message)
        LOG_INFO() << "Re-sending Init Messages with minimum level " << (int) minLevel << ":";

    for (std::vector<std::pair<QString, Logger::LogLevel> >::iterator it = control->mInitMessages.begin(); it != control->mInitMessages.end(); it++)
    {
        if ((int)minLevel <= (int)it->second)
        {
            LOG_MESSAGE(it->second) << it->first.toStdString().c_str();
        }
    }

    useInitLogging(initLogging);
}

void Carbon::storeInitMessage(QString message, Logger::LogLevel level)
{
    if (level != Logger::Debug)
        getLogControl()->mInitMessages.push_back(std::make_pair(message, level));
}

void Carbon::close()
{
    if (mClosed)
        return;

    emit aboutToShutdown();

    //Remove tools first
    LOG_DEBUG() << "Delete settings dialog...";
    delete mSettingsDialog;

    //Remove all plugins next, so they can still use the managers
    LOG_DEBUG() << "Delete plugin manager...";
    delete mPluginManager;

    //Remove menu manager since there are no entries from plugins anymore 
    LOG_DEBUG() << "Delete menu manager...";
    delete mMenuManager;

    //Delete windows and attachment points
    LOG_DEBUG() << "Delete window manager...";
    delete mWindowManager;

    //Delete simulation manager & threads
    LOG_DEBUG() << "Delete simulation manager...";
    delete mSimulationManager;

    //Delete opengl manager
    LOG_DEBUG() << "Delete OpenGl manager...";
    delete mOpenGLManager;

    //Delete communication manager
    LOG_DEBUG() << "Delete simulation manager...";
    delete mCommunicationManager;

    mClosed = true;
    mInitialized = false;
}

void Carbon::exit()
{
    mApplication.exit();
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void Carbon::registerStaticCarbonPointer(Carbon* carbonPtr)
{
    if (Carbon::carbon == 0)
    {
        Carbon::carbon = carbonPtr;
    }
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool Carbon::initMenu()
{
    //Fill the menu and toolbars
    return mMenuManager->initMenu();
}

bool Carbon::initPluginManager()
{
    //Load plugin libraries
    return mPluginManager->init();
}

bool Carbon::initSetup()
{
    //Load setups
    mSimulationManager->init();

    //Init simulation
    bool includedSetup = true;
    QString setup = mSimulationManager->getDefaultSetup(); //Default setup
    if (mStartSetup.size() != 0)
    {
        //Relative path to setup (checked, by name)
        if (mSimulationManager->containsSetupIncludePath(mStartSetup))
            setup = mStartSetup;
        else
        {
            //Absolute path to setup (checked, by path)
            QFileInfo info(mStartSetup);
            QString path = info.absoluteFilePath();
            if (mSimulationManager->containsSetupIncludePath(path))
            {
                setup = path;
            }
            else if (info.exists())
            {
                //Absolute path to setup (unchecked, by path)
                setup = path;
                includedSetup = false;
            }
        }
    }

    if (includedSetup)
        return mSimulationManager->initializeSimulation(setup);
    else
        return mSimulationManager->initializeSimulationFromFile(setup);
}

bool Carbon::initSettingsWidgets()
{
    //Add gui settings widgets to the settings dialog
    mSimulationManager->initializeSettingWidget();
    mPluginManager->initializeSettingWidget();

    return true;
}

boost::shared_ptr<Carbon::CarbonLogControl> Carbon::getLogControl()
{
    if (Carbon::get() != 0)
        return Carbon::get()->mCarbonLogControl;
    else
        return Carbon::mCarbonLogControlStatic;
}

//--------------------------------------------------------------
// MetaTypes
//--------------------------------------------------------------

Q_DECLARE_METATYPE(boost::shared_ptr<LogMessage>)
Q_DECLARE_METATYPE(boost::shared_ptr<SparkProperty::Property>)
Q_DECLARE_METATYPE(boost::shared_ptr<Topic>)
Q_DECLARE_METATYPE(SimulationTask::ETaskExecutionState)
Q_DECLARE_METATYPE(Logger::LogLevel)
Q_DECLARE_METATYPE(OpenGLManager::EGLStateSignal)