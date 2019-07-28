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

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
/**
 * \file settingswidget.h
 * \brief Contains SettingsWidget class, which contains setting control Gui elements.
 */

#include "carbon.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QWidget>

class Settings;
class AbstractPlugin;

/*!
 \brief Base Class for Widgets that contain setting control Gui elements.

 SettingWidgets are provided to the SettingsDialog and are displayed there under a
 topic name and page name.
 SettingWidgets can be designed and implemented like any other gui element but it is
 strongly encouraged to implement the virtual functions applyChanges(), undoChanges(),
 updateSettings() and the signal settingChanged() as well as the slot updateSetting().
 This way all widgets are guaranteed to work the same way. They allow changing and
 reverting setting changes freely and applying them using the SettingDialogs apply button.

 The SettingsWidget communicate with the creating plugin/frame using a Settings object.
 If the SettingsWidget only contains Settings that should be kept over several program starts
 (should be the usual case) the creator can use the global Settings object for this purpose.
*/
class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    // public functions
    /*!
     \brief Basic constructor.

     \param settings Settings object over which to communicate.
     \param parent parent widget
    */
    SettingsWidget(boost::shared_ptr<Settings> settings, QWidget* parent = 0);
    /*!
     \brief Basic destructor.
    */
    virtual ~SettingsWidget();

    /*!
     \brief Apply all changes in the window to the Settings object.
    */
    virtual void applyChanges();
    /*!
     \brief Undo all changes in the window to the Settings object.
    */
    virtual void undoChanges();
    /*!
     \brief Update all setting displays from the Settings object.
    */
    virtual void updateSettings();

public: signals:
    // public signals

    /*!
     \brief Notification signal that a setting in the settings object has changed.

     \param name name of the setting that changed
    */
    void settingChanged(QString name);

    /*!
     \brief Notification signal that an instance setting in the settings object has changed.

     Instance settings are two-step settings that try to load a value for the actual instance and falling back to a static value if there is no instance value.
     \param name name of the setting that changed
    */
    void instanceSettingChanged(QString name);

public slots:
    // public slots

    /*!
     \brief Slot to receive update signal for one specific setting in the Settings object.

     \param name name of the setting to update
    */
    virtual void updateSetting(QString name);

protected:
    // protected members

    boost::shared_ptr<Settings> mSettings; /*!< Pointer to Setting object. */
};

#endif //SETTINGSWIDGET_H
