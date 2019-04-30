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

#ifndef IMESSAGERECEIVER_H
#define IMASSAGERECEIVER_H
/**
 * \file imessagereceiver.h
 * \brief Contains IMessageReceiver interface and a ThreadMessageQueue.
 */

#include "logger.h"
#include "logmessage.h"
#include "messagereceiverappender.h"
#include "messageobjectappender.h"

#include <QString>
#include <QDateTime>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class IMessageReceiver;

/*!
 \brief A Message Queue for messages sent from different threads. Uses Qt queued connections.

 When using the queued message mode of IMessageReceiver messages are queued using a ThreadMessageQueue. It sends all incoming messages from any
 thread and receives it via a queued slot in the main thread that created the queue.
*/
class ThreadMessageQueue : public QObject
{
    Q_OBJECT

public:
    /*!
     \brief Constructor.

     \param receiveTarget target to receive queued messages
    */
    ThreadMessageQueue(IMessageReceiver* receiveTarget);

    /*!
     \brief Queues a message, sends it to the main thread and outputs the message by calling the receive-target.

     \param message message to queue
     \param level log level of the message
    */
    void queueMessage(const QString& message, Logger::LogLevel level);
    /*!
     \brief Queues a message object, sends it to the main thread and outputs the message by calling the receive-target.

     \param object message to queue
    */
    void queueMessageObject(const boost::shared_ptr<LogMessage>& object);

private: signals:
    /*!
     \brief Emitted when a message was queued. Is received by receiveMessage().

     \param message message to emit
     \param level log level of the message
    */
    void queuedMessage(QString message, Logger::LogLevel level);
    /*!
     \brief Emitted when a message object was queued. Is received by receiveMessage().

     \param object message to emit
    */
    void queuedMessageObject(boost::shared_ptr<LogMessage> object);

private slots:
    /*!
     \brief Receives an emitted queued message in the main thread and sends it to the receive target.

     \param message message to receive
     \param level log level of the message
    */
    void receiveMessage(QString message, Logger::LogLevel level);
    /*!
     \brief Receives an emitted queued message in the main thread and sends it to the receive target.

     \param object message to receive
    */
    void receiveMessageObject(boost::shared_ptr<LogMessage> object);

private:
    IMessageReceiver* mReceiveTarget; /*!< Target for queued messages. */
};


/*!
 \brief A MessageReceiver interface class with receive functions that can be called by a MessageReceiverAppender and MessageObjectAppender.

 A Class that wants to receive detailed Messages to a non-static function may derive from IMessageReceiver.
 It features two possibilities to receive Messages, and completely handles the Appender-Control.\n

 First possibility: Receiving formatted Messages without meta-information.\n
 Messages will be formatted as specified in the format string (see AbstractStringAppender).\n
 Usage:\n
 Overload receiveMessage()\n
 Initialize Settings with setMessageLogLevel(), setMessageFormat()\n
 Start receiving with startReceivingMessages()\n
 Stop receiving with stopReceivingMessages()\n

 Second possibility: Receiving Message-Objects with detailed information and format on demand.\n
 Messages will be sent as LogMessage and can be stored and handled manually. When needed, they can be formatted and return a QString
 in the correct format (see AbstractStringAppender).\n
 Usage:\n
 Overload receiveMessageObject()\n
 Initialize Settings with setMessageObjectLogLevel(), setMessageObjectFormat()\n
 Start receiving with startReceivingMessageObjects()\n
 Stop receiving with stopReceivingMessageObjects()\n

 Both receiving options can be combined so one can possibly receive any message as formatted string and only receive a LogMessage object for
 Warning and Error Messages.
 Message Objects will always be given as boost::shared_pointer, so no unneccesary copies are created and they delete automatically if
 there should be no MessageHandling by the derived class.
 If the receiving is stopped, the contained MessageReceiverAppender/MessageObjectAppender is unregistered (temporarily). He will not be able
 to retrieve messages he missed. Same goes for Messages of the wrong LogLevel.

 IMessageReceivers can either receive messages and/or message objects queued or non-queued.
 When the derived virtual functions are not thread-safe, or when it calls Qt-Gui-Elements the constructors receiveMessagesQueued flag can be set. In this case messages
 from the MessageObjectAppender and MessageReceiverAppender are not directly given to the IMessageReceiver but are queued and sent to the main thread using
 a ThreadMessageQueue and the qt-connect-queue. For the queue to work the IMessageReceivers thread needs to be in an event loop (QThread::exec()).

 \warning Especially when interacting with QtGui-elements like QTextEdit, QWidget etc., the IMessageReceiver HAS to be used in queued mode, since only the owning thread
 may change Gui parameters. Otherwise the application might crash in release mode.

 If you dont want the messages to be queued you can set the constructors queue flag to false. In this case the thread that sent the log message will call receiveMessage()
 and receiveMessageObject() directly.

 \note After some extensions IMessageReceiver is no pure Interface Class anymore, but will keep the name to signalize using that multiple inheritance
 here is not a problem.
*/
class IMessageReceiver
{
public:
    //public types

public:

    /*!
     \brief Virtual receive function called by receiveMessageQueued when a new message arrives.

     \param message formatted message
     \param level log level of the message
    */
    virtual void receiveMessage(QString message, Logger::LogLevel level);
    /*!
     \brief Virtual receive function called by receiveMessageObjectQueued when a new message arrives.

     \param object message content and meta information as LogMessage
    */
    virtual void receiveMessageObject(boost::shared_ptr<LogMessage> object);

    /*!
     \brief Receive function called by MessageReceiverAppender when a new message arrives.

     Calls the virtual receive function directly if mQueueInterThreadMessages is false.
     Queues them by sending them to the owning thread if mQueueInterThreadMessages is true.

     \param message formatted message
     \param level log level of the message
    */
    void receiveMessageQueued(const QString& message, Logger::LogLevel);
    /*!
     \brief Receive function called by MessageObjectAppender when a new message arrives.

     Calls the virtual receive function directly if mQueueInterThreadMessages is false.
     Queues them by sending them to the owning thread if mQueueInterThreadMessages is true.

     \param object message content and meta information as LogMessage
    */
    void receiveMessageObjectQueued(const boost::shared_ptr<LogMessage>& object);

protected:

    /*!
     \brief Constructor which gives the option of queued or non-queued message receiving.

     When the derived virtual functions are not thread-safe, or when it calls Qt-Gui-Elements the receiveMessagesQueued flag can be set. In this case messages
     from the MessageObjectAppender and MessageReceiverAppender are not directly given to the IMessageReceiver but are queued and sent to the main thread using
     a ThreadMessageQueue and the qt-connect-queue. For the queue to work the IMessageReceivers thread needs to be in an event loop (QThread::exec()).

     \warning Especially when interacting with QtGui-elements like QTextEdit, QWidget etc, the receiveMessagesQueued flag HAS to be set, since only the owning thread
     may change Gui parameters.

     If you dont want the messages to be queued you can set receiveMessagesQueued to false. In this case the thread that sent the log message will call receiveMessage()
     and receiveMessageObject() directly.

     \param receiveMessagesQueued if true, messages are queued to be executed by the IMessageReceivers owning thread
    */
    IMessageReceiver(bool receiveMessagesQueued);
    /*!
     \brief Basic destructor.
    */
    virtual ~IMessageReceiver();

    /*!
     \brief Returns the log level for formatted messages.
    */
    Logger::LogLevel getMessageLogLevel();
    /*!
     \brief Returns the log level for message objects.
    */
    Logger::LogLevel getMessageObjectLogLevel();
    /*!
     \brief Returns true if appender for formatted messages is currently registered.
    */
    bool isReceivingMessages();
    /*!
     \brief Returns true if appender for messages objects is currently registered.
    */
    bool isReceivingMessageObjects();
    /*!
     \brief Returns message format for formatted messages.
    */
    QString getMessageFormat();
    /*!
     \brief Returns message format for message objects.

     Does have no impact on the MessageObject, but is used when the formatMessage() function is called.
    */
    QString getMessageObjectFormat();

    /*!
     \brief Sets the LogLevel for formatted messages.

     \param level minimum level to receive
    */
    void setMessageLogLevel(Logger::LogLevel level);
    /*!
     \brief Sets the MessageFormat for formatted messages.

     \param format format to use, see AbstractStringAppender
    */
    void setMessageFormat(QString format);
    /*!
     \brief Sets the MessageFormat used to format LogMessage objects with formatMessage().

     \param format format to use, see AbstractStringAppender
    */
    void setMessageObjectFormat(QString format);
    /*!
     \brief Sets the LogLevel for message objects.

     \param level minimum level to receive
    */
    void setMessageObjectLogLevel(Logger::LogLevel level);
    /*!
     \brief Sets the LogLevel for both types of messages.

     \param level minimum level to receive
    */
    void setLogLevel(Logger::LogLevel level);

    /*!
     \brief Starts receiving formatted messages.
    */
    void startReceivingMessages();
    /*!
     \brief Starts receiving formatted messages.

     \param format format to use, see AbstractStringAppender
     \param level minimum log level to receive
    */
    void startReceivingMessages(QString format, Logger::LogLevel level);
    /*!
     \brief Stops receiving formatted messages.
    */
    void stopReceivingMessages();
    /*!
     \brief Starts receiving message objects.
    */
    void startReceivingMessageObjects();
    /*!
     \brief Starts receiving message objects.

     \param level minimum log level to receive
    */
    void startReceivingMessageObjects(Logger::LogLevel level);
    /*!
     \brief Stops receiving message objects.
    */
    void stopReceivingMessageObjects();

    /*!
     \brief Format a LogMessage using the current format string.

     \param message message to format
     \return formatted string
    */
    QString formatMessage(LogMessage& message);
    /*!
     \brief Format a LogMessage using the current format string.

     \param message message to format
     \return formatted string
    */
    QString formatMessage(boost::shared_ptr<LogMessage> message);

private:
    MessageReceiverAppender mMessageReceiverAppender; /*!< Appender for formatted messages. */
    MessageObjectAppender mMessageObjectAppender; /*!< Appender for message objects. */

    bool mReceivingMessages; /*!< True if receiving formatted messages is activated. */
    bool mReceivingMessageObjects; /*!< True if receiving message objects is activated. */
    bool mQueueInterThreadMessages; /*!< If true, messages received are queued and sent to the main thread using queued slots. */
    ThreadMessageQueue* mThreadMessageQueue; /*!< Message Queue */
};

#endif //IMASSAGERECEIVER_H
