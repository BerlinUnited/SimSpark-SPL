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

#ifndef LOGFRAME_H
#define LOGFRAME_H
/**
 * \file logframe.h
 * \brief Contains LogFrame class.
 */

#include <cutelogger/imessagereceiver.h>
#include <attachableframe.h>
#include <settings.h>

#include <list>
#include <QString>

#include "ui_logframe.h"

class FunctionAppender;
namespace LogFrameUtil
{
    class LogSettings;
}

/*!
 \brief AttachableFrame that manages and displays LogMessages of all program sources.

 LogFrame implements the IMessageReceiver interface and is therefore able to receive log messages from the logging system.
 The messages are stored as LogMessage objects up to a specific amount. When needed and signaled by the User via the ToolButtons,
 the Messages displayed in the contained TextEdit are reformatted and displayed again. So it is possible to view additional information
 for every message when needed, for example to closely inspect an error that happened.

 Incoming Messages are displayed in order of their arrival, but are stored in order of their timestamp.
 When reformatting the message list (for example by changing the message format or visibility for a log level) the messages will be redisplayed
 like they are found in the message list (sorted).

 The Layout is defined in logframe.ui and can be modified using QtDesigner.

 \note
 It may be helpful to sort incoming messages directly using some kind of diplay delay or repainting in the textedit. But if the delay
 between incoming asynchronous messages is too high, messages may pop up and resort themselves visibly, what can be a nuisance. So at least
 it would have to be optional.
*/
class LogFrame : public AttachableFrame, public IMessageReceiver
{
    Q_OBJECT
    GUI_FRAME

protected:
    //protected types
    /*!
     \brief Helper-Type for LogMessage pointers returned by the IMessageReceiver.
    */
    typedef boost::shared_ptr<LogMessage> TLogMessagePtr;
    /*!
     \brief Helper-Type defining a list of Messages.

     It is advisable to stick with using a list, since logging of asynchronous messages requires a bit of
     message-sorting at the end of the list to order them after their timestamp (using Bubble Sort) and popping old messages
     at the front of the list.
    */
    typedef std::list<TLogMessagePtr> TMessages;

public:
    // public functions

    /*!
     \brief Basic constructor.

     Initializes UI elements, settings and provided menu objects
    */
    LogFrame();

    /*!
     \brief Basic destructor.
    */
    virtual ~LogFrame();

    /*!
     \brief Post-construct init function.
    */
    virtual void init(const QStringList& parameters);

protected:

    /*!
     \brief (Re-)Initializes the message source combo box.

     Called in the constructor and when the log source list of the Logger changed.
    */
    void initializeMessageSources();

    /*!
     \brief Derived receiveing function to receive LogMessage objects.

     \param message incoming message object
    */
    virtual void receiveMessageObject(TLogMessagePtr message);

    /*!
     \brief Stores a message, sorted by its timestamp.

     \param message message to store
    */
    void storeMessage(TLogMessagePtr message);

    /*!
     \brief Formats all messages with the current format string and refills the logTextEdit.
    */
    void reformatMessages();
    /*!
     \brief Defines the logTextEdit text color based on the log level.

     \param level level of the next message
    */
    void setPrintColor(Logger::LogLevel level);

    /*!
     \brief Returns true of the message is currently wanted to be visible in the logTextEdit.

     \param message message to check
    */
    bool showingMessage(TLogMessagePtr& message);
    /*!
     \brief Returns true of the messages source is currently wanted to be visible in the logTextEdit.

     \param message message to check
    */
    bool showingMessageSource(TLogMessagePtr& message);
    
    /*!
     \brief Generates the message format based on user-specified elements.
    */
    void generateMessageFormat();
    /*!
     \brief Loads settings from the QSettings object.
    */
    void loadSettings();
    /*!
     \brief Loads default settings from the Ui class.
    */
    void loadDefaultSettings();
    /*!
     \brief Saves current settings to the QSettings object.
    */
    void saveSettings();
    /*!
     \brief Pops messages from the front until the amount is smaller or equal the maximum specified amount.
    */
    void popMessages();
    /*!
     \brief Reloads display of store message count.
    */
    void updateMessageAmount();
    /*!
     \brief Updates state of checkbuttons.
    */
    void updateCheckButtons();

    /*!
     \brief Tries to select the message output with the given name.
    */
    void chooseMessageOutput(const QString& name);

public: signals:

public slots:
    /*!
     \brief Sets the visibility of debug messages and calls reformatMessages() if necessary.

     \param state state of visibility
    */
    void debugToggled(bool state);
    /*!
     \brief Sets the visibility of info messages and calls reformatMessages() if necessary.

     \param state state of visibility
    */
    void infoToggled(bool state);
    /*!
     \brief Sets the visibility of warning messages and calls reformatMessages() if necessary.

     \param state state of visibility
    */
    void warningToggled(bool state);
    /*!
     \brief Sets the visibility of error messages and calls reformatMessages() if necessary.

     \param state state of visibility
    */
    void errorToggled(bool state);

    /*!
     \brief Specifies if the timestamp should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void timeToggled(bool state);
    /*!
     \brief Specifies if message source information should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void sourceToggled(bool state);
    /*!
     \brief Specifies if log level information should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void levelToggled(bool state);
    /*!
     \brief Specifies if source file name should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void fileToggled(bool state);
    /*!
     \brief Specifies if the source file line should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void lineToggled(bool state);
    /*!
     \brief Specifies if the source function name should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void nameToggled(bool state);
    /*!
     \brief Specifies if the message content should be included in messages.

     Generates a new format string and reformats messages if necessary.
     \param state state of visibility
    */
    void messageToggled(bool state);

    /*!
     \brief Message source channel changed.

     \param index new message source
    */
    void outputChanged(int index);
    /*!
     \brief Log level to consider changed.

     \param index new minimum index
    */
    void logLevelChanged(int index);
    /*!
     \brief Updates a LogFrame instance setting from the setting file.

     \param name name of the setting
    */
    void updateInstanceSetting(QString name);

    /*!
     \brief Updates a LogFrame class setting from the setting file.

     \param name name of the setting
    */
    void updateSetting(QString name);

    /*!
     \brief Updates the list of log sources from the Logger.

     Called by the Gui when the log source list changed.
    */
    void updateLogSourceAdded();

    /*!
     \brief Makes the frame ready to be deleted. Called by Carbons aboutToShutdown() signal.
    */
    void cleanup();

private:
    // private members

    Ui::LogFrame ui; /*!< Gui-layout object. */
    boost::shared_ptr<Settings> mSettings; /*!< Settings object pointer. */
    static LogFrameUtil::LogSettings* mSettingsWidget; /*!< Static settings gui widget. */
    static bool mFirstConsole; /*!< True for constructor of first log frame. Causes display of initialization warning and error messages.*/

    TMessages mMessages; /*!< List of all message objects. */

    bool mShowDebug;    /*!< Visibility of debug messages. */
    bool mShowInfo;     /*!< Visibility of info messages. */
    bool mShowWarning;  /*!< Visibility of warning messages. */
    bool mShowError;    /*!< Visibility of error messages. */
    bool mShowFatal;    /*!< Visibility of fatal messages. */

    bool mShowTime;     /*!< Visibility of timestamp. */
    bool mShowSource;   /*!< Visibility of message source. */
    bool mShowLevel;    /*!< Visibility of log level. */
    bool mShowFile;     /*!< Visibility of source file name. */
    bool mShowLine;     /*!< Visibility of source file line. */
    bool mShowName;     /*!< Visibility of source function name. */
    bool mShowMessage;  /*!< Visibility of message content. */

    QString mLastSource; /*!< Last displayed message source. */

    int mMaximumMessages; /*!< Visibility of timestamp. */

    std::vector<int> mMessageSources; /*!< Visible message sources. */
    bool mShowAllSources; /*!< If true, all message sources are displayed. Otherwise mMessageSources specifies message sources to  display. */
    int mMaxMessageSources; /*!< Amount of message sources. Size of combo box that containing them is 1 bigger (index 0 is 'all') */
};

#endif // CONSOLEFRAME_H
