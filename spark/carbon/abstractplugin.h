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

#ifndef ABSTRACTPLUGIN_H
#define ABSTRACTPLUGIN_H
/**
 * \file abstractplugin.h
 * \brief Contains AbstractPlugin, AbstractPluginInstantiator and PluginFactory classes.
 *
 * \warning PluginFactory and AbstractPlugin have to reside in the same file!! The automatic registration of plugins to the factory needs the initialization
 * of static variables for the factory, which can only be guaranteed if they are contained in the same file. Removing one of the classes may result in undefined
 * behaviour during program start.
 */

#include "plugintype.h"
#include "cutelogger/logger.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include <vector>
#include <QString>
#include <QStringList>
#include <map>
#include <QWidget>
#include <QIcon>
#include <QSettings>
#include <QAbstractTableModel>

class PluginManager;
class PluginFactory;
class PluginInstantiatorTableModel;
class PluginDefinition;
class Settings;
class Carbon;

/*!
  \brief Export macro to start exporting plugins from a shared library.

  <b>Usage:</b>\n
  Define one or more plugin classes derived from Plugin, SignalPlugin or AttachableFrame.
  Register the plugins in the plugin factory as plugins ready to be exported using REGISTER_EXPORT_PLUGIN, REGISTER_EXPORT_SIGNALPLUGIN, or
  REGISTER_EXPORT_FRAME.
  In a separate export header file, use PLUGINS_EXPORT_BEGIN and PLUGINS_EXPORT_END to export the plugins with PLUGIN_EXPORT.
  Using the export macros in a file with a Qt derived file (a file that will be parsed by the qt meta object compiler) won't work.

  The main application will call the export function defined by this macro.
  This will register the static logger singleton, the static carbon pointer, the plugin factory and plugin class information (source library and class id)
  in the shared library.
*/
#define PLUGINS_EXPORT_BEGIN()\
    extern "C"{\
    LIB_EXPORT int ExportPlugins(void* pluginFactory, int nextClassId, const char* libraryName, void* loggerRegistration, void* carbon){ \
    if (pluginFactory == 0) \
    { \
        printf("ERROR: Illegal plugin factory pointer."); \
        return 0; \
    } \
    PluginFactory::registerStaticFactory((PluginFactory*)pluginFactory); \
    if (loggerRegistration != 0) Logger::registerStaticLogger((Logger::LoggerRegistration)loggerRegistration); \
    if (carbon != 0) Carbon::registerStaticCarbonPointer((Carbon*)carbon);

/*!
  \brief Export macro to stop exporting plugins from a shared library.

  <b>Usage:</b>\n
  Define one or more plugin classes derived from Plugin, SignalPlugin or AttachableFrame.
  Register the plugins in the plugin factory as plugins ready to be exported using REGISTER_EXPORT_PLUGIN, REGISTER_EXPORT_SIGNALPLUGIN, or
  REGISTER_EXPORT_FRAME.
  In a separate export header file, use PLUGINS_EXPORT_BEGIN and PLUGINS_EXPORT_END to export the plugins with PLUGIN_EXPORT.
  Using the export macros in a file with a Qt derived file (a file that will be parsed by the qt meta object compiler) won't work.

  The main application will call the export function defined by this macro.
  This will register the static logger singleton, the static carbon pointer, the plugin factory and plugin class information (source library and class id)
  in the shared library.
*/
#define PLUGINS_EXPORT_END()\
    return nextClassId; \
    }}

/*!
  \brief Export macro to export a single plugin from a shared library.

  <b>Parameter:</b>\n
  CLASS: Name of the plugin class.

  <b>Usage:</b>\n
  Define one or more plugin classes derived from Plugin, SignalPlugin or AttachableFrame.
  Register the plugins in the plugin factory as plugins ready to be exported using REGISTER_EXPORT_PLUGIN, REGISTER_EXPORT_SIGNALPLUGIN, or
  REGISTER_EXPORT_FRAME.
  In a separate export header file, use PLUGINS_EXPORT_BEGIN and PLUGINS_EXPORT_END to export the plugins with PLUGIN_EXPORT.
  Using the export macros in a file with a Qt derived file (a file that will be parsed by the qt meta object compiler) won't work.

  The main application will call the export function defined by the plugins GUI_PLUGIN, GUI_SIGNALPLUGIN or GUI_FRAME macro.
  This will export the plugin to the main applications plugin factory with the library name and (incremented) class id given by the
  main application.
*/
#define PLUGIN_EXPORT(CLASS)\
    CLASS::exportPlugin((PluginFactory*)pluginFactory, nextClassId, libraryName); \
    nextClassId = nextClassId + 1;

/*!
 \brief Base class for Gui Plugins.

 <b>General:</b>\n
 Plugins are the main building blocks of the Guis and define its visible parts (AttachableFrame) and executable parts (Plugin, SignalPlugin).
 They can be managed using the PluginManagers PluginDialog in the Gui main menu and they can be created by PluginDefinitions in SimulationSetups
 managed by the SimulationManager. A Simulation can load and initialize plugins (instantiate the class objects) and it can execute the plugins
 in a PluginThread.

 Plugins register themselves using the creation Macros of the derived classes at a factory and can be created or removed at
 runtime. All plugins have a unique classId and className. When instantiated by the PluginFactory they receive a unique
 pluginId and can be assigned a caption.

 <b>Contents:</b>\n
 Plugins may create Menus, MenuActions, Toolboxes and ToolboxActions directly or from a tool definition file (see MenuManager)
 thus providing the GUI with control options for the plugin. They may also define a SettingsWidget displayed in the main
 Settings Dialog.

 <b>Access to Plugins:</b>\n
 Plugins can be searched an utilized at runtime by searching for their name or definition directly using findPlugins().
 A better and order-independant way is to use the CommunicationManager to create communication channels between plugins.

 <b>Creation:</b>\n
 Plugins are registered using registration macros, depending on the type of plugin you want to create.\n

 Simple Plugin:\n
 Derive from Plugin\n
 Register using GUI_PLUGIN in the class definition and REGISTER_PLUGIN in the .cpp file.\n

 Signal sending and receiving Plugin:\n
 Derive from SignalPlugin\n
 Register using GUI_SIGNALPLUGIN in the class definition and REGISTER_SIGNALPLUGIN in the .cpp file.\n

 AttachableFrame Plugin:\n
 Derive from AttachableFrame\n
 Register using GUI_FRAME in the class definition and REGISTER_FRAME in the .cpp file.\n

 <b>Class Information:</b>\n
 Not all of the information of a plugins has to be (or can be) specified before complete startup of the manager classes.
 So plugins can specify some information like for example a class icon or description text when the plugin is created for the first time.
 These information fields can be set with setClassInfo and are stored by the Gui Settings, so the information is still known even if there is no
 instantiated plugin of this class.\n

 <b>Execution:</b>\n
 Plugins can be executed using the virtual run() method. This can be done either by other plugins via the PluginManager or in the run() method of a PluginThread.

 Plugins that are called by other plugins are called immediately in the callers thread. Plugins that run indefinitely will cause the calling thread to freeze.
 Plugins and SignalPlugins can be started in PluginThreads of a Simulation. An endless loop in a plugin started in another thread naturally wont freeze the other threads.

 If your derived plugin is not meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_LOOP).
 If your derived plugin is only meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_ONCE).
 Otherwise the PluginManager will allow any execution of the Plugin (ET_ANY).

 /warning AttachableFrame plugins should usually never have the need of being executed manually since they can receive direct user interaction. Their execution
 type is ET_NEVER, which means they cant be run in another thread and cant be executed in another thread. If however you wish to enable execution of the virtual
 run(), pause(), stop() and reset() functions, you can enable it using setExecutionType(). In this case make sure not to call any functions that emit
 non-queued signals and never call functions or emit signals which result in the (non-main-)thread changing any Gui-Object properties. When another thread except for
 the main thread changes the gui layout this will most likely result in a crash (or sometimes in a deadlock).

 /todo Loading and managing single plugins is currently quite easy. However having many plugins that depend on each other is inconvenient.
 A solution to this problem would be having a Dependency-System between Plugins.
 This way, loading a plugin could always search for an instantiated plugin of the required type or initialize it with default parameters if needed.
*/
class AbstractPlugin
{
    friend PluginManager;
    friend PluginFactory;

public:
    //public types

    /*!
     \brief Defines how the plugin is able to be executed.
    */
    enum EExecutionType
    {
        ET_ANY,     /*!< Any execution type is ok. This is the default for Plugins and SignalPlugins. The plugin has to check for the thread it is running in itself. */
        ET_ONCE,    /*!< The plugins run() function has to return on its own. The plugin can be called by the main thread or other plugins. It can be executed in a PluginThread, but it will stop right after the plugin execution. */
        ET_LOOP,    /*!< The plugins run() function will not stop on its own before stop() has been called. It must only be executed in another thread.*/
        ET_NEVER    /*!< The plugin cant be executed manually. This is the default for all AttachableFrames.*/
    };

    /*!
     \brief Plugin class information fields.

     Can be set by any plugin instance to specify class properties.
     Every plugin info id is converted to a unique string when loading or saving class data.
     This string is used as a string to save the class info with in the global settings object.
    */
    enum EPluginInfo
    {
        PI_DESCRIPTION = 0, /*!< QString: Short description text for the plugin class. */
        PI_TAGS,            /*!< QStringList: List of Tags for the plugin class. */
        PI_ICON_NAME        /*!< QString: Name of an icon for the class. */
    };

protected:
    // protected types

    /*!
     \brief Helper-Type containing a list of tags.
    */
    typedef std::vector<QString> TTagList;

public:
    // public functions
    /*!
     \brief Basic destructor.
    */
    virtual ~AbstractPlugin();

    /*!
     \brief Init function that is called after the plugin is created by the manager.

     During constructor, the plugin has no caption yet. Therefore several functions like accessing settings with the plugin caption wont work in the constructor.
     Derive from init() instead.
     init() is called with the list of specified parameters in the plugin definition.

     \param parameters list of parameters
     \sa Settings
    */
    virtual void init(const QStringList& parameters = QStringList()) = 0;

    /*!
     \brief Returns id of the plugin OBJECT (unique for every created plugin).
    */
    int getPluginId() const;

    /*!
     \brief Returns the caption.
    */
    const QString& getCaption() const;

    /*!
     \brief Returns true if the derived class claims to posess a visual representation using a QWidget.
    */
    bool hasVisualRepresentation() const;
    /*!
     \brief Returns the visual representation or 0 if there is none.

     This function will log an error if called without having a visual representation. It should only be called for
     QWidget derived plugins.
    */
    virtual QWidget* getVisualRepresentation() const;

    /*!
     \brief Returns true if the derived plugin claims to be a QObject.
    */
    bool isQObject() const;
    /*!
     Returns the plugin as QObject or 0 if it is no QObject.

     This function will log an error if called without being a Qobject. It should only be called for
     QObject derived plugins.
    */
    virtual QObject* getQObject() const;

    /*!
     \brief Returns the plugin type.
    */
    EPluginType getPluginType() const;
    /*!
     \brief Returns the plugin type as string.
    */
    QString getPluginTypeString() const;
    /*!
     \brief Returns true if the plugin is flagged as extension.
    */
    bool isExtension() const;

    /*!
     \brief Returns the plugin class id (for new derived class).

     Derived virtual function is created automatically with the GUI_PLUGIN, GUI_SIGNALPLUGIN or GUI_FRAME, macro.
    */
    virtual int getClassId() const = 0;
    /*!
     \brief Returns the plugin class name (for new derived class).

     Derived virtual function is created automatically with the GUI_PLUGIN, GUI_SIGNALPLUGIN or GUI_FRAME, macro.
    */
    virtual const QString& getClassName() const = 0;
    /*!
     \brief Returns the plugin classes maximum amount of instances.

     Be sure to always handle possible instantiation of multiple objects of the same class when specifying an
     amount bigger than one.
    */
    int getMaxInstances() const;
    /*!
     \brief Returns the plugin classes object index.

     Unique for each object of one class.
    */
    virtual int getInstanceIndex() const;

    /*!
     \brief Sets the execution type of the Plugin.
     */
    void setExecutionType(EExecutionType type);
    /*!
     \brief Returns the execution type of the Plugin.
     */
    EExecutionType getExecutionType() const;

    /*!
     \brief Return pointer to the definition that created this plugin.

     Used by SimulationManager. Is used by the plugin to update the definition whenever its relevant data changes.
     If no definition was specified, returns null pointer!
     \return pointer to specified definition or null pointer
    */
    boost::shared_ptr<PluginDefinition> getPluginDefinition() const;
    /*!
     \brief Sets pointer to the definition that created this plugin.

     Used by Simulation. Is used by the plugin to update the definition whenever its relevant data changes.
    */
    void setPluginDefinition(boost::shared_ptr<PluginDefinition> definition);

    /*!
     \brief Sets the caption.

     \param caption caption to use
    */
    virtual void setCaption(const QString& caption);

    /*!
     \brief Abstract function to signal a plugin to load data from a QSettings save file.

     \todo Using a QSettings object to save data is the easy solution and well suited for most cases. However, if there are frames who produce a large amount of
     data, QSettings should be replaced with a file-streaming solution to avoid loading too much data into the memory.

     \param data source file
     \return true if something was loaded
    */
    virtual bool loadData(boost::shared_ptr<QSettings> data) {return false;}
    /*!
     \brief Abstract function to signal a plugin to save its data to a QSettings save file.

     \todo Using a QSettings object to save data is the easy solution and well suited for most cases. However, if there are frames who produce a large amount of
     data, QSettings should be replaced with a file-streaming solution to avoid loading too much data into the memory.

     \param data target file
     \return true if something was saved
    */
    virtual bool saveData(boost::shared_ptr<QSettings> data) const {return false;}

    /*!
     \brief Run function that is called when the plugin is executed by either another plugin, the plugin manager, or when they are executed in another thread.

     Plugins that are not executed in another thread but dont return from run() will cause the Gui to freeze.

     The EExecutionType of the plugin specifies if it can be executed from the main thread and plugins, from another thread or not at all.

     \return returns true if running was successful
    */
    virtual bool run() = 0;
    /*!
     \brief Stop function to stop a plugin executed in another thread.

     The function is called when the plugin is executed in another thread and is requested to stop its execution as soon as possible. The Thread will notice that the
     plugin is stopped when it returns from its run() function.

     The EExecutionType of the plugin specifies if it can be executed from the main thread and plugins, from another thread or not at all.

     \return returns true if stopping was successful. Returns false if plugin could not be stopped or was not running in the first place
    */
    virtual bool stop() = 0;
    /*!
     \brief Pause function to pause a plugin executed in another thread.

     The function is called when the plugin is executed in another thread and is requested to pause or unpause its execution until the next time pause(),
     stop() or run() is call. Plugins executed in the main thread are expected to only take fractions of a second of time so pause() is not needed and wont be called.

     The EExecutionType of the plugin specifies if it can be executed from the main thread and plugins, from another thread or not at all.

     \return returns true if pausing/unpausing was successful or false if plugin pause state did not have to be changed
    */
    virtual bool pause(bool state) = 0;
    /*!
     \brief Restart function to stop and start a plugin executed in another thread.

     The function is called when the plugin is executed in another thread and is requested to stop and restart its execution.
     Plugins executed in the main thread are expected to only take fractions of a second of time so restart() is not needed and wont be called. Instead reoccurring calls
     all use run().

     The EExecutionType of the plugin specifies if it can be executed from the main thread and plugins, from another thread or not at all.

     \return returns true if restarting was successful
    */
    virtual bool restart() = 0;

    /*!
     \brief Returns class information for this class as QVariant.

     The class information is stored in the global gui settings.
     \param name name of the field to read out
     \return data field or empty QVariant if this information was not specified yet
    */
    QVariant getClassInfo(const QString& name);
    /*!
     \brief Returns class information for this class as QVariant.

     The class information is stored in the global gui settings.
     \param info id of the data field to read out
     \return data field or empty QVariant if this information was not specified yet
    */
    QVariant getClassInfo(EPluginInfo info);
    /*!
     \brief Returns class information for a plugin class as QVariant.

     The class information is stored in the global gui settings.

     \param classId id of the class
     \param name name of the field to read out
     \return data field or empty QVariant if this information was not specified yet
    */
    static QVariant getClassInfo(int classId, const QString& name);
    /*!
     \brief Returns class information for a plugin class as QVariant.

     The class information is stored in the global gui settings.
     \param classId id of the plugin class
     \param info id of the data field to read out
     \return data field or empty QVariant if this information was not specified yet
    */
    static QVariant getClassInfo(int classId, EPluginInfo info);
    /*!
     \brief Returns the name of a predefined class information field.

     \param info id of the data field to get the name for
     \return name of the field
    */
    static QString getClassInfoName(EPluginInfo info);

    /*!
      \brief Returns the Carbon registered to the plugin class.

      When plugins are exported from shared libraries the Carbon of the application is registered to the plugin so access to the static Carbon
      instance can be done in the plugin class.
    */
    Carbon* getCarbon();

protected:
    // protected functions

    /*!
     \brief Constructor for derived classes.

     \param type type of the plugin class
    */
    AbstractPlugin(EPluginType type);

    /*!
     \brief Post-initialization of plugin information that cant be handed into the constructor.

     Called between the constructor and the init() call of the plugin.
     In the plugin constructor the pluginId is not specified yet (it is zero).

     Called by the PluginManager.

     \param carbon carbon instance the plugin may work with
     \param pluginId id of the plugin in the Carbons PluginManager
    */
    void initAbstractPlugin(Carbon* carbon, int pluginId);

    /*!
     \brief Stores class information for this class as QVariant.

     The plugin class exports class information before first plugin initialization.
     The class information is stored in the global gui settings.
     \param classId id of the plugin class
     \param info id of the data field to save data to
     \param data data to save
    */
    static void setClassInfo(int classId, EPluginInfo info, const QVariant& data);
    /*!
     \brief Stores class information for this class as QVariant.

     The plugin class exports class information before first plugin initialization.
     The class information is stored in the global gui settings.
     \param classId id of the plugin class
     \param name name of the data field to save data to
     \param data data to save
    */
    static void setClassInfo(int classId, const QString& name, const QVariant& data);

    /*!
     \brief Specify class information.

     Class information can be used by the Gui to display properties of the plugin class to the user.
     \param info id of the information field
     \param value value for the field
    */
    void setClassInfo(EPluginInfo info, const QVariant& value);
    /*!
     \brief Specify class information.

     Class information can be used by the Gui to display properties of the plugin class to the user.
     \param name name of the information field
     \param value value for the field
    */
    void setClassInfo(const QString& name, const QVariant& value);

    /*!
     \brief Specifies if the plugin is an extension.

     Is called and should only be called by the PluginManager!
    */
    void setIsExtension(bool extension);

    /*!
     \brief Returns the PluginManager of the Carbon registered to the plugin class.
    */
    PluginManager* getPluginManager();

protected:
    // protected members

    QString mCaption;              /*!< Caption of the plugin. */
    EPluginType mPluginType;       /*!< Type of the plugin. */
    TTagList mTagList;             /*!< Tag list of the plugin. */
    int mPluginId;                 /*!< Unique object id. */
    bool mHasVisualRepresentation; /*!< True for QWidget derived plugins. */
    bool mIsQObject;               /*!< True for QObject derived plugins. */
    bool mIsExtension;             /*!< True for plugins that are "Extensions" and will be loaded with every SimulationSetup. */
    EExecutionType mExecutionType; /*!< Defines, how a plugin can be executed. */

    boost::shared_ptr<PluginDefinition> mPluginDefinition; /*!< Pointer to the definition that created the plugin in the current Simulation. */

private:
    //private members

    boost::shared_ptr<Settings> mPrivateSettings; /*!< Private pointer to the global settings object. */
    PluginManager* mParentPluginManager; /*!< PluginManager that created the plugin. */
    Carbon* mParent; /*!< Controller of the main application. */
};

/*!
 \brief Base Class for Plugin Instantiators.

 The plugin registration macros create an instantiator that is called by the PluginFactory.
*/
class AbstractPluginInstantiator
{
public:
    //public functions
    /*!
     \brief Basic constructor.

     \param classId unique id of the plugin class
     \param name unique name of the plugin class
     \param caption default caption for instances of the plugin class
     \param type type of the plugin
     \param maxInstanceCount maximum amount of instances of this plugin allowed at a time
     \param library name of the library the plugin class was loaded from (empty for built-in plugins)
    */
    AbstractPluginInstantiator(int classId, const QString& name, const QString& caption, EPluginType type,
        int maxInstanceCount, const char* library);
    /*!
     \brief Basic destructor.
    */
    virtual ~AbstractPluginInstantiator();

    /*!
     \brief Abstract instantiation function.
    */
    virtual AbstractPlugin* instantiate() const = 0;

    /*!
     \brief Returns the class name.
    */
    const QString& getName() const;
    /*!
     \brief Returns the default caption for instances of this class.
    */
    const QString& getCaption() const;
    /*!
     \brief Returns the source library for the plugin. Empty for built-in plugins.
    */
    const QString& getLibrary() const;
    /*!
     \brief Returns the plugin type.
    */
    EPluginType getPluginType() const;
    /*!
     \brief Returns the class id.
    */
    int getId() const;
    /*!
     \brief Returns maximum amount of instances for this class.
    */
    int getMaxInstanceCount() const;

protected:
    //protected members

    const int mId;                 /*!< Unique class id of the plugin class. */
    const QString mName;           /*!< Unique name of the plugin class. */
    const QString mDefaultCaption; /*!< Default caption for instances of the plugin class. */
    const QString mLibrary;        /*!< Library the plugin is included in. Empty for built-in plugins. */
    const EPluginType mPluginType; /*!< Type of the plugin. */
    const int mMaxInstanceCount;   /*!< Maximum amount of instances for this plugin class. */
};

/*!
 \brief Factory that registers PluginInstantiators and instantiates them for the PluginManager.

 \warning Do not remove PluginFactory from abstractplugin.h file.
 Plugin initialization uses Macros that are called so early, that a static PluginFactory might not be initialized yet
 if not located in the same file than the registered plugins base class.

 /todo Loading and managing single plugins is currently quite easy. However having many plugins that depend on each other is inconvenient.
 A solution to this problem would be having a Dependency-System between Plugins.
 This way, loading a plugin could always search for an instantiated plugin of the required type or initialize it with default parameters if needed.
*/
class PluginFactory
{
public:
    //public types

    /*!
     \brief Helper-Type containing a map of Plugin instantiators with their class id.
    */
    typedef std::map<int, AbstractPluginInstantiator*> TInstantiatorMap;

public:
    //public functions

    /*!
     \brief Returns the class id of a derived plugin class by its (unique) name.

     \param name name of the plugin class
     \return class id of the derived plugin class, 0 if nothing is found
    */
    int getClassIdByName(const QString& name);

    /*!
     \brief Returns the class name of a derived plugin class by its (unique) id.

     \param id id of the plugin class
     \return class name of the derived plugin class, empty string if nothing is found
    */
    QString getClassNameById(int id);

    /*!
     \brief Returns the source library of a derived plugin class by its (unique) id.

     \param id id of the plugin class
     \return class name of the derived plugin class, empty string if nothing is found or plugin is built-in
    */
    QString getSourceLibraryById(int id);

    /*!
     \brief Returns the source library of a derived plugin class by its (unique) class name.

     \param name name of the plugin class
     \return class name of the derived plugin class, empty string if nothing is found or plugin is built-in
    */
    QString getSourceLibraryByName(const QString& name);

    /*!
     \brief Returns the plugin type of a derived plugin by its (unique) id.

     \param id id of the plugin class
     \return plugin type of the derived plugin class, PT_NONE if nothing is found
    */
    EPluginType getPluginType(int id);

    /*!
     \brief Calls the instantiator and makes the factory produce one of the registered plugins by its class id.

     Is called by the PluginManager.

     \param classId identification of the plugin
     \return new instance as Plugin pointer
    */
    AbstractPlugin* getPluginInstance(int classId);
    /*!
     \brief Returns the maximum amount of instances of a plugin class.

     \param classId plugin classId
     \return max instance amount
    */
    int getMaxInstanceCount(int classId);
    /*!
     \brief Returns true if another plugin with the given class id can be instantiated.

     \param classId id
     \param logError if true, logs an appropriate error message if false is returned
    */
    bool canCreatePlugin(int classId, bool logError = false);

    /*!
     \brief Checks if a plugin with specified name has been registered already.

     \param name name of class to search for
     \return true if class with given name is registered
    */
    bool containsClass(const QString& name);

    /*!
     \brief Returns a table model for the list of registered instantiators.
    */
    boost::shared_ptr<PluginInstantiatorTableModel> getTableModel();

    /*!
     \brief Registration function for new Plugin derived class. Is called using Registration Macros.

     The Macro to use depends on the type of the Plugin you want to create.

     \sa AbstractPlugin

     \param classId id of the plugin class
     \param func instantiator function (automatically generated by GUI_PLUGIN Macro etc.)
    */
    bool registerPluginInstantiator(int classId, AbstractPluginInstantiator* func);

    /*!
     \brief Displays an error message directly and adds it to the static error message list.

    Since errors may occur before the logger is initialized they are stored here and read out after logger initialization.
    If carbon has been initialized already the message is logged normally.

     \param message message to log
    */
    void addErrorMessage(const QString& message);

    /*!
     \brief Returns const reference to the instantiator map.
    */
    const TInstantiatorMap& getInstantiators();

    /*!
     \brief Returns (and creates if neccessary) the static PluginFactory.
     */
    static PluginFactory& getFactory();

    /*!
     \brief Defines the static factory again. Used in libraries when plugins are exported.

     Plugins may use the static PluginFactory indirectly.
     On Windows the static variables are not shared so they need to be redefined.
    */
    static void registerStaticFactory(PluginFactory* factory);

public:
    //public members

    std::vector<QString> mErrorMessages; /*!< List of errors that occured during initialization of plugins. Will be read out automatically by carbon. */

private:
    //private members

    TInstantiatorMap mInstantiatorMap;   /*!< Map containing all plugin instantiator functions. */
    static PluginFactory* mFactory; /*!< Only static instance of the plugin factory. Initialized with first getFactory() call. */
};

#endif //PLUGIN_H
