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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
/**
 * \file pluginmanager.h
 * \brief Contains PluginManager class, which registers and manages Plugins, SignalPlugins and AttachableFrames.
 */

#include <QObject>
#include <map>
#include <vector>
#include <QAbstractTableModel>
#include <QMutex>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include "simulationsetup.h"

class Carbon;
class QDomDocument;
class PluginFactory;
class AbstractPlugin;
class AbstractPluginInstantiator;
class Plugin;
class SignalPlugin;
class AttachableFrame;
class PluginTableModel;
class Settings;
class SimulationManager;
class LibraryLoader;

namespace GuiSettings
{
    class PluginSettings;
}

/*!
 \brief Manager class that registers, instantiates and removes plugins.

 Plugins that are registered to the PluginFactory with one of the plugin registration macros can be instantiated by the PluginManager.
 For example, the SimulationManager loads SimulationSetup files and uses the PluginManager to create plugins based on PluginDefinitions in the SimulationSetup.
 The user can manually explore the list of registered plugin classes and instantiate them or remove plugin instances with a PluginDialog.
 When loading a list of plugins, the manager can control which plugins are kept, which ones are reloaded, which ones have to be removed, and which ones habe to be added.

 Plugins are stored with different kinds of identification values. These different values can be used to search and access plugins.
 They are also used to identify them during one program execution or over several program executions (e.g. to save [[Gui Settings Concept|Settings]] for the plugins).

 <b>Class Id:</b>\n
 The class id directly cosrresponds to a class id und is unique for every plugin class.\n
 It is stored as instantiators in the PluginFactory.\n
 It is different in every program execution.\n
 <b>Plugin Id:</b>\n
 The plugin id directly corresponds to a single plugin instance and is unique for this plugin.\n
 It is stored as first entry in the PluginManagers plugin map.\n
 It is different in every program execution.\n
 <b>Instance Id:</b>\n
 The instance id is an identification for the instance of a plugin.\n
 It is stored in the PluginManager. Plugins not in the management system wont have an instance id. The first plugin of a given class has instance id 1, the second one has instance id 2 etc. When removing the first plugin instance, the second will have instance id 1.\n
 It is different in every program execution and is dynamic (may change any time).\n
 <b>Plugin Name:</b>\n
 The plugin class name directly corresponds to a class id and is unique in the system. It can safely be used as identification for the plugin class.\n
 It is stored in the instantiator of the PluginFactory and in the class.\n
 It is the same in every program execution.\n
 <b>Plugin Caption:</b>\n
 The caption of a plugin is freely choosable. It is advised to not to change it too often, because settings for plugin instances are saved with its caption as identification.\n
 It is stored in the plugin instance.\n
 It CAN be different for every plugin instance and can be changed at runtime. Unless changed manually, plugin instances will have the same name in every program execution.\n

 Plugins can be searched an utilized at runtime by searching for their name, ids, or definition directly using findPlugins() in the PluginManager.
 A better and order-independant way is to use the CommunicationManager (see [[Graphical User Interface Development]]) to create communication channels between plugins.

 The manager handles deleting plugins.
 Some plugins (derived from AttachableFrame and attached to a QWidget) are deleted by Qt and notify the manager of the plugins destruction.
*/
class PluginManager : public QObject
{
    Q_OBJECT

    friend Carbon;
    friend PluginFactory;
    friend LibraryLoader;

public:
    // protected types
    /*!
     \brief Helper-Type containing a map of Plugins with their plugin ids.
    */
    typedef std::map<int, AbstractPlugin* > TPluginMap;

    /*!
     \brief Helper-Type containing a list of plugin class ids and their plugin ids as well as instance ids.
    */
    typedef std::map<int, std::map<int, int> > TPluginInstanceMap;

    /*!
     \brief Helper-Type containing a list of filenames and a reference count.
    */
    typedef std::vector<std::pair<QString, int> > TFileList;

public:
    // public constructor functions

    /*!
     \brief Basic constructor.

     \param parent parent carbon instance
    */
    PluginManager(Carbon* parent);
    /*!
     \brief Basic destructor.
    */
    virtual ~PluginManager();

protected:
    //protected constructor functions

    /*!
     \brief Loads settings and loads extension plugins.

     \return true if loading was successful, false if error happened
    */
    bool init();

    /*!
     \brief Creates and registers the SettingsWidget for the PluginManager.
    */
    void initializeSettingWidget();

public:
    //public functions

    /*!
     \brief Returns a new table Model for the plugin list.
    */
    boost::shared_ptr<PluginTableModel> getTableModel();

    /*!
     \brief Deletes, loads and reloads all plugins as specified in the given simulation setup.

     Called by the SimulationManager and the PluginManager itself.

     \param setup simulation setup to use for loading
    */
    bool loadPlugins(const SimulationSetup& setup);
    /*!
     \brief Deletes, loads and reloads all plugins as specified in the given lists.

     Called by the SimulationManager.

     \param addList contains precise definitions of plugins to add
     \param removeList contains vague definitions of plugins to remove, only needed if removeall is false
     \param reloadall if true, all plugins that do not have to be deleted will be reloaded, unless they specify their reload behaviour otherwise
     \param removeall if true, all plugins that are not specified to be kept, will be deleted
    */
    bool loadPlugins(const SimulationSetup::TPluginDefinitions& addList, const SimulationSetup::TPluginDefinitions& removeList, bool reloadall, bool removeall);

    /*!
     \brief Deletes a plugin completeley if it is contained in the managed list.

     The plugin will of course be inaccesible afterwards.

     \param plugin reference to plugin to delete
     \param removeDefFromSimulation if true, the manager tries to remove the definition that created this plugin from the current simulation setup
     \return true if the plugin was found in the list
    */
    bool deletePlugin(AbstractPlugin& plugin, bool removeDefFromSimulation = false);
    /*!
     \brief Deletes a plugin completely if it is contained in the managed list.

     \param pluginId id of the plugin
     \param removeDefFromSimulation if true, the manager tries to remove the definition that created this plugin from the current simulation setup
     \return true if the plugin was found in the list
    */
    bool deletePlugin(int pluginId, bool removeDefFromSimulation = false);

    /*!
     \brief Reloads a plugin by deleting it completeley and recreating it if it is contained in the managed list.

     \param plugin reference to plugin to reload
     \param definition pointer to definition with additional information on the reinitialization (e.g. new attachment point)
     \return pointer to reloaded plugin, 0 if plugin was not found
    */
    AbstractPlugin* reloadPlugin(AbstractPlugin& plugin, PluginDefinition* definition);
    /*!
     \brief Reinitializes a plugin by reconnecting it to a different attachment point and calling its init function again.

     \param plugin reference to plugin to reinitialize
     \param definition pointer to definition with additional information on the reinitialization (e.g. new attachment point)
     \return pointer to plugin, 0 if plugin was not found
    */
    AbstractPlugin* reinitPlugin(AbstractPlugin& plugin, PluginDefinition* definition);

    /*!
     \brief Creates a new Plugin by a plugin definition.

     The plugin definitions is copied into the created plugin.
     \param definition definition of the plugin class, parameters and so on
     \param addDefToSimulation if true the plugin definition is added to the currently initialized simulations 'simulation setup definition'
     \return pointer to new plugin, or 0 if an error occurred
    */
    AbstractPlugin* createPlugin(const PluginDefinition& definition, bool addDefToSimulation = false);

    /*!
     \brief Creates a new Plugin by a plugin definition.

     The plugin definition is stored in the plugin as shared pointer if creation of the plugin was successful.
     After creation, the id of the plugin definition equals the plugin id.
     \param definition definition of the plugin class, parameters and so on
     \param addDefToSimulation if true the plugin definition is added to the currently initialized simulations 'simulation setup definition'
     \return pointer to new plugin, or 0 if an error occurred
    */
    AbstractPlugin* createPlugin(boost::shared_ptr<PluginDefinition> definition, bool addDefToSimulation = false);

    /*!
     \brief Creates a new simple Plugin by its class name.

     \param name name of the plugin class
     \param caption caption for the plugin
     \param parameters list of parameters for the plugin initialization
     \return pointer to new plugin, or 0 if an error occurred
    */
    Plugin* createSimplePlugin(const QString& name, const QString& caption, const QStringList& parameters = QStringList());
    /*!
     \brief Creates a new SignalPlugin by its class name.

     \param name name of the plugin class
     \param caption caption for the plugin
     \param parameters list of parameters for the plugin initialization
     \return pointer to new plugin, or 0 if an error occurred
    */
    SignalPlugin* createSignalPlugin(const QString& name, const QString& caption, const QStringList& parameters = QStringList());
    /*!
     \brief Creates a new AttachableFrame Plugin by its class name.

     The first of the given attachment points is used.
     \param name name of the plugin class
     \param caption caption for the plugin
     \param targets names of the target attachment points
     \param parameters list of parameters for the plugin initialization
     \return pointer to new plugin, or 0 if an error occurred
    */
    AttachableFrame* createAttachableFrame(const QString& name, const QString& caption, const QStringList& targets, const QStringList& parameters = QStringList());
    /*!
     \brief Creates a new AttachableFrame Plugin by its class name.

     \param name name of the plugin class
     \param caption caption for the plugin
     \param target name of the target attachment point
     \param parameters list of parameters for the plugin initialization
     \return pointer to new plugin, or 0 if an error occurred
    */
    AttachableFrame* createAttachableFrame(const QString& name, const QString& caption, const QString& target, const QStringList& parameters = QStringList());

    /*!
     \brief Searches plugins in the plugin instance list that matches different criteria.

     name, caption and type can be general (empty string or -1) to match with all plugins.
     All matching plugins are added to a list and returned.
     When using findPlugin() to search for a single plugin in the manager you should make sure, that the SimulationSetup
     uses unique captions, so they can be identified by caption.

     \param name class name of the plugin to search for. empty string matches with all plugins
     \param caption caption of the plugin to search for. empty string matches with all plugins
     \param type type of the plugin to search for. -1 matches with all plugins
     \return list of plugins that match the criteria
    */
    std::vector<AbstractPlugin*> findPlugins(const QString& name, const QString& caption = QString(""), int type = -1);

    /*!
     \brief Searches a plugin in the plugin instance list that matches different criteria.

     name, caption and type can be general (empty string or -1) to match with all plugins. If the description matches with more than one plugin,
     a warning is sent. When using findPlugin() to search for a plugin in the manager you should make sure, that the SimulationSetup
     uses unique captions, so they can be identified by caption.

     If the description matches with more than one plugin, the first match is returned.

     \param name class name of the plugin to search for. empty string matches with all plugins
     \param caption caption of the plugin to search for. empty string matches with all plugins
     \param type type of the plugin to search for. -1 matches with all plugins
     \return pointer to first plugin that matches the criteria or 0 if nothing was found
    */
    AbstractPlugin* findPlugin(const QString& name, const QString& caption = QString(""), int type = -1);
    /*!
     \brief Declares or undeclares a plugin an essential extension so it will be loaded will all simulation setups.

     A Plugin declared as a extension will not be unloaded when loading a new SimulationSetup in the SimulationManager.
     They will be loaded first before loading any plugin list with the PluginManager.

     If the SimulationSetup declares the plugin too, the declaration will stay in the setup definition.
     This will result in double instantiation of the plugin if it can be instantiated more than once.
     The extension flag will never be stored within a SimulationSetup, but the plugins flagged as extension are stored
     in an additional SimulationSetup which contains the extensions.

     The PluginManager uses a SimulationSetup object (only with a list of plugins, without tasks) to store these plugin definitions.

     \param pluginId plugin to declare or undeclare extension
     \param extension true for declaration, false for undeclaration
     \param updateCurrentSimulationSetup if true, a declaration will remove the plugin from the current simulations setup and a undeclaration will add it
     \return true if plugin was found and changes could be applied
    */
    bool declareExtensionPlugin(int pluginId, bool extension, bool updateCurrentSimulationSetup);

    /*!
     \brief Called by a plugin that is destroyed by an external source (QtGui).

     Removes the plugin from the list and referencing entities (WindowManager etc.) without calling
     delete.
     Since this function is called from the base destructor, virtual functions and properties
     of the derived classes cant be used anymore.
     \param pluginId id of the plugin
    */
    void notifyDestruction(int pluginId);
    /*!
     \brief Returns a pointer to a plugin instance with the searched id.

     If the plugin is not in the list, 0-pointer is returned.

     \warning Do not access plugins from any other than the main thread because they might be deleted in between getting the pointer and dereferencing it.
     Since the plugin might be deleted by the Gui-Core itself, the only safe way to access the plugin from another thread is using the plugin managers call interface.

     It should not be neccessary to store a plugin pointer. Store its pluginId (instance id) instead. Either way, the signal pluginAboutToBeRemoved() informs about
     removal of the plugin. If the plugin was removed by the gui itself however this signal is emitted in the destructor of the plugin and it must not be dereferenced
     anymore in the mean time. So, dont store plugin pointers in any other than the main thread.

     \param pluginId id of the plugin
    */
    AbstractPlugin* getPlugin(int pluginId) const;
    /*!
     \brief Returns the instance index of a derived plugin object.

     \param plugin reference to plugin
     \return instance index
    */
    int getInstanceIndex(const AbstractPlugin& plugin) const;
    /*!
     \brief Returns the current instance count of a derived plugin class.

     \param classId id of the plugin class
     \return instance count
    */
    int getInstanceCount(int classId) const;
    /*!
     \brief Returns the maximum amount of instances of a plugin class.

     \param classId plugin classId
     \return max instance amount
    */
    int getMaxInstanceCount(int classId) const;

    /*!
     \brief Creates context menu entries for every instantiated plugin in the plugin manager.

     \param menu menu to insert entries to
     \param actionReceivers list to fill action receivers for context menu entries into
     \param signalTarget optional object to call a slot from when a plugin entry was clicked
     \param slot Slot of the optional object to invoke when a plugin entry was clicked. signature is (int) and contains the plugin id
     \param matchDefinition optional plugin definition to restrict the set of plugins that generate entries. Only matching plugins will have an entry in the list
    */
    void createPluginContextMenu(QMenu* menu, std::vector<NumberedActionReceiver*>& actionReceivers,
        QObject* signalTarget = 0, const char* slot = "", const PluginDefinition& matchDefinition = PluginDefinition());

    /*!
     \brief Returns information on a plugin class.

     \param classId id of the class
     \param info type of information requested
     \return the information in the respective type of the field, or invalid QVariant if nothing is specified
    */
    QVariant getPluginClassInfo(int classId, AbstractPlugin::EPluginInfo info);
    /*!
     \brief Returns information on a plugin class.

     \param classId id of the class
     \param field name of information requested
     \return the information in the respective type of the field, or invalid QVariant if nothing is specified
    */
    QVariant getPluginClassInfo(int classId, const QString& field);

    /*!
     \brief Specify class information.

     Class information can be used by the Gui to display properties of the plugin class to the user.
     \param classId id of the plugin class
     \param info id of the information field
     \param value value for the field
    */
    void setPluginClassInfo(int classId, AbstractPlugin::EPluginInfo info, const QVariant& value);
    /*!
     \brief Specify class information.

     Class information can be used by the Gui to display properties of the plugin class to the user.
     \param classId id of the plugin class
     \param name name of the information field
     \param value value for the field
    */
    void setPluginClassInfo(int classId, const QString& name, const QVariant& value);

    /*!
     \brief Calls loadData() for all instantiated plugins with the given data object.

     \param data pointer to source data object
     \return true if anything was loaded
    */
    bool loadData(boost::shared_ptr<Settings> data);
    /*!
     \brief Calls loadData() for all instantiated plugins with the given data object.

     \param data pointer to target data object
     \return true if anything was loaded
    */
    bool saveData(boost::shared_ptr<Settings> data);

    /*!
     \brief Returns the plugin library loader as const pointer.
    */
    const LibraryLoader* getLibraryLoader() const;
    /*!
     \brief Returns the plugin library loader as pointer.
    */
    LibraryLoader* getLibraryLoader();

    /*!
     \brief Adds an include directory from a file dialog to the list of include directories for plugin libraries.

     Plugins included in these directories are linked to the application.
     \return true if adding was successful or directory was already contained
    */
    bool addPluginIncludeDirectory();
    /*!
     \brief Adds an include directory to the list of include directories for plugin libraries.

     Plugins included in these directories are linked to the application.
     \param path absolute path to the directory
     \return true if adding was successful or directory was already contained
    */
    bool addPluginIncludeDirectory(const QString& path);
    /*!
     \brief Removes an include directory from the list of include directories for plugin libraries.

     \param path absolute path to the directory
     \return true if removing was successful
    */
    bool removePluginIncludeDirectory(const QString& path);
    /*!
     \brief Adds a single plugin library file by file dialog to the list of included plugin libraries.

     Plugins removed this way will NOT be unlinked from the application but wont be loaded anymore in the next program execution.
     \return true if adding was successful or file was already contained
    */
    bool addPluginIncludeFile();
    /*!
     \brief Adds a single plugin library file to the list of included plugin libraries.

     Plugins added this way will be linked to the application.
     \param path absolute path to the file
     \return true if adding was successful or file was already contained
    */
    bool addPluginIncludeFile(const QString& path);
    /*!
     \brief Removes a single plugin library file from the list of included plugin libraries.

     Plugins removed this way will NOT be unlinked from the application but wont be loaded anymore in the next program execution.
     \param path absolute path to the file
     \return true if removing was successful
    */
    bool removePluginIncludeFile(const QString& path);

public:
    //public static functions

    /*!
     \brief Returns new class id (for new derived class).

     Does not have to be called manually.
     \return new class id
    */
    static int getNextClassId();
    /*!
     \brief Define next class id (for new derived class).

     Does not have to be called manually.
     The next id is never lowered but only increased.
    */
    static void setNextClassId(int id);

protected:
    //protected functions

    /*!
     \brief Returns new object id (for new derived class object).

     Does not have to be called manually.
     \return new class id
    */
    int getNextPluginId();

public: signals:

    /*!
     \brief Emitted when a setting value used by the PluginManager has been changed.

     All Settings are changed in the PluginManagers own settings group.

     \param name name of the setting
    */
    void settingChanged(QString name);

public slots:

    /*!
     \brief Update a setting from the PluginManagers default settings group by name.

     \param name name of the setting
    */
    void updateSetting(QString name);

private:
    // private functions

    /*!
     \brief Deletes a plugin completely if it is contained in the managed list.

     Private function assured to be called with correct plugin and class ids.
     May simply remove it from the list (for automatically deleted frames)
     or call delete on the plugin (for others).
     \param pluginId id of the plugin
     \param classId id of the plugin class
     \param erase if true, delete is called
     \param removeDefFromSimulation if true, the manager tries to remove the definition that created this plugin from the current simulation setup
     \return true if the plugin was found in the list
    */
    bool deletePlugin(int pluginId, int classId, bool erase, bool removeDefFromSimulation = false);

    /*!
     \brief Removes a pluginId-indexId entry.

     \param classId class id
     \param pluginId plugin id
    */
    void removeInstanceIndex(int classId, int pluginId);

    /*!
     \brief Adds a pluginId-indexId entry.

     \param classId class id
     \param pluginId plugin id
    */
    void addInstanceIndex(int classId, int pluginId);

    /*!
     \brief Returns the include path list for plugin libraries.
    */
    const QStringList& getPluginIncludeDirectories() const;
    /*!
     \brief Returns the include list for single plugin libraries.
    */
    const QStringList& getPluginIncludeFiles() const;
    /*!
     \brief Initializes plugin libray loader and loads included plugin libraries.

     All plugin libraries included in the plugin include directories or the single include file list will be loaded, linked to the application and their
     plugins will be imported.
    */
    void initLibraries();

public: signals:
    //public signals

    /*!
     \brief Emitted when a new plugin is added.
    */
    void pluginAdded(int pluginId);

    /*!
     \brief Emitted when a new plugin is added.
    */
    void pluginAdded(AbstractPlugin* plugin);

    /*!
     \brief Emitted when a plugin is removed.
    */
    void pluginRemoved(int pluginId);

    /*!
     \brief Emitted when a plugin is added.
    */
    void pluginOfClassAdded(int classId);

    /*!
     \brief Emitted when a plugin is removed.
    */
    void pluginOfClassRemoved(int classId);

    /*!
     \brief Emitted when a plugin is going to be deleted.

     Note, that plugins which are deleted by the Qt object management (e.g. AttachableFrames)
     wont be deleted by the PluginManager himself, and therefore dont emit this signal.
     However, they emit aboutToRemovePlugin().
     In the case of external deletion (QWidget derived plugins) the pointer must not be casted to a derived class anymore.
    */
    void aboutToDeletePlugin(AbstractPlugin*);

    /*!
     \brief Emitted when a plugin is going to be removed.

     Note, that plugins which are deleted by the Qt object management (e.g. AttachableFrames)
     wont be deleted by the PluginManager himself, and therefore dont emit aboutToDeletePlugin().
     However, they still emit this signal.
     In the case of external deletion (QWidget derived plugins) the pointer must not be casted to a derived class anymore.
    */
    void aboutToRemovePlugin(AbstractPlugin*);

private:
    // private members

    Carbon* mParent;                                    /*!< Parent carbon instance. */
    LibraryLoader* mLibraryLoader;                      /*!< Loader for plugin libraries. */
    GuiSettings::PluginSettings* mPluginSettingsWidget; /*!< PluginManager settings widget. */

    TPluginMap mPluginMap;                          /*!< Map of all plugins with their plugin ids. */
    TPluginInstanceMap mPluginInstanceMap;          /*!< Map of all plugin ids and their instance ids. */
    boost::shared_ptr<Settings> mSettings;          /*!< Pointer to global Settings object */
    boost::shared_ptr<SimulationSetup> mExtensions; /*!< List of plugins that have to be loaded with every simulation. (contains no tasks) */

    //Settings
    QString mExtensionFile;                 /*!< File to load the SimulationSetup with the extension plugins from. */
    QString mDefaultPluginPath;             /*!< Default path to search plugin libraries in. */
    QStringList mPluginIncludeDirectories;  /*!< List of include directories for plugin libraries. */
    QStringList mPluginIncludeFiles;        /*!< List of single plugin libraries to include. */

    static int mNextClassId;  /*!< Next class id. */
    int mNextPluginId;        /*!< Next plugin instance id. */

    mutable QMutex mIncludePathsMutex;      /*!< Mutex to lock access to include paths. */
};

/*!
 \brief Table model for the plugin managers plugin list.

 Created by the plugin manager.
*/
class PluginTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    //public functions
    /*!
     \brief Basic constructor.

     \param list data reference
    */
    PluginTableModel(PluginManager::TPluginMap& list);
    /*!
     \brief Basic destructor.
    */
    virtual ~PluginTableModel();

    /*!
     \brief Returns row count.
    */
    int rowCount (const QModelIndex & parent = QModelIndex() ) const;
    /*!
     \brief Returns column count.
    */
    int columnCount (const QModelIndex & parent = QModelIndex() ) const;

    /*!
     \brief Change model data.

     \param index index
     \param value value
     \param role role of the call
    */
    bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    /*!
     \brief Returns model data.

     \param index index
     \param role role of the call
    */
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole ) const;

    /*!
     \brief Changes header data.

     \param section index
     \param orientation axis
     \param value value
     \param role role (not used)
    */
    bool setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);
    /*!
     \brief Reads header data.

     \param section index
     \param orientation axis
     \param role role (not used)
    */
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /*!
     \brief Returns handle flags for cells.

     \param index index
    */
    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    /*!
     \brief Returns plugin at model index.

     \param index index
     \return 0 if index is illegal
    */
    AbstractPlugin* getPluginAt(int index) const;

public: signals:

    /*!
     \brief Emitted, when a plugin caption was changed in the model.

     \param plugin changed plugin
    */
    void pluginCaptionChanged(AbstractPlugin* plugin);

    /*!
     \brief Emitted, when a plugin attachment was changed in the model.

     \param plugin changed plugin
     \param attachment new attachment point
    */
    void pluginAttachmentChanged(AbstractPlugin* plugin, QString attachment);

public slots:

    /*!
     \brief Updates the caption of the model.

     \param index
    */
    void updateCaption(int index);

    /*!
     \brief Updates the model when a plugin was added at the end.

     Sent by PluginDialog.
    */
    void updatePluginAdded();

    /*!
     \brief Updates the model when a plugin was added somewhere.

     Sent by PluginManager.
    */
    void updatePluginAdded(int pluginId);

    /*!
     \brief Updates the model when a plugin was removed.

     Sent by PluginManager.
    */
    void updatePluginRemoved(int pluginId);

private:
    //private members

    PluginManager::TPluginMap& mPlugins;   /*!< Model data reference. */
    std::vector<QVariant> mHeaderData;     /*!< Model header data. */
    PluginManager* mPluginManager;         /*!< Parent plugin manager. */
    SimulationManager* mSimulationManager; /*!< Pointer to the simulation manager. */
};

#endif //PLUGINMANAGER_H
