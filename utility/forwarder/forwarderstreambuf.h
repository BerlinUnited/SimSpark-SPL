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
#ifndef _FORWARDERSTREAMBUF_H_
#define _FORWARDERSTREAMBUF_H_

/*! \class ForwarderStreamBuf
  $Id: forwarderstreambuf.h,v 1.1 2002/08/12 09:52:07 fruit Exp $

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
    typedef std::char_traits<char> TTraits;
    typedef traits_type::int_type TInt;
#define FORWARDER_SB_EOF TTraits::eof()
#else
    typedef char TTraits;
    typedef int  TInt;
#define FORWARDER_SB_EOF EOF
#endif
private:
    typedef std::pair<unsigned int, std::ostream*> TMaskStream;
    typedef std::vector<TMaskStream> TMaskStreams;

    // functions
    //
public:
    ForwarderStreamBuf(unsigned int size);
    virtual ~ForwarderStreamBuf();

    void addStream(std::ostream* stream, unsigned int mask);
    void setCurrentPriority(unsigned int priority);

protected:
    // these functions implement the streambuf interface ... handle with care
    TInt overflow(TInt c);
    int sync();

private:
    ForwarderStreamBuf(const ForwarderStreamBuf& obj);
    ForwarderStreamBuf& operator=(const ForwarderStreamBuf& obj);
    
    //! multiplex to all registered streams
    void forward(const char* buffer, unsigned int length);
    //! stream out complete internal buffer
    void putBuffer();
    //! stream out a single character
    void putChar(TInt chr);

    // members
    //
private:
    //! size of the internal buffer to use
    unsigned int    mSize;
    unsigned int    mCurrentPriority;
    TMaskStreams    mStreams;
};

} //namespace Utility

#endif // _FORWARDERSTREAMBUF_H_
