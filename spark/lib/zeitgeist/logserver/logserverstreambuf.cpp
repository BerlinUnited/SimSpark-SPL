/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include <iostream>
#include <algorithm>
#include "logserverstreambuf.h"

using namespace std;
using namespace zeitgeist;

LogServerStreamBuf::LogServerStreamBuf(unsigned int size) :
streambuf(), mSize(size), mCurrentPriority(0xffffffff)
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

LogServerStreamBuf::~LogServerStreamBuf()
{
    // flush buffer
    sync();
    SyncStreams();

    // delete streams
    while (! mStreams.empty())
    {
        if (
            (mStreams.back().mStream != &std::cout) &&
            (mStreams.back().mStream != &std::cerr)
            )
            {
                delete mStreams.back().mStream;
            }

        mStreams.pop_back();
    }

    // delete buffer
    delete[] pbase();
}

void LogServerStreamBuf::AddStream(std::ostream *stream, unsigned int mask, bool syncStream)
{
    TMaskStreams::iterator i;
    i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

    if (i == mStreams.end())
    {
        MaskStream pstream(mask, stream, syncStream);
        mStreams.push_back(pstream);
    }
    else
    {
        i->mMask |= mask;
        i->mSync = syncStream; //overwrite
    }
}

bool LogServerStreamBuf::RemoveStream(const std::ostream *stream)
{
    // flush buffer
    sync();
    SyncStreams();

    TMaskStreams::iterator i;
    i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

    if (i != mStreams.end())
        {
            mStreams.erase(i);
            return true;
        }
    return false;
}

void LogServerStreamBuf::RemoveAllStreams()
{
    mStreams.clear();
}

bool
LogServerStreamBuf::SetPriorityMask(const std::ostream *stream, unsigned int mask)
{
        // flush buffer
        sync();
        SyncStreams();

        TMaskStreams::iterator i;
        i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

        if (i != mStreams.end())
        {
                i->mMask = mask;
                return true;
        }
        return false;
}

unsigned int LogServerStreamBuf::GetPriorityMask(const std::ostream *stream) const
{
        TMaskStreams::const_iterator i;
        i = find_if(mStreams.begin(), mStreams.end(), MaskStreamEQ(stream));

        if (i != mStreams.end())
        {
                return i->mMask;
        }
        return 0;
}

void LogServerStreamBuf::SetCurrentPriority(unsigned int priority)
{
    sync();
    SyncStreams();

    mCurrentPriority = priority;
}

/*!
    This routine is called by the iostream library if our internal buffer is
    overflowing (the put area is full).
*/
LogServerStreamBuf::TIntType LogServerStreamBuf::overflow(TIntType c)
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

/*!
    This routine synchronizes the internal state with the external state. It is
    used to flush the streambuf object.
*/
int LogServerStreamBuf::sync()
{
        PutBuffer();

        return 0;
}

void LogServerStreamBuf::Forward(const char *buffer, unsigned int length)
{
        TMaskStreams::iterator i;
        for (i=mStreams.begin(); i!= mStreams.end(); ++i)
        {
                if ((*i).mMask & mCurrentPriority)
                {
                        (*i).mStream->write(buffer, length);
                }
        }
}

void LogServerStreamBuf::PutBuffer()
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

void
LogServerStreamBuf::PutChar(TIntType chr)
{
        char a[1];
        a[0] = chr;
        Forward(a, 1);
}

void 
LogServerStreamBuf::SyncStreams()
{
        TMaskStreams::iterator i;
        for (i=mStreams.begin(); i!= mStreams.end(); ++i)
        {
                if ((*i).mSync)
                {
                        (*i).mStream->flush();
                }
        }
}