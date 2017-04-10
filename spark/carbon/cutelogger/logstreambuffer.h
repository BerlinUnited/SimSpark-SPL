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

#ifndef LOGSTREAMBUFFER_H
#define LOGSTREAMBUFFER_H
/**
 * \file logstreambuffer.h
 * \brief Contains LogStreamBuffer class.
 */

#include <vector>
#include <streambuf>

#include "logger.h"

/*!
 \brief Stream buffer for the LogStream class.

 A LogStreamBuffer is created by a LogStream.
*/
class LogStreamBuffer : public std::streambuf
{
protected:
    //protected types

    /*!
     \brief Traits type definition.
    */
    typedef std::char_traits<char> TTraitsType;
    /*!
     \brief Int traits type definition.
    */
    typedef traits_type::int_type TIntType;

public:
    //public functions

    /*!
     \brief Complete Constructor.

     \param level log level of the stream
     \param size buffer size of the stream
     \param logsource source id of the stream datas source, as obtained by the Logger
    */
    LogStreamBuffer(Logger::LogLevel level, unsigned int size, unsigned int logsource);
    /*!
     \brief Destructor.
    */
    virtual ~LogStreamBuffer();

    /*!
     \brief Returns the buffer size.
    */
    unsigned int getSize() const;
    /*!
     \brief Returns the log level of messages in this buffer.
    */
    Logger::LogLevel getLogLevel() const;
    /*!
     \brief Returns the source id for messages in this buffer.
    */
    unsigned int getLogSource() const;

protected:
    // virtual protected functions
    // these functions implement the streambuf interface ... handle with care

    /*!
     \brief Called by the iostream library in case of stack overflow (put area is full).

     \param type traits int type
    */
    TIntType overflow(TIntType type);
    /*!
     \brief Output the contained data.

     Synchronizes the internal state with the external state. It is used to flush the streambuf object.
     To be called when new data has been written to the stream. Called by zeitgeist when it was specified to sync the output stream.
    */
    int sync();

private:
    //private functions

    /*!
     \brief Copy Constructor.
    */
    LogStreamBuffer(const LogStreamBuffer &obj);
    /*!
     \brief Copy operator.
    */
    LogStreamBuffer& operator=(const LogStreamBuffer &obj);

    /*!
     \brief Send the buffered message to the Logger.

     \param buffer buffer
     \param length number of chars
    */
    void Forward(const char *buffer, unsigned int length);

    /*!
     \brief Sends out complete internal buffer.
    */
    void PutBuffer();

    /*!
     \brief Sends out a single character.
    */
    void PutChar(TIntType chr);

private:
    //private members

    unsigned int mSize;         /*!< Internal buffer size. */
    Logger::LogLevel mLogLevel; /*!< Log level for all messages. */
    unsigned int mLogSource;    /*!< Log source identifier for all messages. */
};

#endif // LOGSTREAMBUFFER_H
