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

#ifndef LOGSTREAM_H
#define LOGSTREAM_H
/**
 * \file logstream.h
 * \brief Contains LogStream class.
 */

#include "logger.h"

#include <iostream>

class LogStreamBuffer;

/*!
 \brief A Logging ostream to receive messages from external sources and send them to the Logger.

 LogStreams are added to the zeitgeist LogServer to receive messages of a specific log level.
 A log stream has a buffer size and a log source that has to be specified upon creation and is used to identify the source of a message when
 sending it to the Logger.
*/
class LogStream: public std::ostream
{

public:
    //public functions

    /*!
     \brief Complete Constructor.

     \param level log level to use for messages
     \param size buffer size of the stream
     \param logsource source identifier as obtained from the Logger
    */
    LogStream(Logger::LogLevel level, unsigned int size = 1024, unsigned int logsource = 0);
    /*!
     \brief Destructor.
    */
    virtual ~LogStream();

    /*!
     \brief Returns the associated LogStreamBuffer.
    */
    const LogStreamBuffer& getStreamBuffer() const;
    /*!
     \brief Returns the associated LogStreamBuffer.
    */
    LogStreamBuffer& getStreamBuffer();

    /*!
     \brief Returns the LogStreamBuffer size.
    */
    unsigned int getSize() const;        
    /*!
     \brief Returns the log level of messages for this log stream.
    */
    Logger::LogLevel getLogLevel() const;
    /*!
     \brief Returns the source id for messages of this log stream.
    */
    unsigned int getLogSource() const;   

    /*!
     \brief Write a message of a given size to the stream.

     \param message message
     \param size stream size
    */
    std::ostream& write(const char* message, std::streamsize size);

private:
    /*!
     \brief Copy Constructor.
    */
    LogStream(const LogStream& obj);
    /*!
     \brief Copy operator.
    */
    LogStream& operator=(const LogStream& obj);

    /*!
     \brief Returns the stream buffer (const).

    */
    const LogStreamBuffer& GetStreamBuf() const;
    /*!
     \brief Returns the stream buffer (non-const).
    */
    LogStreamBuffer& GetStreamBuf();
};

#endif //ZEITGEIST_LOGSERVER_H
