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

#include "settings.h"
#include "abstractplugin.h"
#include "carbon.h"

#include <QThread>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Settings::Settings(const QString& filename, QObject *parent, QSettings::Format format, QSettings::Scope scope) :
    QSettings(format, scope, "SimSpark", filename, parent),
    mGroupSemaphore(1)
{
    mPlugin = 0;
    mActivePluginInstance = false;
    mActivePlugin = false;
}

bool Settings::beginInstanceGroup(const AbstractPlugin* plugin, bool lock, int timeout)
{
    LOG_ASSERT(plugin);

    if (lock)
    {
        bool locked = lockSettings(timeout);
        if (!locked)
        {
            LOG_ERROR() << "Failed to lock Settings object.";
            return false;
        }
    }

    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to open plugin instance group in Settings object. Illegal caller thread.";
        return false;
    }

    if (mActivePluginInstance || mActivePlugin)
    {
        LOG_ERROR() << "There is already an active plugin setting group.";
        return false;
    }

    mActivePluginInstance = true;
    QSettings::beginGroup(plugin->getClassName());
    QSettings::beginGroup(plugin->getCaption());

    mPlugin = const_cast<AbstractPlugin*>(plugin);
    return true;
}

bool Settings::beginGroup(const AbstractPlugin* plugin, bool lock, int timeout)
{
    LOG_ASSERT(plugin);

    if (lock)
    {
        bool locked = lockSettings(timeout);
        if (!locked)
        {
            LOG_ERROR() << "Failed to lock Settings object.";
            return false;
        }
    }

    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to open plugin group in Settings object. Illegal caller thread.";
        return false;
    }

    if (mActivePluginInstance || mActivePlugin)
    {
        LOG_ERROR() << "There is already an active plugin setting group.";
        return false;
    }

    mActivePlugin = true;
    QSettings::beginGroup(plugin->getClassName());

    mPlugin = const_cast<AbstractPlugin*>(plugin);
    return true;
}

bool Settings::beginGroup(const QString& name, bool lock, int timeout)
{
    if (lock)
    {
        bool locked = lockSettings(timeout);
        if (!locked)
        {
            LOG_ERROR() << "Failed to lock Settings object.";
            return false;
        }
    }

    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to open group in Settings object. Illegal caller thread.";
        return false;
    }

    if (mActivePluginInstance || mActivePlugin)
    {
        LOG_ERROR() << "There is already an active plugin setting group.";
        return false;
    }

    QSettings::beginGroup(name);
    return true;
}

bool Settings::endGroup(bool unlock)
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to end group in Settings object. Illegal caller thread.";
        return false;
    }

    if (mActivePluginInstance)
    {
        QSettings::endGroup();
        QSettings::endGroup();
        //LOG_DEBUG() << "Settings: End instance group. ";
        mActivePluginInstance = false;
        mPlugin = 0;
    }
    else if (mActivePlugin)
    {
        QSettings::endGroup();
        //LOG_DEBUG() << "Settings: End plugin group. ";
        mActivePlugin = false;
        mPlugin = 0;
    }
    else
    {
        //LOG_DEBUG() << "Settings: End group. ";
        QSettings::endGroup();
    }

    if (unlock)
    {
        bool unlocked = unlockSettings();
        if (!unlocked)
        {
            LOG_ERROR() << "Could not unlock Settings object.";
            return false;
        }
    }

    return true;
}

QVariant Settings::instanceValue(const QString& name, const QVariant& defaultValue)
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to read instance value from Settings object. Illegal caller thread.";
        return QVariant();
    }

    if (mActivePluginInstance)
    {
        //Instance value in actual group
        if (contains(name))
            return QSettings::value(name, defaultValue);
        else
        {
            //Search parent group
            QSettings::endGroup();
            QVariant ret = QSettings::value(name, defaultValue);
            QSettings::beginGroup(mPlugin->getCaption());
            return ret;
        }
    }
    else if (mActivePlugin)
    {
        //Search subgroup (instance caption)
        QSettings::beginGroup(mPlugin->getCaption());
        bool hasKey = contains(name);

        if (!hasKey)
        {
            QVariant ret = QSettings::value(name, defaultValue);
            QSettings::endGroup();
            return ret;
        }
        //Search this group (class name)
        else
        {
            QSettings::endGroup();
            return QSettings::value(name, defaultValue);
        }
    }

    //Nothing found
    LOG_WARNING() << "Cant return instance value without registered plugin.";
    return QVariant();
}

QVariant Settings::value(const QString& name, const QVariant& defaultValue)
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to read value from Settings object. Illegal caller thread.";
        return QVariant();
    }

    if (mActivePluginInstance)
    {
        QSettings::endGroup();
        QVariant ret = QSettings::value(name, defaultValue);
        QSettings::beginGroup(mPlugin->getCaption());
        return ret;
    }

    return QSettings::value(name, defaultValue);
}

void Settings::setInstanceValue(const QString& name, const QVariant& value)
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to write instance value to Settings object. Illegal caller thread.";
        return;
    }

    if (mActivePluginInstance)
    {
        QSettings::setValue(name, value);
    }
    else if (mActivePlugin)
    {
        QSettings::beginGroup(mPlugin->getCaption());
        QSettings::setValue(name, value);
        QSettings::endGroup();
    }
    else
    {
        LOG_ERROR() << "Cant store instance setting value without registered plugin.";
    }
}

void Settings::setValue(const QString& name, const QVariant& value)
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to set value to Settings object. Illegal caller thread.";
        return;
    }

    if (mActivePluginInstance)
    {
        QSettings::endGroup();
        QSettings::setValue(name, value);
        QSettings::beginGroup(mPlugin->getCaption());
    }
    else if (mActivePlugin)
    {
        QSettings::setValue(name, value);
    }
    else
        QSettings::setValue(name, value);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool Settings::lockSettings(int timeout)
{
    QMutexLocker locker(&mLockMutex);
    if (mGroupSemaphore.tryAcquire(1, timeout))
    {
        mLocker = QThread::currentThread();
        return true;
    }

    return false;
}

bool Settings::unlockSettings()
{
    if (!assertLockerThread())
    {
        LOG_ERROR() << "Failed to unlock Settings object.";
        return false;
    }

    QMutexLocker locker(&mLockMutex);
    mLocker = 0;
    mGroupSemaphore.release();

    return true;
}

bool Settings::assertLockerThread()
{
    if (mLocker == 0)
    {
        //No locker thread
        LOG_WARNING() << "Access to Settings-object acquired without locking it. " \
            "Use the lock-flag for the first beginGroup() or beginInstanceGroup() call to prevent multiple access from different threads and loss of data.";
        return true;
    }

    if (QThread::currentThread() != mLocker)
    {
        LOG_ERROR() << "Cant access Settings object while it is locked in another thread. "\
             "Use the lock-flag for the first beginGroup() or beginInstanceGroup() call to prevent multiple access from different threads and loss of data.";
        return false;
    }

    return true;
}
