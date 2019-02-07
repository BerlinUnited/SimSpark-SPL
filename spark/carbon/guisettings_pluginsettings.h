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

#ifndef GUISETTINGS_PLUGINSETTINGS_H
#define GUISETTINGS_PLUGINSETTINGS_H
/**
 * \file guisettings_pluginsettings.h
 * \brief Contains PluginSettings class, a SettingsWidget for the PluginManager.
 */

#include "settingswidget.h"
#include "ui_guisettings_pluginsettings.h"

namespace GuiSettings
{

/*!
    \brief SettingsWidget for the PluginManagers general settings.

    Contains widgets for plugin include paths and files.
    Include directories define absolute paths to look in for files of the respective type.
    Include files define absolute paths for files.
    
    All libraries that are included as single files or contained in one of the included library directories are loaded at the start of the program.
*/
class PluginSettings : public SettingsWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
        \brief Basic constructor.

        \param settings settings object to communicate over
        \param parent parent widget
    */
    PluginSettings(boost::shared_ptr<Settings> settings, QWidget* parent = 0);
    /*!
        \brief Basic destructor.
    */
    virtual ~PluginSettings();

protected:
    // protected functions

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

private:
    //private functions

    /*!
        \brief Formats the include path QStringList from the settings object and converts it to a single enter-separated string.

        \param input string list from settings
    */
    QString formatText(const QStringList& input) const;
    /*!
        \brief Creates a string list from a single enter separated string.

        \param input string from ui QPlainTextEdit
    */
    QStringList deformatText(const QString& input) const;

public slots:

    /*!
        \brief Slot to receive update signal for one specific setting in the Settings object.

        \param name name of the setting to update
    */
    virtual void updateSetting(QString name);

    /*!
        \brief Changes the Plugin include directories from the ui object.
    */
    void editPluginPaths();
    /*!
        \brief Changes the Plugin include files from the ui object.
    */
    void editPluginFiles();

private:
    // private members

    Ui::PluginSettings ui; /*!< UI definition object. */

    QStringList mPluginPaths; /*!< Plugin include directories. */
    QStringList mPluginFiles; /*!< Plugin include files. */
};

} //namespace GuiSettings

#endif //GUISETTINGS_PLUGINSETTINGS_H
