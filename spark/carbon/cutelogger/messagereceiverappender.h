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
#ifndef MESSAGERECEIVERAPPENDER_H
#define MESSAGERECEIVERAPPENDER_H
/**
 * \file messagereceiverappender.h
 * \brief Contains MessageReceiverAppender class, which formats and sends messages to an IMessageReceiver.
 */

#include "abstractstringappender.h"

class IMessageReceiver;

//! Sends log records to an IMessageReceiver by calling his receiveMessage() function.
class MessageReceiverAppender : public AbstractStringAppender
{
  public:
    /*!
     \brief Basic constructor.

     \param receiver pointer to object whose receiveMessage() function should be called.
    */
    MessageReceiverAppender(IMessageReceiver* receiver);
    /*!
     \brief Basic destructor.
    */
    virtual ~MessageReceiverAppender();

  protected:
    //! Calls the target function.
    /**
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);

  private:
    IMessageReceiver* mReceiver; /*!< Interface to call. */
};

#endif // MESSAGERECEIVERAPPENDER_H
