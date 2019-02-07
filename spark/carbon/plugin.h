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

#ifndef PLUGIN_H
#define PLUGIN_H
/**
 * \file plugin.h
 * \brief Contains Plugin and PluginInstantiator classes.
 */

#include "abstractplugin.h"

#include <vector>
#include <QString>
#include <map>

/**
   \brief First Macro for new Plugin initialization. To be used in the class definition (header file).

   Allows registration of the Plugin with REGISTER_PLUGIN or REGISTER_EXPORT_PLUGIN Macro.
   Derives virtual functions of the base class, granting access to the class id and class name.

   Example:
   \code
   //DerivedPlugin.h
   #include "plugin.h"

   class DerivedPlugin : public Plugin
   {
       GUI_PLUGIN()
       ...
   };
   \endcode
*/
#define GUI_PLUGIN \
private: \
    static int mClassId; \
protected: \
    static bool mPluginInitialized; \
    static const QString mClassName; \
    static const QString mDefaultCaption; \
    static Plugin* getInstance(); \
public: \
    virtual int getClassId() const {return mClassId;}; \
    virtual const QString& getClassName() const {return mClassName;}; \
    virtual const QString& getDefaultCaption() const {return mDefaultCaption;}; \
    static void exportPlugin(PluginFactory* factory, int classId, const char* library = ""); \
    static void saveClassInfo(); 

/**
  \brief Second Macro for new in-application plugin initialization. To be used in the cpp file.

   \warning This macro registers a plugin that is defined in the main application itself! 
   To register a plugin that should be exported from a shared library, use REGISTER_EXPORT_PLUGIN instead.

   Registers the Plugin in the PluginFactory and receives a new class id.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Plugins of this type. They may be renamed upon initialization.\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "plugin" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.

   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Example:
   \code
   //DerivedPlugin.cpp
   #include "DerivedPlugin.h"

   REGISTER_PLUGIN(DerivedPlugin, "MyDerivedPlugin")
   \endcode

   \todo The plugin registration uses some kind of dirty trick. The registration is done when including the cpp file,
   by using the initialization of unused variable mPluginInitialized to call the registration function. As long as REGISTER_PLUGIN is called
   once and only once (and with a unique name for each registered frame) this works just fine and the usage is really simple, since
   only the own .h and .cpp files have to be edited.
   To be more fail safe it would be good to add a template initialization function automatically calling the registration for each
   class that is attempted to be instantiated by the factory (Abstract Factory Pattern with template). This could eliminate the need to use the
   second macro and would only register it if it is actually included somewhere.
*/
#define REGISTER_PLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_PLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) {LOG_ERROR() << "Cannot export Plugin " << mClassName << \
        ". To export a registered Plugin use REGISTER_EXPORT_PLUGIN macro instead of REGISTER_PLUGIN.";} \
    bool CLASS::mPluginInitialized =  \
        PluginFactory::getFactory().registerPluginInstantiator( CLASS::mClassId, new PluginInstantiator(CLASS::mClassId, NAME, CAPTION, CLASS::getInstance, AMOUNT)); 

/**
   \brief Plugin definition macro. 
   
   Code file counterpart to the GUI_PLUGIN macro. 
   Used by REGISTER_PLUGIN and REGISTER_EXPORT_PLUGIN.
*/
#define DEFINE_PLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    int CLASS::mClassId = PluginManager::getNextClassId(); \
    const QString CLASS::mClassName = NAME; \
    const QString CLASS::mDefaultCaption = CAPTION; \
    Plugin* CLASS::getInstance() { return (Plugin*)(new CLASS()); } 

/**
  \brief Second macro defining a plugin initialization function to export it from a shared library. To be used in the cpp file.

   \warning This macro registers plugins that should be exported from a shared library.
   For plugins that are defined in the main application itself, use REGISTER_PLUGIN instead.

   Registers an export function for the Plugin that registers the class in the PluginFactory and receives a new class id
   as soon as the libraries plugins are exported.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Plugins of this type. They may be renamed upon initialization.\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "plugin" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.

   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Plugins are exported from a shared library using export macros.
   A shared library can export any amount of plugins as long as they have different names.

   Example:
   \code
   //DerivedPlugin.cpp
   #include "DerivedPlugin.h"
   #include "Export.h"

   REGISTER_EXPORT_PLUGIN(DerivedPlugin, "MyDerivedPlugin")

   //Export.h
   PLUGINS_EXPORT_BEGIN()
   PLUGIN_EXPORT(DerivedPlugin)
   PLUGINS_EXPORT_END()
   \endcode

*/
#define REGISTER_EXPORT_PLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_PLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) { \
    if (factory) \
    { \
        CLASS::mClassId = classId; \
        LOG_DEBUG() << "Exporting Plugin (" << #CLASS << ", " << NAME << ", " << CAPTION << ", " << AMOUNT << ", " << library << ")"; \
        factory->registerPluginInstantiator( classId, new PluginInstantiator(classId, NAME, CAPTION, CLASS::getInstance, AMOUNT, library)); \
        CLASS::saveClassInfo(); \
    }}

/*!
 \brief Base Plugin class for simple Plugins.

 <b>General:</b>\n
 Simple plugins dont have a visual representation and are not able to emit or catch Qt Signals.
 They are useful to create simple executable program parts that can be executed in another thread.

 <b>Execution:</b>\n
 Plugins can be executed by calling their run() method. This is done is done by another plugin or in the run() method of a PluginThread.

 Plugins that are called by other plugins are called immediately in the callers thread. Plugins that run indefinitely will cause the calling thread to freeze.
 Plugins and SignalPlugins can be started in PluginThreads of a Simulation. An endless loop in a plugin started in another thread naturally wont freeze the other threads.
 
 If your derived plugin is not meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_LOOP).
 If your derived plugin is only meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_ONCE).
 Otherwise the PluginManager will allow any execution of the Plugin (ET_ANY).\n

 <b>Expected execution behaviour:</b>\n
 When SignalPlugins are executed by another thread, the virtual functions run(), stop(), pause() and restart() are called.
 In this case, the plugins are expected to behave in the following way:\n
 When run() is called, the Plugin is expected to start its execution. If the execution does not possess a loop or cant be started, the plugin may return immediately.\n
 When stop() is called, the Plugin is expected to return from its run() routine as soon as possible.\n
 When pause(true) is called, the PluginThread is expected to halt until pause(false) or stop() is called. If the plugin CANT be stopped at the moment, it has to
 return false. Returning true or emitting the paused() signal will notify the SimulationManager that the plugin is paused. It is recommended to emit paused()
 started() and finished(). Although not needed by the Gui itself when returning true in case of a pause() command, other plugins may rely on the information.\n
 When restart() is called, the Plugin is expected to reset its state if possible and run again from the beginning or continue running respectively.

 \sa AbstractPlugin, SignalPlugin, PluginThread
*/
class Plugin : public AbstractPlugin
{

public:
    // public functions

    /*!
     \brief Basic destructor.
    */
    virtual ~Plugin();

    /*!
     \brief Virtual function to signal a plugin to load data from a QSettings save file.

     \param data source file
    */
    virtual void loadData(QSettings& data);
    /*!
     \brief Virtual function to signal a plugin to save its data to a QSettings save file.
    */
    virtual void saveData(QSettings& data);

    /*!
     \brief Returns the plugin class id (for new derived class).

     Derived virtual function is created automatically with the GUI_PLUGIN, GUI_SIGNALPLUGIN or GUI_FRAME, macro.
    */
    virtual int getClassId() const = 0;

protected:
    // protected functions

    /*!
     \brief Constructor for derived class.
    */
    Plugin();
};

/*!
 \brief Instantiator for simple plugins.

 The plugin registration macros create an instantiator that is called by the PluginFactory.
*/
class PluginInstantiator : public AbstractPluginInstantiator
{
public:
    //public functions

    /*!
     \brief Basic constructor.

     \param id id of the class
     \param name name of the class
     \param caption default caption of the class
     \param (instantiateFunc)() instantiation function
     \param maxInstanceCount maximum amount of instances
     \param library name of the library the plugin class was loaded from (empty for built-in plugins)
    */
    PluginInstantiator(int id, const QString& name, const QString& caption, Plugin* (*instantiateFunc)(void), 
        int maxInstanceCount, const char* library = "");
    /*!
     \brief Basic Destructor.
    */
    virtual ~PluginInstantiator(); 

    /*!
     \brief Instantiates a Plugin derived class.

     \param caption alternative caption for the plugin
    */
    Plugin* instantiate(const QString& caption) const;
    /*!
     \brief Instantiates a Plugin derived class.
    */
    Plugin* instantiate() const;

protected:
    //protected members

    Plugin* (*mInstantiateFunc)(void); /*!< Instantiation function pointer. */
};

#endif //PLUGIN_H
