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

#ifndef MENUMANAGER_H
#define MENUMANAGER_H
/**
 * \file menumanager.h
 * \brief Contains MenuManager class, which manages global and provided Menu Entries and Tools.
 */

#include "carbon.h"

#include <QObject>
#include <QString>
#include <QMenuBar>
#include <QSize>
#include <map>
#include <vector>

class Carbon;
class QMenu;
class QToolBar;
class QAction;
class AttachableFrame;
class AbstractPlugin;
class QMenuBar;
class NumberedActionReceiver;
class SparkController;

namespace MenuSystem
{
    class MenuObject;
    class File;
    class Menu;
    class MenuAction;
    class ToolBar;
}

/*!
 \brief Manager class for the MainWindow's and attached Plugins's Menus and Toolboxes.

 At time of initialization the MenuManager creates global Menus and Toolboxes.
 At time of frame instantiation they provide the MenuManager with additional Menus, Actions and Toolboxes for their control.
 Tools, Menus and so on are named and stored as MenuObject.

 While Plugins and SignalPlugins may only provide global toolboxes and menu entries, AttachableFrames can create and provide local entries that are build in
 into their frame at a specified location.
*/
class MenuManager : public QObject
{
    Q_OBJECT

public:
    // public types
    /*!
     \brief Helper-Type
    */
    typedef std::vector<MenuSystem::MenuObject*> TMenuObjects;

    /*!
     \brief Location of an own or provided MenuObject.
    */
    enum EMenuObjectLocation
    {
        MOL_MAIN, /*!< In main window. */
        MOL_LOCAL /*!< Locally in the window that provided the MenuObject. */
    };

    /*!
     \brief Type of a provided MenuObject.
    */
    enum EMenuObjectType
    {
        MOT_MENU, /*!< QMenu to be displayed in a QMenuBar. */
        MOT_ACTION, /*!< QAction to be displayed in a QMenu. */
        MOT_ACTIONGROUP, /*!< List of QActions to be displayed in a QMenu. */
        MOT_TOOLBAR, /*!< QToolBar to be displayed in a QWidget/AttachableFrame. */
        MOT_FILE /*!< Definition of an xml-file that contains several kinds of MenuObjects. */
    };

    /*!
     \brief Different default menu styles.
    */
    enum EMenuStyle
    {
        MS_MAIN_MENU, /*!< Default style for MainMenu. */
        MS_FRAME_MENU /*!< Default style for AttachableFrames. */
    };

    /*!
     \brief Different default ToolBar styles.
    */
    enum EToolBarStyle
    {
        TS_SMALL, /*!< Toolbar with small icons, without names. */
        TS_BIG, /*!< Toolbar with big icons, without names. */
        TS_BIG_NAMED /*!< Toolbar with big icons and tool names. */
    };

public:
    // public functions
    /*!
     \brief Basic constructor.

     \param parent the creating Carbon
    */
    MenuManager(Carbon* parent);
    /*!
     \brief Basic destructor
    */
    virtual ~MenuManager();

    /*!
     \brief Returns default icon size for toolbars.
    */
    QSize getDefaultToolBarIconSize();
    /*!
     \brief Returns big icon size for toolbars.
    */
    QSize getBigToolBarIconSize();

    /*!
     \brief Returns the MenuManager of the active Carbon instance.
    */
    static MenuManager* getManager();
    /*!
     \brief Returns the main MenuBar of the active MenuManager.
    */
    static QMenuBar* getMenuBar();

     /*!
     \brief Changes a toolbar style to a fixed style.

     Should be called by every Plugin when creating a new toolbar for the main window to unify the look of all toolbars.
     \param bar toolbar to change
     \param style predefined style to use
    */
    static void changeToolBarStyle(QToolBar* bar, EToolBarStyle style);

     /*!
     \brief Changes a menu style to a fixed style.

     Should be called by every Plugin when creating a new menu to unify the look of all toolbars.
     \param menu menu object to change
     \param style predefined style to use
    */
    static void changeMenuStyle(QMenu* menu, EMenuStyle style);

    /*!
     \brief Searches fot a MenuObject with 4 specific properties.

     \param creator creator of the object, 0 for MenuManager
     \param menuName name of the object
     \param location location of the object
     \param type type of the object
    */
    MenuSystem::MenuObject* findMenuObject(const AbstractPlugin* creator, const QString& menuName, EMenuObjectLocation location, EMenuObjectType type) const;

    /*!
     \brief Initializes the menu by creating default menu entries.
    */
    bool initMenu();

    /*!
     \brief Allows providing Qt MenuObjects to the manager from the outside, for example from the constructor of a Plugin.

     Priveded MenuObjects are displayed in the main window or the calling frame, depending on parameters.
     Even if the Menu is only used locally in a Frame, it may provide it to the MenuManager so he can track and control activation properties and
     look of the MenuObject.
     The name of a MenuObject is used to identify it when specifying a location to insert new menu objects to.
     The name has to be unique locally.
     E.g. in one Menu there may only be one Action with the same name, in one MenuBar there may only be one Menu with the same name etc.

     \param caller Plugin that created the menu
     \param menuName locally unique name
     \param menu pointer to the menu
     \param location target location of the menu
     \return pointer to the created menu object, or 0 if creation failed
    */
    MenuSystem::Menu* provideMenu(const AbstractPlugin* caller, const QString& menuName, QMenu* menu, EMenuObjectLocation location = MOL_MAIN);
    /*!
     \brief Allows providing Qt MenuObjects to the manager from the outside, for example from the constructor of a Plugin.

     Priveded MenuObjects are displayed in the main window or the calling frame, depending on parameters.
     Even if the Menu is only used locally in a Frame, it may provide it to the MenuManager so he can track and control activation properties and
     look of the MenuObject.
     The name of a MenuObject is used to identify it when specifying a location to insert new menu objects to.
     The name has to be unique locally.
     E.g. in one Menu there may only be one Action with the same name, in one MenuBar there may only be one Menu with the same name etc.

     \param caller Plugin that created the action
     \param menuName name of the menu to insert action in
     \param actionName name of the action
     \param action pointer to the action
     \param location target location of the action
     \return pointer to the created menu object, or 0 if creation failed
    */
    MenuSystem::MenuAction* provideMenuAction(const AbstractPlugin* caller, const QString& menuName, const QString& actionName, QAction* action, EMenuObjectLocation location = MOL_MAIN);
    /*!
     \brief Allows providing Qt MenuObjects to the manager from the outside, for example from the constructor of a Plugin.

     Priveded MenuObjects are displayed in the main window or the calling frame, depending on parameters.
     Even if the Menu is only used locally in a Frame, it may provide it to the MenuManager so he can track and control activation properties and
     look of the MenuObject.
     The name of a MenuObject is used to identify it when specifying a location to insert new menu objects to.
     The name has to be unique locally.
     E.g. in one Menu there may only be one Action with the same name, in one MenuBar there may only be one Menu with the same name etc.

     \param caller Plugin that created the toolBar
     \param toolBarName locally unique name
     \param toolBar pointer to the toolBar
     \param location target location of the toolBar
     \return pointer to the created menu object, or 0 if creation failed
    */
    MenuSystem::ToolBar* provideToolBar(const AbstractPlugin* caller, const QString& toolBarName, QToolBar* toolBar, EMenuObjectLocation location = MOL_MAIN);
    /*!
     \brief Specifies a path to an xml-file with many Menu Objects to provide the MenuManager with.

     This function allows specifying, editing and deleting buttons and other MenuEntries, Toolboxes etc. without having to recompile the GUI.

     \param caller Plugin that provided the file
     \param containerName locally unique name for the object container
     \param fileName filename of the xml-file
     \return pointer to the created menu object, or 0 if creation failed
    */
    MenuSystem::File* provideSourceFile(const AbstractPlugin* caller, const QString& containerName, const QString& fileName);

    /*!
     \brief Deletes all MenuObjects that have been created by a specific plugin.

     Is called automatically when a plugin destructor is called.
     \param origin frame to delete MenuObject from
    */
    void cleanEntries(const AbstractPlugin* origin);

    /*!
     \brief Initializes signal/slot connections to update the menu when the simulation environment changes.

     Is called by the Carbon.
    */
    void initConnections();

private:
    // private functions

    /*!
     \brief Checks if parameters are ok and manager has been initialized already.

     Used by provide-functions.

     \param caller calling plugin
     \param objectName name of the object
     \param object pointer to object
    */
    bool assertProvidedParameters(const AbstractPlugin* caller, const QString& objectName, const QObject* object, EMenuObjectType type) const;

    /*!
     \brief Adds a menu and names it.

     \param menu menu to add
     \param name name to give
    */
    bool addNamedMenu(QMenu* menu, const QString& name);
    /*!
     \brief Adds a toolbar and names it.

     \param toolBar toolbar to add
     \param name name to give
    */
    bool addNamedToolBar(QToolBar* toolBar, const QString& name);

public slots:
    //-----------------------------
    // File Menu / Tools
    //-----------------------------

    /*!
     \brief Load simulation.
    */
    void load();
    /*!
     \brief Save simulation to the current file.
    */
    void save();
    /*!
     \brief Save simulation.
    */
    void saveAs();
    /*!
     \brief Load simulation without loading data files for the plugins.
    */
    void loadClean();
    /*!
     \brief Save simulation setup to the current file without saving plugin data files.
    */
    void saveClean();
    /*!
     \brief Save simulation setup without saving plugin data files.
    */
    void saveCleanAs();
    /*!
     \brief Exit program.
    */
    void exit();
    /*!
     \brief Reload and reinitialize the currently loaded setup from its file.
    */
    void reload();
    /*!
     \brief Open setup from the simulation manager list, displayed in the dynamic setup submenu.
    */
    void openSetup(int index);

    /*!
     \brief Recreates menu actions to open one of the contained SimulationSetups of the SimulationManager.
    */
    void updateSimulationSetupList();

    //-----------------------------
    // Simulation Menu / Tools
    //-----------------------------

    /*!
     \brief Try to run the current Simulation.
    */
    void runSimulation();
    /*!
     \brief Try to stop the current Simulation.
    */
    void stopSimulation();
    /*!
     \brief Try to pause the current Simulation.
    */
    void pauseSimulation();
    /*!
     \brief Try to restart the current Simulation.
    */
    void restartSimulation();

    //-----------------------------
    // File Menu / Tools
    //-----------------------------
    /*!
     \brief Show help window.

     \todo Add a help topic browser based on Wiki, Whatsthis and/or Doxygen.
    */
    void help();
    /*!
     \brief Show about window.
    */
    void about();
    /*!
     \brief Enter whatsthis-mode.
    */
    void whatsThis();

    //-----------------------------
    // Log Menu / Tools
    //-----------------------------
    /*!
     \brief Send the carbon init log messages.
    */
    void sendInitMessages();

    /*!
     \brief Sets the log level of SimSpark to Debug.
    */
    void setSparkLogLevelDebug();
    /*!
     \brief Sets the log level of SimSpark to Info.
    */
    void setSparkLogLevelInfo();
    /*!
     \brief Sets the log level of SimSpark to Warning.
    */
    void setSparkLogLevelWarning();
    /*!
     \brief Sets the log level of SimSpark to Error.
    */
    void setSparkLogLevelError();

    /*!
     \brief Sets the log level of SimSpark.

     The log level specifies which messages are sent to the Gui logging system.
    */
    void setSparkLogLevel(int level);

    /*!
     \brief Updates menu actions for Spark log level.
    */
    void updateSparkLogLevel(SparkController* source, Logger::LogLevel logLevel);

    /*!
     \brief Removes a spark controller from the list of controller to update.

     Called when controller is removed.
    */
    void removeSparkController(QObject* controller);

    //-----------------------------
    // Tools Menu / Tools
    //-----------------------------
    /*!
     \brief Show settings dialog.
    */
    void showSettings();

    /*!
     \brief Show plugins dialog.
    */
    void showPlugins();

    //-----------------------------
    // Plugin Menu / Tools
    //-----------------------------
    /*!
     \brief Show settings dialog.
    */
    void createPlugin(int classId);
    /*!
     \brief Update content and icons of the plugin toolbar.
    */
    void updatePluginToolBar();
    /*!
     \brief Clear action receivers for plugin toolbar clicks.
    */
    void clearCreatePluginReceivers();

    //-----------------------------
    /*!
     \brief Makes the MenuManager ready to be deleted. Removes all provided menu objects.

     Called by Carbons aboutToShutdown() signal.
    */
    void cleanup();

public:
    QMenuBar* mMenuBar;     /*!< Main QMenuBar of the MenuManager and the application. */

private:
    // private members

    static QSize mDefaultToolBarIconSize; /*!< Size of normal icons. */
    static QSize mBigToolBarIconSize;     /*!< Size of big icons. */

    bool mInitialized; /*!< True after initialization. */
    bool mClosed;      /*!< True after cleanup and ready to shutdown. */
    Carbon* mParent;   /*!< Carbon instance that created the MenuManager. */

    std::vector<NumberedActionReceiver*> mFileMenuSetupsActionReceivers; /*!< Receiver for click actions on setup eintries. */
    std::vector<NumberedActionReceiver*> mCreatePluginActionReceivers; /*!< Receiver for instancing plugins. */
    QMenu* mFileMenu;       /*!< File menu. */
    QMenu* mFileMenuSetups; /*!< File menus setups submenu. */
    QMenu* mWindowMenu;     /*!< Window menu. */
    QMenu* mHelpMenu;       /*!< Help menu. */
    QMenu* mLogMenu;        /*!< Log menu. */
    QMenu* mToolsMenu;      /*!< Tools menu. */
    QMenu* mSimulationMenu; /*!< Simulation menu. */

    QToolBar* mFileToolBar;         /*!< File tools. */
    QToolBar* mWindowToolBar;       /*!< Window tools. */
    QToolBar* mSimulationToolBar;   /*!< Simulation tools. */
    QToolBar* mPluginToolBar;       /*!< Plugin tools. */

    TMenuObjects mMenuObjects;      /*!< List of MenuObjects. */

    //Log command targets
    QList<SparkController*> mSparkController; /*!< List of created spark controllers. */
    QAction* mSparkDebugLevelAction;   /*!< Menu action to change spark log level. */
    QAction* mSparkInfoLevelAction;    /*!< Menu action to change spark log level. */
    QAction* mSparkWarningLevelAction; /*!< Menu action to change spark log level. */
    QAction* mSparkErrorLevelAction;   /*!< Menu action to change spark log level. */
};

#endif // MENUMANAGER_H
