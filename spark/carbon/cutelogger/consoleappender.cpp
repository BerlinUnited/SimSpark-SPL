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

// Local
#include "consoleappender.h"

// STL
#include <iostream>


void ConsoleAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                             const char* function, unsigned int source, const QString& message)
{
    if (logLevel <= Logger::Info)
        std::cout << qPrintable(formattedString(timeStamp, logLevel, file, line, function, source, message));
    else
        std::cerr << qPrintable(formattedString(timeStamp, logLevel, file, line, function, source, message));
}
