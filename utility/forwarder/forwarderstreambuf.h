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
#ifndef UTILITY_FORWARDERSTREAMBUF_H
#define UTILITY_FORWARDERSTREAMBUF_H

/*! \class ForwarderStreamBuf
  $Id: forwarderstreambuf.h,v 1.2 2002/08/16 15:41:55 fruit Exp $

  ForwarderStreamBuf

  The ForwarderStreamBuf is a special std::streambuf derived class, which can
  stream to a list of streams. In addition to the multiplexing functionality
  this class also supports priorization of streamed data. Every registered
  stream is associated with a priority mask. This is a bitfield mask which
  tells the system which kind of messages should be muxed to a particular
  stream. The ForwarderStreamBuf also manages the current priority level to
  be used for incoming data. If the state is changed, the buffers are flushed!

  HISTORY:
  The forwarder subsystem was taken from a student project at the AI
  Research Group, Koblenz University. Original development by 
  Marco Koegler <koegler@uni-koblenz.de>, 
  Marcus Rollmann <rollmark@uni-koblenz.de>,
  Alexander Fuchs <alexf@uni-koblenz.de>, 
  et.al.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <functional>
#include <vector>

#ifdef HAVE_STREAMBUF
#include <streambuf>
#else
#include <streambuf.h>
#endif

#include "forwarder.h"

namespace Utility
{

class ForwarderStreamBuf : public std::streambuf
{
    // types
    //
protected:
#if HAVE_STREAMBUF
    typedef std::char_trait>s<char> TraitsType;
    typedef traits_type::int_type IntType;
#define FORWARDER_SB_EOF TraitsType::eof()
#else
    typedef char TraitsType;
    typedef int  IntType;
#define FORWARDER_SB_EOF EOF
#endif
private:
    typedef std::pair<unsigned int, std::ostream*> MaskStream;
    typedef std::vector<MaskStream> MaskStreams;

    // functions
    //
public:
    ForwarderStreamBuf(unsigned int size);
    virtual ~ForwarderStreamBuf();

    /*! Add a stream to the list of streams.

        First, it is checked if the stream is already in. If the stream
        is found, we only install a new priority mask, so no stream can
        be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
    */
    void addStream(std::ostream* stream, unsigned int mask);
    /*! Remove a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool removeStream(const std::ostream* stream);
    /*! Set priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @param mask     the new priority mask
        @return         true if the stream was found
    */
    bool setPriorityMask(const std::ostream* stream, unsigned int mask);
    /*! Get priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @return         the priority mask; 0 if stream was not found
    */
    unsigned int getPriorityMask(const std::ostream* stream) const;

    /*! Set the current priority level. 

        All data which is streamed into the forwarder after this point
        will use the current priority level. Before the priority level
        is adjusted, all buffered data is flushed.

        @param priority current priority level (see EPriorityLevel)
    */
    void setCurrentPriority(unsigned int priority);

protected:
    // these functions implement the streambuf interface ... handle with care
    IntType overflow(IntType c);
    int sync();

private:
    ForwarderStreamBuf(const ForwarderStreamBuf& obj);
    ForwarderStreamBuf& operator=(const ForwarderStreamBuf& obj);
    
    //! multiplex to all registered streams
    void forward(const char* buffer, unsigned int length);
    //! stream out complete internal buffer
    void putBuffer();
    //! stream out a single character
    void putChar(IntType chr);

    //! A predicate to compare streams in a MaskStream list (or vector). 
    class MaskStreamEQ : public unary_function<MaskStream,bool>
    {
    private:
        const std::ostream* stream;
    public:
        explicit MaskStreamEQ(const std::ostream *str) : stream(str) {}
        bool operator ()(const MaskStream& ms)
        { return ms.second == stream; }
    };

    // members
    //
private:
    //! size of the internal buffer to use
    unsigned int    M_size;
    unsigned int    M_current_priority;
    MaskStreams     M_streams;
};

} //namespace

#endif // UTILITY_FORWARDERSTREAMBUF_H
