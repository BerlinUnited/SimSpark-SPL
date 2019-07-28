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

#ifndef OPENGLMANAGER_H
#define OPENGLMANAGER_H
/**
 * \file openglmanager.h
 * \brief Contains OpenGLManager class.
 */

#include <QObject>
#include <QList>
#include <QMutex>

class GLWidget;
class Carbon;
class GLHandle;

/*!
 \brief Manages offering and accessing of GLWidgets by different threads in the application.

 <b>General:</b>\n
 The OpenGLManager is a managing entity to coordinate creation, access and destruction of OpenGL rendering
 contexts during program execution. Usually an OpenGL Widget (GLWidget derived from QGLWidget) is meant to
 be integrated into the Gui and therefore has to be executed in the main thread. Even rendering has to
 be done in the main thread.
 An OpenGL rendering context may only be made current in one thread at a time and it has to be made current
 by the using thread before calling any OpenGL functions.
 As there might be different plugins running in different SimulationTasks that want to access the same OpenGL
 rendering context, they need to coordinate the access to the context without knowing each other. In this
 case they can use the OpenGLManager to offer the context or request access to it.

 <b>Registration:</b>\n
 The OpenGLManager manages GLWidgets, which either create an own QGLContext, or share it beween multiple QGLWidgets.
 After creation, a GLWidget can be registered to the OpenGLManager with a set of EGLWidgetFlags, using registerGLWidget().
 The flags tell a requesting thread, what kind of GLWidget is offered. The actual properties are consensous.
 The offerer has to make sure all requirements are met.
 Registration flags can be changed by calling registerGLWidget() again.
 Before the GLWidget is destroyed, is has to be unregistered with unregisterGLWidget().

 The OpenGLManager only controls the GLWidgets, not their QGLContext! If there are more GLWidgets who share the same
 QGLContext, you must only register one of them, so the manager wont grant access to the same context via different GLWidgets.

 <b>Access:</b>\n
 An offered GLWidget can be accessed and released by calling lockGLWidget() and unlockGLWidget() from any thread.
 Locking will stop rendering of the widget (if active), prevent update functions like resize or repaint, send a
 "GlStateSignal", and make the context current in the calling thread.
 The GlState signal is a signal carrying an EGLStateSignal, which tells receivers (usually the owner of the GLWidget),
 how the caller wishes to access the GLWidget. Again, these definitions are consensous.
 The receiver can interpret them in any way, but should always make sure to catch and handle the signal.
 To prevent deadlocks, the receiver must never call a function from another thread (which might have called
 the signal and is blocked waiting for the receiver function to return).\n

 <b>Why should i use this?:</b>\n
 Why offering a GLWidget and accessing it via the manager instead of simply sharing it between the program
 entities who need them?\n
 For two reasons:\n
 1) Thread safety: You cant be sure all plugins run in the same thread. They might (they usually run in the main thread), but they
 could actually be executed in a SimulationTask, e.g. a PluginThread, thus running in a different thread.
 Thread safety in OpenGL access can be tricky. You wont recognize where an error happened if the context is made
 current in the wrong thread. The OpenGL-ErrorState wont tell where or why the error happened.\n
 2) Extensibility: When offering GLWidgets or using offered GLWidgets, you separate the two implementations from one another, so 
 either side of the deal can always be exchanged with an alternative, a more general or a more specialized implementation.\n

 <b>Todo:</b>\n
 \todo There was no need yet to extend the OpenGLManager further. If there should be the need to use more rendering
 threads that have to communicate on a very frequent basis (e.g. in every frame) there should be some extensions:\n
 1) timed locking/unlocking\n
 2) access queues and/or priorities\n
 3) more options on callbacks/slots to notify about locks\n
 4) Asynchronous signals (request/reply glstate signals) without having to block the calling thread\n
 5) handling of shared QGLContexts
*/
class OpenGLManager : public QObject
{
  Q_OBJECT

public:
    // public types

    /*!
     \brief Flags to identify a registered GLWidget by properties.

     When registering a GLWidget, the flags describe its properties consensously for possible users.
     Use getUniqueFlag() to generate a unique flag combination starting with GWF_FLAG_0 (8).
    */
    enum EGLWidgetFlags
    {
        GWF_UNDEFINED = 0, /*!< Unaccessible GLWidget. Should not be used. */
        GWF_NONE = 1,      /*!< Default empty GLWidget. Flag for occupied normal GLWidget. */
        GWF_NORMAL = 2,    /*!< Default empty GLWidget. Can be used for everything. */
        GWF_SERVER = 4,    /*!< SparkGLWidget. Can be used to start SimSpark server with. */
        GWF_FLAG_0 = 8,    /*!< Generic flag to identify widgets with. */
        GWF_FLAG_1 = 16,   /*!< Generic flag to identify widgets with. */
        GWF_FLAG_2 = 32,   /*!< Generic flag to identify widgets with. */
        GWF_FLAG_3 = 64,   /*!< Generic flag to identify widgets with. */
        GWF_FLAG_4 = 128,  /*!< Generic flag to identify widgets with. */
        GWF_FLAG_5 = 256,  /*!< Generic flag to identify widgets with. */
        GWF_FLAG_6 = 512,  /*!< Generic flag to identify widgets with. */
        GWF_FLAG_7 = 1024, /*!< Generic flag to identify widgets with. */
        GWF_FLAG_8 = 2048, /*!< Generic flag to identify widgets with. */
        GWF_FLAG_9 = 4096  /*!< Generic flag to identify widgets with. */
    };

    /*!
     \brief Parameter for glStateSignal(), specifies the kind of access to the rendering context.

     Used to specify what the caller intends to do with the rendering context when calling lock or unlock.
    */
    enum EGLStateSignal
    {
        GSS_NONE = 0,   /*!< Undefined access. */
        GSS_ACCESS,     /*!< Begin of short-time access. (e.g. loading textures managed in another thread) */
        GSS_END_ACCESS, /*!< End of short-time access. (e.g. end of loading textures) */
        GSS_OWNED,      /*!< Permanent access. (e.g. starting a render loop) */
        GSS_FREED       /*!< Permanent release. (e.g. quitting thread) */
    };

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param parent parent carbon instance
    */
    OpenGLManager(Carbon* parent);
    /*!
     \brief Destructor.
    */
    virtual ~OpenGLManager();

    /*!
     \brief Returns the flags for a registered GLWidget.

     \param widget pointer to widget
     \return flags of the widget, or 0 if the widget was not found
    */
    int getGLWidgetFlags(GLWidget* widget);
    /*!
     \brief Returns true if the specified widget is registered and has the specified flag.

     \param widget widget to check
     \param flag flag to check for
     \return true if widget is registered and has the specified flag
    */
    bool hasGLWidgetFlag(GLWidget* widget, EGLWidgetFlags flag);

    /*!
     \brief Returns true if the widget is registered.
    */
    bool hasWidget(GLWidget* widget);

    /*!
     \brief Register a widget with a set of flags.

     Existant registration is replaced.
     Registration might fail if the access to the widgets was locked.

     \param widget widget to register
     \param flags flags for the widget
     \return true if adding was successful
    */
    bool registerGLWidget(GLWidget* widget, int flags = GWF_NORMAL);
    /*!
     \brief Register a widget with a single flag.

     Existant registration is replaced.
     Registration might fail if the access to the widgets was locked.

     \param widget widget to register
     \param flag flag for the widget
     \return true if adding was successful
    */
    bool registerGLWidget(GLWidget* widget, EGLWidgetFlags flag = GWF_NORMAL);
    /*!
     \brief Unregister a widget.

     Deregistration might fail if the access to the widgets was locked.

     \param widget widget to unregister
     \return true if widget was registered and could be unregistered
    */
    bool unregisterGLWidget(GLWidget* widget);

    /*!
     \brief Returns a registered widget withe given EGLWidgetFlags.

     \param flags list of flags to look for
     \returns the first widget found with matching flags
    */
    GLWidget* getRegisteredWidget(int flags);
    /*!
     \brief Returns a registered widget withe given EGLWidgetFlags.

     \param flags list of flags to look for
     \param unique storage that is set to true if there is only one widget with matching flags, or false otherwise
     \returns the first widget found with matching flags
    */
    GLWidget* getRegisteredWidget(int flags, bool& unique);

    /*!
     \brief Send a glStateSignal() to receivers registered for any GLWidget.

     Usually does not need to be called manually, even when accessing the GLWidget from another thread.
     Use lockGLWidget() or unlockGLWidget() instead.

     The signal is sent with a QueuedConnection, thus it can be used cross threads but might not be handled
     synchronously. The waitReturn flags causes usage of a blocked queued signal, stopping the
     calling thread until the function returns. Make sure to ALWAYS return from a function receiving
     the signal and never calling a function in a possible caller thread to prevent deadlocks.

     \param widget widget to send the signal to
     \param signal signal to send
     \param waitReturn if true, the function call with block the thread until the GLWidget function returns
    */
    void sendGLStateSignal(GLWidget* widget, EGLStateSignal signal, bool waitReturn);
    /*!
     \brief Connect to a glStateSignal for a GLWidget.

     Connects both glStateSignal() (non-blocked queued connection) and glStateSignalBlocked (blocked connection).

     The signal is sent with a QueuedConnection, thus it can be used cross threads but might not be handled
     synchronously. The waitReturn flags causes usage of a blocked queued signal, stopping the
     calling thread until the function returns. Make sure to ALWAYS return from a function receiving
     the signal and never calling a function in a possible caller thread to prevent deadlocks.

     \param source widget to receive signals for
     \param target receiver of the signal
     \param member slot to call
     \param queued if true, messages are sent via queued connection
     \param blocked if true, messages are sent via blocked queued connection
    */
    void connectGLStateSignal(GLWidget* source, QObject* target, const char* member, bool queued, bool blocked);

    /*!
     \brief Access a GLWidgets rendering context and locks it down for other threads.

     1) Stops rendering of the GLWidget\n
     2) Prevents updating the GLWidget (resize and repaint)\n
     3) Sends a glStateSignal() for the widget. If If blockedLock is true, the calling thread is blocked until
     all receivers return\n
     4) Makes the context current in the calling thread\n

     \param widget widget to lock
     \param lockSignal signal to send to connected receivers before locking
     \param blockedLock if true, the caller waits for the receivers to return before actually locking
     \return true if locking succeeded
    */
    bool lockGLWidget(GLWidget* widget, EGLStateSignal lockSignal = GSS_NONE, bool blockedLock = true);
    /*!
     \brief Release a GLWidgets rendering context and make it available for other threads.

     1) Calls doneCurrent on the openGLContext in the calling thread\n
     2) Re-enables rendering and updating of the GLWidget\n
     3) Sends a glStateSignal() for the widget. If If blockedLock is true, the calling thread is blocked until
     all receivers return\n

     \param widget widget to unlock
     \param unlockSignal signal to send to connected receivers after unlocking
     \param blockedUnlock if true, the caller waits for the receivers to return before continuing
    */
    void unlockGLWidget(GLWidget* widget, EGLStateSignal unlockSignal = GSS_NONE, bool blockedUnlock = false);

public:
    //public static functions

    /*!
     \brief Returns a unique flag combination that has not been used yet.
    */
    static int getUniqueFlag();

    /*!
     \brief Creates an error message for an OpenGl error code.

     \param error OpenGL error code
    */
    static QString getErrorMessage(int error);
    /*!
     \brief Creates an error message for an OpenGl error code as std::string.

     \param error OpenGL error code
    */
    static std::string getErrorMessageStd(int error);

protected:
    //protected static functions

    /*!
     \brief Raises the next automatically generated flag to the currently used flag plus the minimum step for custom flags if neccessary.

     \param flag flag that was used
    */
    static void updateFlagUsed(int flag);

protected:
    //proceted functions

    /*!
     \brief Lock access mutex to the list of registered GLWidgets.
    */
    bool lockWidgets();
    /*!
     \brief Unlock access mutex to the list of registered GLWidgets.
    */
    void unlockWidgets();

public slots:	
    //public slots

    /*!
     \brief Unregisters a GLWidget when it was destroyed.

     Sends a warning. GLWidgets should be unregistered before destruction.

     \param widget
    */
    void glWidgetDestroyed(QObject* widget);
    
public:
    //public static members

    static int mNextFlag; /*!< Next unique flag. */

private:
    // private members

    Carbon* mParent;          /*!< Parent carbon instance. */
    QList<GLHandle> mWidgets; /*!< List of GLWidgets with their associated flags. */

    QMutex mWidgetMutex;      /*!< Mutex to coordinate access to the widget list. */
};

/*!
 \brief Handle object that is associated with one GlWidget.

 Forwards glStateSignal() and glStateSignalBlocked().
*/
class GLHandle : public QObject
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Complete constructor.

     \param widget widget to handle
     \param flags flags to describe the widget
    */
    GLHandle(GLWidget* widget, int flags) :
        mWidget(widget), mFlags(flags){}
    /*!
     \brief Copy constructor.
    */
    GLHandle(const GLHandle& obj) :
        mWidget(obj.mWidget), mFlags(mFlags){}
    /*!
     \brief Copy operator. Returns reference to the handle.
    */
    GLHandle& operator=(const GLHandle& obj)
        {mWidget = obj.mWidget; mFlags = mFlags; return *this;}

public: signals:

    /*!
     \brief Signal to inform about access to a registered GLWidget.

     Used for non-blocked queued connections.

     \param widget GLWidget associated with the signal
     \param signal signal
    */
    void glStateSignal(GLWidget* widget, OpenGLManager::EGLStateSignal signal);
    /*!
     \brief Signal to inform about access to a registered GLWidget.

     Used for blocked queued connections.

     \param widget GLWidget associated with the signal
     \param signal signal
    */
    void glStateSignalBlocked(GLWidget* widget, OpenGLManager::EGLStateSignal signal);

public slots:

    /*!
     \brief Causes emitting glStateSignal().

     Used by OpenGLManager.
     \param signal signal to emit
    */
    void emitSignal(OpenGLManager::EGLStateSignal signal);
    /*!
     \brief Causes emitting glStateSignalBlocked().

     Used by OpenGLManager.
     \param signal signal to emit
    */
    void emitSignalBlocked(OpenGLManager::EGLStateSignal signal);

public:
    GLWidget* mWidget; /*!< Handled widget. */
    int mFlags; /*!< Flags describing the widget. */
};

#endif //OPENGLMANAGER_H
