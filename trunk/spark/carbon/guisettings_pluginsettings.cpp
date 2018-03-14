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

#include "guisettings_pluginsettings.h"
#include "settings.h"

namespace GuiSettings
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

PluginSettings::PluginSettings(boost::shared_ptr<Settings> settings, QWidget* parent) :
    SettingsWidget(settings, parent)
{
    ui.setupUi(this);
    
    //No clue why, but the palette of this frame has gray background instead of white
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);
}

PluginSettings::~PluginSettings()
{

}

void PluginSettings::applyChanges()
{
    if (mSettings->beginGroup("gui_PluginManager", true)) //lock
    {
        mSettings->setValue("mPluginPaths", mPluginPaths);
        mSettings->setValue("mPluginFiles", mPluginFiles);

        mSettings->endGroup(true); //unlock

        emit settingChanged("mPluginPaths");
        emit settingChanged("mPluginFiles");
    }
    else
        LOG_ERROR() << "Could not access Settings object to save changes in simulation settings to.";
}

void PluginSettings::undoChanges()
{
    //Only simple settings, read out from settings file again
    updateSettings();
}

void PluginSettings::updateSettings()
{
    if (mSettings->beginGroup("gui_PluginManager", true)) //lock
    {
        mPluginPaths = mSettings->value("mPluginPaths", mPluginPaths).toStringList();
        mPluginFiles = mSettings->value("mPluginFiles", mPluginFiles).toStringList();

        ui.pluginsPathsEdit->setPlainText(formatText(mPluginPaths));
        ui.pluginsFilesEdit->setPlainText(formatText(mPluginFiles));

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to update changes in plugin settings from.";
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

QString PluginSettings::formatText(const QStringList& input) const
{
    QString out("");
    for (QStringList::ConstIterator it = input.begin(); it != input.end(); it++)
    {
        out += *it + "\n";
    }

    return out;
}

QStringList PluginSettings::deformatText(const QString& input) const
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

void PluginSettings::updateSetting(QString name)
{
    if      (name.compare("mPluginPaths") == 0) ui.pluginsPathsEdit->setPlainText(formatText(mPluginPaths));
    else if (name.compare("mPluginFiles") == 0) ui.pluginsFilesEdit->setPlainText(formatText(mPluginFiles));
}

void PluginSettings::editPluginPaths()
{
    mPluginPaths = deformatText(ui.pluginsPathsEdit->toPlainText().replace('\\', "/"));
}

void PluginSettings::editPluginFiles()
{
    mPluginPaths = deformatText(ui.pluginsFilesEdit->toPlainText().replace('\\', "/"));
}

} //namespace