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

#ifndef SETUPFRAME_H
#define SETUPFRAME_H
/**
 * \file setupframe/setupframe.h
 * \brief Contains SetupFrame plugin class.
 */

#include <attachableframe.h>
#include <simulationsetup.h>
#include <settings.h>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include <QListView>
#include <QString>

#include "ui_setupframe.h"

class QFileSystemModel;
class NumberedActionReceiver;

/*!
 \brief AttachableFrame plugin that allows loading, creating and saving SimulationSetups as contained in the SimulationManager.

 The Frame displays all Setups loaded in the SimulationManager. The setups can be reloaded, changed, initialized and executed.
 Setups can be explored in three tabs: Server, Tasks (threads and processes) and Gui-Plugins.
 They control the executables, scripts, references, parameters, plugins to load or unload and properties for every Setup.
 Values are always checked to be legal and meanuingful. Whenever a value is changed the frame will ask to save the changed setup before switching
 the setup or terminating the program.
 Explorer views show the file system to search and include script and executable paths from the respective include directories of the
 SimulationManager.

 SetupFrame works with definitions for Tasks (TaskDefinition), SimulationSetups (SimulationSetup) and Plugins (PluginDefinition).
 The currently selected SimulationSetup is contained in mCurrentSetup.\n

 As usual much of the functionality works using the signals and slots defined in the UI. So they update automatically whenever relevant changes
 have been made.
 Functions that edit whole parts of the gui and call one update afterwards, or those that might cause circular calls, use a simple
 guard mReactToChangeSetup that causes most of the other set-value functions to return immediately.
 When using the guard, make sure to not to rely on it being set when calling a function that sets/resets it itself.

 \note Before editing or debugging anything, make sure to inspect the connections using the QtDesigner.

 \warning
 Calling any slots from the outside has to be done with great care. Not all of the functions are safe to be used as standalone and in worst case
 might cause the frame to reach an illegal state.

 Generally, all the update-slots are always safe to use as they only update the view from the current setup. Changing a setup from the SimulationManager
 wont affect the SetupFrame because it uses a local copy of the edited SimulationSetup. Reloading with reloadSetups() will safely update the
 whole view.
*/
class SetupFrame : public AttachableFrame
{
    Q_OBJECT
    GUI_FRAME

public:
    // public functions

    /*!
     \brief Constructor.
    */
    SetupFrame();
    /*!
     \brief Destructors.

     Calls cleanup().
    */
    virtual ~SetupFrame();
    /*!
     \brief Initialization function called by the plugin manager.
    */
    virtual void init(const QStringList& parameters);

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
     \brief Checks if the current setup is saved and opens a save dialog if not.

     \return true if the setup is saved, was saved successfully or user denied saving without aborting
    */
    bool checkSaved();

    /*!
     \brief Loads the initially selected setup. Initializes non-changing ui element contents.
    */
    void loadDefaultSetup();

    /*!
     \brief Select a plugin of the current setup. Updates its view.

     Chosen plugin information is showed in the plugin page.

     \param plugin plugin to show
    */
    void choosePlugin(const PluginDefinition& plugin);
    /*!
     \brief Deselects currently chosen plugin. Resets its view.
    */
    void unchoosePlugin();

    /*!
     \brief Select a task of the current setup. Updates its view.

     Chosen task information is showed in the tasks page.

     \param task task to show
    */
    void chooseTask(const TaskDefinition& task);
    /*!
     \brief Deselects currently chosen task. Resets its view.
    */
    void unchooseTask();
    /*!
     \brief Changes a contained tasks name to a unique name by adding increasing numbers.

     \param task task to change name in
    */
    void setUniqueTaskName(TaskDefinition& task);

    /*!
     \brief Returns currently selected plugin of the current setup, or 0 if none exist.
    */
    PluginDefinition* getCurrentPlugin() const;
    /*!
     \brief Returns currently selected task of the current setup, or 0 if none exist.
    */
    TaskDefinition* getCurrentTask() const;

    /*!
     \brief Notifies that the setup is or is not up to date anymore.

     Updates the view of the setup name.

     \param state true if it has changed, false if it is up to date again
    */
    void updateSetupChanged(bool state = true);

    /*!
     \brief Displays a custom ContextMenu for one of the filesystem TreeViews.

     Used for the server pages explorer TreeView and the task pages eplorer TreeView.

     \param pos local position to pop up
     \param view view that requestet the menu
     \param modelIndex reference to store clicked modelindex to
    */
    void showExplorerTreeViewContextMenu(QPoint pos, QTreeView* view, QModelIndex& modelIndex);
    /*!
     \brief Clears all action receivers for context menu actions.
    */
    void clearActionReceivers();
    /*!
     \brief Expands a file system view and scrolls to an include path or file defined by its index.

     Called by context menu NumberedActionReceiver.
     Used for servers and tasks as well as firsts and scripts.

     \param number index of file to scroll to
     \param executable if true, an executable path will be searched, if false, a script path
    */
    void scrollToRegisteredExplorerFile(int number, bool first) const;
    /*!
     \brief Insert a context-menu-selected file from one of the file explorers to a LineEdit.

     Used by both views context menus.

     \param view view to read from
     \param modelIndex model index to read from
     \param target line edit to write to
     \param executable true to search for executable, false for scripts
     \param absolute if true, path is added absolute, if false, it is added relative if possible
    */
    void setLineEditTextFromExplorer(const QTreeView* view, const QModelIndex& modelIndex, QLineEdit* target, bool executable, bool absolute = false);

    /*!
     \brief Load a file path of an first or script for any task using a file dialog.

     \param type 0: first, 1: main script, 2: secondary script
    */
    void loadTaskEntry(int type, QLineEdit* target);

    /*!
     \brief Display a context menu to quick-load Task-Definitions at the given position.

     \param pos local popup position
    */
    void showLoadTaskContextMenu(QPoint pos);

public: signals:
    //public signals

    /*!
     \brief Emitted when the SetupFrame started a simulation.

     You might want to receive the signal from the SimulationManager instead, to receive signals for all started Simulations.

     \param name name of the setup
     \param path filepath of the setup
    */
    void simulationStarted(QString name, QString path);
    /*!
     \brief Emitted when the SetupFrame stopped a simulation.

     You might want to receive the signal from the SimulationManager instead, to receive signals for all stopped Simulations.
    */
    void simulationStopped();

public slots:
    //public slots

    /*!
     \brief Calls checkSaved() to ask the user if the current setup should be saved.
    */
    void askSaveChanges();
    /*!
     \brief Updates file path of the current setup.

     Called whenever a setup was saved.

     \param name name of the setup
     \param path new filepath
    */
    void updateSetupSaved(QString name, QString path);

    /*!
     \brief Start the SetupFrameUtil::SetupDialog to add a new Setup.

     Also, asks if the file should be added to the include paths.
     Can specify name, caption and target file path.
    */
    void addSetup();
    /*!
     \brief Insert a setup into the setup list by copying or appending it.

     \param setup to copy from
     \param copy if true, a deep copy is done, otherwise the shared pointer is saved
     \return index where it was added or updated in the setup list
    */
    int insertSetup(boost::shared_ptr<SimulationSetup> setup, bool copy = true);
    /*!
     \brief Removes the currently selected Setup from the managed list and the simulationManager.

     Also, asks if the file should be deleted from the file system.
     Otherwise it will reappear in the list after the next update from the SimulationManager.
    */
    void removeSetup();
    /*!
     \brief Starts the SetupFrameUtil::SetupDialog to rename the currently selected Setup or change its target file.

     Can change name, caption and target file path.
    */
    void renameSetup();
    /*!
     \brief Changes the currently selected Setup to another one from the setup combo box.

     \param index index of new setup
    */
    void changeSetup(int index);
    /*!
     \brief Tells SimulationManager to initialize the currently selected Setup.
    */
    void initSetup();
    /*!
     \brief Tells SimulationManager to initialize and execute the currently selected Setup.
    */
    void startSetup();
    /*!
     \brief Tells SimulationManager to stop the currently initialized Setup.
    */
    void stopSetup();
    /*!
     \brief Reloads the list of available SimulationSetups from the SimulationManager.

     SimulationManager will reload the complete list of manged setups.
     Setups with files in the include paths will be removed from the managed setup list in the SetupFrame and reloaded.
    */
    void reloadSetups();
    /*!
     \brief Load a SimulationSetup with a FileDialog.
    */
    void loadSetup();
    /*!
     \brief Load a SimulationSetup from a file.

     \param path absolute filepath to load from
    */
    void loadSetup(QString path);
    /*!
     \brief Save currently selected SimulationSetup to its target file.
    */
    void saveSetup();
    /*!
     \brief Updates the widgets on all pages from the current Setup.
    */
    void updateSetupDisplay();
    /*!
     \brief Updates the widgets on the plugin page from the currently selected Plugin.
    */
    void updatePluginDisplay();
    /*!
     \brief Updates the plugin list widget on the plugin page from the PluginFactory instantiator list.
    */
    void updatePluginListDisplay();
    /*!
     \brief Updates the list of plugins to add on the plugin page from the currently selected Setups plugin list.
    */
    void updateLoadPluginsDisplay();
    /*!
     \brief Updates the list of plugins to remove on the plugin page from the currently selected Setups plugin list.
    */
    void updateUnloadPluginsDisplay();
    /*!
     \brief Updates the widgets on the tasks page from the currently selected task.
    */
    void updateTaskDisplay();
    /*!
     \brief Updates the list of threads on the threads page from the currently selected Setups thread list.
    */
    void updateTaskListDisplay();
    /*!
     \brief Updates the list of all available Setups from the SimulationManagers Setup list.
    */
    void updateSetupListDisplay();
    /*!
     \brief Updates the combo box items representing available Setups from the SimulationManagers Setup list.
    */
    void updateSetupComboBoxItems();
    /*!
     \brief Updates the icons of the Setup list items depending on if they are initialized or running (or not).
    */
    void updateSetupListIcons();
    /*!
     \brief Updates the setup list item at given index.
    */
    void updateSetupListDisplaySingle(int index);

    /*!
     \brief Chose next page.
    */
    void increaseStackedWidgetPage();
    /*!
     \brief Chose previous page.
    */
    void decreaseStackedWidgetPage();

    /*!
     \brief Add a new default plugin to the load-list of the current setup.
    */
    void addLoadPlugin();
    /*!
     \brief Add a new plugin as chosen in the plugin list and add it to the load-list of the current setup.
    */
    void addLoadPluginFromPluginList();
    /*!
     \brief Overwrite name, class, caption and type information of the currently selected plugin from the plugin chosen in the plugin list.
    */
    void overwriteLoadPluginFromPluginList();
    /*!
     \brief Remove the currently selected plugin in the load-list.
    */
    void removeLoadPlugin();
    /*!
     \brief Add a new default plugin definition to the unload-list of the current setup.
    */
    void addUnloadPlugin();
    /*!
     \brief Remove the currently selected plugin in the unload-list.
    */
    void removeUnloadPlugin();
    /*!
     \brief Change the type of the currently selected plugin.

     Note, that a plugin always has to have a unique name and a fixed type, so there is only one valid type for the wished plugin class name.
     The selection is meant to give a confirmation of the plugin type for load-plugin definitions and allows specifying a vague definition
     ('All plugins with type Frame') in the unload-plugin list.

     There will be a warning when a plugins class name and type dont match.
     Using the plugin list to choose a new plugin to add always chooses the correct type.

     \param index index of new plugin type
    */
    void changePluginType(int index);
    /*!
     \brief Changes the currently selected plugins caption.

     \param value new caption
    */
    void editPluginCaption(QString value);
    /*!
     \brief Changes the currently selected plugins caption using the text of the UIs plugin caption QLineEdit.
    */
    void editPluginCaption();
    /*!
     \brief Changes the currently selected plugins name.

     \param value new name
    */
    void editPluginName(QString value);
    /*!
     \brief Changes the currently selected plugins name using the text of the UIs plugin name QLineEdit.
    */
    void editPluginName();
    /*!
     \brief Changes the currently selected plugins parameters by converting the text of the UIs plugin parameters QPlainTextEdit to a list of strings.

     The parameters are used to initialize the plugin.
     See the plugins documentation for available parameters.
    */
    void editPluginParameters();
    /*!
     \brief Changes the currently selected plugins attachment points by converting the text of the QPlainTextEdit to a list of strings.

     Only available for attachable frame plugins.
     When loading the setup, the plugin is attached to the first available and free attachment point in the list.
    */
    void editPluginAttachments();
    /*!
     \brief Changes the currently selected plugins PluginDefinition::EReloadDefinition to index.
    */
    void changePluginReload(int index);
    /*!
     \brief Changes the current setups SimulationSetup::mRemoveAll specification.

     \param state new state
    */
    void changeRemoveAll(bool state);
    /*!
     \brief Changes the current setups SimulationSetup::mReloadAll specification.

     \param state new state
    */
    void changeReloadAll(bool state);
    /*!
     \brief Chooses a plugin of the load-list and displays its properties in the view.

     \param index index of the plugin
    */
    void chooseLoadPlugin(int index);
    /*!
     \brief Chooses a plugin of the unload-list and displays its properties in the view.

     \param index index of the plugin
    */
    void chooseUnloadPlugin(int index);
    /*!
     \brief Chooses a plugin of the load-list and displays its properties in the view.

     \param index index of the plugin
    */
    void chooseLoadPlugin(QModelIndex index);
    /*!
     \brief Chooses a plugin of the unload-list and displays its properties in the view.

     \param index index of the plugin
    */
    void chooseUnloadPlugin(QModelIndex index);
    /*!
     \brief Opens a context menu for the plugin list at pos.

     Allows adding new plugins or overwriting the current plugin with the data of the selected plugin from the plugin list.

     \param pos position to popup
    */
    void showPluginListContextMenu(QPoint pos);

    /*!
     \brief Add a new task to the current setup.
    */
    void addTask();
    /*!
     \brief Remove currently selected task.
    */
    void removeTask();
    /*!
     \brief Choose the current setups task from index.

     \param index index of the view
    */
    void chooseTask(QModelIndex index);
    /*!
     \brief Choose the current setups task from index.

     \param index index in the setups task list
    */
    void chooseTask(int index);
    /*!
     \brief Change the type of the currently selected task and set its values to the target types default values.

     \param index index of new type
    */
    void changeTaskType(int index);
    /*!
     \brief Change the QThread::Priority of the currently selected task.

     \param index target priority
    */
    void changeTaskPriority(int index);
    /*!
     \brief Change the name of the currently selected task.

     \note the Gui names the 'name' property 'caption' to point out that this value does not have to be unique like a SimulationSetups or plugins name.

     \param value new caption
    */
    void editTaskCaption(QString value);
    /*!
     \brief Change the name of the currently selected task from the UIs task caption QLineEdit.

     \note the Gui names the 'name' property 'caption' to point out that this value does not have to be unique like a SimulationSetups or plugins name.
    */
    void editTaskCaption();
    /*!
     \brief Change the parameters of the currently selected task by converting the UIs task parameters QPlainTextEdit to a list of parameter strings.
    */
    void editTaskParameters();

    /*!
     \brief Load an first for the currently selected task using a file dialog.
    */
    void loadTaskFirst();
    /*!
     \brief Load a main initialization script for the currently selected task using a file dialog.
    */
    void loadTaskSecond();
    /*!
     \brief Load a spark initialization script for the currently selected task using a file dialog.
    */
    void loadTaskThird();

    /*!
     \brief Store a text for the currently selected tasks first QLineEdit and change its color to depending on if the file could be found.

     If the file is in the include directories or given absolutely and it exists, the text color of the edit will be black, otherwise it is
     set to red.

     taskFirstCheckText() stores the text to the current setup

     \param text text to store
    */
    void taskFirstCheckText(QString text);
    /*!
     \brief Apply the changed tasks first text.

     The value is defined with taskFirstCheckText()
     Logs a warning if the first could not be found.
    */
    void taskFirstChangeText();
    /*!
     \brief Store a text for the currently selected tasks main script QLineEdit and change its color to depending on if the file could be found.

     If the file is in the include directories or given absolutely and it exists, the text color of the edit will be black, otherwise it is
     set to red.

     taskSecondCheckText() stores the text to the current setup

     \param text text to store
    */
    void taskSecondCheckText(QString text);
    /*!
     \brief Apply the changed tasks main script text.

     The value is defined with taskSecondCheckText()
     Logs a warning if the file could not be found.
    */
    void taskSecondChangeText();
    /*!
     \brief Store a text for the currently selected tasks secondary script QLineEdit and change its color to depending on if the file could be found.

     If the file is in the include directories or given absolutely and it exists, the text color of the edit will be black, otherwise it is
     set to red.

     taskThirdCheckText() stores the text to the current setup

     \param text text to store
    */
    void taskThirdCheckText(QString text);
    /*!
     \brief Apply the changed tasks secondary script text.

     The value is defined with taskThirdCheckText()
     Logs a warning if the file could not be found.
    */
    void taskThirdChangeText();

    /*!
     \brief Add a new default script to the currently selected task.
    */
    void addTaskListEntry();
    /*!
     \brief Remove the selected script of the currently selected task.
    */
    void removeTaskListEntry();
    /*!
     \brief Move the selected script of the currently selected task one position up in its list.
    */
    void moveTaskListEntryUp();
    /*!
     \brief Move the selected script of the currently selected task one position down in its list.
    */
    void moveTaskListEntryDown();
    /*!
     \brief Select the script with index row of the current task.

     \param row task to choose
    */
    void chooseTaskListEntry(int row);
    /*!
     \brief Store a text for a tasks additional script by its respective UI QListWidgetItem.

     \param item item to read text from, specifies target script by the items index
    */
    void editTaskListEntry(QListWidgetItem* item);

    /*!
     \brief Store a text for the currently selected tasks additional script and change its color to depending on if the file could be found.

     If the file is in the include directories or given absolutely and it exists, the text color of the edit will be black, otherwise it is
     set to red.

     \param text text to store
    */
    void editCurrentTaskListEntry(QString text);
    /*!
     \brief Checks if the text of the current tasks QListWidgetItem is a script file and contained in the include paths and colors the text respectively.

     If it is a valid script file and included in one of the include paths, the text is set to black, otherwise it will be red.
    */
    void checkCurrentTaskListEntry();
    /*!
     \brief Checks if the text of the given tasks QListWidgetItem is a script file and contained in the include paths and colors the text respectively.

     If it is a valid script file and included in one of the include paths, the text is set to black, otherwise it will be red.

     \param item item to check
    */
    void checkTaskListEntry(QListWidgetItem* item);

    /*!
     \brief Opens a context menu for the tasks file explorer QTreeView.

     \param pos position to popup
    */
    void showTasksExplorerTreeViewContextMenu(QPoint pos);
    /*!
     \brief Calls scrollToRegisteredExplorerFile() with the number of the calling NumberedActionReceiver and script.

     Used by context menus and called by mScriptPathActionReceivers.

     \param number include path clicked
    */
    void onTasksScriptPathClick(int number);
    /*!
     \brief Calls scrollToRegisteredExplorerFile() with the number of the calling NumberedActionReceiver and executable flag.

     Used by context menus and called by mExecutablePathActionReceivers.

     \param number include path clicked
    */
    void onTasksExecutablePathClick(int number);
    /*!
     \brief Sets the text of the tasks first line edit from the currently selected path in the tasks file system TreeView.

     \param absolute if true, the path is added absolutely, if false it is added relative to an include path if possible
    */
    void setTaskFirstTextFromExplorer(bool absolute = false);
    /*!
     \brief Sets the text of the tasks main script line edit from the currently selected path in the tasks file system TreeView.

     \param absolute if true, the path is added absolutely, if false it is added relative to an include path if possible
    */
    void setTaskSecondTextFromExplorer(bool absolute = false);
    /*!
     \brief Sets the text of the tasks secondary script line edit from the currently selected path in the tasks file system TreeView.

     \param absolute if true, the path is added absolutely, if false it is added relative to an include path if possible
    */
    void setTaskThirdTextFromExplorer(bool absolute = false);

    /*!
     \brief Makes the SetupFrame ready to be deleted.

     Asks to save unsaved changes and saves Settings.
    */
    void cleanup();

    /*!
     \brief Shows a dropdown menu with entries to initialize task data fields with.

     Used to quickly choose a plugin definition for a PluginThread task definition.

     \param index index of the task
    */
    void onLoadTaskContextMenuClick(int index);

private:
    // private members

    Ui::SetupFrame ui; /*!< Gui-layout object. */
    boost::shared_ptr<Settings> mSettings; /*!< Settings object. */
    SimulationManager* mSimulationManager; /*!< Pointer to SimulationManager */
    std::vector<boost::shared_ptr<SimulationSetup>> mSetupList; /*!< List of setups displayed in the setup combo box with their filename and the displayed name as well as flag if they are in the include paths.*/

    QString mLastDirectory; /*!< Last Directory used to load Setups. */

    bool mSaving;                   /*!< True if currently trying to save. */
    bool mSetupChanged;             /*!< True if the setup has changed since it was saved the last time. */
    bool mReactToChangeSetup;       /*!< Guard to prevent multiply changing gui elements whenever editing a ui object. */
    bool mReactToEditListEntry;     /*!< Guard pro prevent changing if a script edit. */
    bool mReactToEditTaskListEntry; /*!< Guard to precent chaning of a task script edit. */

    QString mDefaultSetup;              /*!< Name of the default setup to load. */
    QString mLastSetup;                 /*!< Name of the last setup loaded. */
    QString mLastSetupPath;             /*!< Name of the last setup loaded. */

    QString mLastServerFirst;           /*!< Path of the last server first chosen in a file dialog. */
    QString mLastSparkProcessFirst;     /*!< Path of the last spark process first chosen in a file dialog. */
    QString mLastProcessFirst;          /*!< Path of the last process first chosen in a file dialog. */
    QString mLastServerSecond;          /*!< Path of the last server main script chosen in a file dialog. */
    QString mLastSparkProcessListEntry; /*!< Path of the last spark process main script chosen in a file dialog. */
    QString mLastServerThird;           /*!< Path of the last server secondary script chosen in a file dialog. */
    QString mLastSparkProcessThird;     /*!< Path of the last spark process secondary script chosen in a file dialog. */

    boost::shared_ptr<SimulationSetup> mCurrentSetup; /*!< Current simulation setup. Copy of one of the SimulationMangers Setups or loaded from a file.  */

    int mInitializedSetupIndex; /*!< Index of the simulations currently initialized setup in the setup list. */

    int mChosenLoadPlugin;          /*!< Index of the chosen load-plugin.  (Default: -1)*/
    int mChosenUnloadPlugin;        /*!< Index of the chosen unload-plugin.  (Default: -1)*/
    int mChosenTask;                /*!< Index of the chosen task.  (Default: -1)*/
    int mLastSetupIndex;            /*!< Index of the last chosen setup.  (Default: -1)*/
    int mChosenListEntry;              /*!< Index of the chosen server script.  (Default: -1)*/
    bool mChosenListEntryIllegal;      /*!< True if the chosen server script could be found by the SimulationManager. */
    int mChosenTaskListEntry;          /*!< Index of the chosen task script.  (Default: -1)*/
    bool mChosenTaskListEntryIllegal;  /*!< True if the chosen task script could be found by the SimulationManager. */

    QFileSystemModel* mFileSystemModel;  /*!< FileSystemModel to be displayed on the server and tasks pages. */
    std::vector<NumberedActionReceiver*> mScriptPathActionReceivers;     /*!< Action receivers fro clicks in the context menus. */
    std::vector<NumberedActionReceiver*> mExecutablePathActionReceivers; /*!< Action receivers fro clicks in the context menus. */
    QModelIndex mExplorerItemIndex;      /*!< Current model index selected in the servers explorer TreeView. */
    QString mExplorerItemPath;           /*!< Current path selected in the servers explorer TreeView. */
    QModelIndex mTasksExplorerItemIndex; /*!< Current model index selected in the tasks explorer TreeView. */
    QString mTasksExplorerItemPath;      /*!< Current path selected in the tasks explorer TreeView. */

    QModelIndex mPluginListItemIndex; /*!< Current model index selected in the plugin pages plugin list. */

    std::vector<NumberedActionReceiver*> mLoadTaskActionReceivers;

    bool mClosed; /*!< True if cleanup has been called. */

    //--------------
    // Settings

    bool mPreFetchExplorerData; /*!< If true, the include directories will be expanded in the view at time of startup. Takes one or two seconds, but will have quicker reactions afterwards. */
};

#endif // SETUPFRAME_H
