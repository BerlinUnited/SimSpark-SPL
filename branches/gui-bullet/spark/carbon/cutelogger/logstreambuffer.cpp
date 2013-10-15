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

#include "logstreambuffer.h"
#include "logger.h"

#include <iostream>
#include <QString>
#include <QByteArray>
#include <QDebug>

using namespace std;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

LogStreamBuffer::LogStreamBuffer(Logger::LogLevel level, unsigned int size, unsigned int logsource) :
    streambuf(), mSize(size), mLogLevel(level), mLogSource(logsource)
{
    // Here we set up the 'put' area, where data is streamed in
    if (mSize)
    {
        char* ptr = new char[mSize];
        setp(ptr, ptr + mSize);
    }
    else
    {
        setp(0, 0);
    }

    // we don't support a 'get' area ... no istream support
    setg(0, 0, 0);
}

LogStreamBuffer::~LogStreamBuffer()
{
    // flush buffer
    sync();

    // delete buffer
    delete[] pbase();
}

unsigned int LogStreamBuffer::getSize() const
{
    return mSize;
}

Logger::LogLevel LogStreamBuffer::getLogLevel() const
{
    return mLogLevel;
}

unsigned int LogStreamBuffer::getLogSource() const
{
    return mLogSource;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

LogStreamBuffer::TIntType LogStreamBuffer::overflow(TIntType c)
{
    // write out the buffered content
    PutBuffer();

    // handle the extra character if it isn't eof
    if (c != TTraitsType::eof())
    {
        // if we don't do buffering
        if (pbase() == epptr())
        {
            // write out the character directly
            PutChar(c);
        }
        else
        {
            // buffer it
            sputc(c);
        }
    }
    return 0;
}

int LogStreamBuffer::sync()
{
    PutBuffer();

    return 0;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void LogStreamBuffer::Forward(const char *buffer, unsigned int length)
{
    //create 0-terminated string, dont use the newline symbol
    std::string str(buffer, (length > 0 && buffer[length-1] == '\n') ? length - 1 : length);

    //log the message
    LOG_S_MESSAGE(mLogLevel, mLogSource) << str.c_str();
}

void LogStreamBuffer::PutBuffer()
{
    // if we have data to stream out
    if (pbase() != pptr())
    {
        int len = (pptr() - pbase());

        // pbase() = buffer address
        Forward(pbase(), len);

        // reset pointers == put area is empty
        setp(pbase(), epptr());
    }
}

void LogStreamBuffer::PutChar(TIntType chr)
{
    char a[1];
    a[0] = chr;
    Forward(a, 1);
}