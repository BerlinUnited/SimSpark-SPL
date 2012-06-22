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

#include "guisettings_simulationsettings.h"
#include "settings.h"

namespace GuiSettings
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

SimulationSettings::SimulationSettings(boost::shared_ptr<Settings> settings, QWidget* parent) :
    SettingsWidget(settings, parent)
{
    ui.setupUi(this);
    
    //No clue why, but the palette of this frame has gray background instead of white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);
}

SimulationSettings::~SimulationSettings()
{

}

void SimulationSettings::applyChanges()
{
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mExecutablePaths", mExecutablePaths);
        mSettings->setValue("mExecutableFiles", mExecutableFiles);
        mSettings->setValue("mScriptPaths", mScriptPaths);
        mSettings->setValue("mScriptFiles", mScriptFiles);
        mSettings->setValue("mSetupPaths", mSetupPaths);
        mSettings->setValue("mSetupFiles", mSetupFiles);

        mSettings->endGroup(true); //unlock

        emit settingChanged("mExecutablePaths");
        emit settingChanged("mExecutableFiles");
        emit settingChanged("mScriptPaths");
        emit settingChanged("mScriptFiles");
        emit settingChanged("mSetupPaths");
        emit settingChanged("mSetupFiles");
    }
    else
        LOG_ERROR() << "Could not access Settings object to save changes in simulation settings to.";
}

void SimulationSettings::undoChanges()
{
    //Only simple settings, read out from settings file again
    updateSettings();
}

void SimulationSettings::updateSettings()
{
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mExecutablePaths = mSettings->value("mExecutablePaths", mExecutablePaths).toStringList();
        mExecutableFiles = mSettings->value("mExecutableFiles", mExecutableFiles).toStringList();
        mScriptPaths = mSettings->value("mScriptPaths", mScriptPaths).toStringList();
        mScriptFiles = mSettings->value("mScriptFiles", mScriptFiles).toStringList();
        mSetupPaths = mSettings->value("mSetupPaths", mSetupPaths).toStringList();
        mSetupFiles = mSettings->value("mSetupFiles", mSetupFiles).toStringList();

        ui.executablesPathsEdit->setPlainText(formatText(mExecutablePaths));
        ui.executablesFilesEdit->setPlainText(formatText(mExecutableFiles));
        ui.scriptsPathsEdit->setPlainText(formatText(mScriptPaths));
        ui.scriptsFilesEdit->setPlainText(formatText(mScriptFiles));
        ui.setupsPathsEdit->setPlainText(formatText(mSetupPaths));
        ui.setupsFilesEdit->setPlainText(formatText(mSetupFiles));

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to update changes in simulation settings from.";
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

QString SimulationSettings::formatText(const QStringList& input) const
{
    QString out("");
    for (QStringList::ConstIterator it = input.begin(); it != input.end(); it++)
    {
        out += *it + "\n";
    }

    return out;
}

QStringList SimulationSettings::deformatText(const QString& input) const
{
    QStringList out;

    int start = 0;
    int end = input.indexOf('\n');
    if (end == -1)
        end = input.size();
    QString next;
    while (start < input.size())
    {
        next = input.mid(start, end-start);
        if (next.compare("") != 0)
            out += next;
        start = end+1;
        end = input.indexOf('\n', start);
        if (end == -1)
        end = input.size();
    }

    return out;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationSettings::updateSetting(QString name)
{
    if      (name.compare("mExecutablePaths") == 0) ui.executablesPathsEdit->setPlainText(formatText(mExecutablePaths));
    else if (name.compare("mExecutableFiles") == 0) ui.executablesFilesEdit->setPlainText(formatText(mExecutableFiles));
    else if (name.compare("mScriptPaths") == 0) ui.setupsPathsEdit->setPlainText(formatText(mScriptPaths));
    else if (name.compare("mScriptFiles") == 0) ui.setupsFilesEdit->setPlainText(formatText(mScriptFiles));
    else if (name.compare("mSetupPaths") == 0) ui.setupsPathsEdit->setPlainText(formatText(mSetupPaths));
    else if (name.compare("mSetupFiles") == 0) ui.setupsFilesEdit->setPlainText(formatText(mSetupFiles));
}

void SimulationSettings::editExecutablePaths()
{
    mExecutablePaths = deformatText(ui.executablesPathsEdit->toPlainText().replace('\\', "/"));
}

void SimulationSettings::editExecutableFiles()
{
    mExecutableFiles = deformatText(ui.executablesFilesEdit->toPlainText().replace('\\', "/"));
}

void SimulationSettings::editSetupPaths()
{
    mSetupPaths = deformatText(ui.setupsPathsEdit->toPlainText().replace('\\', "/"));
}

void SimulationSettings::editSetupFiles()
{
    mSetupFiles = deformatText(ui.setupsFilesEdit->toPlainText().replace('\\', "/"));
}

void SimulationSettings::editScriptPaths()
{
    mScriptPaths = deformatText(ui.scriptsPathsEdit->toPlainText().replace('\\', "/"));
}

void SimulationSettings::editScriptFiles()
{
    mScriptFiles = deformatText(ui.scriptsFilesEdit->toPlainText().replace('\\', "/"));
}

} //namespace