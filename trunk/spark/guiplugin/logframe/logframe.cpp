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

#include "logframe.h"
#include "logframe_logsettings.h"
#include "export.h"

#include <settingsdialog.h>
#include <pluginmanager.h>

#include <QLabel>
#include <iostream>
#include <QThread>

using namespace LogFrameUtil;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(LogFrame, "logframe", "Log", 1000)

//Static class information
void LogFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION, 
        tr("The log frame collects all log messages sent in the application, formats them as specified in a tool bar and displays " \
           "them in a message window.\n" \
           "It can format and sort incoming messages based on their source and additional meta information. "));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":script");

    //Tags
    QStringList tags;
    tags << "log" << "message" << "console";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

LogSettings* LogFrame::mSettingsWidget = 0;
bool LogFrame::mFirstConsole = true;

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

LogFrame::LogFrame() :
      mSettings(getCarbon()->getSettings()),
      IMessageReceiver(true) //IMessageReceiver with queued message receiving, since our virtual receiveMessage() function must only be called by the owning thread
{
    //------------------------
    //Layout & Design
    ui.setupUi(this);
    loadFrameDesign();

    //------------------------
    //Non-Changing contents

    //Log Source combo box content
    mMaxMessageSources = 0;
    initializeMessageSources();
}

LogFrame::~LogFrame()
{
    cleanup();
}

void LogFrame::init(const QStringList& parameters)
{
    //------------------------
    //Settings

    //Default settings
    loadDefaultSettings();

    //Load settings from file (if existant)
    loadSettings();

    //------------------------
    //Add static settings window to settingsdialog. Settingsdialog handles pointer deletion.
    
    SettingsDialog* dialog = getCarbon()->getSettingsDialog();

    QString topic = tr("Log Frame");
    QString page = tr("General");

    if(mSettingsWidget == 0)
    {
        bool occupied = dialog->hasPage(topic, page);

        if (occupied)
            LOG_ERROR() << "Topic " << topic << " in Settings dialog already has a page " << page << " in use.";
        else
        {
            //Add new Settings widget to settings dialog
            mSettingsWidget = new LogSettings(this, mSettings);
            dialog->includeSettingWidget(topic, page, (SettingsWidget**)(&mSettingsWidget), this, ":document");
            //Settingsdialog manages the pointer. mSettingsWidget-pointer will be set to 0 when mSettingsWidget is deleted
        }
    }
    else
    {
        //Tell settings dialog this object is using the settings widget too
        dialog->registerToSettingWidget(topic, page, this);
    }

    if (mSettingsWidget != 0)
    {
        connect(mSettingsWidget, SIGNAL(instanceSettingChanged(QString)), this, SLOT(updateSetting(QString)));
        connect(mSettingsWidget, SIGNAL(instanceSettingChanged(QString)), this, SLOT(updateInstanceSetting(QString)));
    }
    
    //------------------------
    //Start Message Receiver, we want to receive MessageObjects to store and reformat them when needed
    setMessageObjectLogLevel(Logger::Debug);
    setMessageObjectFormat("%t{HH:mm:ss.zzz}: %m"); //See abstractstringappender.h or AbstractStringAppender documentation
    startReceivingMessageObjects();

    connect(getCarbon(), SIGNAL(logSourceAdded(int)), this, SLOT(updateLogSourceAdded()));
    connect(getCarbon(), SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));

    //------------------------
    //If this is the first logger, display warning error messages that occurred before loading of the window
    if (mFirstConsole)
        getCarbon()->sendInitMessages(Logger::Warning, false);
    
    mFirstConsole = false;
}

void LogFrame::receiveMessageObject(TLogMessagePtr message)
{
    //This message visible?
    if (showingMessage(message))
    {
        //Set color depending on loglevel
        setPrintColor(message->mLogLevel);

        //Insert it formatted
        ui.logTextEdit->append(formatMessage(message));
    }

    //Store it unformatted
    storeMessage(message);
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void LogFrame::initializeMessageSources()
{
    QString selected;
    bool reinit = mMaxMessageSources != 0;
    if (reinit)
    {
        //Reinitialization
        selected = ui.outputComboBox->currentText();
    }

    mMaxMessageSources = Logger::getLogSourceCount();
    ui.outputComboBox->clear();
    ui.outputComboBox->addItem("All");
    for (unsigned int i = 0; i < (unsigned int)mMaxMessageSources; i++)
    {
        ui.outputComboBox->addItem(QString(Logger::getLogSourceName(i).c_str()));
    }
    
    if (reinit)
    {
        //Choose previous if possible
        int index= ui.outputComboBox->findText(selected);
        ui.outputComboBox->setCurrentIndex(index == -1 ? 0 : index);
    }
    else
    {
        ui.outputComboBox->setCurrentIndex(0);
    }

    mShowAllSources = true;
}

void LogFrame::storeMessage(TLogMessagePtr message)
{
    //Store messages in order of their timestamp.
    //Since most of the messages only have to be rearranged a few places, bubblesort is effective.
    //(using a map or even multimap would be slower)

    if (mMessages.empty())
        mMessages.push_back(TLogMessagePtr(message));
    else
    {
        mMessages.push_back(TLogMessagePtr(message));
        TMessages::reverse_iterator el_it = mMessages.rbegin();
        TMessages::reverse_iterator prev_it = el_it;
        prev_it++;
        while (prev_it != mMessages.rend() &&
               ((*prev_it)->mTimeStamp > (*el_it)->mTimeStamp))
        {
            //swap
            TLogMessagePtr store(*el_it);
            *el_it = TLogMessagePtr(*prev_it);
            *prev_it = TLogMessagePtr(store);

            el_it++;
            prev_it++;
        }
        
        popMessages();
        updateMessageAmount();
    }
}

void LogFrame::setPrintColor(Logger::LogLevel level)
{
    switch(level)
    {
    case Logger::Debug:
        ui.logTextEdit->setTextColor(Qt::blue);
        break;
    case Logger::Info:
        ui.logTextEdit->setTextColor(Qt::black);
        break;
    case Logger::Warning:
        ui.logTextEdit->setTextColor(Qt::darkYellow);
        break;
    case Logger::Error:
        ui.logTextEdit->setTextColor(Qt::red);
        break;
    case Logger::Fatal:
        ui.logTextEdit->setTextColor(Qt::darkRed);
        break;
    }
}

void LogFrame::reformatMessages()
{
    //Clear all
    ui.logTextEdit->clear();

    //Add messages again with new format
    for(TMessages::iterator it = mMessages.begin(); it != mMessages.end(); it++)
    {
        if (showingMessage(*it))
        {
            setPrintColor((*it)->mLogLevel);
            ui.logTextEdit->append(formatMessage(*it));
        }
    }
}

bool LogFrame::showingMessage(TLogMessagePtr& message)
{
    bool showingLevel = false;
    switch(message->mLogLevel)
    {
    case Logger::Debug:     showingLevel = mShowDebug; break;
    case Logger::Info:      showingLevel = mShowInfo; break;
    case Logger::Warning:   showingLevel = mShowWarning; break;
    case Logger::Error:     showingLevel = mShowError; break;
    case Logger::Fatal:     showingLevel = mShowFatal; break;
    default:
        showingLevel = true;
        break;
    }
    return showingLevel && (mShowAllSources || showingMessageSource(message));
}

bool LogFrame::showingMessageSource(TLogMessagePtr& message)
{
    for (auto it = mMessageSources.begin(); it != mMessageSources.end(); it++)
    {
        if (*it == message->mMessageSourceId)
            return true;
    }

    return false;
}

void LogFrame::loadSettings()
{
    if (mSettings->beginInstanceGroup(this, true)) //lock
    {
        mMaximumMessages = mSettings->instanceValue("mMaximumMessages", mMaximumMessages).toInt();
        mShowDebug       = mSettings->instanceValue("mShowDebug", mShowDebug).toBool();
        mShowInfo        = mSettings->instanceValue("mShowInfo", mShowInfo).toBool();
        mShowWarning     = mSettings->instanceValue("mShowWarning", mShowWarning).toBool();
        mShowError       = mSettings->instanceValue("mShowError", mShowError).toBool();
        mShowFatal       = mSettings->instanceValue("mShowFatal", mShowFatal).toBool();

        mShowTime        = mSettings->instanceValue("mShowTime", mShowTime).toBool();
        mShowSource      = mSettings->instanceValue("mShowSource", mShowSource).toBool();
        mShowLevel       = mSettings->instanceValue("mShowLevel", mShowLevel).toBool();
        mShowFile        = mSettings->instanceValue("mShowFile", mShowFile).toBool();
        mShowLine        = mSettings->instanceValue("mShowLine", mShowLine).toBool();
        mShowName        = mSettings->instanceValue("mShowName", mShowName).toBool();
        mShowMessage     = mSettings->instanceValue("mShowMessage", mShowMessage).toBool();

        mLastSource      = mSettings->instanceValue("mLastSource", mLastSource).toString();
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to load LogFrame settings from. Keeping previous values (default values).";

    updateMessageAmount();
    updateCheckButtons();
    generateMessageFormat();
    chooseMessageOutput(mLastSource);
}

void LogFrame::loadDefaultSettings()
{
    mMaximumMessages = 1000;
    mShowDebug       = ui.debugToolButton->isChecked();
    mShowInfo        = ui.infoToolButton->isChecked();
    mShowWarning     = ui.warningToolButton->isChecked();
    mShowError       = ui.errorToolButton->isChecked();
    mShowFatal       = true;

    mShowTime        = ui.timeToolButton->isChecked();
    mShowSource      = ui.sourceToolButton->isChecked();
    mShowLevel       = ui.levelToolButton->isChecked();
    mShowFile        = ui.fileToolButton->isChecked();
    mShowLine        = ui.lineToolButton->isChecked();
    mShowName        = ui.nameToolButton->isChecked();
    mShowMessage     = ui.messageToolButton->isChecked();

    mLastSource      = "All";

    updateMessageAmount();
    generateMessageFormat();
    chooseMessageOutput(mLastSource);
}

void LogFrame::saveSettings()
{
    if (mSettings->beginInstanceGroup(this, true)) //lock
    {
        mSettings->setValue("mMaximumMessages", mMaximumMessages);

        mSettings->setInstanceValue("mMaximumMessages", mMaximumMessages);
        mSettings->setInstanceValue("mShowDebug", mShowDebug);
        mSettings->setInstanceValue("mShowInfo", mShowInfo);
        mSettings->setInstanceValue("mShowWarning", mShowWarning);
        mSettings->setInstanceValue("mShowError", mShowError);
        mSettings->setInstanceValue("mShowFatal", mShowFatal);

        mSettings->setInstanceValue("mShowTime", mShowTime);
        mSettings->setInstanceValue("mShowSource", mShowSource);
        mSettings->setInstanceValue("mShowLevel", mShowLevel);
        mSettings->setInstanceValue("mShowFile", mShowFile);
        mSettings->setInstanceValue("mShowLine", mShowLine);
        mSettings->setInstanceValue("mShowName", mShowName);
        mSettings->setInstanceValue("mShowMessage", mShowMessage);

        mSettings->setInstanceValue("mLastSource", mLastSource);

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save LogFrame settings to.";
}

void LogFrame::popMessages()
{
    while (mMessages.size() > (unsigned int)mMaximumMessages)
        mMessages.pop_front();
}

void LogFrame::updateMessageAmount()
{
    ui.amountLabel->setText(QString("%1/%2").arg(mMessages.size()).arg(mMaximumMessages));
}

void LogFrame::updateCheckButtons()
{
    ui.debugToolButton->setChecked(mShowDebug);
    ui.infoToolButton->setChecked(mShowInfo);
    ui.warningToolButton->setChecked(mShowWarning);
    ui.errorToolButton->setChecked(mShowError);

    ui.timeToolButton->setChecked(mShowTime);
    ui.sourceToolButton->setChecked(mShowSource);
    ui.levelToolButton->setChecked(mShowLevel);
    ui.fileToolButton->setChecked(mShowFile);
    ui.lineToolButton->setChecked(mShowLine);
    ui.nameToolButton->setChecked(mShowName);
    ui.messageToolButton->setChecked(mShowMessage);

    generateMessageFormat();
    reformatMessages();
}

void LogFrame::chooseMessageOutput(const QString& name)
{
    int index = ui.outputComboBox->findText(name);

    if (index == ui.outputComboBox->currentIndex())
        return;

    if (index > 0)
        ui.outputComboBox->setCurrentIndex(index);
    else
        ui.outputComboBox->setCurrentIndex(0);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void LogFrame::updateInstanceSetting(QString name)
{
    //Contains only Settings that may be changed my the SettingsWidget
    if (mSettings->beginInstanceGroup(this, true)) //lock
    {
        if (name.compare("mMaximumMessages") == 0) 
        {
            mMaximumMessages = mSettings->instanceValue("mMaximumMessages", mMaximumMessages).toInt();
            popMessages();
            updateMessageAmount();
        }
        else
        {
            LOG_WARNING() << "Unknown Setting " << name << ".";
        }
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Failed to access Setting object.";
}

void LogFrame::updateSetting(QString name)
{
    //Contains only Settings that may be changed my the SettingsWidget
    if (mSettings->beginGroup(this, true)) //lock
    {
        if (name.compare("mMaximumMessages") == 0) 
        {
            mMaximumMessages = mSettings->value("mMaximumMessages", mMaximumMessages).toInt();
            popMessages();
            updateMessageAmount();
        }
        else
        {
            LOG_WARNING() << "Unknown Setting '" << name << "'.";
        }

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to update Setting " << name;
}

void LogFrame::debugToggled(bool state)
{
    if (state != mShowDebug)
    {
        mShowDebug = state;
        reformatMessages();
    }
}

void LogFrame::infoToggled(bool state)
{
    if (state != mShowInfo)
    {
        mShowInfo = state;
        reformatMessages();
    }
}

void LogFrame::warningToggled(bool state)
{
    if (state != mShowWarning)
    {
        mShowWarning = state;
        reformatMessages();
    }
}

void LogFrame::errorToggled(bool state)
{
    if (state != mShowError)
    {
        mShowError = state;
        reformatMessages();
    }
}

void  LogFrame::timeToggled(bool state)
{
    if (state != mShowTime)
    {
        mShowTime = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::levelToggled(bool state)
{
    if (state != mShowLevel)
    {
        mShowLevel = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::sourceToggled(bool state)
{
    if (state != mShowSource)
    {
        mShowSource = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::fileToggled(bool state)
{
    if (state != mShowFile)
    {
        mShowFile = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::lineToggled(bool state)
{
    if (state != mShowLine)
    {
        mShowLine = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::nameToggled(bool state)
{
    if (state != mShowName)
    {
        mShowName = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void  LogFrame::messageToggled(bool state)
{
    if (state != mShowMessage)
    {
        mShowMessage = state;
        generateMessageFormat();
        reformatMessages();
    }
}

void LogFrame::outputChanged(int index)
{
    if (index == 0)
    {
        mShowAllSources = true;
    }
    else
    {
        mShowAllSources = false;
        mMessageSources.clear();
        mMessageSources.push_back(index-1);
    }

    reformatMessages();
}

void LogFrame::logLevelChanged(int index)
{
    int compare1, compare2;
    switch (getMessageLogLevel())
    {
    case Logger::Debug: compare1 = 0; break;
    case Logger::Info: compare1 = 1; break;
    case Logger::Warning: compare1 = 2; break;
    case Logger::Error: compare1 = 3; break;
    case Logger::Fatal: compare1 = 4; break;
    }

    switch (getMessageObjectLogLevel())
    {
    case Logger::Debug: compare2 = 0; break;
    case Logger::Info: compare2 = 1; break;
    case Logger::Warning: compare2 = 2; break;
    case Logger::Error: compare2 = 3; break;
    case Logger::Fatal: compare2 = 4; break;
    }

    if ((compare1 != index) || (compare2 != index))
    {
        switch(index)
        {
        case 0:
            setMessageLogLevel(Logger::Debug);
            setMessageObjectLogLevel(Logger::Debug);
            break;
        case 1:
            setMessageLogLevel(Logger::Info);
            setMessageObjectLogLevel(Logger::Info);
            break;
        case 2:
            setMessageLogLevel(Logger::Warning);
            setMessageObjectLogLevel(Logger::Warning);
            break;
        case 3:
            setMessageLogLevel(Logger::Error);
            setMessageObjectLogLevel(Logger::Error);
            break;
        case 4:
            setMessageLogLevel(Logger::Fatal);
            setMessageObjectLogLevel(Logger::Fatal);
            break;
        default:
            LOG_ERROR() << "Illegal log level '" << index << "'.";
            break;
        }
    }
}

void LogFrame::generateMessageFormat()
{
    QString format("");

    if (mShowLevel)
        format.append("[%l]");
    if (mShowTime)
    {
        if (mShowLevel) format.append(" ");
        format.append("%t{HH:mm:ss.zzz}");
    }
    if (mShowSource)
    {
        if (mShowTime || mShowLevel)
            format.append(" ");
        format.append("(%S)");
    }
    if ((mShowLevel || mShowTime || mShowSource) && (mShowFile || mShowLine))
        format.append(" ");
    if (mShowFile)
    {
        format.append("'%f'");
        if (mShowLine) format.append(", line %i");
    }
    else if (mShowLine)
    {
        format.append("line %i");
    }
    if (mShowName)
    {
        if (mShowLevel || mShowTime || mShowFile || mShowLine)
            format.append(" ");
        format.append("<%C>");
    }
    if ((mShowLevel || mShowTime || mShowFile) && !mShowName && mShowMessage)
        format.append(": ");
    if (mShowName && mShowMessage)
        format.append(" ");
    if (mShowMessage)
        format.append("%m");

    setMessageFormat(format);
    setMessageObjectFormat(format);
}

void LogFrame::updateLogSourceAdded()
{
    initializeMessageSources();
}

void LogFrame::cleanup()
{
    stopReceivingMessageObjects();
    saveSettings();
}
