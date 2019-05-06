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

#ifndef ATTACHABLEFRAME_H
#define ATTACHABLEFRAME_H
/**
 * \file attachableframe.h
 * \brief Contains AttachableFrame and AttachableFrameInstantiator classes as well as registration Macros.
 */

#include <QFrame>
#include <QString>
#include <QSettings>
#include <map>

#include "menumanager.h"
#include "abstractplugin.h"

/**
   \brief First Macro for new AttachableFrame initialization. To be used in the class definition (header file).

   Allows registration of the Frame with REGISTER_FRAME or REGISTER_EXPORT_FRAME Macro.
   Derives virtual functions of the base class, granting access to the frame type, class id and class name.

   Example:
   \code
   //DerivedFrame.h
   #include "attachableframe.h"

   class DerivedFrame : public AttachableFrame
   {
       GUI_FRAME()
       ...
   };
   \endcode
*/
#define GUI_FRAME \
private: \
    static int mClassId; \
protected: \
    static bool mPluginInitialized; \
    static const QString mClassName; \
    static const QString mDefaultCaption; \
    static AttachableFrame* getInstance(); \
public: \
    virtual int getClassId() const {return mClassId;}; \
    virtual const QString& getClassName() const {return mClassName;}; \
    virtual const QString& getDefaultCaption() const {return mCaption;}; \
    static void exportPlugin(PluginFactory* factory, int classId, const char* library = ""); \
    static void saveClassInfo(); 

/**
  \brief Second Macro for new in-application attachable frame plugin initialization. To be used in the cpp file.

   \warning This macro registers an attachable frame plugin that is defined in the main application itself! 
   To register a plugin that should be exported from a shared library, use REGISTER_EXPORT_FRAME instead.

   Registers the AttachableFrame in the PluginFactory and receives a new class id.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Frames of this type. They may be renamed upon initialization
           (depending on the PluginDefinition file).\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "frame" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.
   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Example:
   \code
   //DerivedFrame.cpp
   #include "DerivedFrame.h"

   REGISTER_FRAME(DerivedFrame, "myderivedframe", "MyDerived", 1)
   \endcode

   \todo The plugin registration uses some kind of dirty trick. The registration is done when including the cpp file,
   by using the initialization of unused variable mPluginInitialized to call the registration function. As long as REGISTER_FRAME is called
   once and only once (and with a unique name for each registered frame) this works just fine and the usage is really simple, since
   only the own .h and .cpp files have to be edited.
   To be more fail safe it would be good to add a template initialization function automatically calling the registration for each
   class that is attempted to be instantiated by the factory (Abstract Factory Pattern with template). This could eliminate the need to use the
   second macro and would only register it if it is actually included somewhere.
*/
#define REGISTER_FRAME(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_FRAME(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) {LOG_ERROR() << "Cannot export AttachableFrame " << mClassName << \
        ". To export a registered AttachableFrame Plugin use REGISTER_EXPORT_FRAME macro instead of REGISTER_FRAME.";} \
    bool CLASS::mPluginInitialized =  \
        PluginFactory::getFactory().registerPluginInstantiator( CLASS::mClassId, new AttachableFrameInstantiator(CLASS::mClassId, NAME, CAPTION, CLASS::getInstance, AMOUNT)); 

/**
   \brief AttachableFrame definition macro. 
   
   Code file counterpart to the GUI_FRAME macro. 
   Used by REGISTER_FRAME and REGISTER_EXPORT_FRAME.
*/
#define DEFINE_FRAME(CLASS, NAME, CAPTION, AMOUNT) \
    int CLASS::mClassId = PluginManager::getNextClassId(); \
    const QString CLASS::mClassName = NAME; \
    const QString CLASS::mDefaultCaption = CAPTION; \
    AttachableFrame* CLASS::getInstance() { return (AttachableFrame*)(new CLASS()); } 

/**
  \brief Second macro defining a signal plugin initialization function to export it from a shared library. To be used in the cpp file.

   \warning This macro registers signal plugins that should be exported from a shared library.
   For plugins that are defined in the main application itself, use REGISTER_PLUGIN instead.

   Registers an export function for the Plugin that registers the class in the PluginFactory and receives a new class id
   as soon as the libraries plugins are exported.\n
   CLASS: Name of the Class.\n
   NAME: Default Name of all Frames of this type. They may be renamed upon initialization
           (depending on the PluginDefinition file).\n
   CAPTION: Default caption of the Class. Can be changed by the user for every instance.\n
   AMOUNT: Maximum amount of instances for this plugin. Default is 1.\n

   It is strongly encouraged to always choose a class name in lower case ending with "frame" so there wont be any namespace issues
   with other plugins and the user is always able to identify the class name easily.
   When choosing an amount of maximum instances bigger than 1, be sure to alsways handle multiple instantiation.
   Providing Settings or menu objects might have to be handled in two categories, static and non-static entries.
   To check which instance is currently executed, use getInstanceIndex().

   Example:
   \code
   //DerivedFrame.cpp
   #include "DerivedFrame.h"

   REGISTER_EXPORT_FRAME(DerivedFrame, "myderivedframe", "MyDerived", 1)

   //Export.h
   PLUGINS_EXPORT_BEGIN()
   PLUGIN_EXPORT(DerivedPlugin)
   PLUGINS_EXPORT_END()
   \endcode
*/
#define REGISTER_EXPORT_FRAME(CLASS, NAME, CAPTION, AMOUNT) \
    DEFINE_FRAME(CLASS, NAME, CAPTION, AMOUNT) \
    void CLASS::exportPlugin(PluginFactory* factory, int classId, const char* library) { \
    if (factory) \
    { \
        CLASS::mClassId = classId; \
        LOG_DEBUG() << "Exporting AttachableFrame Plugin (" << #CLASS << ", " << NAME << ", " << CAPTION << ", " << AMOUNT << ", " << library << ")"; \
        factory->registerPluginInstantiator( classId, new AttachableFrameInstantiator(classId, NAME, CAPTION, CLASS::getInstance, AMOUNT, library)); \
        CLASS::saveClassInfo(); \
    }}

/*!
 \brief Base Plugin class for Frame-Widgets that can be attached to Attachmentpoints.

 <b>General:</b>\n
 AttachableFrames work like any other plugin, except for being able to utilize a visual representation.
 They can create Menus, MenuActions, Toolboxes and ToolboxActions and provide them not only globally, but locally too,
 thus providing the GUI with control options for the frame.
 They may also provide Settings Widgets for the SettingsDialog or communicate with other plugins. 
 Derived Frames may be of one of the in EFrameType defined types.\n

 <b>Attaching AttachableFrames:</b>\n
 The attachment points for AttachableFrames are specified in the SimulationSetup loaded by the SimulationManager.\n

 <b>Communication:</b>\n
 AttachableFrames are usually connected to the MainWindow managed by the main thread.
 Therefore all their functions should be executed in the main thread if they can cause changes in any gui property managed by the main thread.
 If you wish to offer functions callable by another thread you usually have to send these commands to the thread in which the plugin is executed
 (the main thread).
 The CommunicationManager offers tools for SignalPlugins and AttachableFrames to communicate with each other even if running in different threads.\n

 <b>Execution:</b>\n
 /warning Although any plugin can be specified to be able to be executed in another Thread (by setting its execution type),
 AttachableFrame plugins should usually never have the need of being executed manually since they can receive direct user interaction.
 Their execution type is ET_NEVER, which means they cant be run in another thread and cant be executed in another thread by default.
 If however you wish to enable execution of the virtual run(), pause(), stop() and reset() functions, you can enable it using setExecutionType().
 In this case make sure not to call any functions that emit non-queued signals and never call functions or emit signals which result in the
 (non-main-)thread changing any Gui-Object properties.
 When another thread except for the main thread changes the gui layout this will most likely result in a crash (or sometimes in a deadlock).

 \sa AbstractPlugin, Plugin, SignalPlugin, SettingsDialog
*/
class AttachableFrame : public QFrame, public AbstractPlugin
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic Destructor

     Destructor notifies MenuManager automatically that the frame was destroyed.
    */
    virtual ~AttachableFrame();

    /*!
     \brief Default implementation for init function. Does nothing.
    */
    virtual void init(const QStringList& parameters = QStringList());

    /*!
     \brief Returns the Plugin frame as QWidget.
    */
    QWidget* getVisualRepresentation() const;
    /*!
     \brief Returns the Plugin frame as QObject.
    */
    QObject* getQObject() const;

    /*!
     \brief Virtual function to signal a frame to load data from a QSettings save file.

     \param data source file
    */
    virtual void loadData(const QSettings& data);
    /*!
     \brief Virtual function to signal a frame to save its data to a QSettings save file.

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
     \brief Derived class constructor.
    */
    AttachableFrame(QString caption);
    /*!
     \brief Derived class constructor.
    */
    AttachableFrame();

    /*!
     \brief Loads the actual defined design properties.

     Calling this function
     within the cunstructor of every derived frame asserts unified color and border look.
    */
    void loadFrameDesign();

    /*!
     \brief Derived virtual run function. Implements warning message as default behaviour.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, run() will never be called.

     \sa AbstractPlugin::run
    */
    virtual bool run();
    /*!
     \brief Derived virtual stop function. Implements warning message as default behaviour.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, stop() will never be called.

     \sa AbstractPlugin::stop
    */
    virtual bool stop();
    /*!
     \brief Derived virtual pause function. Implements warning message as default behaviour.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, pause() will never be called.

     \sa AbstractPlugin::pause
    */
    virtual bool pause(bool state);
    /*!
     \brief Derived virtual reset function. Implements warning message as default behaviour.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, restart() will never be called.

     \sa AbstractPlugin::restart
    */
    virtual bool restart();

public: signals:
    //public signals

    /*!
     \brief Should be emitted whenever pause(true) was called and the plugin was paused.

     It is encouraged to emit this signal in a derived plugin when pause(true) was called.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, paused() never has to be emitted.
    */
    void paused();
    /*!
     \brief Should be emitted whenever run() reaches its end.

     It is encouraged to emit this signal in a derived plugin when the run() method is finishing.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, finished() never has to be emitted.
    */
    void finished();
    /*!
     \brief Should be emitted whenever run() was called and the plugin just started the execution of the run routine.

     It is encouraged to emit this signal in a derived plugin when run() was called.
     Although the Gui core does not rely on derived plugins emitting this signal, other plugins might do so.

     AttachableFrames cant be executed unless the derived plugin changes the execution type with setExecutionType() to any other than ET_NEVER.
     If a derived plugin is not executable, running() never has to be emitted.
    */
    void running();

public slots:
    //slots to call from menu objects

    /*!
     \brief Sends the given text as Debug Message.

     The message is sent to the logger, annotated, formatted and forwarded to all registered MessageReceivers.
     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.
     \param text text to log
    */
    virtual void logDebug(QString text);
    /*!
     \brief Sends the given text as Info Message.

     The message is sent to the logger, annotated, formatted and forwarded to all registered MessageReceivers.
     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.
     \param text text to log
    */
    virtual void logInfo(QString text);
    /*!
     \brief Sends the given text as Warning Message.

     The message is sent to the logger, annotated, formatted and forwarded to all registered MessageReceivers.
     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.
     \param text text to log
    */
    virtual void logWarning(QString text);
    /*!
     \brief Sends the given text as Error Message.

     The message is sent to the logger, annotated, formatted and forwarded to all registered MessageReceivers.
     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.
     \param text text to log
    */
    virtual void logError(QString text);
    /*!
     \brief Sends the given text as Fatal Message, aborting the application.

     The message is sent to the logger, annotated, formatted and forwarded to all registered MessageReceivers.
     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.

     Fatal messages should only be used in a case of an unrecoverable error. The application will be terminated.
     \param text text to log
    */
    virtual void logFatal(QString text);

    /*!
     \brief Sends the given message via TCP/IP a port.

     This fuction is meant to be used in an Tooldefinition-xml-File which a Frame can provide the MenuManager with.

     \param port port to send to
     \param message message to send
    */
    virtual void sendMessage(int port, QString message);

private:
    // private members

    QString mFrameCaption; /*!< Caption of this frame. */
};

/*!
 \brief Instantiator for AttachableFrame plugins.

 The plugin registration macros create an instantiator that is called by the PluginFactory.
*/
class AttachableFrameInstantiator : public AbstractPluginInstantiator
{
public:
    //public functions

    /*!
     \brief Basic constructor.

     \param id id of the class
     \param name name of the class
     \param caption default caption for plugins of the class
     \param (instantiateFunc)() instantiation function
     \param maxInstanceCount maximum amount of instances
     \param library name of the library the plugin class was loaded from (empty for built-in plugins)
    */
    AttachableFrameInstantiator(int id, const QString& name, const QString& caption, AttachableFrame* (*instantiateFunc)(void), 
        int maxInstanceCount, const char* library = "");
    /*!
     \brief Basic Destructor.
    */
    virtual ~AttachableFrameInstantiator();

    /*!
     \brief Instantiates an AttachableFrame derived class.

     \param caption alternative caption for the frame
    */
    AttachableFrame* instantiate(const QString& caption) const;
    /*!
     \brief Instantiates an AttachableFrame derived class.
    */
    AttachableFrame* instantiate() const;

protected:
    //protected members

    AttachableFrame* (*mInstantiateFunc)(void); /*!< Instantiation function pointer. */
};

#endif // ATTACHABLEFRAME_H
