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

#include "imessagereceiver.h"

#include <iostream>
#include <QThread>

ThreadMessageQueue::ThreadMessageQueue(IMessageReceiver* receiveTarget)
{
    mReceiveTarget = receiveTarget;

    connect(this, SIGNAL(queuedMessage(QString, Logger::LogLevel)), this, SLOT(receiveMessage(QString, Logger::LogLevel)));
    connect(this, SIGNAL(queuedMessageObject(boost::shared_ptr<LogMessage>)), this, SLOT(receiveMessageObject(boost::shared_ptr<LogMessage>)));
}

void ThreadMessageQueue::receiveMessage(QString message, Logger::LogLevel level)
{
    mReceiveTarget->receiveMessage(message, level);
}

void ThreadMessageQueue::receiveMessageObject(boost::shared_ptr<LogMessage> object)
{
    mReceiveTarget->receiveMessageObject(object);
}

void ThreadMessageQueue::queueMessage(const QString& message, Logger::LogLevel level)
{
    emit queuedMessage(message, level);
}

void ThreadMessageQueue::queueMessageObject(const boost::shared_ptr<LogMessage>& object)
{
    emit queuedMessageObject(object);
}

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

IMessageReceiver::~IMessageReceiver()
{
    stopReceivingMessageObjects();

    delete mThreadMessageQueue;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

IMessageReceiver::IMessageReceiver(bool useThreadMessageQueue)
    : mMessageReceiverAppender(this),
      mMessageObjectAppender(this)
{
    mReceivingMessages = false;
    mReceivingMessageObjects = false;

    mQueueInterThreadMessages = true;

    if (mQueueInterThreadMessages)
    {
        mThreadMessageQueue = new ThreadMessageQueue(this);
    }
    else
    {
        mThreadMessageQueue = 0;
    }
}

void IMessageReceiver::receiveMessage(QString message, Logger::LogLevel level)
{
    std::cerr << "Unhandled message " << message.toStdString() << " received in IMessageReceiver base class. " <<
                     "When activating startReceivingMessages(), be sure to override virtual void receiveMessage(QString, Logger::LogLevel).";
}

void IMessageReceiver::receiveMessageObject(boost::shared_ptr<LogMessage> newMessage)
{
    std::cerr << "Unhandled message object " << newMessage->mMessage.toStdString() << " received in IMessageReceiver base class. " <<
                     "When activating startReceivingMessageObjects(), be sure to override virtual void receiveMessageObjects(shared_ptr<MessageObject>).";
}

void IMessageReceiver::receiveMessageQueued(const QString& message, Logger::LogLevel level)
{
    if (!mQueueInterThreadMessages)
        receiveMessage(message, level);
    else
        mThreadMessageQueue->queueMessage(message, level);
}

void IMessageReceiver::receiveMessageObjectQueued(const boost::shared_ptr<LogMessage>& object)
{
    if (!mQueueInterThreadMessages)
        receiveMessageObject(object);
    else
    {
        mThreadMessageQueue->queueMessageObject(object);
    }
}

Logger::LogLevel IMessageReceiver::getMessageLogLevel()
{
    return mMessageReceiverAppender.detailsLevel();
}

Logger::LogLevel IMessageReceiver::getMessageObjectLogLevel()
{
    return mMessageObjectAppender.detailsLevel();
}

bool IMessageReceiver::isReceivingMessages()
{
    return mReceivingMessages;
}

bool IMessageReceiver::isReceivingMessageObjects()
{
    return mReceivingMessageObjects;
}

QString IMessageReceiver::getMessageFormat()
{
    return mMessageReceiverAppender.format();
}

QString IMessageReceiver::getMessageObjectFormat()
{
    return mMessageObjectAppender.format();
}

void IMessageReceiver::startReceivingMessages()
{
    if (mReceivingMessages)
        return;
    else
    {
        Logger::registerAppender(&mMessageReceiverAppender);
        mReceivingMessages = true;
    }
}

void IMessageReceiver::startReceivingMessageObjects()
{
    if (mReceivingMessageObjects)
        return;
    else
    {
        Logger::registerAppender(&mMessageObjectAppender);
        mReceivingMessageObjects = true;
    }
}

void IMessageReceiver::startReceivingMessages(QString format, Logger::LogLevel level)
{
    setMessageFormat(format);
    setLogLevel(level);
    startReceivingMessages();
}

void IMessageReceiver::startReceivingMessageObjects(Logger::LogLevel level)
{
    setMessageObjectLogLevel(level);
    startReceivingMessageObjects();
}

void IMessageReceiver::stopReceivingMessages()
{
    Logger::unregisterAppender(&mMessageReceiverAppender);
    mReceivingMessages = false;
}

void IMessageReceiver::stopReceivingMessageObjects()
{
    Logger::unregisterAppender(&mMessageObjectAppender);
    mReceivingMessageObjects = false;
}

void IMessageReceiver::setLogLevel(Logger::LogLevel level)
{
    setMessageLogLevel(level);
    setMessageObjectLogLevel(level);
}

void IMessageReceiver::setMessageLogLevel(Logger::LogLevel level)
{
     mMessageReceiverAppender.setDetailsLevel(level);
}

void IMessageReceiver::setMessageObjectLogLevel(Logger::LogLevel level)
{
    mMessageObjectAppender.setDetailsLevel(level);
}

void IMessageReceiver::setMessageFormat(QString format)
{
    mMessageReceiverAppender.setFormat(format);
}

void IMessageReceiver::setMessageObjectFormat(QString format)
{
    mMessageObjectAppender.setFormat(format);
}

QString IMessageReceiver::formatMessage(boost::shared_ptr<LogMessage> message)
{
    return mMessageObjectAppender.formatMessage(*message);
}

QString IMessageReceiver::formatMessage(LogMessage& message)
{
    return mMessageObjectAppender.formatMessage(message);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

