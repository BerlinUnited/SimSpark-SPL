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

// Local
#include "fileappender.h"

// STL
#include <iostream>

#include <QFileInfo>
#include <QDir>


FileAppender::FileAppender(const QString& fileName)
{
  setFileName(fileName);
}


QString FileAppender::fileName() const
{
  QMutexLocker locker(&mLogFileMutex);
  return mLogFile.fileName();
}


void FileAppender::setFileName(const QString& filename)
{
  QMutexLocker locker(&mLogFileMutex);
  if (mLogFile.isOpen())
    mLogFile.close();

  QFileInfo info(filename);
  QString absPath = info.dir().absolutePath();

  //Create the directory
  if (!info.dir().exists())
  {
      bool success = QDir(QDir::rootPath()).mkpath(absPath);
      if (!success)
          std::cerr << "<FileAppender::setFileName> Cannot create directory for the log file " << qPrintable(mLogFile.fileName()) << std::endl;
  }

  mLogFile.setFileName(filename);
}


void FileAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                          const char* function, unsigned int source, const QString& message)
{
  QMutexLocker locker(&mLogFileMutex);

  if (!mLogFile.isOpen())
  {
    if (mLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      mLogStream.setDevice(&mLogFile);
    }
    else
    {
      std::cerr << "<FileAppender::append> Cannot open the log file " << qPrintable(mLogFile.fileName()) << std::endl;
      return;
    }
  }

  mLogStream << formattedString(timeStamp, logLevel, file, line, function, source, message);
  mLogStream.flush();
}
