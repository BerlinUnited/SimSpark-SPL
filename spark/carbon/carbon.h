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

#ifndef CARBON_H
#define CARBON_H
/**
 * \file carbon.h
 * \brief Contains Carbon class, the main control system for the gui system.
 */

#include <QDebug>
#include <QObject>
#include <QStringList>

#include "cutelogger/logger.h"
#include "cutelogger/consoleappender.h"
#include "cutelogger/fileappender.h"
#include "cutelogger/debugappender.h"
#include "cutelogger/functionappender.h"

#include <map>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#if defined(_MSC_VER)
        // shared library export/import stuff
        #define LIB_EXPORT __declspec(dllexport)
        #define LIB_IMPORT __declspec(dllimport)
        // hide the attribute definitions from MSVC
        #define  __attribute__(x)  /*NOTHING*/
#elif   defined(__GNUC__)
        #define LIB_EXPORT
        #define LIB_IMPORT
#endif

class QApplication;
class QMainWindow;
class MenuManager;
class WindowManager;
class PluginManager;
class SimulationManager;
class Carbon;
class SettingsDialog;
class OpenGLManager;
class CommunicationManager;
class AbstractPlugin;

class Settings;

/*!
 \brief Main controller class of the Carbon library.

 Controls Initialization of the Logging System, and the manager elements.
 Only one carbon instance may be initialized at a time. After initialization the controller is globally accesible with Carbon::get().

 \sa MenuManager
 \sa OpenGLManager
 \sa CommunicationManager
 \sa WindowManager
 \sa SimulationManager
 \sa PluginManager
*/
class Carbon : public QObject
{
    Q_OBJECT

public:
    // protected types

    /*!
      \brief Collection for Carbon log data and definitions.

      One static instance is initialized before creation of the Carbon and will be used by the first Carbon to be created.
    */
    struct CarbonLogControl
    {
        /*!
          \brief Constructor.
        */
        CarbonLogControl() :
            mFileAppender(0),
            mDebugAppender(0),
            mConsoleAppender(0),
            mInitMessageAppender(0),
            mUseFileLogging(true),
            mUseDebugLogging(true),
            mUseConsoleLogging(true),
            mUseInitLogging(true),
            mLoggingInitialized(false)
            {
                mInitMessages.clear();
            }
        /*!
          \brief Destructor.
        */
        ~CarbonLogControl(){}

        FileAppender* mFileAppender; /*!< Log appender for file logging. */
        DebugAppender* mDebugAppender; /*!< Log appender for debug logging. */
        ConsoleAppender* mConsoleAppender; /*!< Log appender for console logging. */
        FunctionAppender* mInitMessageAppender; /*!< Log appender to log init messages, so they can be resent later. */
        bool mUseFileLogging; /*!< Specifies if file logging is used. */
        bool mUseDebugLogging; /*!< Specifies if debug logging is used. */
        bool mUseConsoleLogging; /*!< Specifies if console logging is used. */
        bool mUseInitLogging; /*!< Specifies if init logging is active. */
        bool mLoggingInitialized; /*!< True after initialization of logging. */
        std::vector<std::pair<QString, Logger::LogLevel> > mInitMessages; /*!< List of messages that have been sent before the initialization was done. */
    };

public:
    // public functions

    /*!
     \brief Basic contructor.
    */
    Carbon(const QApplication& app);
    /*!
     \brief Basic destructor.
    */
    virtual ~Carbon();

    /*!
     \brief Returns the pointer to the static carbon instance.

     Must not be called before initialization.
    */
    static Carbon* get();

    /*!
     \brief Returns the current layouts source library name.

     The library is located in the default layout library folder.
    */
    const QString& getLayoutFile() const;
    /*!
     \brief Returns the WindowManager.
    */
    WindowManager* getWindowManager() const;
    /*!
     \brief Returns the MenuManager.
    */
    MenuManager* getMenuManager() const;
    /*!
     \brief Returns the PluginManager.
    */
    PluginManager* getPluginManager() const;
    /*!
     \brief Returns the SimulationManager.
    */
    SimulationManager* getSimulationManager() const;
    /*!
     \brief Returns the OpenGLManager.
    */
    OpenGLManager* getOpenGLManager() const;
    /*!
     \brief Returns the CommunicationManager.
    */
    CommunicationManager* getCommunicationManager() const;

    /*!
     \brief Returns the Settings Dialog Window.
    */
    SettingsDialog* getSettingsDialog() const;
    /*!
     \brief Returns pointer to global Settings Object.
    */
    boost::shared_ptr<Settings> getSettings() const;

    /*!
     \brief Checks if first initialization (Constructor + InitApp) have been completed successfully.

     Afterwards Carbon is accessible via Carbon::get().
     This does not mean that the GuiInitialization has finished!
     \return true if Carbon initialization succeeded.
    */
    static bool isInitialized();

    /*!
     \brief Initialized with commandline options.

     Called once.

     \param argc commandline parameter count
     \param argv commandline argument list
    */
    int initApp(int argc, char** argv);
    /*!
     \brief Prints commandline options.
    */
    void printHelp() const;
    /*!
     \brief Prints a greeting.
    */
    void printGreeting() const;
    /*!
     \brief Processes commandline options

     \param argc commandline parameter list
     \param argv[] commandline argument list
    */
    bool processCmdLine(int argc, char* argv[]);

    /*!
     \brief Loads and initializes the main window of the Gui.

     The main window is defined by an external shared layout library.
     The window to load can be specified by the command line parameters.
     \return true if loading was successful, false otherwise
    */
    bool loadLayout();

    /*!
     \brief Initialization of the Gui.

     Starts WindowManager, MenuManager etc.
     \return truue if no errors occured
    */
    bool initGui();

    /*!
     /brief Notification from the logger that a new log source was added.

     Emits logSourceAdded.

     \param id id of the log source
    */
    void updateLogSourceAdded(int id);

    /*!
     \brief Initializes global logging.

     Called once, before initialization of the first Carbon object.
    */
    static bool initLogging();
    /*!
     \brief Registers or unregisters a log appender for standard out stream.
    */
    static void useConsoleLogging(bool use);
    /*!
     \brief Registers or unregisters a log appender for a log file.
    */
    static void useFileLogging(bool use);
    /*!
     \brief Registers or unregisters a logger for platform dependent debug streams.
    */
    static void useDebugLogging(bool use);
    /*!
     \brief Registers or unregisters a logger for platform dependent debug streams.
    */
    static void useInitLogging(bool use);

    /*!
     \brief Set the absolute root path for user data.

     Data will be stored to 'extensions', 'log', and 'setups' subfolders.
     \param path root path
    */
    static void setUserDataFolder(const QString& path);

    /*!
     \brief Returns the absolute root path for user data.

     Should not be changed after creation of a carbon.
     Initially it is ${HOME}/carbon/
     \return absolute path to the user data root dir
    */
    static const QString& getUserDataFolder();

    /*!
     \brief Re-sends all messages stored before the initialization of the Carbon was over as new log messages.

     The new messages have the same log level as the original messages.

     \param minLevel minimum level of messages to send
     \param sendTitleMessage sends a log message informing that messages are being resent as info message
    */
    static void sendInitMessages(Logger::LogLevel minLevel = Logger::Debug, bool sendTitleMessage = true);
    /*!
     \brief Store a new init log message.

     Called by the Carbon.
     Useless after initialization.

     \param message message to store
     \param level log level of the message
    */
    static void storeInitMessage(QString message, Logger::LogLevel level);

    /*!
     \brief Defines the static carbon pointer again. Used in libraries when plugins are exported.

     Plugins may use the static carbon pointer indirectly.
     On Windows the static variables are not shared so they need to be redefined.
    */
    static void registerStaticCarbonPointer(Carbon* carbon);

private:
    // private functions

    /*!
     \brief Initializes the MenuManager.

     Contains global Menu entries and global Toolboxes.
    */
    bool initMenu();
    /*!
     \brief Initializes the PluginManager and extension plugins that are used in every simulation (including background plugins and AttachableFrames).
    */
    bool initPluginManager();
    /*!
     \brief Initializes default SimulationSetup with its additional plugins (including background plugins and AttachableFrames).
    */
    bool initSetup();
    /*!
     \brief Initializes SettingsWidgets of the gui core and provides them to the SettingsDialog.
    */
    bool initSettingsWidgets();
    /*!
     \brief Returns the log appender collection for the static carbon instance, or the static log control object if no cotnroller was found.
    */
    static boost::shared_ptr<CarbonLogControl> getLogControl();

public: signals:

    /*!
     \brief Emitted when the application is about to quit and all plugins and control elements should get ready to quit.

     This signal is crucial to receive for any plugin that contains references to outer resources like logger-callbacks or MenuEntries.
     Since Frames have to be deleted with deleteLater() it is up to Qt to decide when they are finally removed. This means, the constructor may sometimes be called
     after the Management-Elements (WindowManager, MenuManager, SimulationManager, PluginManager) have been destroyed.
     Using these Resources in the destructor may cause a crash.

     Receiving the aboutToShutdown() signal and connecting it to a cleanup function, that is called by the destructor too is the safe way to remove plugins.
    */
    void aboutToShutdown();

    /*!
     \brief Emitted when a new log source was added in the Logger.

     \param id id of the log source
    */
    void logSourceAdded(int id);

public slots:

    /*!
     \brief Close the Carbon and all management objects. Called by the applications aboutToQuit() signal.

     To manually exit the application call "exit()".
    */
    void close();

    /*!
     \brief Close the application.

     Gracefully shuts down all application elements and asks to save contents.
    */
    void exit();

public:
    // public members

    QStringList mInitialLayoutDirectories;      /*!< List of directories for gui layouts. Can be defined by command line parameters. Will be added to the respective include path. */
    QStringList mInitialPluginDirectories;      /*!< List of directories for gui plugins. Can be defined by command line parameters. Will be added to the respective include path. */
    QStringList mInitialSetupDirectories;       /*!< List of directories for simulation setups. Can be defined by command line parameters. Will be added to the respective include path.*/
    QStringList mInitialScriptDirectories;      /*!< List of directories for script search paths. Can be defined by command line parameters. Will be added to the respective include path.*/
    QStringList mInitialExecutableDirectories;  /*!< List of directories for executables search paths. Can be defined by command line parameters. Will be added to the respective include path.*/

private:
    // private members

    static Carbon *carbon; /*!< Static pointer to active carbon instance. */

    const QApplication& mApplication; /*!< Reference to the application. */

    WindowManager* mWindowManager;               /*!< WindowManager pointer. */
    MenuManager* mMenuManager;                   /*!< MenuManager pointer. */
    PluginManager* mPluginManager;               /*!< PluginManager pointer. */
    SimulationManager* mSimulationManager;       /*!< SimulationManager pointer. */
    OpenGLManager* mOpenGLManager;               /*!< OpenGLManager pointer. */
    CommunicationManager* mCommunicationManager; /*!< CommunicationManager pointer. */

    SettingsDialog* mSettingsDialog;       /*!< Settings Dialog Window. */
    boost::shared_ptr<Settings> mSettings; /*!< Global Settings object. May be stored in different places.*/

    std::string mScriptPath;        /*!< ScriptPath for Ruby scripts. */
    std::string mDefaultAgentPort;  /*!< Default port for agent communication. */
    std::string mDefaultServerPort; /*!< Default port for monitor communication. */

    QString mLayoutFile;        /*!< Name of the current main window library of the Gui. */
    QString mDefaultLayoutFile; /*!< Name of the default main window library of the Gui. */
    QString mStartSetup;        /*!< Name of absolute path of simulation setup file to initialize simulation with. */
    bool mUseJre;        /*!< Usage of Java Runtime Environment. */

    bool mInitialized; /*!< True after primary initialization. */
    bool mClosed;      /*!< True after close() has been called. */

    //Static
    static QString mUserDataFolder; /*!< Root path for user data. */

    //Logging
    static boost::shared_ptr<Carbon::CarbonLogControl> mCarbonLogControlStatic; /*!< Log appender collection for carbon. Created if there is no static gui controller yet. */
    boost::shared_ptr<Carbon::CarbonLogControl> mCarbonLogControl; /*!< Log appender collection for carbon. Created from the static log control and used afterwards. */
};

#endif // CARBON_H
