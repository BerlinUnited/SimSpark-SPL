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

#ifndef SIGNALPLUGIN_H
#define SIGNALPLUGIN_H
/**
 * \file signalplugin.h
 * \brief Contains SignalPlugin and SignalPlugininstantiator classes.
 */

#include "abstractplugin.h"

#include <vector>
#include <QString>
#include <QObject>
#include <QSettings>
#include <map>

/**
   \brief First Macro for new SignalPlugin initialization. To be used in the class definition (header file).

   Allows registration of the SignalPlugin with REGISTER_SIGNALPLUGIN or REGISTER_EXPORT_SIGNALPLUGIN Macro.
   Derives virtual functions of the base class, granting access to the class id and class name.

   Example:
   \code
   //DerivedSignalPlugin.h
   #include "signalplugin.h"

   class DerivedSignalPlugin : public SignalPlugin
   {
       GUI_SIGNALPLUGIN()
       ...
   };
   \endcode
*/
#define GUI_SIGNALPLUGIN \
private: \
    static int mClassId; \
protected: \
    static bool mPluginInitialized; \
    static const QString mClassName; \
    static const QString mDefaultCaption; \
    static SignalPlugin* getInstance(); \
public: \
    virtual int getClassId() const {return mClassId;}; \
    virtual const QString& getClassName() const {return mClassName;}; \
    virtual const QString& getDefaultCaption() const {return mDefaultCaption;}; \
    static void exportPlugin(PluginFactory* factory, int classId, const char* library = ""); \
    static void saveClassInfo(); 

/**
  \brief Second Macro for new in-application signal plugin initialization. To be used in the cpp file.

   \warning This macro registers a signal plugin that is defined in the main application itself! 
   To register a plugin that should be exported from a shared library, use REGISTER_EXPORT_SIGNALPLUGIN instead.

   Registers the SignalPlugin in the PluginFactory and receives a new class id.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Plugins of this type. They may be renamed upon initialization.\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "signalplugin" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.

   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Example:
   \code
   //DerivedSignalPlugin.cpp
   #include "DerivedSignalPlugin.h"

   REGISTER_SIGNALPLUGIN(DerivedSignalPlugin, "MyDerivedSignalPlugin")
   \endcode

   \todo The plugin registration uses some kind of dirty trick. The registration is done when including the cpp file,
   by using the initialization of unused variable mPluginInitialized to call the registration function. As long as REGISTER_SIGNALPLUGIN is called
   once and only once (and with a unique name for each registered frame) this works just fine and the usage is really simple, since
   only the own .h and .cpp files have to be edited.
   To be more fail safe it would be good to add a template initialization function automatically calling the registration for each
   class that is attempted to be instantiated by the factory (Abstract Factory Pattern with template). This could eliminate the need to use the
   second macro and would only register it if it is actually included somewhere.
*/
#define REGISTER_SIGNALPLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_SIGNALPLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) {LOG_ERROR() << "Cannot export Plugin " << mClassName << \
        ". To export a registered SignalPlugin use REGISTER_EXPORT_SIGNALPLUGIN macro instead of REGISTER_SIGNALPLUGIN.";} \
    bool CLASS::mPluginInitialized =  \
        PluginFactory::getFactory().registerPluginInstantiator( CLASS::mClassId, new SignalPluginInstantiator(CLASS::mClassId, NAME, CAPTION, CLASS::getInstance, AMOUNT)); 

/**
   \brief SignalPlugin definition macro. 
   
   Code file counterpart to the GUI_SIGNALPLUGIN macro. 
   Used by REGISTER_SIGNALPLUGIN and REGISTER_EXPORT_SIGNALPLUGIN.
*/
#define DEFINE_SIGNALPLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    int CLASS::mClassId = PluginManager::getNextClassId(); \
    const QString CLASS::mClassName = NAME; \
    const QString CLASS::mDefaultCaption = CAPTION; \
    SignalPlugin* CLASS::getInstance() { return (SignalPlugin*)(new CLASS()); } 

/**
  \brief Second macro defining an attachable frame plugin initialization function to export it from a shared library. To be used in the cpp file.

   \warning This macro registers attachable frame plugins that should be exported from a shared library.
   For plugins that are defined in the main application itself, use REGISTER_SIGNALPLUGIN instead.

   Registers an export function for the Plugin that registers the class in the PluginFactory and receives a new class id
   as soon as the libraries plugins are exported.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Plugins of this type. They may be renamed upon initialization.\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "signalplugin" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.

   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Example:
   \code
   //DerivedSignalPlugin.cpp
   #include "DerivedSignalPlugin.h"

   REGISTER_EXPORT_SIGNALPLUGIN(DerivedSignalPlugin, "MyDerivedSignalPlugin")

   //Export.h
   PLUGINS_EXPORT_BEGIN()
   PLUGIN_EXPORT(DerivedPlugin)
   PLUGINS_EXPORT_END()
   \endcode
*/
#define REGISTER_EXPORT_SIGNALPLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_SIGNALPLUGIN(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) { \
    if (factory) \
    { \
        CLASS::mClassId = classId; \
        LOG_DEBUG() << "Exporting SignalPlugin (" << #CLASS << ", " << NAME << ", " << CAPTION << ", " << AMOUNT << ", " << library << ")"; \
        factory->registerPluginInstantiator( classId, new SignalPluginInstantiator(classId, NAME, CAPTION, CLASS::getInstance, AMOUNT, library)); \
        CLASS::saveClassInfo(); \
    }}

/*!
 \brief Base Plugin class for signaling Plugins.

 <b>General:</b>\n
 SignalPlugins dont have a visual representation but they are able to emit and receive Qt Signals.
 They are useful to add gui Elements that dont need an own visual representation but need to communicate with other
 Gui elements or want to attach additional main gui elements.

 For example, they can be used to:\n
 Include new Tools or Menu Entries\n
 Provide complex function servers communicating over signals and slots\n
 Provide configurable services\n

 <b>Signals and Slots:</b>\n
 SignalPlugins can receive and send signals, so they can communicate with other Widgets using the CommunicationManager.
 Like any other plugin they can provide a SettingsWidget for the SettingsDialog of the gui core.\n

 <b>Execution:</b>\n
 SignalPlugins can be executed by calling their run() method. This is done is done by another plugin or in the run() method of a PluginThread.

 SignalPlugins that are called by other plugins are called immediately in the callers thread. SignalPlugins that run indefinitely will cause
 the calling thread to freeze.
 SignalPlugins and SignalPlugins can be started in PluginThreads of a Simulation. An endless loop in a plugin started in another thread naturally
 wont freeze the other threads.
 
 If your derived SignalPlugin is not meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_LOOP).
 If your derived SignalPlugin is only meant to be called by the main thread or other plugins directly, set the execution type with setExecutionType(ET_ONCE).
 Otherwise the PluginManager will allow any execution of the SignalPlugin (ET_ANY).\n

 <b>Expected execution behaviour:</b>\n
 When SignalPlugins are executed by another thread, the virtual functions run(), stop(), pause() and restart() are called.
 In this case, the plugins are expected to behave in the following way:\n
 When run() is called, the Plugin is expected to start its execution. If the execution does not possess a loop or cant be started, the plugin may return immediately.\n
 When stop() is called, the Plugin is expected to return from its run() routine as soon as possible.\n
 When pause(true) is called, the PluginThread is expected to halt until pause(false) or stop() is called. If the plugin CANT be stopped at the moment, it has to
 return false. Returning true or emitting the paused() signal will notify the SimulationManager that the plugin is paused. It is recommended to emit paused()
 started() and finished(). Although not needed by the Gui itself when returning true in case of a pause() command, other plugins may rely on the information.\n
 When restart() is called, the Plugin is expected to reset its state if possible and run again from the beginning or continue running respectively.

 <b>Communication with executable Plugins:</b>\n
 The receiving and emitting of signals has to be done with care if the plugin might run in a different thread.
 If the plugin is executed in another thread (as possible if the execution type is ET_ANY), the SignalPlugin
 has to connect to plugins in the main thread with queued connections (wich is controlled by a flag when offering or requesting
 Communication with the CommunicationManager) and it may never directly invoke a function that changes the gui layout in the main thread.\n

 \sa AbstractPlugin, Plugin, AttachableFrame
*/
class SignalPlugin : public QObject, public AbstractPlugin
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic destructor.
    */
    virtual ~SignalPlugin();

    /*!
     \brief Virtual function to signal a plugin to load data from a QSettings save file.

     \param data source file
    */
    virtual void loadData(const QSettings& data);
    /*!
     \brief Virtual function to signal a plugin to save its data to a QSettings save file.

     \param data target file
    */
    virtual void saveData(QSettings& data) const;

    /*!
     \brief Returns the plugin class id (for new derived class).

     Derived virtual function is created automatically with the GUI_PLUGIN, GUI_SIGNALPLUGIN or GUI_FRAME, macro.
    */
    virtual int getClassId() const = 0;

protected:
    // protected functions

    /*!
     \brief Constructor for derived plugins.
    */
    SignalPlugin();

private:
    // private functions

public: signals:

    /*!
     \brief Should be emitted whenever pause(true) was called and the plugin was paused.

     It is encouraged to emit this signal in a derived plugin when pause(true) was called.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.
    */
    void paused();
    /*!
     \brief Should be emitted whenever run() reaches its end.

     It is encouraged to emit this signal in a derived plugin when the run() method is finishing.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.
    */
    void finished();
    /*!
    /*!
     \brief Should be emitted whenever run() was called and the plugin just started the execution of the run routine.

     It is encouraged to emit this signal in a derived plugin when run() was called.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.
    */
    void running();

public slots:

protected:
    // protected members

private:
    // private members
};

/*!
 \brief Instantiator for simple plugins.

 The plugin registration macros create an instantiator that is called by the PluginFactory.
*/
class SignalPluginInstantiator : public AbstractPluginInstantiator
{
public:
    //public functions

    /*!
     \brief Basic constructor.

     \param id id of the class
     \param name name of the class
     \param caption default caption for instances of the class
     \param (instantiateFunc)() instantiation function
     \param maxInstanceCount maximum amount of instances
     \param library name of the library the plugin class was loaded from (empty for built-in plugins)
    */
    SignalPluginInstantiator(int id, const QString& name, const QString& caption, SignalPlugin* (*instantiateFunc)(void), 
        int maxInstanceCount, const char* library = "");
    /*!
     \brief Basic Destructor.
    */
    virtual ~SignalPluginInstantiator(); 

    /*!
     \brief Instantiates a SignalPlugin derived class.

     \param caption alternative caption of the plugin
    */
    SignalPlugin* instantiate(const QString& caption) const;
    /*!
     \brief Instantiates a SignalPlugin derived class with default caption.
    */
    SignalPlugin* instantiate() const;

protected:
    //protected members

    SignalPlugin* (*mInstantiateFunc)(void); /*!< Instantiation function pointer. */
};

#endif //PLUGIN_H
