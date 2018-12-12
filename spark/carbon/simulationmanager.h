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

#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H
/**
 * \file simulationmanager.h
 * \brief Contains SimulationManager class.
 */

#include "simulationsetup.h"
#include "taskdefinition.h"

#include <QObject>
#include <vector>
#include <map>
#include <QFile>
#include <QStringList>
#include <QMutex>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class ServerThread;
class SimulationTask;
class Simulation;
class Carbon;
class Settings;
class TaskDefinition;
class NumberedActionReceiver;
class NamedActionReceiver;

class QDomDocument;
class QDomElement;

namespace GuiSettings
{
    class SimulationSettings;
}

/*!
 \brief Manages Simulation Setups. Loads and saves Simulation States.

 <b>General:</b>\n
 The SimulationManager controls initialization and interaction with a Simulation, loads and saves SimulationSetups and stores a list of preset
 SimulationSetups which are loaded from include paths and can be requested and used by other files.
 Since these preset SimulationSetups include the first setup that is loaded into the MainWindow, it is the essential tool to initialize the final
 setup and layout of the Gui and its plugins.\n

 <b>Preset SimulationSetups:</b>\n
 The SimulationManager loads all availabe SimulationSetups from Spark Simulation Setup xml-files (*.3s, *.xml) in the specified include directories.
 The default include directory is "/gui/setups/" but can be changed and extended using the SimulationManagers SettingsWidget in the SettingsDialog.
 The Manager can store additional include directories or include files or load single files or directories at runtime (e.g. using the SparkFrame).\n

 \warning Include paths are used by both the SimulationManager itself and the SimulationTasks. When accessing the list directly (not via a
 SimulationManager function) you need to lock and unlock the access to the list via lockIncludePaths() and unlockIncludePaths().\n

 Preset SimulationSetups can be created and modified by plugins like the SetupFrame. They define the initial state of a Simulation.
 Since they are managed in a list of the SimulationManager, preset setups are required to have a unique name.
 When creating a new SimulationSetup or chaning an existing one, the SimulationManager checks if the name is not already used by another setup
 in one of the include directories. It is recommended to use sufficiently long and precise names so there wont be any name clashes.
 The name of other setups (initialized from file directly) can be chosen freely (the name does not have to be changed when creating multiple
 save files originating from the same preset SimulationSetup).\n

 <b>Initializing a SimulationSetup:</b>\n
 The SimulationManager can initialize a Simulation using one of the available preset SimulationSetups from its list (using the "file" main menu), or
 by loading a SimulationSetup from a file.
 The SimulationManager will initialize the specified simulation as a Simulation object.
 It contains a list of Gui-Plugins to load and a list of SimulationTasks to initialize.
 Afterwards, the Simulation can be executed.
 At one time there can only one SimulationSetup be initialized and/or running. \n

 <b>SimulationSetups:</b>\n
 The setup defines the set of used Gui-Plugins (AttachableFrame, SignalPlugin, Plugin) and tasks (SimulationThread & SimulationProcess) to run.
 When choosing one SimulationSetup from the list of available setups or from a file, it is initialized (its plugins are loaded and tasks are created).
 When running the current Simulation, its tasks are executed. While the Simulation is running, the tasks can be paused, stopped or restarted
 individually. Starting any task while the Simulation is paused will restart it.
 Changing the Setup stops the Simulation, closing all open tasks an reinitializing the plugins using the PluginManager, as specified
 by the SimulationSetup.\n

 The specification of a SimulationSetup can be done using the Guis SparkFrame plugin, or by creating a new ".3s" file. The example file "DefaultSetupGrid.3s"
 demonstrates the syntax. A Setup always consists of:\n
 - a unique non-empty name\n
 - a non-empty caption\n
 - a list of plugins to add\n
 - a list of plugins to remove\n
 - a list of tasks to create\n

 By default, all active plugins are reloaded when loading a new SimulationSetup. To prevent this, specify the attribute 'reloadall = "false"' in the
 "addplugins" node (for all plugins) or specify the attribute 'reload = "false"' in the plugin node (for a single plugin).
 By default, none of the active plugins are deleted when loading a new SimulationSetup.
 To delete plugins, specify the attribute 'removeall = "true"' in the "removeplugins" node (for all plugins) or add vague plugin definitions to the
 "removeplugins" node. An empty "frame" node for example, will delete all AttachableFrame plugins. A "frame" node with a 'name = "Console"' attribute
 will delete all Frames of the Console class. A "frame" node with a 'type = "MONITOR"' attribute will delete all Monitor plugins.
 ReferenceList and Attachmentpoints of the PluginDefinition are ignored here.

 /sa SimulationSetup, Simulation
*/
class SimulationManager : public QObject
{
    Q_OBJECT

    friend SimulationTask;

public:
    // public types

    /*!
     \brief List of SimulationSetup pointers.
    */
    typedef std::vector<boost::shared_ptr<SimulationSetup> > TSetupList;

public:
    // public functions

    /*!
     \brief Basic constructor.

     Calls initialization of the SimulationSetup list by loading from include files.

     \param parent parent controller
    */
    SimulationManager(Carbon* parent);
    /*!
     \brief Basic destructor.
    */
    virtual ~SimulationManager();
    /*!
     \brief Creates and registers a SettingsWidget to the SettingsDialog.

     Called by Carbon.
    */
    void initializeSettingWidget();

    /*!
     \brief Called by simulation manager to initialize the manager.
    */
    void init();

    /*!
     \brief Returns the list of all loaded setups. The list is updated if anything has changed in the include directories.
    */
    const TSetupList& getSetupList() const;

    /*!
     \brief Returns the index of a SimulationSetup by name.

     The index may change when setups are reloaded.
     when accessing the setups regularly, make sure to receive the
     setupListChanged(), and aboutToReloadSetupList() signals and update
     the view.

     \param name name to search for
     \return the index of the found setup, or -1 if nothing was found
    */
    int getSetupIndex(const QString& name) const;
    /*!
     \brief Returns the index of a SimulationSetup by its file path.

     The index may change when setups are reloaded.
     when accessing the setups regularly, make sure to receive the
     setupListChanged(), and aboutToReloadSetupList() signals and update
     the view.

     \param path path to search for
     \return the index of the found setup, or -1 if nothing was found
    */
    int getSetupIndexByFilePath(const QString& path) const;
    /*!
     \brief Returns the index of the currently initialized simulation setup in the list of default simulation setups.

     If the currently initialized setup was not loaded from the include directories, returns -1.
    */
    int getInitializedSetupIndex() const;
    /*!
     \brief Returns the simulation setup of the current simulation if it is initialized, or 0 otherwise.
    */
    const SimulationSetup* getInitializedSetup() const;
    /*!
     \brief Returns the name of the Setup that has been loaded manually last (if no setup is initialized it is the last sessions last setup name).
    */
    const QString& getLastLoadedSetup() const;
    /*!
     \brief Returns the file path of the Setup that has been loaded manually last (if no setup is initialized it is the last sessions last setup path).
    */
    const QString& getLastLoadedSetupPath() const;
    /*!
     \brief Returns the name of the Setup that has been initialized last (if no setup is initialized it is the last sessions last setup name).
    */
    const QString& getLastInitializedSetup() const;
    /*!
     \brief Returns the file path of the Setup that has been initialized last (if no setup is initialized it is the last sessions last setup path).
    */
    const QString& getLastInitializedSetupPath() const;

    /*!
     \brief Returns the default path to search plugin libraries in.
    */
    const QString& getDefaultPluginPath() const;
    /*!
     \brief Returns the name of the default setup to load.
    */
    const QString& getDefaultSetup() const;
    /*!
     \brief Returns the default setup include path.
    */
    const QString& getDefaultSetupPath() const;
    /*!
     \brief Returns the default setup save state path.
    */
    const QString& getDefaultSavePath() const;
    /*!
     \brief Returns the default extension text used to load SimulationSetups.
    */
    const QString& getDefaultSetupExtensionText() const;
    /*!
     \brief Returns the default extension suffix used to save SimulationSetups.
    */
    const QString& getDefaultSetupExtensionSuffix() const;
    /*!
     \brief Returns the default extensions used to load executable files.
    */
    const QStringList& getExecutableExtensions() const;
    /*!
     \brief Returns the default extension text used to load executable files.
    */
    const QString& getExecutableExtensionText() const;
    /*!
     \brief Returns the default extension suffix used to save SimulationSetups.
    */
    const QString& getExecutableSuffix() const;
    /*!
     \brief Returns the default extensions used to load script files.
    */
    const QStringList& getScriptExtensions() const;
    /*!
     \brief Returns the default extension text used to load script files.
    */
    const QString& getScriptExtensionText() const;
    /*!
     \brief Returns the default extension suffix used for script files.
    */
    const QString& getScriptSuffix() const;
    /*!
     \brief Returns the include path list for script files.
    */
    const QStringList& getScriptIncludeDirectories() const;
    /*!
     \brief Returns the include path list for executable files.
    */
    const QStringList& getExecutableIncludeDirectories() const;
    /*!
     \brief Returns the include path list for simulation setup files.
    */
    const QStringList& getSetupIncludeDirectories() const;
    /*!
     \brief Returns the include file list for single script files.
    */
    const QStringList& getScriptIncludeFiles() const;
    /*!
     \brief Returns the include file list for single executable files.
    */
    const QStringList& getExecutableIncludeFiles() const;
    /*!
     \brief Returns the include file list for single simulation setup files.
    */
    const QStringList& getSetupIncludeFiles() const;
    /*!
     \brief Returns true if a SimulationSetup with the given name is in the list.

     \param name name to search for
    */
    bool containsSetup(const QString& name) const;
    /*!
     \brief Returns the default extensions for script files and executable files.
    */
    QStringList getFileExtensions() const;

    /*!
     \brief Returns a pointer to the permanent spark server thread. Is a null pointer if no server has been started yet.
    */
    boost::shared_ptr<ServerThread> getSparkServerThread() const;
    /*!
     \brief Takes a pointer to a Spark server thread and returns a shared pointer for it. This thread is the only one who will create and manage a SimSpark object.

     Returns null pointer when there is already a spark server thread initialized.
    */
    boost::shared_ptr<ServerThread> initSparkServerThread(ServerThread* server);

    /*!
     \brief Sets the default setup (by name).

     This setup is specified by the main window. It is loaded at the end of the Carbon initialization.
     The setup has to be located in one of the include directories. If the location is not the default setup path or a default include path, add the location in
     the main window constructor.
     \param value name of the setup
    */
    void setDefaultSetup(const QString& value);
    /*!
     \brief Changes the default path to search plugin libraries in.

     This path is added to the include paths and used as default path when searching for
     Plugin libraries.

     \param value absolute path
    */
    void setDefaultPluginPath(const QString& value);
    /*!
     \brief Changes the path for preset SimulationSetups.

     This path is added to the include paths and used as default path when searching for
     SimulationSetups.

     \param value absolute path
    */
    void setDefaultSetupPath(const QString& value);
    /*!
     \brief Changes the path for SimulationSetup save states.

     \param value absolute path
    */
    void setDefaultSavePath(const QString& value);
    /*!
     \brief Change the setup extension text used to load SimulationSetups.

     \param value string value
    */
    void setDefaultSetupExtensionText(const QString& value);
    /*!
     \brief Change the setup extension suffix for new SimulationSetups.

     \param value string value
    */
    void setDefaultSetupExtensionSuffix(const QString& value);

    /*!
     \brief Adds a new include directory or file.

     \param path absolute or relative path to a directory or file
    */
    void addSetupIncludePath(const QString& path);
    /*!
     \brief Removes a contained include directory or file.

     \param path absolute or relative path to a directory or file
    */
    void removeSetupIncludePath(const QString& path);

    /*!
     \brief Returns true if the specified path is in the includeDirectory list.

     \param path absolute or relative path to a directory
    */
    bool containsSetupIncludeDirectory(const QString& path) const;
    /*!
     \brief Returns true if the specified path is in the includeFile list.

     \param path absolute or relative path to a file
    */
    bool containsSetupIncludeFile(const QString& path) const;
    /*!
     \brief Returns true if the specified path is in the includeFile or includeDirectory list.

     \param path absolute or relative path to a directory or file
    */
    bool containsSetupIncludePath(const QString& path) const;

    /*!
     \brief Opens a dialog to load a new/reload a Simulation Setup from file.

     If it is already in the contained list, it is reloaded, otherwise it is added to this sessions SimulationSetup list.
     Setups with conflicting names cant be loaded.

     The savestate specifies the type of the SimulationSetup. Preset setups do not contain data to load and are located in one of the include directories.
     Saved SimulationSetups contain plugin data and are located in any save directory. Overwriting a save state is possible anytime and there may be many
     saves of the same basic preset SimulationSetup with the same setup name. The names of included preset simulation setups has to be unique.

     If the loadData flag is true, a save .ini file for plugin data will be searched in the same directory and with the same file name than the simulation
     setup. It will be loaded and the plugins loadData function will be called.
     \param loadFromSavePath if true, the last save path will be used as default path to open a dialog in, otherwise the default setup path
     \param loadData if true, tries to load plugin data from .ini file
     \return true if a setup was loaded or updated
    */
    bool loadSimulationSetupByDialog(bool loadFromSavePath = false, bool loadData = false);
    /*!
     \brief Opens a dialog to save or overwrite the currently initialized Simulation Setup as new preset setup (not save state) to a file.

     If it is already in the contained list, it is updated. If it is in one of the include directories, it is added to this sessions SimulationSetup list.
     Setups with conflicting names cant be saved in the include directories.

     The savestate specifies the type of the SimulationSetup. Preset setups do not contain data to load and are located in one of the include directories.
     Saved SimulationSetups contain plugin data and are located in any save directory. Overwriting a save state is possible anytime and there may be many
     saves of the same basic preset SimulationSetup with the same setup name. The names of included default simulation setups has to be unique.

     If the saveData flag is true, the saveData() function of all plugins will be called and their data will be saved to an .ini file in the same directory and with
     the same name than the simulation setup.
     \param overwrite if true, the setup will be overwritten without asking (if possible)
     \param saveData if true, plugin data will be saved too
     \return the saved setup if saving was successful, null pointer otherwise
    */
    boost::shared_ptr<SimulationSetup> saveSimulationSetupByDialog(bool overwrite = false, bool saveData = false);
    /*!
     \brief Removes a contained SimulationSetup and may delete its file completely.

     Cant remove the setup that is currently initialized.
     If askRemove is true and the specified setup belogns to an existing save file, a MessageBox asks if the SimulationSetups save file should be deleted completely.
     If remove is true and the user is not asked, the file is deleted. (Use this when you use a custom decision beforehand)

     \param file absolute path of the setup to remove
     \param askRemove show MessageBox to decide if the file should be removed
     \param remove remove the file when there is no MessageBox shown
     \return index of removed file or -1 in case of error
    */
    int removeSimulationSetup(const QString& file, bool askRemove = true, bool remove = false);
    /*!
     \brief Removes a contained SimulationSetup and may delete its file completely.

     Cant remove the setup that is currently initialized.
     If askRemove is true and the specified setup belogns to an existing save file, a MessageBox asks if the SimulationSetups save file should be deleted completely.
     If remove is true and the user is not asked, the file is deleted. (Use this when you use a custom decision beforehand)

     \param index index of the setup to remove
     \param askRemove show MessageBox to decide if the file should be removed
     \param remove remove the file when there is no MessageBox shown
     \return true in case of success
    */
    bool removeSimulationSetup(int index, bool askRemove = true, bool remove = false);

    /*!
     \brief Clears and reloads all simulation setups from the include paths.

     All files from specified includeFiles and includeDirectories are searched and
     loaded.
    */
    void loadSimulationSetups();
    /*!
     \brief Loads a single SimulationSetup from an (absolute) filepath and adds it to the current setup list.

     The file has to be a Spark Simulation Setup file. (*.xml, *.3s)
     If 'store' is true, the path (if existant) is added to the include files or
     include directories.

     \param path absolute path to the file
     \param store save the path to the include paths
     \return the loaded setup, or null pointer if loading failed
    */
    boost::shared_ptr<SimulationSetup> loadSimulationSetup(const QString& path, bool store = false);
    /*!
     \brief Saves a new or existing setup to its target file.

     \param setup setup pointer
     \param overwrite if true, the setup overwrites the target file without asking (if possible)
     \param rememberPath if true, the next time a save dialog is opened, it will be in the folder of this path
     \return pointer to the saved or updated setup, or null pointer if saving failed
    */
    boost::shared_ptr<SimulationSetup> saveSimulationSetup(boost::shared_ptr<SimulationSetup> setup, bool overwrite = false, bool rememberPath = true);
    /*!
     \brief Saves all existing Simulation Setups to their target file.

     If AskForAll is true, there will be one MessageBox asking if all setups should be saved.
     If AskForEach is true and AskForAll-MessageBox hasnt be answered with yes, there will be a MessageBox for every single unsaved setup.

     Setups are only saved if they are flagged as out of date (mSaved == false) and are flagged as toSave (mDontSave == false).

     \param askForAll show messagebox for all setups at once
     \param askForEach show messagebox for all setups one at a time
    */
    bool saveAllSimulationSetups(bool askForAll, bool askForEach);
    /*!
     \brief Updates a contained setup.

     \param index in the setup list
     \param setup setup reference
     \param askReloadCurrent if true, the user will be asked if the current Simulation should be reloaded in case it is updated
     \return true if setup was found
    */
    bool updateSimulationSetup(int index, const SimulationSetup& setup, bool askReloadCurrent = true);
    /*!
     \brief Updates a contained setup.

     \param path absolute file path of the setup
     \param setup setup reference
     \param askReloadCurrent if true, the user will be asked if the current Simulation should be reloaded in case it is updated
     \return true if setup was found
    */
    bool updateSimulationSetup(const QString& path, const SimulationSetup& setup, bool askReloadCurrent = true);

    /*!
     \brief Initializes a simulation by a SimulationSetup name from the list of SimulationSetups.

     The current simulation will be stopped and the new simulations plugins and tasks (threads and processes) will be initialized. Before closing the Simulation object will
     ask to save the data of the current simulation.

     After initialization isInitialized() returns true and the Simulation can be run with runSimulation().

     \param name name of the setup to initialize
     \param reinitialize if true, the simulation is stopped and initialized even if it is the current simulation
     \return true if initialization was successful
    */
    bool initializeSimulation(const QString& name, bool reinitialize = false);
    /*!
     \brief Initializes a simulation by a SimulationSetup file at the given path.

     The current simulation will be stopped and the new simulations plugins and tasks (threads and processes) will be initialized. Before closing the Simulation object will
     ask to save the data of the current simulation. The file will not be added to the current list of preset simulation setups.
     Used to load save states.

     After initialization isInitialized() returns true and the Simulation can be run with runSimulation().

     \param path file path of the setup file to initialize
     \param reinitialize if true, the simulation is stopped and initialized even if it is the current simulation
     \param loadData if true, tries to load plugin data from an ini file with the same name and in the same directory than the simulation setup
     \return true if initialization was successful
    */
    bool initializeSimulationFromFile(const QString& path, bool reinitialize = false, bool loadData = false);

    /*!
     \brief Returns a pointer to the current Simulation or 0.

     The Simulation object is created in the constructor of the SimulationManager. This does not mean a setup has been loaded and the Simulation is not neccessarily
     initialized yet. Use simulationInitialized() to check.
    */
    const boost::shared_ptr<Simulation> getSimulation() const;
    /*!
     \brief Returns a pointer to the current Simulation or 0.

     The Simulation object is created in the constructor of the SimulationManager. This does not mean a setup has been loaded and the Simulation is not neccessarily
     initialized yet. Use simulationInitialized() to check.
    */
    boost::shared_ptr<Simulation> getSimulation();

    /*!
     \brief Returns true if a Simulation has been initialized.
    */
    bool simulationInitialized() const;
    /*!
     \brief Returns true if a Simulation has been initialized and is running.
    */
    bool simulationRunning() const;
    /*!
     \brief Returns true if a Simulation has been initialized, was running and is currently paused.
    */
    bool simulationPaused() const;
    /*!
     \brief Execute the currently loaded Simulation and optionally all of its tasks.

     \param runTasks if true, all of the contained SimulationTasks are started too
    */
    bool runSimulation(bool runTasks = true);
    /*!
     \brief Stops the currently loaded Simulation and all its tasks.

     Stops and and removes all tasks of the Simulation, optionally removing the simspark server or not.

     \warning If stopserver is true, the main simulation ServerThread will be stopped. This must only be done when quitting the program.
     Otherwise the zeitgeist core will crash the next time runSimulation() is called.

     \param stopserver if true, main server will be stopped too
    */
    bool stopSimulation(bool stopserver = false);
    /*!
     \brief Pauses the currently loaded Simulation and all its tasks.
    */
    bool pauseSimulation();
    /*!
     \brief Restarts the currently loaded Simulation and all its tasks by stopping and re-running the Simulation.
    */
    bool restartSimulation();

    /*!
     \brief Returns a new TaskDefinition containing a task of a specific type with current default data for this task type.

     The default values can be specified using the Settings of the SimulationManager.
    */
    boost::shared_ptr<TaskDefinition> newTaskDefinition(TaskDefinition::ETaskType type) const;

    /*!
     \brief Returns true if the include paths for executables contain a file with the given name.

     \warning
     If the name is a complete path to a file, its name is extracted and used to search for. If you want to search for a sub-folder of an include
     directory, add this directory to the include paths.
    */
    bool hasExecutable(const QString& name) const;
    /*!
     \brief Returns the absolute path of a file with the given name (or path) if it is contained in any of the executable
     include directories, or empty string if not.

     \warning
     If the name is a complete path to a file, its name is extracted and used to search for. If you want to search for a sub-folder of an include
     directory, add this directory to the include paths.
    */
    QString findExecutable(const QString& name) const;
    /*!
     \brief Returns true if the include paths for executables contain a file with the given name.

     \warning
     If the name is a complete path to a file, its name is extracted and used to search for. If you want to search for a sub-folder of an include
     directory, add this directory to the include paths.
    */
    bool hasScript(const QString& name) const;
    /*!
     \brief Returns the absolute path of a file with the given name (or path) if it is contained in any of the executable
     include directories, or empty string if not.

     \warning
     If the name is a complete path to a file, its name is extracted and used to search for. If you want to search for a sub-folder of an include
     directory, add this directory to the include paths.
    */
    QString findScript(const QString& name) const;
    /*!
     \brief Returns the absolute path of a file with the given name (or path) if it is contained in any of the provided file lists.

     \warning
     If the name is a complete path to a file, its name is extracted and used to search for. If you want to search for a sub-folder of an include
     directory, add this directory to the include paths.

     \param name name to search for or path to a file to search for
     \param includeDirectories include directories to search in
     \param includeFiles single include files to compare with
     \param type type name to log
     \param suffix file suffixes to consider
    */
    QString findFile(const QString& name, const QStringList& includeDirectories, const QStringList& includeFiles, const std::string& type,
                     const QString& suffix) const;
    /*!
     \brief Searches all include directories for script files and stores them in the target list.

     \param target list to store found files in
     \param absolute if true, absolute file paths are stored, otherwise only file names
    */
    void findScripts(QStringList& target, bool absolute) const;
    /*!
     \brief Searches the given directories for script files and stores them in the target list.

     \param target list to store found files in
     \param directories directories to search in
     \param absolute if true, absolute file paths are stored, otherwise only file names
    */
    void findScripts(QStringList& target, const QStringList& directories, bool absolute) const;
    /*!
     \brief Searches directories for files and stores them in a target list.

     The list will not be cleared.

     \param target list to store found files in
     \param includeDirectories list of directories to search in
     \param includeFiles list of files to check
     \param filters list of file filters to apply (e.g. "*.xml")
     \param absolute if true, results are stored as absolute file paths, otherwise only filenames are stored
    */
    void findFiles(QStringList& target,const QStringList& includeDirectories, const QStringList& includeFiles,
                     const QStringList& filters, bool absolute) const;

    QVariant getDefaultTaskData(TaskDefinition::ETaskType type, TaskDefinition::ETaskData field) const; /*!< \brief Returns a task definitions default value. */
    QVariant getDefaultTaskData(TaskDefinition::ETaskType type, int field) const;                       /*!< \brief Returns a task definitions default value. */
    void setDefaultTaskData(TaskDefinition::ETaskType type, TaskDefinition::ETaskData field, const QVariant& value); /*!< \brief Sets a task definitions default value. */
    void setDefaultTaskData(TaskDefinition::ETaskType type, int field, const QVariant& value);                       /*!< \brief Sets a task definitions default value. */

    /*!
     \brief Locks access to the include path list.

     The include paths are used by SimulationThreads and the SimulationManager in the main thread and therefore need to be locked.
    */
    void lockIncludePaths();
    /*!
     \brief Unlocks access to the include path list.

     The include paths are used by SimulationThreads and the SimulationManager in the main thread and therefore need to be locked.
    */
    void unlockIncludePaths();

    /*!
     \brief Create context menu entries for a list of files and files in a list of include directories.

     \param menu menu to add entries to
     \param actionReceivers list to fill action receivers for menu entry clicks into
     \param signalTarget optional target object to call when a generated entry is clicked
     \param slot target function of the optional target object to call with signature (QString) when a generated entry is clicked
     \param includePaths list of include path to search files in
     \param includeFiles list of files to generate entries for directly
     \param filters file filters (format: "*.rb;*.rsg"...)
     \param absolute if true, absolute filepaths will be displayed in the context menu, otherwise relative paths will be used
    */
    void createIncludeFileContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers,
        QObject* signalTarget = 0, const char* slot = "", const QStringList& includePaths = QStringList(), const QStringList& includeFiles = QStringList(),
        const QStringList& filters = QStringList("*.rb"), bool absolute = true);
    /*!
     \brief Create context menu entries for script files in include file and include directory list

     \param menu menu to add entries to
     \param actionReceivers list to fill action receivers for menu entry clicks into
     \param signalTarget optional target object to call when a generated entry is clicked
     \param slot target function of the optional target object to call with signature (QString) when a generated entry is clicked
     \param absolute if true, absolute filepaths will be displayed in the context menu, otherwise relative paths will be used
    */
    void createScriptContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers,
        QObject* signalTarget = 0, const char* slot = "", bool absolute = true);
    /*!
     \brief Create context menu entries for executable files in include file and include directory list

     \param menu menu to add entries to
     \param actionReceivers list to fill action receivers for menu entry clicks into
     \param signalTarget optional target object to call when a generated entry is clicked
     \param slot target function of the optional target object to call with signature (QString) when a generated entry is clicked
     \param absolute if true, absolute filepaths will be displayed in the context menu, otherwise relative paths will be used
    */
    void createExecutableContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers,
        QObject* signalTarget = 0, const char* slot = "", bool absolute = true);

protected:
    // protected functions

    /*!
     \brief Initialize settings with default settings.
    */
    void loadDefaultSettings();
    /*!
     \brief Initialize settings by loading from global settings file.
    */
    void loadSettings();
    /*!
     \brief Save settings to global settings file.
    */
    void saveSettings();

    /*!
     \brief Loads a simulation setup from an opened document.

     \param document dom document
     \param file filepath, used to log
     \param name defines header information of error and warning messages
     \return the loaded setup, or null pointer if loading was not successful
    */
    boost::shared_ptr<SimulationSetup> loadSimulationSetup(QDomDocument& document, const QString& file, const QString& name);
    /*!
     \brief Loads a simulation setup from an opened document and stores it in target.

     \param target container for setup data
     \param document dom document
     \param file filepath, used to log
     \param name defines header information of error and warning messages
     \return true if loading was successful
    */
    bool loadSimulationSetup(SimulationSetup& target, QDomDocument& document, const QString& file, const QString& name);
    /*!
     \brief Loads plugin definitions from an addplugins or removeplugins node.

     \param element root element
     \param target list to store definitions to
     \param msg error message header
     \param vague if true, vague definitions (with sparse information) can be loaded
     \return true if loading of at least one plugin was successful
    */
    bool loadPluginDefinitions(QDomElement& element, SimulationSetup::TPluginDefinitions& target, const QString& msg, bool vague = false);
    /*!
     \brief Loads thread definitions from a threads node.

     \param element root element
     \param target target list to store to
     \param msg error message header
     \return true if loading of at least one thread definition was successful
    */
    bool loadTaskDefinitions(QDomElement& element, SimulationSetup::TTaskDefinitions& target, const QString& msg);

    /*!
     \brief Save the SimulationSetup of the current Simulation to a file.

     If the given path is empty, the file is saved to its default target file. Before overwriting a "default" setup the user has to confirm.

     \param err error code storage, 1 in case of error, 2 if user decided not to save, 0 otherwise
     \param path alternative path
     \return saved or updated setup, null pointer if nothing was saved
    */
    boost::shared_ptr<SimulationSetup> saveCurrentSimulationSetup(int& err, const QString& path = QString());
    /*!
     \brief Saves a simulation setup to its target file.

     Called by other save functions to save a preset SimulationSetup in the current setup list.

     \param setup setup reference
     \param err error code storage, 1 in case of error, 2 if user decided not to save, 0 otherwise
     \param overwrite if true, the currently chosen file is overwritten without asking if possible
     \param rememberPath if true, the next time a save dialog is opened, it will be in the folder of this path
     \return saved or updated setup, null pointer if nothing was saved
    */
    boost::shared_ptr<SimulationSetup> saveSimulationSetupProtected(boost::shared_ptr<SimulationSetup> setup, int& err, bool overwrite = false, bool rememberPath = true);
    /*!
     \brief Saves a simulation setup to its target file. The setup reference will be COPIED to a new shared pointer of a SimulationSetup.

     Called by other save functions to save a new or overwrite a simulation setup state.

     \param setup setup reference
     \param err error code storage, 1 in case of error, 2 if user decided not to save, 0 otherwise
     \param overwrite if true, the currently chosen file is overwritten without asking if possible
     \param rememberPath if true, the next time a save dialog is opened, it will be in the folder of this path
     \return saved or updated setup, null pointer if nothing was saved
    */
     boost::shared_ptr<SimulationSetup> saveSimulationSetupProtected(SimulationSetup& setup, int& err, bool overwrite = false, bool rememberPath = true);
    /*!
     \brief Adds thread nodes to the element.

     \param doc parent document
     \param element target element
     \param source data to store
     \param msg error message header
     \return true if successful
    */
    bool saveTaskDefinitions(QDomDocument& doc, QDomElement& element, const SimulationSetup::TTaskDefinitions& source, const QString& msg);
    /*!
     \brief Adds plugin nodes to the element.

     \param doc parent document
     \param element target element
     \param source data to store
     \param msg error message header
     \param vague if true, "empty" data fields wont be saved
     \return true if successful
    */
    bool savePluginDefinitions(QDomDocument& doc, QDomElement& element, const SimulationSetup::TPluginDefinitions& source, const QString& msg, bool vague = false);

    /*!
     \brief Adds a new include directory if not contained already.

     \param path absolute path
     \return true if the directory was not contained already
    */
    bool addIncludeDirectory(const QString& path);
    /*!
     \brief Adds a new include file if not contained already.

     \param path absolute path
     \return true if the path was not contained already
    */
    bool addIncludeFile(const QString& path);
    /*!
     \brief Removes an include directory.

     \param path absolute path
     \return true if there was a directory to remove
    */
    bool removeIncludeDirectory(const QString& path);
    /*!
     \brief Removes an include file.

     \param path absolute path
     \return true if there was a file to remove
    */
    bool removeIncludeFile(const QString& path);

    /*!
     \brief Returns next id for a simulation task.

     With every call the task id is increased by one.
    */
    int getNextTaskId();

private:
    // private functions

    /*!
     \brief Specify a file to save a setup to.

     Asks the user for permission if the setups specified file or the chosen file is already used by another registered SimulationSetup.

     \param setup setup that is about to be saved
     \param tryExistingFirst if true, the setups contained filename is tried to use as target file first
    */
    bool chooseFile(SimulationSetup& setup, bool tryExistingFirst);

    /*!
     \brief Removes all SimulationSetup definitions one at a time. This wont remove the currently initialized SimulationSetup.
    */
    void clearSetupList();

public: signals:
    //public signals

    /*!
     \brief Emitted when a single entry was added to the setup list.

     Is not emitted when the list is reloaded completely.
     Use setupListEntryAdded() for this.

     \param index index of the new entry
    */
    void setupListSingleEntryAdded(int index);
    /*!
     \brief Emitted when a single entry was removed from the setup list.

     Is not emitted when the list is reloaded completely.
     Use setupListEntryRemoved() for this.

     \param index former index of the entry
    */
    void setupListSingleEntryRemoved(int index);
    /*!
     \brief Emitted whenever a new entry was added to the setup list.

     When the list is reloaded, this signal is emitted many times. You might want to receive setupListSingleEntryAdded() and
     setupListReloaded() or aboutToReloadSetupList() instead.

     \param index index of the new entry
    */
    void setupListEntryAdded(int index);
    /*!
     \brief Emitted whenever an enty was removed from the setup list.

     When the list is reloaded, this signal is emitted many times. You might want to receive setupListSingleEntryRemoved() and
     setupListReloaded() or aboutToReloadSetupList() instead.

     \param index former index of the entry
    */
    void setupListEntryRemoved(int index);
    /*!
     \brief Emitted when an enty was of the setup list was changed.

     \param index index of the entry
    */
    void setupListEntryChanged(int index);
    /*!
     \brief Emitted after the setup list was reloaded completely.
    */
    void setupListReloaded();
    /*!
     \brief Emitted before the setup list is reloaded completely.

     The setup list is cleared immediately afterwards.
    */
    void aboutToReloadSetupList();

    /*!
     \brief Emitted before a new Simulation is initialized.

     Use this signal to fetch or store data before the setup begins to be closed. When reaching the destructor of a plugin or task the other plugins and tasks
     are not guaranteed to be still running.

     Name and path specify wich setup is going to be initialized. If the setup is a save state, the SimulationManager might not contain a preset setup with this name
     or a setup with this name is not identical to the save sate. Use access via filename to identify the setup.

     You might want to receive the signal of the current Simulation aboutToBeStopped() instead.

     \param name name of the setup
     \param path file path of the setup
    */
    void aboutToInitializeSetup(QString name, QString path);
    /*!
     \brief Emitted after a new setup from the setup list has been initialized.

     \param name name of the setup
     \param path path of the setup file, unique for all setups
    */
    void setupInitialized(QString name, QString path);
    /*!
     \brief Emitted after the current SimulationSetup has been started or unpaused. Threads will still need time to start up.

     \param name name of the current SimulationSetup
    */
    void setupRunning(QString name);
    /*!
     \brief Emitted after the current SimulationSetup has been stopped.

     \param name name of the current SimulationSetup
    */
    void setupStopped(QString name);
    /*!
     \brief Emitted after the current SimulationSetup has been paused.

     \param name name of the current SimulationSetup
    */
    void setupPaused(QString name);
    /*!
     \brief Emitted when a setup has been saved.

     Includes preset setups and save states.

     \param name name of the setup
     \param path file path of the setup
    */
    void setupSaved(QString name, QString path);

    /*!
     \brief Emitted when a setting value used by the SimulationManager has been changed.

     All Settings are changed in the SimulationManagers own settings group.

     \param name name of the setting
    */
    void settingChanged(QString name);

public slots:
    //public slots

    /*!
     \brief Makes the SimulationManager ready to be deleted. Stops current Simulation.

     Called by Carbons aboutToShutdown() signal.
    */
    void cleanup();

    /*!
     \brief Update a setting from the SimulationManagers default settings group by name.

     \param name name of the setting
    */
    void updateSetting(QString name);

private:
    // private members

    Carbon* mParent; /*!< Pointer to parent carbon instance. */
    GuiSettings::SimulationSettings* mSimulationSettingsWidget; /*!< General Settings Widget. */

    boost::shared_ptr<Simulation> mSimulation; /*!< Actually initialized SimulationSetup. Initially an empty Simulation is loaded.*/
    TSetupList mSetupList; /*!< List of all available simulation setups */
    bool mReloadingSetups; /*!< True if currently reloading the setup list. */
    bool mClosed; /*!< True after cleanup has been called. */

    /*! Unique server. Must never be deleted, but may only be resetted.
        Instead of initializing a new server, this one is used and reinitialized.*/
    boost::shared_ptr<ServerThread> mServerThread;

    boost::shared_ptr<Settings> mSettings; /*!< Pointer to global settings object. */

    int mNextTaskId; /*!< Next simulation task id. */

    QString mDefaultPluginPath;             /*!< Default path to load plugin libraries from. */
    QString mDefaultSetupPath;              /*!< Default path to load preset setups from and save them to. */
    QString mDefaultSavePath;               /*!< Default path to load setup save states from and save them to. */
    QStringList mDefaultSetupExtensions;    /*!< Default extensions to include when loading setup files. */
    QString mDefaultSetupExtensionText;     /*!< Default extensions to load as spark simulation setup files.*/
    QString mDefaultSetupExtensionSuffix;   /*!< Default extension to use for new simulation setup files.*/
    QStringList mSetupIncludeDirectories;   /*!< List of include directories for simulation setups. */
    QStringList mSetupIncludeFiles;         /*!< List of single include files for simulation setups. */
    QStringList mScriptIncludeDirectories;  /*!< List of include directories for script files. */
    QStringList mScriptIncludeFiles;        /*!< List of single include files for script files. */
    QStringList mExecutableIncludeDirectories; /*!< List of include directories. */
    QStringList mExecutableIncludeFiles;    /*!< List of single include files. */
    QStringList mExecutableExtensions;      /*!< Default extensions to include when loading executable files. */
    QString mExecutableExtensionText;       /*!< Default extensions to use for executable files.*/
    QString mExecutableSuffix;          /*!< Default suffix for executable files. */
    QStringList mScriptExtensions;      /*!< Default extensions to include when loading executable files. */
    QString mScriptExtensionText;       /*!< Default extensions to use for executable files.*/
    QString mScriptSuffix;              /*!< Default suffix for executable files. */
    QString mDefaultSetup;              /*!< Name of the default setup to load. */
    QString mLastLoadedSetup;           /*!< Name of the last setup loaded manually. */
    QString mLastInitializedSetup;      /*!< Name of the last setup initialized from the setup list. */
    QString mLastLoadedSetupPath;       /*!< Path of the last setup loaded manually. */
    QString mLastInitializedSetupPath;  /*!< Path of the last setup initialized from the setup list. */

    QList<QVariant> mDefaultServerThreadData; /*!< New task definition default values. */
    QList<QVariant> mDefaultPluginThreadData; /*!< New task definition default values. */
    QList<QVariant> mDefaultProcessData;      /*!< New task definition default values. */
    QList<QVariant> mDefaultSparkProcessData; /*!< New task definition default values. */

    mutable QMutex mIncludePathsMutex; /*!< Mutex to lock access to include paths. */
};

#endif //SIMULATIONMANAGER_H
