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

#ifndef SETTINGS_H
#define SETTINGS_H
/**
 * \file settings.h
 * \brief Contains Settings class, which extends QSettings by thread safety and comfortability functions.
 */

#include <QSettings>
#include <QSemaphore>
#include <QMutex>
#include <math.h>

class AbstractPlugin;
class QThread;

/*!
    \brief Settings class for the Gui and all its plugins.

    It uses basically the same functions like a normal QSettings object with the addition of a Plugin-Setting mode.\n
    By calling beginGroup(AbstractPlugin*) all following entries "Y" will be stored in a subgroup "X" making the complete
    setting path "X/Y", where X is the ClassName of the Plugin and Y the name of the setting.\n
    By calling beginInstanceGroup(AbstractPlugin*) all following entries "Z" will be stored in a subgroup "X/Y" making the complete
    setting path "X/Y/Z", where X is the ClassName of the Plugin, Y its Caption and Z the name of the setting.\n

    This system is meant to provide easy access to settings for plugins which allow multiple instances. When creating a new instance of
    a multi-instance-plugin, use instanceGroups insted of normal groups to load and store data for the actual plugin instance.
    When reading a value with Settings::instanceValue() the instance value is returned. If there is no such a value, the classes
    default value Settings::value() is returned.\n

    Like usual all beginGroup() and beginInstanceGroup() calls have to be ended with endGroup(), with the addional limitation, that there
    must not be any further groups below a Plugin-Setting group.

    Calling value() during an open instanceGroup or calling instanceValue() during an open pluginGroup works too, but is less efficient.
    When no plugin is set currently, setValue() and value() work just like QSettings functions.

    \todo This settings system currently does not handle renaming instances. SettingsWidget should contain a slot receiving a change of
    an instances caption and ask if current settings should be renamed (when there is only one instance), copied (when there are more
    instances), deleted (since settings might not be used anymore) or ignored (when they might be used again).
    and a setting value storing this decision would be good too.

*/
class Settings : public QSettings
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Basic constructor.

     A Settings object is created by the Carbon automatically. Use this object to access program settings.

     \param filename filename of the settings file
     \param parent parent object
     \param format defines where and how to save the setup (as .ini or native, e.g. registry)
     \param scope for which users in the system should the settings be used. system wide or user dependant
    */
    explicit Settings(const QString& filename, QObject *parent = 0, 
        QSettings::Format format = QSettings::IniFormat, QSettings::Scope scope = QSettings::SystemScope);

    /*!
     \brief Begins a instance setting group for a plugin instance. Ended with endGroup(true).

     All following settings will be stored to and loaded from "PluginClassName/PluginCaption/".
     For maximum efficiency, only use instanceValue() and setInstanceValue() after this call until endGroup()
     has been called.

     beginInstanceGroup() always has to be followed by endGroup() with the same "lock" definition.
     For the outmost group, use beginInstanceGroup(plugin, true) to lock the access to the settings object for other threads.
     The corresponding endGroup(true) call releases access again.

     \param plugin plugin to save instance settings for
     \param lock if true, the access to the settings object will be prevented for all but the locking threads until unlock has been called with the paired endGroup() call
     \param timeout time im ms to wait for access to the Settings object
     \returns true if the group could be opened; fails if there is already a plugin or instance group opened or the access could not be locked.
    */
    bool beginInstanceGroup(const AbstractPlugin* plugin, bool lock = false, int timeout = INT_MAX);
    /*!
     \brief Begins a setting group for a plugin class.

     All following settings will be stored to and loaded from "PluginClassName/".
     For maximum efficiency, only use value() and setValue() after this call until endGroup()
     has been called.

     beginGroup() always has to be followed by endGroup() with the same "lock" definition.
     For the outmost group, use beginGroup(plugin, true) to lock the access to the settings object for other threads.
     The corresponding endGroup(true) call releases access again.

     \param plugin plugin to save instance settings for
     \param lock if true, the access to the settings object will be prevented for all but the locking threads until unlock has been called with the paired endGroup() call
     \param timeout time im ms to wait for access to the Settings object
     \returns true if the group could be opened; fails if there is already a plugin or instance group opened or the access could not be locked.
    */
    bool beginGroup(const AbstractPlugin* plugin, bool lock = false, int timeout = INT_MAX);
    /*!
     \brief Begins a new subgroup in the settings object.

     Simply calls the QSettings beginGroup function, with the exception that no new groups can be opened after beginning a
     plugin class or instance settings group, until endGroup() was called.

     beginGroup() always has to be followed by endGroup() with the same "lock" definition.
     For the outmost group, use beginGroup("name", true) to lock the access to the settings object for other threads.
     The corresponding endGroup(true) call releases access again.

     \param name name of the group
     \param lock if true, the access to the settings object will be prevented for all but the locking threads until unlock has been called with the paired endGroup() call
     \param timeout time im ms to wait for access to the Settings object
     \returns true if the group could be opened; fails if there is already a plugin or instance group opened or the access could not be locked.
    */
    bool beginGroup(const QString& name, bool lock, int timeout = INT_MAX);
    /*!
     \brief Begins a new subgroup in the settings object.

     Simply calls the QSettings beginGroup function, with the exception that no new groups can be opened after beginning a
     plugin class or instance settings group, until endGroup() was called.

     beginGroup() always has to be followed by endGroup() with the same "lock" definition.
     For the outmost group, use beginGroup("name", true) to lock the access to the settings object for other threads.
     The corresponding endGroup(true) call releases access again.

     \param name name of the group
     \returns true if the group could be opened; fails if there is already a plugin or instance group opened or the access could not be locked.
    */
    bool beginGroup(const QString& name) {return beginGroup(name, false, INT_MAX);}
    /*!
     \brief End the current instanceGroup, pluginGroup or default settings group.

     Set the unlock flag to true if this endGroup closes a beginGroup or beginInstanceGroup with the lock flag true
     \param unlock if true, access to the settings object is allowed again. Only the locking thread can unlock the settings.
     \return true if unlocking was successful; may only fail if the caller thread is illegal
    */
    bool endGroup(bool unlock = false);

    /*!
     \brief Returns an instance value for a registered plugin.

     If there is no instance value for this plugin instance, a class setting value is searched and returned.

     Can only be used after calling beginInstanceGroup(AbstractPlugin*) or beginGroup(AbstractPlugin*).
     For maximum efficiency it should be called after beginning an instance group rather than a class goup.

     \param name name of the setting
     \param defaultValue default value to return when neither instance nor class setting could be found
    */
    QVariant instanceValue(const QString& name, const QVariant& defaultValue = QVariant());
    /*!
     \brief Returns a static class setting value for a registered plugin or a simple setting value if no plugin is registered.

     If there is currently a Plugin registered by beginInstanceGroup() or beginGroup(AbstractPlugin*), the classes value is
     returned.
     For maximum efficiency it should be called after beginning a class group rather than an instance goup.

     If no plugin is registered, the normal settings value of the QSettings object is returned.

     \param name name of the setting
     \param defaultValue default value to return when class setting or standard setting could not be found
    */
    QVariant value(const QString& name, const QVariant& defaultValue = QVariant());
    /*!
     \brief Set a value for a currently registered plugin instance.

     Can only be used after calling beginInstanceGroup(AbstractPlugin*) or beginGroup(AbstractPlugin*).
     For maximum efficiency it should be called after beginning an instance group rather than a class goup.

     \param name name of the setting
     \param value value to store
    */
    void setInstanceValue(const QString& name, const QVariant& value);
    /*!
     \brief Set a value for a currently registered plugin or a standard setting value.

     If there is currently a Plugin registered by beginInstanceGroup() or beginGroup(AbstractPlugin*), the setting is
     stored as a class value.
     For maximum efficiency it should be called after beginning an instance group rather than a class goup.

     If no plugin is registered, the normal QSettings::setValue() function is called.

     \param name name of the setting
     \param value value to store
    */
    void setValue(const QString& name, const QVariant& value);

protected:
    //protected members

    /*!
     \brief Tries to get exclusive access to the Settings object with the current thread.

     A lock call has to be ended by an unlock call from the same thread.
     \param timeout time in ms to wait for access
     \return true if access was successful, false in case of timeout
    */
    bool lockSettings(int timeout);
    /*!
     \brief Releases exclusive access to the Settings object from the current thread.

     The unlock call has to be done in the same thread than the lock call.
     \return true if unlocking was successful, false in case of illegal caller thread
    */
    bool unlockSettings();

    /*!
     \brief Returns true if the current thread is the one that locked access to the Settings object.

     Logs error message if the thread is different.
     Logs warning message if there is no lock active.
     \return true if locked and locker thread is current thread, or if not locked
    */
    bool assertLockerThread();

private:
    //private members

    bool mActivePlugin;         /*!< True if there is currently a plugin class group in use. */
    bool mActivePluginInstance; /*!< True if there is currently a plugin instance group in use. */

    AbstractPlugin* mPlugin;    /*!< Reference to currently registered plugin. */

    QThread* mLocker;           /*!< Pointer to thread that locked the Settings when opening a group. */
    QSemaphore mGroupSemaphore; /*!< Semaphore protecting agains multiple access. */
    QMutex mLockMutex;          /*!< Mutex to safely lock and unlock. */
};

#endif // SETTINGS_H
