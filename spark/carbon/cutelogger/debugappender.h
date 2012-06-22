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

#ifndef DEBUGAPPENDER_H
#define DEBUGAPPENDER_H
/**
 * \file debugappender.h
 * \brief Contains DebugAppender class, which formats and sends log messages to platform specific debug output streams.
 */

#include "abstractstringappender.h"

/*!
 \brief Writes the log records to the platform specific debug output stream.

 Supports Windows, Unix (Linux, OSX), and Solaris debug output streams.
*/
class DebugAppender : public AbstractStringAppender
{
  protected:
    //! Writes the log record to the debug output stream.
    /**
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);
};

#endif // DEBUGAPPENDER_H
