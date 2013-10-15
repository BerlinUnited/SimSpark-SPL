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

#include "logmessage.h"

LogMessage::LogMessage(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
               const char* function, unsigned int source, const QString& message)
    : mTimeStamp(timeStamp),
      mFile(file),
      mFunction(function),
      mMessage(message),
      mMessageSourceId(source)
{
  mLogLevel = logLevel;
  mLine = line;
}

LogMessage::LogMessage(const LogMessage& ref) 
    : mTimeStamp(ref.mTimeStamp),
      mFile(ref.mFile),
      mFunction(ref.mFunction),
      mMessage(ref.mMessage),
      mLogLevel(ref.mLogLevel),
      mLine(ref.mLine),
      mMessageSourceId(ref.mMessageSourceId)
{
}

LogMessage::LogMessage()
    : mFile(""),
      mFunction(""),
      mMessage("empty"),
      mLogLevel(Logger::Debug),
      mLine(0),
      mMessageSourceId(0)
{
}