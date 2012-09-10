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

#include "logstream.h"
#include "logstreambuffer.h"
#include <stdarg.h>

using namespace std;

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

LogStream::LogStream(Logger::LogLevel level, unsigned int size, unsigned int logsource) :
    ostream(new LogStreamBuffer(level, size, logsource))
{
}

LogStream::~LogStream()
{
    flush();

    // delete the created streambuf
    delete rdbuf();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

unsigned int LogStream::getSize() const
{
    return getStreamBuffer().getSize();
}

Logger::LogLevel LogStream::getLogLevel() const
{
    return getStreamBuffer().getLogLevel();
}

unsigned int LogStream::getLogSource() const
{
    return getStreamBuffer().getLogSource();
}

std::ostream& LogStream::write(const char* c, std::streamsize n)
{
    return *this;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

const LogStreamBuffer& LogStream::getStreamBuffer() const
{
    LogStreamBuffer* streamBuffer = (LogStreamBuffer*)(rdbuf());
    return *streamBuffer;
}

LogStreamBuffer& LogStream::getStreamBuffer()
{
    LogStreamBuffer* streamBuffer = (LogStreamBuffer*)(rdbuf());
    return *streamBuffer;
}