/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: logserver.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "logserver.h"
#include "logserverstreambuf.h"
#include <stdarg.h>

using namespace std;
using namespace zeitgeist;

LogServer::LogServer(unsigned int size) :
    Node(), ostream(new LogServerStreamBuf(size))
{
}

LogServer::~LogServer()
{
    flush();

    // delete the created streambuf
    delete rdbuf();
}

void LogServer::AddStream(std::ostream* stream, unsigned int mask, bool syncStream)
{
    GetStreamBuf().AddStream(stream, mask, syncStream);
}

bool LogServer::RemoveStream(const std::ostream* stream)
{
    return GetStreamBuf().RemoveStream(stream);
}

void LogServer::RemoveAllStreams()
{
    return GetStreamBuf().RemoveAllStreams();
}

unsigned int LogServer::GetPriorityMask(const std::ostream* stream) const
{
    return GetStreamBuf().GetPriorityMask(stream);
}

bool LogServer::SetPriorityMask(const std::ostream* stream, unsigned int mask)
{
    return GetStreamBuf().SetPriorityMask(stream, mask);
}

LogServer& LogServer::Priority(unsigned int prio)
{
    GetStreamBuf().SetCurrentPriority(prio);
    return *this;
}

void
LogServer::Printf(const char *inFormat, ...)
{
    const int size = 4096;
    char copyBuffer[size];
    va_list args;
    va_start(args, inFormat);
    if (vsnprintf(copyBuffer, size, inFormat, args) == size)
    {
        copyBuffer[size-1] = 0;
    }
    va_end(args);
    (*this) << copyBuffer;
    this->flush();
}

const LogServerStreamBuf& LogServer::GetStreamBuf() const
{
    LogServerStreamBuf* streamBuf = (LogServerStreamBuf*)(rdbuf());
    return *streamBuf;
}

LogServerStreamBuf& LogServer::GetStreamBuf()
{
    LogServerStreamBuf* streamBuf = (LogServerStreamBuf*)(rdbuf());
    return *streamBuf;
}
