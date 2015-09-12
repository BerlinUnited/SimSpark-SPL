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

#ifndef GUISETTINGS_SIMULATIONSETTINGS_H
#define GUISETTINGS_SIMULATIONSETTINGS_H
/**
 * \file guisettings_simulationsettings.h
 * \brief Contains SimulationSettings class, a SettingsWidget for the SimulationManager.
 */

#include "settingswidget.h"
#include "ui_guisettings_simulationsettings.h"

namespace GuiSettings
{

/*!
    \brief SettingsWidget for the SimulationManagers general settings.

    Contains widgets for include paths for SimulationSetup, executable and script files.
    Include directories define absolute paths to look in for files of the respective type.
    Include files define absolute paths for files.
    Files contained either directly or as include path can be addressed by name only in the SimulationManager.
*/
class SimulationSettings : public SettingsWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
        \brief Basic constructor.

        \param settings settings object to communicate over
        \param parent parent widget
    */
    SimulationSettings(boost::shared_ptr<Settings> settings, QWidget* parent = 0);
    /*!
        \brief Basic destructor.
    */
    virtual ~SimulationSettings();

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
        \brief Changes the executable include directories from the ui object.
    */
    void editExecutablePaths();
    /*!
        \brief Changes the executable include files from the ui object.
    */
    void editExecutableFiles();
    /*!
        \brief Changes the script include directories from the ui object.
    */
    void editScriptPaths();
    /*!
        \brief Changes the script include files from the ui object.
    */
    void editScriptFiles();
    /*!
        \brief Changes the SimulationSetup include directories from the ui object.
    */
    void editSetupPaths();
    /*!
        \brief Changes the SimulationSetup include files from the ui object.
    */
    void editSetupFiles();

private:
    // private members

    Ui::SimulationSettings ui; /*!< UI definition object. */

    QStringList mExecutablePaths; /*!< Executable include directories. */
    QStringList mExecutableFiles; /*!< Executable include files. */
    QStringList mScriptPaths; /*!< Script include directories. */
    QStringList mScriptFiles; /*!< Script include files. */
    QStringList mSetupPaths;  /*!< SimulationSetup include directories. */
    QStringList mSetupFiles;  /*!< SimulationSetup include files. */
};

} //namespace GuiSettings

#endif //GUISETTINGS_SIMULATIONSETTINGS_H
