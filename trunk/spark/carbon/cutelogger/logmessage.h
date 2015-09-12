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

#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H
/**
 * \file logmessage.h
 * \brief Contains LogMessage class that wraps a single log message with meta information.
 */

#include "logger.h"

#include <QDateTime>
#include <QString>

/*!
 \brief Information container class for log messages and meta information.

 Used by MessageObjectAppender to send log message objects to an object of a class derived from IMessageReceiver.
 Contains the message and meta information: timestamp and log level as well as origin filename, line and function name.
*/
class LogMessage
{
public:
    /*!
     \brief Complete constructor.
    */
    LogMessage(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                   const char* function, unsigned int source, const QString& message);
    /*!
     \brief Copy constructor.
    */
    LogMessage(const LogMessage& ref);
    /*!
     \brief Default constructor.
    */
    LogMessage();

public:
    QString mMessage; /*!< Log message. */
    QDateTime mTimeStamp; /*!< Timestamp at which the message was thrown. */
    Logger::LogLevel mLogLevel; /*!< Log level of the message. */
    std::string mFile; /*!< File the message originated from. */
    int mLine; /*!< Line of the file the message originated from. */
    std::string mFunction; /*!< Name of the function the message originated from. */
    unsigned int mMessageSourceId; /*!< Source id of the source the message originated from. Sources can be specified in the Logger. */
};

#endif //LOGMESSAGE_H
