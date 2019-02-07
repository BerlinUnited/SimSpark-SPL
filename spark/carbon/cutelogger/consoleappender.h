/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

  Copyright (c) 2010 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  Modified by: Patrick Geib
  This file is part of Carbon
  $Id$
*/

#ifndef CONSOLEAPPENDER_H
#define CONSOLEAPPENDER_H
/**
 * \file consoleappender.h
 * \brief Contains ConsoleAppender class, which formats and sends log messages to the console.
 */

#include "abstractstringappender.h"

//! Writes the log records to the std::cerr output stream.
class ConsoleAppender : public AbstractStringAppender
{
  protected:
    //! Writes the log record to the std::cerr stream.
    /**
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);
};

#endif // CONSOLEAPPENDER_H
