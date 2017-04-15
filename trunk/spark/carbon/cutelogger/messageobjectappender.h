/*
  Copyright (c) 2010 Patrick Geib (patrick dot geib at online dot de)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
#ifndef MESSAGEOBJECTAPPENDER_H
#define MESSAGEOBJECTAPPENDER_H
/**
 * \file messageobjectappender.h
 * \brief Contains MessageObjectAppender class, which sends log messages as MessageObject to an IMessageReceiver.
 */

#include "abstractstringappender.h"
#include "logmessage.h"

class IMessageReceiver;

/*!
 \brief Sends log records to an IMessageReceiver by calling his receiveMessageObject() function.

 If needed, messages are sent to another receiver thread using queued connections.
*/
class MessageObjectAppender : public AbstractStringAppender
{
  public:
    /*!
     \brief Basic constructor.

     \param receiver pointer to object whose receiveMessageObject() function should be called.
    */
    MessageObjectAppender(IMessageReceiver* receiver);
    /*!
     \brief Basic destructor.
    */
    virtual ~MessageObjectAppender();

    /*!
     \brief Returns the given message as formatted string using the contained format string.

     \param message message to format
    */
    QString formatMessage(LogMessage& message);

  protected:
    //! Calls the target function.
    /**
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);

  private:
    IMessageReceiver* mReceiver; /*!< Interface with function to call. */

    //QReadWriteLock mSelfMutex; /*!< Mutex for appender access. */
};

#endif // MESSAGEOBJECTAPPENDER_H
