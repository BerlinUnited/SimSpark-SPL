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
#include <algorithm>

#include "forwarderstreambuf.h"

using namespace std;
using namespace Utility;

ForwarderStreamBuf::ForwarderStreamBuf(unsigned int size) :
streambuf(), M_size(size), M_current_priority(0xffffffff)
{
    // Here we set up the 'put' area, where data is streamed in
    if (M_size)
    {
        char* ptr = new char[M_size];
        setp(ptr, ptr + M_size);
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

    // delete mask-stream elements. The streams will not be deleted.
    while (M_streams.size() != 0)
    {
        M_streams.pop_back();
    }

    // delete buffer
    delete[] pbase();
}

void
ForwarderStreamBuf::addStream(std::ostream* stream, unsigned int mask)
{
    MaskStreams::iterator i;
    i = find_if(M_streams.begin(), M_streams.end(), MaskStreamEQ(stream));

    if (i == M_streams.end()) 
    {
        MaskStream pstream(mask, stream);
        M_streams.push_back(pstream);
    } 
    else 
    {
        i->first = mask;
    }
}

bool
ForwarderStreamBuf::removeStream(const std::ostream* stream)
{
    // flush buffer
    sync();

    MaskStreams::iterator i;
    i = find_if(M_streams.begin(), M_streams.end(), MaskStreamEQ(stream));

    if (i != M_streams.end()) 
    {
        M_streams.erase(i);
        return true;
    } 
    return false;
}

bool
ForwarderStreamBuf::setPriorityMask(const std::ostream* stream, 
                                    unsigned int mask)
{
    // flush buffer
    sync();

    MaskStreams::iterator i;
    i = find_if(M_streams.begin(), M_streams.end(), MaskStreamEQ(stream));

    if (i != M_streams.end()) 
    {
        i->first = mask;
        return true;
    }
    return false;
}

unsigned int
ForwarderStreamBuf::getPriorityMask(const std::ostream* stream) const
{
    MaskStreams::const_iterator i;
    i = find_if(M_streams.begin(), M_streams.end(), MaskStreamEQ(stream));

    if (i != M_streams.end()) 
    {
        return i->first;
    }
    return 0;
}

void 
ForwarderStreamBuf::setCurrentPriority(unsigned int priority)
{
    sync();
    M_current_priority = priority;
}

/*!
    This routine is called by the iostream library if our internal buffer is
    overflowing (the put area is full).
*/
ForwarderStreamBuf::IntType 
ForwarderStreamBuf::overflow(IntType c)
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
    MaskStreams::iterator i;
    for (i=M_streams.begin(); i!= M_streams.end(); ++i)
    {
        if ((*i).first & M_current_priority)
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
ForwarderStreamBuf::putChar(IntType chr)
{
    char a[1];
    a[0] = chr;
    forward(a, 1);
}
