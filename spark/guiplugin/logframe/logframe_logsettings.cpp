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

#include "logframe_logsettings.h"
#include "logframe.h"

#include <abstractplugin.h>
#include <settings.h>

namespace LogFrameUtil
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

LogSettings::LogSettings(LogFrame* frame, boost::shared_ptr<Settings> settings, QWidget* parent)
    : SettingsWidget(settings, parent)
{
    ui.setupUi(this);

    mCreator = frame;
}

LogSettings::~LogSettings()
{

}

//--------------------------------------------------------------
// Protected virtual
//--------------------------------------------------------------

void LogSettings::applyChanges()
{
    if (mSettings->beginInstanceGroup(mCreator, true)) //lock
    {
        mSettings->setInstanceValue("mMaximumMessages", ui.messageAmountSpinBox->value());
        mSettings->setValue("mMaximumMessages", ui.messageAmountSpinBox->value());

        mSettings->endGroup(true); //unlock

        emit settingChanged("mMaximumMessages");
        emit instanceSettingChanged("mMaximumMessages");
    }
}

void LogSettings::undoChanges()
{
    //Only simple settings, read out from settings file again
    updateSettings();
}

void LogSettings::updateSettings()
{
    if (mSettings->beginInstanceGroup(mCreator, true)) //lock
    {
        int val = mSettings->instanceValue("mMaximumMessages", 1000).toInt();
        mSettings->endGroup(true); //unlock

        ui.messageAmountSpinBox->setValue(val);
    }
    else
        LOG_ERROR() << "Failed to access Settings object.";
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void LogSettings::updateSetting(QString name)
{
    //Only needed if single updateSetting calls are invoked
    LOG_INFO() << "Not implemented!";
}

} //namespace LogFrameUtil