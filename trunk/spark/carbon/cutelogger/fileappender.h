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

  Extended by: Patrick Geib
  This file is part of Carbon
  $Id$
*/

#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H
/**
 * \file fileappender.h
 * \brief Contains FileAppender class, which formats and sends log messages to a file stream.
 */

// Logger
#include "abstractstringappender.h"

// Qt
#include <QFile>
#include <QTextStream>


//! Writes the log records to a plain text file.
class FileAppender : public AbstractStringAppender
{
  public:
    //! Constructs the new file appender assigned to file with the given name.
    FileAppender(const QString& fileName = QString());

    //! Returns the name set by setFileName() or to the FileAppender constructor.
    /**
     * \sa setFileName()
     */
    QString fileName() const;

    //! Sets the name of the file. The name can have no path, a relative path, or an absolute path.
    /**
     * \sa fileName()
     */
    void setFileName(const QString&);

  protected:
    //! Write the log record to the file.
    /**
     * \sa fileName()
     * \sa AbstractStringAppender::format()
     */
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, unsigned int source, const QString& message);

  private:
    QFile mLogFile; /*!< File to log to. */
    QTextStream mLogStream;  /*!< Log stream. */
    mutable QMutex mLogFileMutex;  /*!< File mutex. */
};

#endif // FILEAPPENDER_H
