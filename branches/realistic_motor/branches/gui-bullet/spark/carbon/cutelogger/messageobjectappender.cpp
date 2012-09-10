/*
  Copyright (c) 2010 Patrick Geib (pgeib at online dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/

#include "messageobjectappender.h"
#include "imessagereceiver.h"

MessageObjectAppender::MessageObjectAppender(IMessageReceiver* receiver)
{
    mReceiver = receiver;
}

MessageObjectAppender::~MessageObjectAppender()
{
    Logger::unregisterAppender(this);
}

QString MessageObjectAppender::formatMessage(LogMessage& message)
{
    return qPrintable(formattedString(message.mTimeStamp, message.mLogLevel, message.mFile.c_str(),
        message.mLine, message.mFunction.c_str(), message.mMessageSourceId, message.mMessage));

}

void MessageObjectAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                             const char* function, unsigned int source, const QString& message)
{
    mReceiver->receiveMessageObjectQueued(boost::shared_ptr<LogMessage>(new LogMessage(timeStamp, logLevel, file, line, function, source, message)));
}
