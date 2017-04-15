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

#ifndef LOGFRAME_LOGSETTINGS_H
#define LOGFRAME_LOGSETTINGS_H
/**
 * \file logframe_logsettings.h
 * \brief Contains LogFrameUtil::LogSettings class, a SettingsWidget for LogFrame.
 */

#include <settingswidget.h>

#include "ui_logframe_logsettings.h"

class LogFrame;

namespace LogFrameUtil
{

/*!
 \brief SettingsWidget for LogFrame.

 Contains settings like maximum amount of messages to keep or color and font
 settings for the LogFrame.

 \todo Application of font and color changes in the log frame not implemented yet.
*/
class LogSettings : public SettingsWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param frame creator
     \param settings settings object to communicate over
     \param parent parent widget
    */
    LogSettings(LogFrame* frame, boost::shared_ptr<Settings> settings, QWidget* parent = 0);
    /*!
     \brief Basic destructor.
    */
    virtual ~LogSettings();

protected:
    // protected virtual functions

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

public slots:
    /*!
     \brief Slot to receive update signal for one specific setting in the Settings object.

     \param name name of the setting to update
    */
    virtual void updateSetting(QString name);

private:
    // private members

    Ui::LogSettings ui;   /*!< UI definition object. */
    int mMaximumMessages; /*!< Maximum amount of messages stored. */
    LogFrame* mCreator;   /*!< Parent frame. */
};

} //namespace LogFrameUtil

#endif //LOGFRAME_SETTINGS_H
