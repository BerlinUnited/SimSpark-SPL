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

#ifndef SIMULATIONSETUP_H
#define SIMULATIONSETUP_H
/**
 * \file simulationsetup.h
 * \brief Contains SimulationSetup class.
 */

#include "taskdefinition.h"
#include "plugindefinition.h"

#include <QObject>
#include <QString>
#include <vector>

/*!
 \brief Defines a simulation program setup which can be loaded from and saved to a file.

 The SimulationManager controls loading and saving SimulationSetups. New Setups can be created using the Gui or Plugins like the SparkFrame.
 Changing the current SimulationSetup, e.g. adding new plugins using the PluginDialog changes the definition in the current Simulation. These
 changes can be saved using the "Save preset Setup" entry in the file menu.\n

 A Simulation Setup is stored in a Spark Simulation Setup xml file (.xml, .3s). It contains general information like name and caption and definitions for plugins and
 tasks to load or unload.

 The SimulationManager copies and initilizes a SimulationSetup in the current Simulation.
*/
class SimulationSetup : public QObject
{
    Q_OBJECT

public:
    // public types

    /*!
     \brief Helper-Type containing a list of frame definitions.
    */
    typedef std::vector<boost::shared_ptr<PluginDefinition>> TPluginDefinitions;

    /*!
     \brief Helper-Type containing a list of task definitions.
    */
    typedef std::vector<boost::shared_ptr<TaskDefinition>> TTaskDefinitions;

    /*!
     \brief Identification for the data fields of a simulation setup.

     Used to emit dataChanged() whenever a data field was added directly.
    */
    enum ESetupData
    {
        SD_NAME,       /*!< Name of the setup (not unique!). */
        SD_FILENAME,   /*!< Target filepath of the setup (always unique!). */
        SD_RELOADALL,  /*!< Reload definition. If true, all plugins will be reloaded upon setup initialization. */
        SD_REMOVEALL,  /*!< Remove definition. If true, all unneeded plugins will be removed upon setup initialization. */
        SD_ADDLIST,    /*!< List of plugin definitions for plugins to add. */
        SD_REMOVELIST, /*!< List of plugin definitions for plugins to remove. */
        SD_TASKLIST,   /*!< List of task definitions for tasks to add. */
        SD_UNDEFINED   /*!< Undefined data field. Used in dataChanged() when all or more than one data field changed at once. */
    };

public:
    // public functions

    /*!
     \brief Complete constructor.

     \param name name of the setup
     \param filename absolute target filepath of the setup
     \param reloadall reloading all kept plugins when initializing the setup
     \param removeall removing all all un-added plugins when initializing the setup
     \param addlist plugins to use when initializing this setup
     \param removelist vague definitions of plugins to remove when initializing the setup
     \param tasklist task to start when running the setup
     \param saved initial value signaling if the setup is synced with its target file
    */
    SimulationSetup(QString name, QString filename, bool reloadall, bool removeall,
        const TPluginDefinitions& addlist, const TPluginDefinitions& removelist, const TTaskDefinitions& tasklist, bool saved = false);

    /*!
     \brief Small constructor.

     \param name name of the setup
     \param filename absolute target filepath of the setup
     \param reloadall reloading all kept plugins when initializing the setup
     \param removeall removing all all un-added plugins when initializing the setup
     \param saved initial value signaling if the setup is synced with its target file
    */
    SimulationSetup(QString name, QString filename, bool reloadall = false, bool removeall = true, bool saved = false);
    /*!
     \brief Copy constructor.

     \param source setup to copy
    */
    SimulationSetup(const SimulationSetup& source);
    /*!
     \brief Copy operator.

     \param source setup to copy
    */
    SimulationSetup& operator=(const SimulationSetup& source);
    /*!
     \brief Basic constructor.
    */
    SimulationSetup();

    /*!
     \brief Basic destructor.
    */
    virtual ~SimulationSetup();

    /*!
     \brief Returns reference to AddPlugin list.
    */
    const TPluginDefinitions& getAddPlugins() const;
    /*!
     \brief Returns reference to RemovePlugin list.
    */
    const TPluginDefinitions& getRemovePlugins() const;
    /*!
     \brief Returns reference to TaskDefinition list.
    */
    const TTaskDefinitions& getTaskDefinitions() const;
    /*!
     \brief Returns reference to AddPlugin list.

     Must not be used for running simulation setups.
    */
    TPluginDefinitions& getAddPlugins();
    /*!
     \brief Returns reference to RemovePlugin list.

     Must not be used for running simulation setups.
    */
    TPluginDefinitions& getRemovePlugins();
    /*!
     \brief Returns reference to TaskDefinition list.

     Must not be used for running simulation setups.
    */
    TTaskDefinitions& getTaskDefinitions();
    /*!
     \brief Returns name.
    */
    const QString& getName() const;
    /*!
     \brief Returns filename.
    */
    const QString& getFileName() const;
    /*!
     \brief Returns true if all plugins that wont be deleted should be reloaded.

     The value might be overwritten by each individual plugin.
    */
    bool getReloadAll() const;
    /*!
     \brief Returns true if all plugins should be deleted before loading new ones.

     The value might be overwritten by each individual plugin in add list.
    */
    bool getRemoveAll() const;

    /*!
     \brief Returns true if a definition of the given type is in the task list.
    */
    bool hasTaskDefinition(TaskDefinition::ETaskType type) const;
    /*!
     \brief Returns a index of the next task definition of the searched task type.

     \param startIndex index to start searching at
     \param type type to search for
     \return index of the found definition or -1 if nothing was found
    */
    int findNextTaskDefinition(int startIndex, TaskDefinition::ETaskType type) const;

    /*!
     \brief Returns a index of the next thread definition in the task list.

     \param startIndex index to start searching at
     \return index of the found definition or -1 if nothing was found
    */
    int findNextThreadDefinition(int startIndex = 0) const;

    /*!
     \brief Returns a index of the next process definition in the task list.

     \param startIndex index to start searching at
     \return index of the found definition or -1 if nothing was found
    */
    int findNextProcessDefinition(int startIndex = 0) const;

    /*!
     \brief Returns a index of the next spark definition in the task list.

     \param startIndex index to start searching at
     \return index of the found definition or -1 if nothing was found
    */
    int findNextSparkDefinition(int startIndex = 0) const;

    /*!
     \brief Returns a index of the next executable definition in the task list.

     \param startIndex index to start searching at
     \return index of the found definition or -1 if nothing was found
    */
    int findNextExecutableDefinition(int startIndex = 0) const;

    /*!
     \brief Insert a new plugin definition to add list.

     Checks, if the definition has enough information to identify a plugin to add.
     The plugin definition is copied and a new shared pointer is created. This will not copy connections on the given addplugin definition.

     \param addplugin reference to definition
     \return pointer to inserted definition or null pointer
    */
    boost::shared_ptr<PluginDefinition> insertAddPlugin(const PluginDefinition& addplugin);
    /*!
     \brief Insert a plugin definition to add list.

     The plugin definition is NOT copied and the given shared pointer is simply stored in the add list.

     \param addplugin definition to add
     \return false if the definition could not be added (e.g. it was illegal)
    */
    bool insertAddPlugin(boost::shared_ptr<PluginDefinition> addplugin);
    /*!
     \brief Insert a new plugin definition to remove list.

     Checks, if the definition has enough information to identify a plugin or group of plugins to remove.
     The plugin definition is copied and a new shared pointer is created. This will not copy connections on the given removeplugin definition.

     \param removeplugin reference to definition
     \return pointer to inserted definition or null pointer
    */
    boost::shared_ptr<PluginDefinition> insertRemovePlugin(const PluginDefinition& removeplugin);
    /*!
     \brief Insert a plugin definition to remove list.

     The plugin definition is NOT copied and the given shared pointer is simply stored in the remove list.

     \param removeplugin definition to add
     \return false if the definition could not be added (e.g. it was illegal)
    */
    bool insertRemovePlugin(boost::shared_ptr<PluginDefinition> removeplugin);

    /*!
     \brief Insert a new task definition.

     The task definition is copied and a new shared pointer is created. This will not copy connections on the given task definition.

     \param task reference to definition
     \return pointer to inserted definition or null pointer
    */
    boost::shared_ptr<TaskDefinition> insertTask(const TaskDefinition& task);
    /*!
     \brief Insert list of plugin definitions to add list.

     Checks for each element, if the definition has enough information to identify a plugin to add.
     The plugin definitions are copied and new shared pointers are created. This will not copy connections on the given addplugin definitions.

     \param addplugins reference to list
    */
    void insertAddPlugins(TPluginDefinitions& addplugins);
    /*!
     \brief Insert list of plugin definitions to remove list.

     Checks for each element, if the definition has enough information to identify a plugin or group of plugins to remove.
     The plugin definitions are copied and new shared pointers are created. This will not copy connections on the given removeplugin definitions.

     \param removeplugins reference to list
    */
    void insertRemovePlugins(TPluginDefinitions& removeplugins);
    /*!
     \brief Insert list of task definitions.

     The task definitions are copied and new shared pointers are created. This will not copy connections on the given task definitions.

     \param tasks reference to list
    */
    void insertTasks(TTaskDefinitions& tasks);
    /*!
     \brief Removes all add-plugin definitions that match the given plugin.

     \param plugin plugin to search for
    */
    int eraseMatchingAddPlugin(const AbstractPlugin* plugin);
    /*!
     \brief Removes all remove-plugin definitions that match the given plugin.

     \param plugin plugin to search for
    */
    int eraseMatchingRemovePlugin(const AbstractPlugin* plugin);
    /*!
     \brief Erases a plugin definition from the add list.

     \param index index in list
    */
    void eraseAddPlugin(int index);
    /*!
     \brief Erases a plugin definition from the remove list.

     \param index index in list
    */
    void eraseRemovePlugin(int index);
    /*!
     \brief Erases a task definition from the task list.

     \param index index in list
    */
    void eraseTask(int index);
    /*!
     \brief Erases a plugin definition from the add list.

     \param definition definition to erase
     \return true if a plugin was deleted
    */
    bool eraseAddPlugin(PluginDefinition* definition);
    /*!
     \brief Erases a plugin definition from the remove list.

     \param definition definition to erase
    */
    bool eraseRemovePlugin(PluginDefinition* definition);
    /*!
     \brief Erases a task definition from the task list.

     \param definition definition to erase
    */
    bool eraseTask(TaskDefinition* definition);
    /*!
     \brief Clears plugin add list.
    */
    void clearAddPlugins();
    /*!
     \brief Clears plugin remove list.
    */
    void clearRemovePlugins();
    /*!
     \brief Clears task list.
    */
    void clearTasks();
    /*!
     \brief Set value for removing all plugins before loading new ones.

     \param removeall value
    */
    void setRemoveAll(bool removeall);
    /*!
     \brief Set value for reloading all plugins that are not removed.

     \param reloadall value
    */
    void setReloadAll(bool reloadall);
    /*!
     \brief Makes the data of the current setup a copy of the sources setup.

     \param source setup to copy from
     \param deep if true, the contained data is deep copied, otherwise shred pointers are copied
    */
    void readData(const SimulationSetup& source, bool deep);

    /*!
     \brief Returns true if the setup is flagged to be and thus going to be saved when asked. (default)
    */
    bool isToSave() const;
    /*!
     \brief Returns true if there havent been any changes since the last saving to the setup.

     Returns true when the setup is in sync with the target file, as long as the file hasnt been changed externally.
    */
    bool isSaved() const;
    /*!
     \brief Tell the setup is has benn saved or changed recently.

     \param state value
    */
    void setSaved(bool state);
    /*!
     \brief Define if the setup should be saved when requested.

     Use this to prevent a setup from being saved (again), e.g. when savign already has been declined at another point, so shutting down the SimulationManager
     wont ask again if the setup should be saved.

     \param state value
    */
    void setDontSave(bool state);

    /*!
     \brief Define the absolute file path and name.

     \param value filepath
    */
    void setFileName(const QString& value);
    /*!
     \brief Set the setup name.

     \param value name
    */
    void setName(const QString& value);

    /*!
     \brief Returns true if the setups originate from the same source file.

     If one of the setups is an unsaved simulation state false is returned.
    */
    bool equals(const SimulationSetup& compare) const;

    /*!
    \brief Returns true if this setup is stored in the currently initialized simulation.
    */
    bool isCurrentSimulation() const;

public: signals:
    //public signals

    /*!
     \brief Emitted when updateDataChanged is called or any data field is set using the access functions.
    */
    void dataChanged(ESetupData field);

public slots:
    //public slots

    /*!
     \brief Emits dataChanged.

     SD_UNDEFINED is used to inform that every field has or might have changed.
    */
    void updateDataChanged(ESetupData field = SD_UNDEFINED);

private:
    // private members

    QString mName;     /*!< Internal name of the setup. (not unique!) */
    QString mFileName; /*!< Absolute filepath for the setup. (unique)*/
    TTaskDefinitions mTaskDefinitions;  /*!< List of definitions for tasks to load. */
    TPluginDefinitions mAddPlugins;     /*!< List of definitions for Plugins to load. */
    TPluginDefinitions mRemovePlugins;  /*!< List of definitions for Plugins to unload. */
    bool mReloadAll;   /*!< If true, all plugins that are not removed will be reloaded. */
    bool mRemoveAll;   /*!< If true, all plugins that are not in add list will be removed. */
    bool mSaved;       /*!< True when the setup has been saved to a file in its actual state. Set to false whenever a value is changed. Has to be set externally.*/
    bool mDontSave;    /*!< If true, the setup wont be saved and there will be no MessageBoxes out of the SimulationManager asking to save. */
};

#endif //SIMULATIONSETUP_H
