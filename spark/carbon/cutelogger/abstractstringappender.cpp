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
#include "abstractstringappender.h"

// Qt
#include <QReadLocker>
#include <QWriteLocker>
#include <QDateTime>

const char formattingMarker = '%';

AbstractStringAppender::AbstractStringAppender()
  : m_format(QLatin1String("%t{yyyy-MM-ddTHH:mm:ss.zzz} [%-7l] <%C> %m\n"))
{}


QString AbstractStringAppender::format() const
{
  QReadLocker locker(&m_formatLock);
  return m_format;
}


void AbstractStringAppender::setFormat(const QString& format)
{
  QWriteLocker locker(&m_formatLock);
  m_format = format;
}


QString AbstractStringAppender::formattedString(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file,
                                                int line, const char* function, unsigned int source, const QString& message) const
{
  QString f = format();
  const int size = f.size();

  QString result;

  int i = 0;
  while (i < f.size())
  {
    QChar c = f.at(i);

    // We will silently ignore the broken % marker at the end of string
    if (c != QLatin1Char(formattingMarker) || (i + 1) == size)
    {
      result.append(c);
    }
    else
    {
      QChar command = f.at(++i);

      // Check for the padding instruction
      int fieldWidth = 0;
      if (command.isDigit() || command.category() == QChar::Punctuation_Dash)
      {
        int j = 1;
        while ((i + j) < size && f.at(i + j).isDigit())
          j++;
        fieldWidth = f.mid(i, j).toInt();

        i += j;
        command = f.at(i);
      }

      // Log record chunk to insert instead of formatting instruction
      QString chunk;

      // Time stamp
      if (command == QLatin1Char('t'))
      {
        if (f.at(i + 1) == QLatin1Char('{'))
        {
          int j = 1;
          while ((i + 2 + j) < size && f.at(i + 2 + j) != QLatin1Char('}'))
            j++;

          if ((i + 2 + j) < size)
          {
            chunk = timeStamp.toString(f.mid(i + 2, j));

            i += j;
            i += 2;
          }
        }

        if (chunk.isNull())
          chunk = timeStamp.toString(QLatin1String("HH:mm:ss.zzz"));
      }

      // Log level
      else if (command == QLatin1Char('l'))
        chunk = Logger::levelToString(logLevel);

      // Uppercased log level
      else if (command == QLatin1Char('L'))
        chunk = Logger::levelToString(logLevel).toUpper();

      // Filename
      else if (command == QLatin1Char('F'))
        chunk = QLatin1String(file);

      // Filename without a path
      else if (command == QLatin1Char('f'))
        chunk = QString(QLatin1String(file)).section('/', -1);

      // Source line number
      else if (command == QLatin1Char('i'))
        chunk = QString::number(line);

      // Function name, as returned by Q_FUNC_INFO
      else if (command == QLatin1Char('C'))
        chunk = QLatin1String(function);

      // Log source id
      else if (command == QLatin1Char('s'))
        chunk = QString::number(source);

      // Log source name
      else if (command == QLatin1Char('S'))
        chunk = Logger::getLogSourceName(source).c_str();

      // Log message
      else if (command == QLatin1Char('m'))
        chunk = message;

      // We simply replace the double formatting marker (%) with one
      else if (command == QLatin1Char(formattingMarker))
        chunk = QLatin1Char(formattingMarker);

      // Do not process any unknown commands
      else
      {
        chunk = QLatin1Char(formattingMarker);
        chunk.append(command);
      }

      result.append(QString(QLatin1String("%1")).arg(chunk, fieldWidth));
    }

    ++i;
  }

  return result;
}
