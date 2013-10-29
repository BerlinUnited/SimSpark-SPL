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

#include "settingswidget.h"
#include "abstractplugin.h"

SettingsWidget::SettingsWidget(boost::shared_ptr<Settings> settings, QWidget* parent)
    : QWidget(parent),
      mSettings(settings)
{
}

SettingsWidget::~SettingsWidget()
{

}

void SettingsWidget::applyChanges()
{
    LOG_WARNING() << "ApplyChanges called on base class. This function only has effect if a derived SettingsWidget reimplements it.";
}

void SettingsWidget::undoChanges()
{
    LOG_WARNING() << "UndoChanges called on base class. This function only has effect if a derived SettingsWidget reimplements it.";
}

void SettingsWidget::updateSettings()
{
    LOG_WARNING() << "UpdateSettings called on base class. This function only has effect if a derived SettingsWidget reimplements it.";
}


void SettingsWidget::updateSetting(QString name)
{
    LOG_WARNING() << "UpdateSetting(QString) called on base class. This function only has effect if a derived SettingsWidget reimplements it.";
}

