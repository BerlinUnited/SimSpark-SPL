/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "forwarderstreambuf.h"

using namespace std;
using namespace Utility;

ForwarderStreamBuf::ForwarderStreamBuf(unsigned int size) :
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

ForwarderStreamBuf::~ForwarderStreamBuf()
{
    // flush buffer
    sync();

    // delete streams
    while (mStreams.size() != 0)
    {
        delete mStreams.back().second;
        mStreams.pop_back();
    }

    // delete buffer
    delete[] pbase();
}

void
ForwarderStreamBuf::addStream(std::ostream* stream, unsigned int mask)
{
    TMaskStream pstream(mask, stream);

    mStreams.push_back(pstream);
}

/*!
    This function is used to set the current priority level. All data which
    is streamed into the forwarder after this point will use the current
    priority level. Before the priority level is adjusted, all buffered data
    is flushed.

    @param priority current priority level (see EPriorityLevel)
*/
void 
ForwarderStreamBuf::setCurrentPriority(unsigned int priority)
{
    sync();
    mCurrentPriority = priority;
}

/*!
    This routine is called by the iostream library if our internal buffer is
    overflowing (the put area is full).
*/

ForwarderStreamBuf::TInt 
ForwarderStreamBuf::overflow(TInt c)
{
    // write out the buffered content
    putBuffer();

    // handle the extra character if it isn't eof
    if (c != FORWARDER_SB_EOF)
    {
        // if we don't do buffering
        if (pbase() == epptr())
        {
            // write out the character directly
            putChar(c);
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
int    
ForwarderStreamBuf::sync()
{
    putBuffer();

    return 0;
}

void
ForwarderStreamBuf::forward(const char* buffer, unsigned int length)
{
    TMaskStreams::iterator i;
    for (i=mStreams.begin(); i!= mStreams.end(); ++i)
    {
        if ((*i).first & mCurrentPriority)
        {
            (*i).second->write(buffer, length);
        }
    }
}

void 
ForwarderStreamBuf::putBuffer()
{
    // if we have data to stream out
    if (pbase() != pptr())
    {
        int len = (pptr() - pbase());
        
        // pbase() = buffer address
        forward(pbase(), len);

        // reset pointers == put area is empty
        setp(pbase(), epptr());
    }
}

void 
ForwarderStreamBuf::putChar(TInt chr)
{
    char a[1];
    a[0] = chr;
    forward(a, 1);
}
